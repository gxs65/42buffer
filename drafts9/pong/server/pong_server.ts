// --- A Node.JS server organising Pong games through Websockets

/////////////
// IMPORTS //
/////////////

// Import external modules
import * as http from 'node:http'; // HTTP networking
import * as fs from 'node:fs'; // filesystem manipulation
import * as ws from 'ws'; // Websocket networking

// Import resources shared with client scripts
// 		- interfaces defining the structure of data going through websockets
// 				(interfaces are only for TS checks, so this import disappears after TS compilation)
// 		- classes and functions for geometry calculations
// 		- constants for base game values
import { Client_message, Client_commands, Server_message,
		 Game_close_description, Game_state_description, Game_init_description,
		 schema_client_message, schema_game_choice } from "../shared/pong_interfaces.js";

// Import classes for computations on game components, and utility functions
import { Ball, Paddle } from "./pong_game_classes.js";
import { ws_safe_send_server } from "../shared/pong_utils.js";

// Lines asking TS to load types for the modules,
// disappear in the JS code post transpilation with `tsc`
import type * as httpt from "node:http";
import type * as wst from "ws";

///////////
// UTILS //
///////////

// CONSTANTS

// Base dimensions (can be modified)
const playground_width = 300;
const playground_height = 150;
const paddle_margin = 10;
const paddle_width = 10;
const paddle_height = 40;
const ball_radius = 4;

// Speed / acceleration / surfaces (for now, can't be modified)
const paddle_speed = 4;
const ball_initial_speed = 0.8;

// Tick intervals  (for now, can't be modified)
const simulation_tick_interval = 10; // ms between each game state update by server
const broadcast_tick_interval = 50; // ms between each game state broadcast to clients by server

// INTERFACES

interface ws_identifier
{
	socket: wst.WebSocket | null; // websocket instance
	ws_key: number; // key identifying the user connected through this websocket
	game_key: number; // key identifying the game to which this player participates
	player_index: number; // player index in the game
}

//////////////////
// GAME OF PONG //
//////////////////

// Class managing a Pong game by
// 		- creating the Ball and Paddle instances whose methods do the calculations
// 		- running the game loop
// 		- sending and receiving messages from clients through websockets
export class Pong_game
{
	// State with respect to websocket manager
	manager: Pong_manager; // reference to the manager that created this instance of <Pong_game>
	status: "waiting" | "starting" | "active" | "paused" | "terminated";
	id: number;
	player_ws_ids: Array<ws_identifier> = [];
	players_input: Array<Client_commands> = []; // 2 objects storing input received from players
	// Game state
	concession: number = -1; // index of a conceding player (-1 means no player conceded)
	last_time: number = -1;
	points: Array<number> = [0, 0];
	ball: Ball | null = null;
	paddles: Array<Paddle> = [];
	winner: number = -1;
	// Dimensions of playground
	playground_width: number = playground_width;
	playground_height: number = playground_height;
	// Other
	logintro: string = "";

	// GAME INIT

	// Constructor does not start the game (game becomes "starting" once there are 2 players in it)
	constructor(id: number, manager: Pong_manager)
	{
		this.manager = manager;
		this.status = "waiting";
		this.id = id;
		this.logintro = `[Game ${this.id}]`;
		console.log(`${this.logintro} New Pong Game with id ${id} created`);
	}

	// Called when a client connected by websocket to enter a game,
	// registers the client's websocket identity
	// and starts the game if there are now 2 players in the game
	add_player_websocket(ws_id: ws_identifier)
	{
		if (this.status != "waiting") // no player may be added if game is already full
			return ;
		this.player_ws_ids.push(ws_id);
		if (this.player_ws_ids.length == 2)
		{
			this.status = "starting";
			this.setup_game();
			this.send_init_message();
			setTimeout(() => this.simulation_tick(), 1000); // 1s delay before game start
			setTimeout(() => this.broadcast_tick(), 1000);
		}
	}

	// Sends setup information to the players
	// (must replicate the code of <broadcast_to_players> instead of using it,
	//  because the message is different for each player : it includes the player's index)
	send_init_message()
	{
		let message: Server_message;
		let message_string: string;
		let player: number;
		let socket: ws.WebSocket | null;

		message = {
			id: "gid",
			data: {
				player_index: -1, // different according to player
				broadcast_interval: broadcast_tick_interval,
				dims: {
					cwidth: this.playground_width, cheight: this.playground_height,
					pwidth: this.paddles[0].width, pheight: this.paddles[0].height,
					ball_radius: this.ball!.radius
				}
			}
		};
		for (player = 0; player < 2; player++)
		{
			message.data.player_index = player;
			message_string = JSON.stringify(message);
			socket = this.player_ws_ids[player].socket;
			if (socket != null && ws_safe_send_server(socket, message_string))
			{
				this.end_by_disconnect(player); // in case of sending error
				break; // #f : should this really be considered a disconnect and not a server error ?
			}
		}
	}

	// Instantiates paddles and ball objects, which will make position and movement calculations,
	// and initializes all client commands to false for the two players
	setup_game()
	{
		this.ball = new Ball(this, simulation_tick_interval,
			this.playground_width / 2, this.playground_height / 2, ball_radius, ball_initial_speed);
		this.paddles.push(new Paddle(this, 0, simulation_tick_interval,
			paddle_margin, this.playground_height / 2,
			paddle_width, paddle_height, paddle_speed));
		this.paddles.push(new Paddle(this, 1, simulation_tick_interval,
			this.playground_width - paddle_margin, this.playground_height / 2,
			paddle_width, paddle_height, paddle_speed));
		this.players_input.push({up: false, down: false, play: false, pause: false, concede: false});
		this.players_input.push({up: false, down: false, play: false, pause: false, concede: false});
	}

	// GAME END

	// Ending method 1 : player disconnect
	// Called by <Pong_manager> when the Websocket of a player in the game becomes closed
	// 		\ if the game had not started <=> had 1 player waiting for opponent, nothing happens
	// 			except notifying <Pong_manager> that this game instance can be erased from memory 
	// 		\ if the game had started, remaining player is declared winner
	end_by_disconnect(disconnected_player_index: number)
	{
		let counterpart_index: number = 1 - disconnected_player_index;
	
		if (this.status == "terminated")
			return ;
		if (this.status == "waiting")
		{
			console.log(`${this.logintro} player disconnect on waiting game`);
			this.status = "terminated";
			this.manager.waiting_game_instance_terminated(this.player_ws_ids[0]);
		}
		else
		{
			console.log(`${this.logintro} player disconnect, victory to ${counterpart_index}`);
			this.winner = counterpart_index;
			this.end("disconnect");
		}
	}

	// Ending method 2 : player win (through points or concession)
	// Called by <Pong_game.simulation_tick> when a player reaches 10 points or concedes,
	// terminates the game and sends the concluding broadcast to players
	end_by_points_or_concession()
	{
		let reason: "points" | "concession";

		if (this.status == "terminated")
			return ;
		if (this.concession != -1)
		{
			reason = "concession";
			this.winner = 1 - this.concession;
		}
		else
		{
			reason = "points";
			this.determine_winner();
		}
		console.log(`${this.logintro} end game reached through ${reason}`);
		this.end(reason);
	}

	// Ending method 3 : server error
	// Called by <Pong_game> methods when they encounter an error, eg. on message parsing
	end_by_error()
	{
		if (this.status == "terminated")
			return ;
		console.log(`${this.logintro} game instance error, terminating game with tie`);
		this.winner = -1;
		this.end("server_error");
	}

	// Sends final broadcast to players
	// and notifies overlying <Pong_manager> that this game instance can be erased from memory
	end(reason: "points" | "concession" | "disconnect" | "server_error")
	{
		this.status = "terminated";
		this.send_close_message(reason);
		this.manager.game_instance_terminated(this.player_ws_ids[0], this.player_ws_ids[1]);
	}
	
	// Determines the game's winner (0, 1, or -1 for 'tied')
	// when the game ended by a player reaching the required amount of points
	determine_winner()
	{
		if (this.points[0] > this.points[1])
			this.winner = 0;
		else if (this.points[1] > this.points[0])
			this.winner = 1;
		else
			this.winner = -1;
	}

	// Composes and broadcasts the closing message
	// /!\ if the game was ended by a player disconnection,
	// 	   then the message gets broadcasted by <broadcast_to_players> only to the remaining player	
	// 	   (the other player's websocket is closed)
	send_close_message(reason: "points" | "concession" | "disconnect" | "server_error")
	{
		let message: Server_message = {
			id: "gcd",
			data: {
				reason: reason,
				winner: this.winner,
				points: [this.points[0], this.points[1]]
			}
		}

		this.broadcast_to_players(message);
	}

	// COMMUNICATION WITH CLIENTS

	// Receives the user commands as a JSON string sent by a player through websockets
	new_player_input(msg: wst.RawData, player: number)
	{
		let input: Client_message;

		try
		{
			input = JSON.parse(msg.toString());
			schema_client_message.parse(input);
			switch (input.id)
			{
				case "cc":
					this.handle_player_commands(input.data, player);
					break;
			}
		}
		catch // could be an error on `JSON.parse` or an error on Zod validation `schema_xxx.parse`
		{
			console.log(`${this.logintro} error on parsing/validation, disregarding message`);
			// #f : actions to take in case of validation failure
		}
	}

	// Sends a message to players, as a JSON string through a websocket
	broadcast_to_players(message: Server_message)
	{
		let player: number;
		let message_string: string;
		let socket: ws.WebSocket | null;

		message_string = JSON.stringify(message);
		for (player = 0; player < 2; player++)
		{
			socket = this.player_ws_ids[player].socket;
			if (socket != null && ws_safe_send_server(socket, message_string))
			{
				this.end_by_disconnect(player); // in case of sending error
				break; // #f : should this really be considered a disconnect and not a server error ?
			}
		}
	}

	// GAME MAINLOOP : RECEIVE COMMANDS FROM PLAYERS, SIMULATE AND BROADCAST GAME STATE

	// When a client sends new commands, processes them
	// 		- checks if the user wants to pause / resume / concede the game
	// 			(because this can be done immediately)
	// 		- stores the input about pressed keys ('up' or 'down')
	// 			(because this must wait to be processed in <simulation_tick>)
	// /!\ for now all players can unilaterally pause/play the game, this could be improved
	handle_player_commands(commands: Client_commands, player: number)
	{
		if (this.concession == -1 && commands.concede)
			this.concession = player;
		if (this.status == "active" && commands.pause)
			this.status = "paused";
		if (this.status == "paused" && commands.play)
			this.status = "active";
		this.players_input[player] = commands;
	}

	// Broadcast loop : sends the current game state to all players at a certain interval
	// (default broadcast interval : 50ms)
	broadcast_tick()
	{
		if (this.status == "terminated")  // when game is terminated, all loops should stop
			return ;

		let message: Server_message = {
			id: "gsd",
			data: {
				ball_pos: {x: this.ball!.position.x, y: this.ball!.position.y},
				paddles_pos: [
					{x: this.paddles[0].position.x, y: this.paddles[0].position.y},
					{x: this.paddles[1].position.x, y: this.paddles[1].position.y},
				],
				points: [this.points[0], this.points[1]],
				status: this.status
			}
		};

		//console.log(`${this.logintro} Broadcast tick`);
		this.broadcast_to_players(message);
		setTimeout(() => this.broadcast_tick(), broadcast_tick_interval);
	}

	// Simulation loop : updates the game state at a certain interval, and checks if game should end
	// (default simulation interval : 10ms)
	simulation_tick()
	{
		let delta_time: number;
		let current_time = performance.now();

		if (this.status == "terminated") // when game is terminated, all loops should stop
			return ;
		if (this.status == "starting") // first game tick
			this.status = "active";

		// (0) register current time
		if (this.last_time == -1) // case where this is the first call of <game_mainloop>
			delta_time = 10;
		else
			delta_time = current_time - this.last_time;
		//console.log(`${this.logintro} Simulation tick with delta ${delta_time}, game status ${this.status}`);
		this.last_time = current_time;

		// (1) check if a player conceded
		if (this.concession != -1)
			this.end_by_points_or_concession();

		// (2) update positions if game is not paused, then check for game end condition
		if (this.status == "active")
			this.update_ball_and_paddles(delta_time);
		if (this.points[0] >= 10 || this.points[1] >= 10)
			this.end_by_points_or_concession();

		// (3) call itself in 10ms
		setTimeout(() => this.simulation_tick(), simulation_tick_interval);
	}

	// Updates game components using the game components instances
	// 		\ forwards the user commands to the instances
	// 		\ lets the instances do the 'physics' calculations by calling their 'update' methods
	// 		\ checks if a player lost a point using the return value of <ball.update>
	update_ball_and_paddles(delta_time: number)
	{
		let ball_out_of_bounds: 0 | 1 | 2;
		let losing_player_index: 0 | 1;

		// Let paddle instances register if they must move (up or down)
		if (!this.ball || !this.paddles[0] || !this.paddles[1]) // #f : handle error
			return ;
		this.paddles[0].process_keys(this.players_input[0]); // #f : meilleur nom pour cette methode
		this.paddles[1].process_keys(this.players_input[1]);

		// Let paddle and ball instances move
		this.paddles[0].update(delta_time, this.ball);
		this.paddles[1].update(delta_time, this.ball);
		ball_out_of_bounds = this.ball.update(delta_time);

		// Check if ball went went through left/right wall <=> a player loses a point
		if (ball_out_of_bounds == 0 || ball_out_of_bounds == 1)
		{
			losing_player_index = ball_out_of_bounds; // index of player who lost point
			this.points[1 - losing_player_index] += 1; // other player gains 1 point
			this.ball.reset(losing_player_index);
		}

		// Slightly adjust ball trajectory if it is too vertical (and away from paddles)
		if (!this.paddles[0].ball_in_range(this.ball) && !this.paddles[1].ball_in_range(this.ball))
			this.ball.adjust_direction();
	}
}

//////////////////
// PONG MANAGER //
//////////////////

// Manager handling the Pong real-time game :
// 		- receives websocket requests from Pong players
// 		- manages all open websockets for Pong players
// 		- creates <Pong_game> instances to run games
// 		- erases <Pong_game> instances when games are terminated
class Pong_manager
{
	server: Transcendance_server;
	waiting_game: Pong_game | null = null; // the Pong game currently waiting for additional players
	games: Map<number, Pong_game> = new Map(); // map containing all ongoing games
	open_websockets: Map<number, ws_identifier> = new Map(); // map containing all currently active websockets
	created_games: number = 0; // counter used to make unique keys for <games>
	created_websockets: number = 0; // counter used to make unique keys for <open_websockets>

	constructor(server: Transcendance_server)
	{
		this.server = server;
	}

	// PLAYER ASSIGNMENT

	// Called when a new player creates a websocket to the server and demands to play Pong :
	// the server devolves the new player's websocket to the Pong manager as <ws_id>, then
	// 		(1) player is assigned to a waiting game, or a new waiting game is created for them
	// 		(2) webSocket event handling is redefined
	new_player(ws_id: ws_identifier)
	{
		ws_id.ws_key = this.created_websockets;
		this.created_websockets++;
		this.open_websockets.set(ws_id.ws_key, ws_id);
		console.log(`[PONGmanager] New player at key ${ws_id.ws_key}`);
		if (this.waiting_game == null) // there is no open game waiting for a player -> create a new game
		{
			ws_id.player_index = 0;
			ws_id.game_key = this.created_games;
			this.waiting_game = new Pong_game(this.created_games, this);
			this.waiting_game.add_player_websocket(ws_id);
			console.log(`[PONGmanager] -> created new Pong Game with key ${ws_id.game_key}`);
		}
		else // there is an open game waiting for a player -> add player to this game (will trigger game start)
		{
			ws_id.player_index = 1;
			ws_id.game_key = this.created_games;
			this.created_games++;
			this.games.set(ws_id.game_key, this.waiting_game);
			this.waiting_game.add_player_websocket(ws_id);
			this.waiting_game = null;
			console.log(`[PONGmanager] -> added client to game ${ws_id.game_key}, game will start`);
		}
		
		console.log(`[PONGmanager] ws_id of new player : wskey ${ws_id.ws_key}, gkey ${ws_id.game_key}, pind ${ws_id.player_index}`);
		ws_id.socket!.on("message", (msg: wst.RawData) => {this.ws_handle_data(msg, ws_id)});
		ws_id.socket!.on("close", () => {this.ws_handle_conn_closed(ws_id)});
	}

	// WEBSOCKET MANAGEMENT

	// Transfers the data received on a websocket to the <Pong_game> instance responsible for that client
	ws_handle_data(msg: wst.RawData, ws_id: ws_identifier)
	{
		let game: Pong_game | undefined;

        console.log(`[PONGmanager] Received data on ws ${ws_id.ws_key}, part of game ${ws_id.game_key}`);
		game = this.games.get(ws_id.game_key);
		if (game === undefined) // data received for a game already erased from memory
			return ;
		game.new_player_input(msg, ws_id.player_index);
	}

	// Handles a close event on a websocket connecting to a player
	// /!\ triggers also when server side closes the websocket
	// 	   -> actions must be taken only if the game to which the player participated is not terminated,
	// 	      in that case the websocket close means the player disconnected and the game must end
	// 	      by calling the ending method of the game instance
	ws_handle_conn_closed(ws_id: ws_identifier)
	{
		let game: Pong_game | undefined;

		console.log(`[PONGmanager] Connection closed on ws ${ws_id.ws_key}, part of game ${ws_id.game_key}`);
		if (this.waiting_game != null && ws_id.game_key == this.waiting_game.id) // special case : waiting player disconnected
		{
			this.waiting_game.end_by_disconnect(ws_id.player_index);
			return ;
		}
		game = this.games.get(ws_id.game_key);
		if (game === undefined) // this game has already been terminated and erased from memory
			return ;
		else if (game.status != "terminated")
			game.end_by_disconnect(ws_id.player_index);
	}

	// Closes a websocket if it is open
	close_websocket(ws_id: ws_identifier)
	{
		if (ws_id.socket != null && ws_id.socket.readyState != ws_id.socket.CLOSED)
			ws_id.socket.close();
	}

	// HANDLING OF TERMINATED GAME

	// Called by a methods of <Pong_game> child instance to notify that it ended,
	//  	- closes websockets that are still open
	// 		- erases game instance and websocket from memory (maps where they were stored)
	game_instance_terminated(ws_id1: ws_identifier, ws_id2: ws_identifier)
	{
		console.log(`[PONGmanager] Pong game id ${ws_id1.game_key} notified end, erasing`);
		this.close_websocket(ws_id1);
		this.close_websocket(ws_id2);
		this.open_websockets.delete(ws_id1.ws_key);
		this.open_websockets.delete(ws_id2.ws_key);
		this.games.delete(ws_id1.game_key);
	}

	// Called by the <waiting_game> child instance when its waiting player disconnected,
	// so the waiting game can be erased (no need to close websocket, since the player disconnected)
	waiting_game_instance_terminated(ws_id1: ws_identifier)
	{
		console.log(`[PONGmanager] Waiting Pong game id ${ws_id1.game_key} can be erased`);
		this.open_websockets.delete(ws_id1.ws_key);
		this.waiting_game = null;
	}
}

/////////////////
// HTTP SERVER //
/////////////////

// Server with 2 roles :
// 		- serve normal HTTP requests for the site's content
// 		- receive WS (websocket) requests meaning that a client wants to play a game,
// 		  and devolve them to the game server handling the game they want to play
class Transcendance_server
{
	hostname: string;
	port: number;
	server: httpt.Server;
	ws_server: wst.WebSocketServer;
	pong_manager: Pong_manager;
	open_websockets: Map<number, ws_identifier> = new Map(); // map containing all currently active websockets
	created_websockets: number = 0; // counter used to make unique keys for <open_websockets>

	// Constructor creates :
	// 		- the networking servers : HTTP server and websocket server
	// 			(the websocket server is "attached" to the HTTP server,
	//  		 since to start the websocket protocol the client sends a special HTTP request)
	// 		- the game managers that will handle Pong and Chess games
	constructor(hostname:string, port: number)
	{
		this.hostname = hostname;
		this.port = port;
		this.server = http.createServer(
			(req: httpt.IncomingMessage, res: httpt.ServerResponse) => this.request_handler(req, res));
		this.ws_server = new ws.WebSocketServer({server: this.server});
		this.ws_server.on("connection",
			(ws: wst.WebSocket) => this.new_player(ws));
		this.server.listen(port, hostname,
			() => {console.log(`### Node.JS server at http://${hostname}:${port}/ ###`);});
		this.pong_manager = new Pong_manager(this);
	}

	// HTTP SERVER FEATURES

	// Handles incoming HTTP request :
	// 		- serves files whem request method is GET
	// 		- ignores websocket requests (with an 'upgrade' header) to be managed by the WS server
	// /!\ the '.' directory is the CWD (where 'node' command has been run),
	// 	   not the directory where this file lies
	// #f : very primal version to be replaced with the true server
	async request_handler(req: httpt.IncomingMessage, res: httpt.ServerResponse)
	{
		console.log(`[HTTPserv] Received new request : ${req.method}, ${req.url}`);

		if (!("upgrade" in req.headers && req.headers["upgrade"] == "websocket") && req.method == "GET")
		{
			switch (req.url)
			{
				case "/shared/pong_utils.js":
					await this.create_response("./pong/shared/pong_utils.js", "text/javascript", res);
					break;
				case "/pong_client.js":
					await this.create_response("./pong/client/pong_client.js", "text/javascript", res);
					break;
				case "/pong.html":
					await this.create_response("./pong/client/pong.html", "text/html", res);
					break;
				default:
				{
					console.log("[HTTPserv] failed to find requested content")
					res.statusCode = 404;
					res.end("Not Found");
				}
			}
		}
		else
		{
			console.log("[HTTPserv] -> websocket request, handled by websocket server");
		}
	}

	// Prepares an HTTP response containing the content of file at <path> in its body
	async create_response(path: string, mime_type: string, res: httpt.ServerResponse)
	{
		fs.readFile(path, 'utf8', (err: Error | null, data: string) => {
			if (err)
			{
				console.log("[HTTPserv] -> error reading file");
				console.error(err);
				res.statusCode = 500;
				res.end("Internal server error");
			}
			else
			{
				console.log("[HTTPserv] -> serving site content");
				res.statusCode = 200;
				res.setHeader("Content-Type", mime_type);
				res.end(data);
			}
		});
	}

	// WEBSOCKET SERVER FEATURES

	// Function called when a client connects to the websocket server to start playing a game
	// 		- registers the player and the websocket in the map <open_websockets>
	// 		- sets up event handling to wait for the first message sent through websocket by the client,
	// 			which will indicate which game (Pong or Chess) the client wants to play
	// 				-> events handlers attached with `ws.once` so that they are called only once
	new_player(ws: wst.WebSocket)
	{
		let ws_id: ws_identifier = {socket: null, ws_key: 0, game_key: 0, player_index: 0};

		if (this.open_websockets.size >= 50) // no more than 50 simultaneous players
		{
			ws.close();
			return ;
		}

		ws_id.socket = ws;
		ws_id.ws_key = this.created_websockets;
		this.created_websockets++;
		this.open_websockets.set(ws_id.ws_key, ws_id);
		console.log(`[WSserv] New player on websocket on ws at key ${ws_id.ws_key}`);
		ws.once("message", (msg: wst.RawData) => {this.ws_devolve_player(msg, ws_id)});
		ws.once("close", () => {this.ws_handle_conn_closed(ws_id)});
	}

	// Called when a new player sends their first message through websocket
	// 		- validates and reads message to devolve player to their chosen game
	// 		- if player did not send a correct game choice message, disconnects from them
	// 		- in any case, deactivates event handlers and removes this connection from <open_websockets>
	// 			(it is now either a closed connection, or handled by one of the game managers)
	ws_devolve_player(msg: wst.RawData, ws_id: ws_identifier)
	{
		let input: Client_message;

		this.open_websockets.delete(ws_id.ws_key)
		try
		{
			input = JSON.parse(msg.toString());
			schema_client_message.parse(input);
			switch (input.id)
			{
				case "cc":
					console.log("[WSserv] Error on new player : commands message received before game choice");
					this.close_websocket(ws_id);
					break;
				case "gc":
					console.log(`[WSserv] New player ${ws_id.ws_key} devolved to ${input.data.game}`);
					this.pong_manager.new_player(ws_id);
					break;
			}
		}
		catch // could be an error on `JSON.parse` or an error on Zod validation `schema_xxx.parse`
		{
			console.log(`[WSserv] error on parsing/validation, disconnecting from new player`);
			this.close_websocket(ws_id);
		}
	}

	// Handles a close event on a websocket connecting to a player
	// /!\ since the server only manages the websocket very briefly
	// 	   (until it receives client's first message),
	// 	   this method should never be called
	ws_handle_conn_closed(ws_id: ws_identifier)
	{
		this.open_websockets.delete(ws_id.ws_key);
	}

	close_websocket(ws_id: ws_identifier)
	{
		if (ws_id.socket != null && ws_id.socket.readyState != ws_id.socket.CLOSED)
			ws_id.socket.close();
	}
}

let pong_server = new Transcendance_server('127.0.0.1', 8080);

/* TODO

+ souci de double close sur les sockets d'une partie qui se termine -> flag interne a Game pour partie terminee
+ impossible de splice-remove dans la liste des WS / des Games, puisque les index sont en dur dans les ws_id
	-> plutot utiliser une map avec des keys qu'on peut ajouter/retirer tranquillement
+ ameliorations souhaitables :
	+ decouplage ticks de simulation / ticks de broadcast
	+ laisser le client faire l'interpolation dans des animframes

+ protection de la communication Websocket : try-catch autour du JSON.parse
+ retester le redressement de trajectoire de la balle
+ au lieu d'avoir plusieurs booleens, utiliser une seule variable de status de la partie :
	waiting | starting | active | paused | terminated
+ standardiser la communication client-serveur
	+ puisque plusieurs types de data envoyee du serveur au client, utiliser une union discriminante
	+ trouver une solution pas trop moche au probleme de l'envoi de messages differents aux deux joueurs (player_index)
		-> la solution est moche, on refait la boucle d'envoi aux joueurs, tant pis
	+ etendre principe de l'union discriminante aux messages envoyes par le client
	+ ajouter le nouveau total de points dans <Game_end_description>
+ definir la structure propre de creation et de fermeture des parties
	+ quand Pong_game rencontre une erreur :
		<Pong_game.end_by_error> -> <Pong_server.game_instance_terminated> -> <Pong_server.erase_game>
	+ quand un client quitte precocement (websocket closed) :
		<Pong_server.ws_handle_conn_closed> -> <Pong_game.end_by_disconnect>
		-> <Pong_server.game_instance_terminated> -> <Pong_server.erase_game>
	+ quand un client concede (fait partie des commandes du client) :
		<Pong_game.end_by_points_or_concession> -> <Pong_server.game_instance_terminated> -> <Pong_server.erase_game>
	+ quand la partie se termine (premier a 10 points) :
		<Pong_game.end_by_points_or_concession> -> <Pong_server.game_instance_terminated> -> <Pong_server.erase_game>
+ creer une fonction safesend qui check qu'un websocket est pret a envoyer quelque chose :
	verification du websocket.readystate + try catch pour une eventuelle exception
		avec utilisation de la valeur de retour pour determiner s'il y a eu deco

+ revoir les contenus des fichiers :
	+ placer les constantes et interfaces dans un fichier JSON qui sera partage par serveur et client
	+ enlever (dans le code server et le code client) les references hard-codees aux constantes
	+ faire une vraie structure avec des repertoires Pong [Serveur, Client, Partage]
+ tester capacite a gerer plusieurs parties en meme temps
	+ permettre plusieurs parties (actuellement seulement 2 WS simultanes autorises)
	+ joli log qui donne les indices des parties en cours
- utilisation d'outils externes :
	+ utiliser des bibliotheques pour valider la forme de l'objet parse (zod)
	\ utiliser un bundler (Vite) pour creer un seul fichier avec toutes les dependances a envoyer au client
	\ utiliser un framework pour le protocole HTTP plutot que de devoir refaire webserv
+ separer le serveur en deux classes : 1 pour le service HTTP/WS general, 1 pour gerer les games de Pong
	(il y aura aussi une classe de serveur pour les games de chess)
- securisation du site
	\ servir les pages en HTTPS plutot que HTTP
	\ utiliser des websockets securises 'wss'
	\ verifier que les clients ne sont pas mal intentionnes (messages trop longs / frequents)
- divers :
	\ revoir la gestion des erreurs et ce qu'elles impliquent au niveau serveur / game / client
		(que faire quand server rencontre une erreur fatale ?)

*/

