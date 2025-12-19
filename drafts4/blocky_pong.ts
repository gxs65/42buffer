// Configuration constants

const canvas_width = 300;
const canvas_height = 150;
const paddle_margin = 10;
const paddle_width = 10;
const paddle_height = 40;
const paddle_speed = 4;
const paddle_colors = ["rgb(200 0 0)", "rgb(0 0 200)"];
const ball_radius = 4;
const ball_initial_speed = 2;
const ball_acceleration = 0.0001;
const max_direction_horiz = 5; // maximal vertical slope
const keys_by_player = [["KeyW", "KeyS"], ["ArrowUp","ArrowDown"]];
const paddle_normal_vectors =
				[	[-0.5, -0.5], 	[0, -1], 	[0.5, -0.5],
					[-1, 0], 		[0, 0], 	[1, 0],
					[-0.5, 0.5], 	[0, 1], 	[0.5, 0.5]];
const wall_normal_vectors = {top: [0, 1], bottom: [0, -1]};

// Utils

interface Client_events
{
	up: boolean,
	down: boolean,
	pause: boolean,
	play: boolean,
	concede: boolean
}

// Pong Game class

class Pong_game
{
	// Game state
	paused: boolean;
	animation_events: Array<string> = [];
	events_for_server: Client_events = {up: false, down: false, pause: false, play: false, concede: false};
	pending_events_for_server: boolean = false;
	// Canvas graphics
	last_time: number;
	frame_counter: number;
	canvas: HTMLCanvasElement | null = null;
	ctx: CanvasRenderingContext2D | null | undefined = null;
	// HTML page elements
	points_info: HTMLParagraphElement | null = null;
	play_button: HTMLButtonElement | null = null;
	pause_button: HTMLButtonElement | null = null;
	concede_button: HTMLButtonElement | null = null;

	// SETUP

	// Constructor : Game starts NOT running
	constructor()
	{
		this.paused = true;
		this.extract_HTML_elements();
		this.create_event_listeners();
		this.last_time = -1;
		this.frame_counter = 0;
		this.animation_events = [];
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

	// Listen to keyboard events for player keys
	create_event_listeners()
	{
		document.addEventListener('keydown', (event) => {this.store_key_event(event, false)});
		document.addEventListener('keyup', (event) => {this.store_key_event(event, true)});
	}

	// Request server to play game (request will be sent with next animation frame)
	play()
	{
		if (this.paused == true)
		{
			this.events_for_server.play = true;
			this.pending_events_for_server = true;
		}
	}
	// Request server to pause game (request will be sent with next animation frame)
	pause()
	{
		if (this.paused == false)
		{
			this.events_for_server.pause = true;
			this.pending_events_for_server = true;
		}
	}
	// Request server to concede (request will be sent with next animation frame)
	concede()
	{
		this.events_for_server.concede = true;
		this.pending_events_for_server = true;
	}

	// GAME MAINLOOP

	// Stores keyboard events (players trying to move their paddle)
	// to be analyzed when generating next frame
	store_key_event(event: KeyboardEvent, key_released: boolean)
	{
		var code = event.code;

		if (!this.running)
			return ;
		if(keys_by_player[0].includes(code) || keys_by_player[1].includes(code))
		{
			event.preventDefault();
			if (key_released)
				this.animation_events.push(code + "r"); // "r" for "released"
			else
				this.animation_events.push(code + "p"); // "p" for "pressed"
		}
	}

	// Prepares a new game frame :
	// 		- sends keyboard events that were received since last frame to be processed by paddle instances
	// 		- erases all drawings on the canvas
	// 		- updates ball and paddles
	// 		- calls itself again with <requestAnimationFrame> (as long as <running> is true)
	// (Method declaration with the syntax `funcname = (paraname) => {funcbody}`,
	//  because it seems to be the only way to avoid issues with call by `requestAnimationFrame`)
	update_game_state = (current_time: DOMHighResTimeStamp) =>
	{
		let delta_time: number;
		let ball_out_of_bounds: 0 | 1 | 2;
		let losing_player_index: 0 | 1;

		// Check if "pause" button was pressed since last frame
		if (!this.running || this.ball == null || this.canvas == null)
			return ;

		// Register current time
		this.frame_counter++;
		if (this.last_time == -1) // case where this frame is first frame after "launch" button was pressed
			delta_time = 16; // 16 milliseconds, delta between 2 frames on the usual 60 frames/second setup
		else
			delta_time = current_time - this.last_time;
		console.log(`--- Frame ${this.frame_counter}, current time ${current_time}, delta ${delta_time}`);
		this.last_time = current_time;

		// Go through keyboard events received since last frame
		for (let i = 0; i < this.animation_events.length; i++)
		{
			let code = this.animation_events[i];
			//console.log("- processing event : " + code);
			if (keys_by_player[0].includes(code.slice(0, code.length - 1)))
				this.paddles[0].process_keyevent(code);
			else if (keys_by_player[1].includes(code.slice(0, code.length - 1)))
				this.paddles[1].process_keyevent(code);
		}
		this.animation_events = []; // emptying event list

		// Update paddles and ball
		this.ctx?.clearRect(0, 0, this.canvas.width, this.canvas.height);
		this.paddles[0].update(delta_time, this.ball);
		this.paddles[1].update(delta_time, this.ball);
		ball_out_of_bounds = this.ball.update(delta_time);
		if (ball_out_of_bounds == 0 || ball_out_of_bounds == 1) // when ball went through left/right wall
		{
			losing_player_index = ball_out_of_bounds; // index of player who lost point
			this.points[1 - losing_player_index] += 1; // other player gains 1 point
			this.points_info!.innerHTML = "Points : " + this.points[0] + "-" + this.points[1];
			this.ball.reset(losing_player_index);
		}

		// Slightly adjust ball direction if it is too vertical (and away from paddles)
		if (!this.paddles[0].ball_in_range(this.ball) && !this.paddles[1].ball_in_range(this.ball))
			this.ball.adjust_direction();

		if (this.running)
			requestAnimationFrame(this.update_game_state);

		// #f reinitialiser une partie de events_for_server
	}

	// OTHER

	// Function for ad-hoc tests
	tests()
	{
		this.paddles[0].position.x = 50;
		this.paddles[0].position.y = 50;
		this.paddles[0].compute_side_attributes();
		this.ball!.position.x = 53;
		this.ball!.position.y = 52;
		this.ball!.radius = 8;
		let res = this.paddles[0].check_collision_with_ball(this.ball!);
		console.log(`Result of collision test : ${res}`);
	}
}

function setup()
{
	let game = new Pong_game();
	//game.tests();
}

window.onload = setup;
const ws = new WebSocket(`ws://${window.location.host}`);
let counter = 0;

ws.onopen = () => {
    setInterval(() => {
        ws.send("ping");
    }, 1000);
};
ws.onmessage = (event) => {
    console.log("Received:", event.data, counter);
	counter++;
};
ws.onclose = () => {
    console.log("Disconnected");
};



/*


+ tenir compte du temps passe depuis la derniere frame dans update_game_state
+ transformer la surface des paddles en demi-cercles
+ creer un petit mecanisme de rectification de la direction de la balle quand elle est trop verticale
+ teleporter la balle si son centre est a l'interieur d'un paddle

- placer la logique du jeu dans le serveur grace a des websockets

*/

/*
I am experimenting with typescript by creating a small game using an HTML canvas. I define a small <Point> class at the beginning of my typescript program :

Unexpectedly, when I try to run my program in my browser, I get an error "Uncaught SyntaxError: bad class member definition" on the line `x:number`.
I am very surprised, since I did not find any reference to that error online, and my Point class is exactly the same as in the typescript documentation example.

My specific use case is a small Node.JS websocket server. It would receive Websocket requests from clients, and store the created Websocket objects in an array. The index of the Websocket in that array would be the main way to identify the client in various functions.
So when the client disconnects, the Websocket closes, and my server should forget the Websocket object in the array. But it cannot simply remove it from the array, since it would modify the indexes of Websockets coming after it in the array. If the array acts like a dictionary, it may be possible to remove a key/value pair without displacing the rest. 
*/
