// Configuration constants

// (TMP, should be replaced by a json file)
const canvas_width = 300;
const canvas_height = 150;
const paddle_margin = 10;
const paddle_width = 10;
const paddle_height = 40;
const paddle_speed = 4;
const paddle_colors = ["rgb(200 0 0)", "rgb(0 0 200)"];
const ball_radius = 4;
const ball_initial_speed = 0.8;
const ball_acceleration = 0.0001;
const max_direction_horiz = 1.5; // maximal vertical slope
const keys_by_player = [["KeyW", "KeyS"], ["ArrowUp","ArrowDown"]];
const paddle_normal_vectors =
				[	[-0.5, -0.5], 	[0, -1], 	[0.5, -0.5],
					[-1, 0], 		[0, 0], 	[1, 0],
					[-0.5, 0.5], 	[0, 1], 	[0.5, 0.5]];
const wall_normal_vectors = {top: [0, 1], bottom: [0, -1]};
const simulation_tick_interval = 10; // ms between each game state update by server
const broadcast_tick_interval = 50; // ms between each game state broadcast to clients by server

// Utils

// Structure holding data about commands received from user
// (if user is human, commands come from keys pressed / buttons clicked),
// 		-> will be sent to server through WS at each animation frame
interface Client_commands
{
	// Booleans indicating whether the "up"/"down" key is currently pressed
	up: boolean,
	down: boolean,
	// Booleans indicating whether the "pause"/"play"/"concede" has been pressed since last send
	pause: boolean,
	play: boolean,
	concede: boolean
}

// Structure holding the data for the current state of the game
// (plus the index of the player to which the structure is sent through websocket)
interface Game_state_description
{
	ball_pos:
	{
		x:number,
		y:number
	},
	paddles_pos:
	[
		{
			x:number,
			y:number
		},
		{
			x:number,
			y:number
		}
	],
	points: [number, number],
	active: boolean
}

// Structure holding data sent by server to clients at the start of a game
// to allow them to setup the game on their side
interface Game_init_description
{
	player_index: number, // index of the recipient player
	broadcast_interval: number
}

// Basic point interface
// (thanks to structural typing, <Game_state_description.ball_pos> is a <Point>)
interface Point
{
	x: number,
	y: number
}

// Function for linear interpolation : returns a point placed between <p1> and <p2>
// at a distance from <p1> equal to `proportion * distance from p1 to p2`
function lerp(p1: Point, p2 : Point, proportion: number): Point
{
	let p3: Point = {x: 0, y: 0};

	p3.x = p1.x + (p2.x - p1.x) * proportion;
	p3.y = p1.y + (p2.y - p1.y) * proportion;
	return (p3);
}

// Pong Game class

class Pong_game
{
	// Game state
	ws: WebSocket;
	active: boolean; // whether the game is active or paused
	terminated: boolean; // whether the game is alive or terminated (<=> a player lost/conceded/disconnected)
	player_index: number = -1; // index of this client in the game (will be 0 or 1 once game starts)
	server_broadcast_interval: number = -1; // interval at which server sends game data, in ms
	game_state_current: Game_state_description | null = null; // current position of paddles/ball on canvas
	game_state_next: Game_state_description | null = null; // positions given by server, to be reached
	commands_for_server: Client_commands = {up: false, down: false, pause: false, play: false, concede: false};
	pending_commands_for_server: boolean = false; // whether new commands must be sent to server
	// Canvas graphics
	time_last_anim_update: number; // time at which the last <update_animation> was executed
	time_last_data_received: number; // time at which the last game state data package was received from server
	frame_counter: number;
	canvas: HTMLCanvasElement | null = null;
	ctx: CanvasRenderingContext2D | null | undefined = null;
	// HTML page elements
	announcement: HTMLParagraphElement | null = null;
	points_info: HTMLParagraphElement | null = null;
	play_button: HTMLButtonElement | null = null;
	pause_button: HTMLButtonElement | null = null;
	concede_button: HTMLButtonElement | null = null;

	// SETUP

	// Constructor : Game starts NOT running (<active> is false)
	constructor()
	{
		this.active = false;
		this.terminated = false;
		this.time_last_anim_update = -1;
		this.time_last_data_received = -1;
		this.frame_counter = 0;
		this.ws = new WebSocket(`ws://${window.location.host}`);
		if (this.extract_HTML_elements())
		{
			this.abort_game("invalidHTML");
			return ;
		}
		this.create_event_listeners();
		this.setup_websocket();
	}

	// Extract elements from HTML DOM, returns <true> if an error occurred
	extract_HTML_elements() : boolean
	{
		this.canvas = document.querySelector("#canvas");
		if (!this.canvas)
			return (true);
		this.ctx = this.canvas.getContext("2d");
		if (!this.ctx)
			return (true);
		this.announcement = document.querySelector("#announcement");
		this.points_info = document.querySelector("#pointsInfo");
		this.play_button = document.querySelector("#play");
		this.pause_button = document.querySelector("#pause");
		this.concede_button = document.querySelector("#concede");
		if (!this.announcement || !this.points_info
			|| !this.play_button || !this.pause_button || !this.concede_button)
			return (true);
		this.announcement.innerHTML = "Waiting for players...";
		this.play_button.addEventListener("click", () => {this.play();});
		this.pause_button.addEventListener("click", () => {this.pause();});
		this.concede_button.addEventListener("click", () => {this.concede();});
		return (false);
	}

	// WEBSOCKET EVENTS MANAGEMENT
	
	// Defines what to do on events on the websocket
	setup_websocket()
	{
		this.ws.onopen = () => {console.log("Client side websocket : opened");};
		this.ws.onmessage = (event) => {this.receive_game_state_data(event.data);};
		this.ws.onclose = () => {this.end_game();};
	}

	// Registers data sent by the server and updates <time_last_data_received>
	// 		- first package sent by server is setup information, which is read to :
	// 			~ get the <player_index> of this player (useful to know which keyboard keys to watch)
	// 			~ get the <server_broadcast_interval>, expected time between server broadcasts
	// 		  	~ start the game mainloop with <requestAnimationFrame>
	// 		- then all following messages are game state data
	// 			~ if <game_state_current> is not set, it becomes set to the new data
	// 			~ if <game_state_current> is set, the new data is registered in <game_state_next>
	// 				so that the <update_animation> loop can use it for interpolation
	// 					(except the 'points' data which is put immediately in <game_state_current>)
	receive_game_state_data(data: string)
	{
		let game_state_data: Game_state_description;
		let game_init_data: Game_init_description;
	
		console.log(`Client websocket : received data from server after ${performance.now() - this.time_last_data_received}`);
		if (this.time_last_data_received == -1) // first message received : setup info
		{
			try
			{
				game_init_data = JSON.parse(data);
				this.player_index = game_init_data.player_index;
				this.server_broadcast_interval = game_init_data.broadcast_interval;
				console.log(`   -> first server message, assigned index ${this.player_index}, starting anim`);
				this.active = true;
				this.announcement!.innerHTML = `[Player ${this.player_index}] Starting game...`;
				requestAnimationFrame(this.update_animation);
			}
			catch {
				console.log(`Error while validating initial data from server`);
				this.abort_game("validation");
			}
		}
		else // next messages received : game state
		{
			try {
				game_state_data = JSON.parse(data);
				this.game_state_next = game_state_data;
				this.active = game_state_data.active;
				if (this.game_state_current == null) // initialization of <game_state_current>
					this.game_state_current = game_state_data;
				else
					this.game_state_current!.points = game_state_data.points;
			}
			catch {
				console.log(`Error while validating game state data from server`);
				this.abort_game("validation");
			}
		}
		this.time_last_data_received = performance.now();
	}

	// Ends game "cleanly" when server side closes the websocket
	end_game()
	{
		console.log("Client side websocket : closed");
		this.terminated = true;
		if (this.announcement != null && this.game_state_current != null)
		{
			if (this.game_state_current.points[0] > this.game_state_current.points[1])
				this.announcement.innerHTML = `[Player ${this.player_index}] Game ended, victory : player 0.`;
			else if (this.game_state_current.points[1] > this.game_state_current.points[0])
				this.announcement.innerHTML = `[Player ${this.player_index}] Game ended, victory : player 1.`;
			else
				this.announcement.innerHTML = `[Player ${this.player_index}] Game ended, tie.`;
		}
	}

	// Ends game "unexpectedly" when an error occurs and is caught
	abort_game(reason: string)
	{
		console.log(`Game aborted because of : ${reason}`)
		this.terminated = true;
		if (this.announcement != null)
			this.announcement.innerHTML = "Game aborted because of an unexpected error.";
	}

	// USER EVENTS MANAGEMENT
	// (user events stored in <commands_for_server> to be sent to server at next animation frame)

	// Listen to keyboard events for player keys
	create_event_listeners()
	{
		document.addEventListener('keydown', (event) => {this.store_key_event(event, false)});
		document.addEventListener('keyup', (event) => {this.store_key_event(event, true)});
	}

	// Associate keyboard event to a paddle move command according to player keys
	// (eg. if this client is player index 0, they use keys 'w' and 's')
	// /!\ checks if <player_index> is different from -1
	// 	   to avoid registering key events until game is really started
	store_key_event(event: KeyboardEvent, key_released: boolean)
	{
		var code = event.code;

		if (this.player_index == -1	|| !(keys_by_player[this.player_index].includes(code)))
			return ;
		event.preventDefault();
		if (code == keys_by_player[this.player_index][0])
		{
			if (key_released)
				this.commands_for_server.up = false;
			else
				this.commands_for_server.up = true;
		}
		else
		{
			if (key_released)
				this.commands_for_server.down = false;
			else
				this.commands_for_server.down = true;
		}
		this.pending_commands_for_server = true;
	}

	// Play button : request server to play game
	play()
	{
		if (this.active == false)
		{
			this.commands_for_server.play = true;
			this.pending_commands_for_server = true;
		}
	}
	// Pause button : request server to pause game
	pause()
	{
		if (this.active == true)
		{
			this.commands_for_server.pause = true;
			this.pending_commands_for_server = true;
		}
	}
	// Concede button : request server to concede
	concede()
	{
		this.commands_for_server.concede = true;
		this.pending_commands_for_server = true;
	}

	// GAME MAINLOOP

	// Update function called when browser is ready for a new frame (usually each 16ms)
	// (Method declaration with the syntax `funcname = (paraname) => {funcbody}`,
	//  because it seems to be the only way to avoid issues with call by `requestAnimationFrame`)
	update_animation = () =>
	{
		let current_time: number = performance.now();
		let delta_time: number;
		let commands_for_server_string: string;
		let bot_checkbox = <HTMLInputElement> document.getElementById("testbot");

		if (this.terminated)
			return ;

		// (0) registers current time
		this.frame_counter++;
		if (this.time_last_anim_update == -1) // case where this frame is first frame
			delta_time = 16; // 16ms, delta between 2 frames on the usual 60 frames/second setup
		else
			delta_time = current_time - this.time_last_anim_update;
		this.time_last_anim_update = current_time;
		console.log(`--- Frame ${this.frame_counter}, current time ${current_time}, delta ${delta_time}`);

		
		// (1) updates game state if game is active
		if (this.active && this.game_state_current != null)
		{
			// (1a) makes game state ('current') progress towards game state ('next') received from server
			this.lerp_game_state(current_time, delta_time);
			//this.log_game_state();
			// (1b) redraws paddles and ball on canvas, according to current game state
			this.draw_canvas(this.game_state_current);
			this.announcement!.innerHTML = `[Player ${this.player_index}] Game active`;
			this.points_info!.innerHTML = "Points : " + this.game_state_current.points[0] + "-"
													  + this.game_state_current.points[1];
		}
		if (!this.active)
			this.announcement!.innerHTML = `[Player ${this.player_index}] Game paused`;
		
		// (2) sends to server the user commands received since last call
		// 		~ nothing sent if no new commands received
		// 			(eg. human user did not press/release any key, and did not press any button)
		// 		~ if this client is a bot, calls bot decision function
		// 			which will overwrite all commands from keyboard events if game is active
		if (bot_checkbox.checked)
			this.bot_decision_making();
		if (this.pending_commands_for_server)
		{
			commands_for_server_string = JSON.stringify(this.commands_for_server);
			this.ws.send(commands_for_server_string);
		}
		this.commands_for_server.play = false;
		this.commands_for_server.pause = false;
		this.commands_for_server.concede = false;
		this.pending_commands_for_server = false;
		
		// (3) calls itself again with <requestAnimationFrame>
		if (!this.terminated)
			requestAnimationFrame(this.update_animation);
	}

	// Makes game state ('current') progress towards game state ('next') received from server
	// using interpolation : objects advance towards their expected position,
	// 		by a distance proportional to the distance between current pos and expected pos
	// The proportion is calculated by comparing the animation tick <delta_time>
	// to the expected time before the next broadcast
	// 		(eg. if they are equal, the animation should go immediately to next game state -> <proportion> = 1)
	lerp_game_state(current_time: number, delta_time: number)
	{
		let gsc = this.game_state_current; // for readability
		let gsn = this.game_state_next;
		let elapsed = current_time - this.time_last_data_received; // time elapsed since last broadcast
		let time_before_broadcast = (this.server_broadcast_interval - elapsed); // time before next broadcast
		let proportion = delta_time / (time_before_broadcast + delta_time);

		if (gsc == null || gsn == null)
			return ;
		//console.log(`	lerping : time before next broadcast ${time_before_broadcast}, proportion ${proportion}`);
		if (proportion < 0) {proportion = 0;}
		if (proportion > 1) {proportion = 1;}
		gsc.ball_pos = lerp(gsc.ball_pos, gsn.ball_pos, proportion);
		gsc.paddles_pos[0] = lerp(gsc.paddles_pos[0], gsn.paddles_pos[0], proportion);
		gsc.paddles_pos[1] = lerp(gsc.paddles_pos[1], gsn.paddles_pos[1], proportion);
	}

	// Draw canvas figures (1 ball and 2 paddles), using the canvas context features
	draw_canvas(gsd: Game_state_description)
	{
		let ctx: CanvasRenderingContext2D = this.ctx!;
		let paddle_index: number;

		ctx.clearRect(0, 0, canvas_width, canvas_height); // erase previous drawings
		for (paddle_index = 0; paddle_index < 2; paddle_index++)
		{
			ctx.fillStyle = paddle_colors[paddle_index];
			ctx.fillRect(gsd.paddles_pos[paddle_index].x - paddle_width / 2,
				gsd.paddles_pos[paddle_index].y - paddle_height / 2,
				paddle_width, paddle_height);
		}
		ctx.strokeStyle = "rgb(0 0 0)";
		ctx.beginPath();
		ctx.arc(gsd.ball_pos.x, gsd.ball_pos.y, ball_radius, 0, Math.PI * 2, false);
		ctx.stroke();
	}

	// BOT DECISIONS

	// Decides automatically whether the player's paddle should go up or down
	// (for now simplistically follows the ball's ordinate)
	bot_decision_making()
	{
		let gsn = this.game_state_next;
		let vertical_dist_to_ball: number;

		if (this.active == false || gsn == null)
			return ;
		vertical_dist_to_ball = gsn.paddles_pos[this.player_index].y - gsn.ball_pos.y;
		if (vertical_dist_to_ball > paddle_height / 4)
			this.commands_for_server.up = true;
		else
			this.commands_for_server.up = false;
		if (vertical_dist_to_ball < (paddle_height / 4) * -1)
			this.commands_for_server.down = true;
		else
			this.commands_for_server.down = false;
		this.pending_commands_for_server = true;
		console.log(`	Bot decision : distance ${vertical_dist_to_ball} -> up ${this.commands_for_server.up} down ${this.commands_for_server.down}`);
	}

	// LOGS

	// Logs the position of game components (current position, and last position broadcasted by server)
	log_game_state()
	{
		let gsc = this.game_state_current;
		let gsn = this.game_state_next;

		console.log(`current : ball ${gsc?.ball_pos.x},${gsc?.ball_pos.y}`);
		console.log(`	paddle 0 ${gsc?.paddles_pos[0].x},${gsc?.paddles_pos[0].y}`);
		console.log(`	paddle 1 ${gsc?.paddles_pos[1].x},${gsc?.paddles_pos[1].y}`);
		console.log(`next : ball ${gsn?.ball_pos.x},${gsn?.ball_pos.y}`);
		console.log(`	paddle 0 ${gsn?.paddles_pos[0].x},${gsn?.paddles_pos[0].y}`);
		console.log(`	paddle 1 ${gsn?.paddles_pos[1].x},${gsn?.paddles_pos[1].y}`);
	}
}

function setup()
{
	let game = new Pong_game();
}

window.onload = setup;



/*


+ tenir compte du temps passe depuis la derniere frame dans update_animation
+ transformer la surface des paddles en demi-cercles
+ creer un petit mecanisme de rectification de la direction de la balle quand elle est trop verticale
+ teleporter la balle si son centre est a l'interieur d'un paddle

+ utiliser <time_last_data_received> pour interpoler la position des game components
+ creer une option pour jouer contre un bot primaire
+ trouver un moyen de n'envoyer les events au serveur que si au moins l'un des 4 events est true
	-> remplace par un flag qui indique s'il y a des commandes pending a envoyer au serveur

+ valider le game state recu du serveur quand on le JSON.parse
+ peaufiner le HTML : zone pour indiquer si le jeu est en attente / actif / paused / terminated

- construire une vraie methode de tests avec des bots lances par node.JS sans rendering
- rendre le bot un peu plus interessant (ca devient la partie IA)
*/

/*
Actually, the part about sharing types between client and server gets my interest. Currently, I have 2 big JS files, one for the server (launched with node.JS), and one for the client (loaded and run by the client's browser). They use exactly the same interfaces describing the structure of data to send as JSON strings through the websockets, so it would be convenient if the definition of those interfaces could be in a separate file that my server would import and that the client would load in addition to the real program. I don't know how to manipulate JS imports to this end. Additionally, it would also be practical to put in the separate file some shared constants and utility functions.
*/

/* JS notes
- structural typing : two objects sharing the same structure are of the same type, even if not defined that way originally
	(-> one can be assigned to the other, etc)
- interfaces are tools used for code clarity only, they disappear at compilation (replaced by complete object structure description)
	and thus they do not exist at runtime, at runtime there exists no reference to interface / other metadata in objects

*/
