// --- A module defining the structure of data-transmission objects for the Pong game,
// 	   shared between Pong client and server

// Previous versions used only TypeScript for static type checking,
// and thus defined only interfaces (see end of this file)
// Current version uses external tool Zod used for runtime validation
// of data received through websockets :
// 		- the lines `export const schema_xxx = z.xxx()` define Zod schema objects
// 		  with various constraints on the types and values of the members
// 		  	(eg. `z.number().max(5)` for a member that must be a number and less than 5)
//  	- at compile time : instructions `export type xxx = z.infer<typeof schema_xxx>`
// 		  let TypeScript infer the type `xxx` from the static type of the Zod schema objects
//	 		(since it's compile time, the `schema_xxx`object does not exist, but its type does exist)
// 		- at runtime : validation through method `schema_xxx.parse(object_that_should_be_xxx)`
// 			(since it's runtime, the `schema_xxx` objects exist and we use their methods)
// /!\ Runtime validation is applied only to 'client-->server' messages, not on the other direction
// 	   so we could have used only interfaces and defined no Zod schema for 'server-->client' messages
// 			but we still defined Zod schemas for all messages to harmonize presentation

import * as z from "zod";

// UTILITY

// 2-dimensional point class
export class Point {
	x: number;
	y: number;
	constructor(x = 0, y = 0) {
		this.x = x;
		this.y = y;
	}
}

export const schema_position = z.object({
	x: z.number(),
	y: z.number()
});

export const schema_dimensions = z.object({
	cwidth: z.number(),
	cheight: z.number(),
	pwidth: z.number(),
	pheight: z.number(),
	ball_radius: z.number()
});
export type Dimensions = z.infer<typeof schema_dimensions>;

// WEBSOCKET COMMUNICATION - SERVER --> CLIENT

// Data about the current state of the game (server --> client), identified as 'gsd'
export const schema_gsd = z.object({
	ball_pos: schema_position,
	paddles_pos: z.array(schema_position),
	points: z.array(z.number()),
	status: z.literal(["waiting", "starting", "active", "paused", "terminated"])
});
export type Game_state_description = z.infer<typeof schema_gsd>;

// Data used to initialize a game (server --> client), identified as 'gid'
export const schema_gid = z.object({
	player_index: z.number(), // index of the recipient player
	broadcast_interval: z.number(), // expected number of ms between server broadcasts
	dims: schema_dimensions // game components dimensions
});
export type Game_init_description = z.infer<typeof schema_gid>;


// Data used to terminate a game (server --> client), identified as 'gcd'
export const schema_gcd = z.object({
	reason: z.literal(["points", "concession", "disconnect", "server_error"]), // how the game ended
	winner: z.number(), // index of winning player (-1 for a tie)
	points: z.array(z.number())
});
export type Game_close_description = z.infer<typeof schema_gcd>;

// "Discriminated union" of different types of server messages :
// all the united types have a common field (here, 'id') which can take a finite set of values
// 		-> when client receives the JSON string from the server,
// 		   it can `JSON.parse` it into a <Server_message> object,
// 		   then read the type and process the data accordingly without another `JSON.parse`
// 		-> TypeScript does not complain about accessing the wrong attributes of what is received,
//		   since the 'id' field is discriminating a particular type in the union,
// 		   and this 'id' field is used in a `switch() case` statement to access the correct attributes
export const schema_server_message = z.discriminatedUnion("id", [
	z.object({
		id: z.literal("gid"),
		data: schema_gid
	}),
	z.object({
		id: z.literal("gsd"),
		data: schema_gsd
	}),
	z.object({
		id: z.literal("gcd"),
		data: schema_gcd
	})
]);
export type Server_message = z.infer<typeof schema_server_message>;

// WEBSOCKET COMMUNICATION - CLIENT --> SERVER

// Data about user commands (client --> server), identified as 'cc'
// (if user is human, commands come from keys pressed / buttons clicked),
export const schema_client_commands = z.object({
	// Booleans indicating whether the "up"/"down" key is currently pressed
	up: z.boolean(),
	down: z.boolean(),
	// Booleans indicating whether the "pause"/"play"/"concede" has been pressed since last send
	pause: z.boolean(),
	play: z.boolean(),
	concede: z.boolean()
});
export type Client_commands = z.infer<typeof schema_client_commands>;

// Data whether player wants to play Pong or Chess (client --> server), identified as 'gc'
export const schema_game_choice = z.object({
	game: z.literal(["pong", "chess"])
});
export type Game_choice = z.infer<typeof schema_game_choice>;

// "Discriminated union" of different types of client messages
export const schema_client_message = z.discriminatedUnion("id", [
	z.object({
		id:z.literal("cc"),
		data: schema_client_commands
	}),
	z.object({
		id:z.literal("gc"),
		data: schema_game_choice
	})
]);
export type Client_message = z.infer<typeof schema_client_message>;






/* PREVIOUS VERSION - WITHOUT ZOD DATA VALIDATION

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
	winner: number, // index of winning player (-1 for a tie)
	points: [number, number]
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
*/

