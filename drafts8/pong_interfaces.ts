// --- A module defining the structure of data-transmission objects for the Pong game,
// 	   shared between Pong client and server

// TEMPLATES

// Template for Message interfaces (<Server_message> and <Client_message>)
// Makes it easier to create the discriminated unions :
// 		instead of writing the interface for each different type to be united,
// 		the different types are defined as instantiations of the template
// 		directly on the line where the union is created
export interface Message<T extends string, D>
{
	id: T,
	data: D
}

// INTERFACES FOR WEBSOCKET COMMUNICATION : SERVER -> CLIENT

// "Discriminated union" of different types of server messages :
// all the united types have a common field (here, 'id') which can take a finite set of values
// 		-> when client receives the JSON string from the server,
// 		   it can `JSON.parse` it into a <Server_message> object,
// 		   then read the type and process the data accordingly without another `JSON.parse`
// 		-> TypeScript does not complain about accessing the wrong attributes of what is received,
//		   since the 'id' field is discriminating a particular type in the union,
// 		   and this 'id' field is used in a `switch() case` statement to access the correct attributes
export type Server_message = Message<"gid", Game_init_description>
							| Message<"gsd", Game_state_description>
							| Message<"gcd", Game_close_description>;

// Data about the current state of the game (server -> client), identified as 'gsd'
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
	status: "waiting" | "starting" | "active" | "paused" | "terminated"
}

// Data used to initialize a game (server -> client), identified as 'gid'
export interface Game_init_description
{
	player_index: number, // index of the recipient player
	broadcast_interval: number, // expected number of ms between server broadcasts
	dims: Dimensions // game components dimensions
}

// Data used to terminate a game (server -> client), identified as 'gcd'
export interface Game_close_description
{
	reason: "points" | "concession" | "disconnect" | "server_error", // how the game ended
	winner: number // index of winning player (-1 for a tie)
	points: [number, number],
}

// INTERFACES FOR WEBSOCKET COMMUNICATION : CLIENT -> SERVER

// "Discriminated union" of different types of client messages
// (useless for now since there is only 1 type of client message, but could be extended)
export type Client_message = Message<"cc", Client_commands>;

// Data about user commands (client -> server), identified as 'cc'
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

// Carrier of data about the dimensions of game components : canvas, paddles, ball
export interface Dimensions
{
	cwidth: number,
	cheight: number,
	pwidth: number,
	pheight: number,
	ball_radius: number
}


