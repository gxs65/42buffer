// --- A module defining the structure of data-transmission objects for the Pong game,
// 	   shared between Pong client and server

// INTERFACES FOR WEBSOCKET COMMUNICATION : CLIENT -> SERVER

// Data about user commands (client -> server)
// (if user is human, commands come from keys pressed / buttons clicked),
// 		-> will be sent to server through WS at each animation frame
export interface Client_commands
{
	// Booleans indicating whether the "up"/"down" key is currently pressed
	up: boolean,
	down: boolean,
	// Booleans indicating whether the "pause"/"play"/"concede" has been pressed since last send
	pause: boolean,
	play: boolean,
	concede: boolean
}

// INTERFACES FOR WEBSOCKET COMMUNICATION : SERVER -> CLIENT

// Template for Messages interface
export interface Message<T extends string, D>
{
	type: T,
	data: D,
}

// "Discriminate union" of different types of server messages :
// 		when client receives the JSON string from the server,
// 		it can `JSON.parse` it into a <Server_message> object,
// 		then read the type and process the data accordingly without another `JSON.parse`
export type Server_message = Message<"gid", Game_init_description>
							| Message<"gsd", Game_state_description>
							| Message<"gcd", Game_close_description>;

// Data about the current state of the game (server -> client)
export interface Game_state_description
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

// Data about the dimensions of game components : canvas, paddles, ball (server -> client)
export interface Dimensions
{
	cwidth: number,
	cheight: number,
	pwidth: number,
	pheight: number,
	ball_radius: number,
}

// Data used to initialize a game (server -> client)
export interface Game_init_description
{
	player_index: number, // index of the recipient player
	broadcast_interval: number,
	dims: Dimensions,
}

// Data used to terminate a game (server -> client)
export interface Game_close_description
{
	reason: "points" | "concession" | "disconnect";
	winner: number; // index of winning player
}

// OTHER INTERFACES

// 2-dimensional point class
export class Point {
	x: number;
	y: number;
	constructor(x = 0, y = 0) {
		this.x = x;
		this.y = y;
	}
}
