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
const ball_initial_speed = 0.3;
const ball_acceleration = 0.0001;
const max_direction_horiz = 5; // maximal vertical slope
const keys_by_player = [["KeyW", "KeyS"], ["ArrowUp","ArrowDown"]];
const paddle_normal_vectors =
				[	[-0.5, -0.5], 	[0, -1], 	[0.5, -0.5],
					[-1, 0], 		[0, 0], 	[1, 0],
					[-0.5, 0.5], 	[0, 1], 	[0.5, 0.5]];
const wall_normal_vectors = {top: [0, 1], bottom: [0, -1]};
const simulation_tick_interval = 10; // ms between each game state update by server
const broadcast_tick_interval = 50; // ms between each game state broadcast to clients by server

// Utils

// Structure holding data about events received from user (keys pressed, buttons clicked),
// will be sent to server through WS at each animation frame
interface Client_events
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

// Pong Game class

class Pong_game
{
	// Game state
	ws: WebSocket;
	active: boolean; // whether the game is active or paused
	terminated: boolean; // whether the game is alive or terminated (<=> a player lost/conceded/disconnected)
	player_index: number = -1; // index of this client in the game (will be 0 or 1 once game starts)
	server_broadcast_interval: number = -1; // interval at which server sends game data, in ms
	game_state_string: string = ""; // data about game state received from Server
	events_for_server: Client_events = {up: false, down: false, pause: false, play: false, concede: false};
	pending_events_for_server: boolean = false;
	// Canvas graphics
	time_last_anim_update: number; // time at which the last <update_animation> was executed
	time_last_data_received: number; // time at which the last game state data package was received from server
	frame_counter: number;
	canvas: HTMLCanvasElement | null = null;
	ctx: CanvasRenderingContext2D | null | undefined = null;
	// HTML page elements
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
		this.extract_HTML_elements();
		this.create_event_listeners();
		this.time_last_anim_update = -1;
		this.time_last_data_received = -1;
		this.frame_counter = 0;
		this.ws = new WebSocket(`ws://${window.location.host}`);
		this.setup_websocket();
	}

	// Extract elements from HTML DOM
	extract_HTML_elements()
	{
		this.canvas = document.querySelector("#canvas");
		if (!this.canvas)
			throw new Error("Invalid Canvas");
		this.ctx = this.canvas.getContext("2d");
		if (!this.ctx)
			throw new Error("Invalid Context");
		this.points_info = document.querySelector("#pointsInfo");
		this.play_button = document.querySelector("#play");
		this.pause_button = document.querySelector("#pause");
		this.concede_button = document.querySelector("#concede");
		if (!this.points_info || !this.play_button || !this.pause_button || !this.concede_button)
			throw new Error("Invalid HTML");
		this.play_button.addEventListener("click", () => {this.play();});
		this.pause_button.addEventListener("click", () => {this.pause();});
		this.concede_button.addEventListener("click", () => {this.concede();});
	}

	// WEBSOCKET EVENTS MANAGEMENT
	
	// Defines what to do on events on the websocket
	setup_websocket()
	{
		this.ws.onopen = () => {console.log("Client side websocket : opened");};
		this.ws.onmessage = (event) => {this.receive_game_state_data(event.data);};
		this.ws.onclose = () => {this.end_game();};
	}

	// Registers data sent by the server and unpdates <time_last_data_received>
	// 		- first thing sent by server is setup information,
	// 		  which is read to get the <player_index> (and then animation loop is started)
	// 		- then all following messages are game state data
	receive_game_state_data(data: string)
	{
		let game_state_data: Game_state_description;
		let game_init_data: Game_init_description;
	
		console.log("Client websocket : received data from server");
		this.game_state_string = data;
		if (this.time_last_data_received == -1)
		{
			game_init_data = JSON.parse(data);
			this.player_index = game_init_data.player_index;
			this.server_broadcast_interval = game_init_data.broadcast_interval;
			console.log(`   -> this was first message, assigned index ${this.player_index}, starting anim`);
			requestAnimationFrame(this.update_animation);
		}
		this.time_last_data_received = performance.now();
	}

	// Ends game when server side closes the websocket
	end_game()
	{
		console.log("Client side websocket : closed");
		this.terminated = true;
		// #f : indiquer fin sur la page
	}

	// USER EVENTS MANAGEMENT
	// (user events stored in <events_for_server> to be sent to server at next animation frame)

	// Listen to keyboard events for player keys
	create_event_listeners()
	{
		document.addEventListener('keydown', (event) => {this.store_key_event(event, false)});
		document.addEventListener('keyup', (event) => {this.store_key_event(event, true)});
	}

	// Associate keyboard event to a paddle move direction according to player keys
	// (eg. if this client is player index 0, they use keys 'w' and 's')
	store_key_event(event: KeyboardEvent, key_released: boolean)
	{
		var code = event.code;

		if (!(keys_by_player[this.player_index].includes(code)))
			return ;
		event.preventDefault();
		if (code == keys_by_player[this.player_index][0])
		{
			if (key_released)
				this.events_for_server.up = false;
			else
				this.events_for_server.up = true;
		}
		else
		{
			if (key_released)
				this.events_for_server.down = false;
			else
				this.events_for_server.down = true;
		}
	}

	// Play button : request server to play game
	play()
	{
		if (this.active == false)
		{
			this.events_for_server.play = true;
			this.pending_events_for_server = true;
		}
	}
	// Pause button : request server to pause game
	pause()
	{
		if (this.active == true)
		{
			this.events_for_server.pause = true;
			this.pending_events_for_server = true;
		}
	}
	// Concede button : request server to concede
	concede()
	{
		this.events_for_server.concede = true;
		this.pending_events_for_server = true;
	}

	// GAME MAINLOOP

	// Update function called when browser is ready for a new frame (usually each 16ms)
	// (Method declaration with the syntax `funcname = (paraname) => {funcbody}`,
	//  because it seems to be the only way to avoid issues with call by `requestAnimationFrame`)
	update_animation = () =>
	{
		let current_time: number = performance.now();
		let delta_time: number;
		let gsd: Game_state_description; // "gsd" for "game_state_data"
		let events_for_server_string: string;

		// (0) registers current time
		this.frame_counter++;
		if (this.time_last_anim_update == -1) // case where this frame is first frame
			delta_time = 16; // 16ms, delta between 2 frames on the usual 60 frames/second setup
		else
			delta_time = current_time - this.time_last_anim_update;
		this.time_last_anim_update = current_time;
		console.log(`--- Frame ${this.frame_counter}, current time ${current_time}, delta ${delta_time}`);

		// (1) redraws paddles and ball on canvas, according to game state received from server
		gsd = JSON.parse(this.game_state_string);
		console.log(`gsd : ${this.game_state_string}`);
		this.active = gsd.active;
		this.draw_canvas(gsd);
		this.points_info!.innerHTML = "Points : " + gsd.points[0] + "-" + gsd.points[1];
		
		// (2) sends to server user events received since last call
		events_for_server_string = JSON.stringify(this.events_for_server);
		this.ws.send(events_for_server_string);
		this.events_for_server.play = false;
		this.events_for_server.pause = false;
		this.events_for_server.concede = false;
		
		// (3) calls itself again with <requestAnimationFrame>
		if (!this.terminated)
			requestAnimationFrame(this.update_animation);
	}

	// Draw canvas figures (1 ball and 2 paddles)
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

- NEXT STEP : utiliser <time_last_data_received> pour interpoler en retard

- trouver un moyen de n'envoyer les events au serveur que si au moins l'un des 4 est true
- peaufiner le HTML : zone pour indiquer si le jeu est en attente / actif / paused / terminated

*/

/*
I am experimenting with typescript by creating a small game using an HTML canvas. I define a small <Point> class at the beginning of my typescript program :

Unexpectedly, when I try to run my program in my browser, I get an error "Uncaught SyntaxError: bad class member definition" on the line `x:number`.
I am very surprised, since I did not find any reference to that error online, and my Point class is exactly the same as in the typescript documentation example.

My specific use case is a small Node.JS websocket server. It would receive Websocket requests from clients, and store the created Websocket objects in an array. The index of the Websocket in that array would be the main way to identify the client in various functions.
So when the client disconnects, the Websocket closes, and my server should forget the Websocket object in the array. But it cannot simply remove it from the array, since it would modify the indexes of Websockets coming after it in the array. If the array acts like a dictionary, it may be possible to remove a key/value pair without displacing the rest. 
*/
