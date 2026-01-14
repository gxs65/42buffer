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
import { Client_commands, Server_message,
		 Game_close_description, Game_state_description, Game_init_description } from "./pong_interfaces.js";

// Import classes for computations on game components
import { Ball, Paddle } from "./pong_game_classes.js";

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
	open: boolean;
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
	// State with respect to websocket server
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

	// GAME INIT

	// Constructor does not start the game (game becomes "starting" once there are 2 players in it)
	constructor(id: number)
	{
		console.log(`New Pong Game with id ${id} created`);
		this.status = "waiting";
		this.id = id;
	}

	// Called when a client connected to websocket server to enter a game,
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
	send_init_message()
	{
		let message: Server_message = {
			type: "gid",
			data: {
				player_index: 0, // #f : different according to player
				broadcast_interval: broadcast_tick_interval,
				dims: {
					cwidth: this.playground_width, cheight: this.playground_height,
					pwidth: this.paddles[0].width, pheight: this.paddles[0].height,
					ball_radius: this.ball!.radius
				}
			}
		};
		
		this.broadcast_to_players(message);
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

	// GAME CLOSE

	// Called by the main game loop when a player reaches 10 points,
	// terminates the game and sends the concluding broadcast to players
	end_by_points()
	{
		this.determine_winner();
		this.status = "terminated";
		this.send_close_message("points");
		// #f : find way to notify server that this game is ended
	}

	// Called by <Pong_server> when the Websocket of a player in the game becomes closed
	// 		- sends the concluding broadcast to the remaining player
	// 		- returns the identifier of the websocket of the other player (the "counterpart")
	end_by_disconnect(disconnected_player_index: number) : ws_identifier
	{
		let counterpart_index: number = 1 - disconnected_player_index;

		this.determine_winner();
		this.status = "terminated";
		this.send_close_message("disconnect");
		return (this.player_ws_ids[counterpart_index]);
	}
	
	// Determines the game's winner (0, 1, or -1 for 'tied')
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
	send_close_message(reason: "points" | "concession" | "disconnect")
	{
		let message: Server_message = {
			type: "gcd",
			data: {
				reason: reason,
				winner: this.winner
			}
		}

		this.broadcast_to_players(message);
	}

	// COMMUNICATION WITH CLIENTS

	// Receives the user commands as a JSON string sent by a player through websockets
	// 		- checks if the user wants to pause / resume / concede the game
	// 			(because this can be done immediately)
	// 		- stores the input about pressed keys ('up' or 'down')
	// 			(because this must wait to be processed in <simulation_tick>)
	new_player_input(msg: wst.RawData, player: number)
	{
		let input: Client_commands;

		try
		{
			input = JSON.parse(msg.toString());
			if (this.concession == -1 && input.concede)
				this.concession = player;
			if (this.status == "active" && input.pause)
				this.status = "paused";
			if (this.status == "paused" && input.play)
				this.status = "active";
			this.players_input[player] = input;
		}
		catch
		{
			console.log(`Pong game ${this.player_ws_ids[0].game_key} - error on validation, disregarding message`);
			// #f : actions to take in case of validation failure
		}
	}

	// Sends a message to players, as a JSON string through a websocket
	broadcast_to_players(message: Server_message)
	{
		let player: number;
		let message_string: string;

		message_string = JSON.stringify(message);
		for (player = 0; player < 2; player++)
		{
			if (this.player_ws_ids[player].socket != null && this.player_ws_ids[player].open)
				this.player_ws_ids[player].socket!.send(message_string);
		}
	}

	// LOOPS : BROADCAST LOOP AND GAME MAINLOOP

	// #f : comment
	broadcast_tick()
	{
		if (this.status == "terminated")  // when game is terminated, all loops should stop
			return ;

		let message: Server_message = {
			type: "gsd",
			data: {
				ball_pos: {x: this.ball!.position.x, y: this.ball!.position.y},
				paddles_pos: [
					{x: this.paddles[0].position.x, y: this.paddles[0].position.y},
					{x: this.paddles[1].position.x, y: this.paddles[1].position.y},
				],
				points: [this.points[0], this.points[1]],
				active: (this.status == "active")
			}
		};

		console.log(`+++ Broadcast tick +++`);
		this.broadcast_to_players(message);
		setTimeout(() => this.broadcast_tick(), broadcast_tick_interval);
	}

	// #f : comment
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
		console.log(`--- Simulation tick with delta ${delta_time}, game status ${this.status}`);
		this.last_time = current_time;

		// (1) check if a player conceded
		if (this.concession != -1)
		{
			this.status = "terminated";
			// #f : cleanly terminate game
		}

		// (2) Update positions if game is not pausedm check for game end condition
		if (this.status == "active")
			this.update_ball_and_paddles(delta_time);
		if (this.points[0] >= 10 || this.points[1] >= 10)
			this.end_by_points();

		// (3) call itself in 10ms
		setTimeout(() => this.simulation_tick(), simulation_tick_interval);
	}

	update_ball_and_paddles(delta_time: number)
	{
		let ball_out_of_bounds: 0 | 1 | 2;
		let losing_player_index: 0 | 1;

		// Let paddle instances register if they must move (up or down)
		if (!this.ball || !this.paddles[0] || !this.paddles[1]) // TMP (errors)
			return ;
		this.paddles[0].process_keys(this.players_input[0]);
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

//////////////////////////
// SERVER (HTTP AND WS) //
//////////////////////////

// Server with 2 roles :
// 		- serve normal HTTP requests for the site's content
// 		- receive WS (websocket) requests meaning that a client wants to play a game,
// 		  and create <Pong_game> objects that will make 2 players play together
class Pong_server
{
	hostname: string;
	port: number;
	server: httpt.Server;
	ws_server: wst.WebSocketServer;

	waiting_game: Pong_game | null = null; // the Pong game currently waiting for additional players
	games: Map<number, Pong_game> = new Map(); // map containing all ongoing games
	open_websockets: Map<number, wst.WebSocket> = new Map(); // map containing all currently active websockets
	created_games: number = 0; // counter used to make unique keys for <games>
	created_websockets: number = 0; // counter used to make unique keys for <open_websockets>

	constructor(hostname:string, port: number)
	{
		this.hostname = hostname;
		this.port = port;
		this.server = http.createServer((req: httpt.IncomingMessage, res: httpt.ServerResponse) => this.request_handler(req, res));
		this.ws_server = new ws.WebSocketServer({server: this.server}); // new WebSocket Server "attached" to general HTTP server
		this.ws_server.on("connection", (ws: wst.WebSocket) => this.setup_websocket(ws));
		this.server.listen(port, hostname, () => {console.log(`Node.JS server at http://${hostname}:${port}/`);});
	}

	// HTTP SERVER FEATURES

	// Handles incoming HTTP request :
	// 		- serves files whem request method is GET
	// 		- ignores websocket requests (with an 'upgrade' header) to be managed by the WS server
	async request_handler(req: httpt.IncomingMessage, res: httpt.ServerResponse)
	{
		console.log(`Received new request : ${req.method}, ${req.url}`);

		if (!("upgrade" in req.headers && req.headers["upgrade"] == "websocket") && req.method == "GET")
		{
			console.log("-> serving site content");
			switch (req.url)
			{
				case "/pong_utils.js":
					await this.create_response("./pong_utils.js", "text/javascript", res);
					break;
				case "/pong_client.js":
					await this.create_response("./pong_client.js", "text/javascript", res);
					break;
				case "/pong.html":
					await this.create_response("./pong.html", "text/html", res);
					break;
				default:
				{
					console.log("	Failed to find requested content")
					res.statusCode = 404;
					res.end("Not Found");
				}
			}
		}
		else
		{
			console.log("-> websocket request, handled by websocket server");
		}
	}

	// Prepares an HTTP response containing the content of file at <path> in its body
	async create_response(path: string, mime_type: string, res: httpt.ServerResponse)
	{
		fs.readFile(path, 'utf8', (err: Error | null, data: string) => {
			if (err)
			{
				console.error(err);
				res.statusCode = 500;
				res.end("Internal server error");
			}
			else
			{
				res.statusCode = 200;
				res.setHeader("Content-Type", mime_type);
				res.end(data);
			}
		});
	}

	// WEBSOCKET SERVER FEATURES

	// Function called when a client sends an HTTP request asking for use of WebSocket protocol
	// 		(1) since websocket creation means user wants to join a game, creates a new <Pong_game> if needed
	// 		(2) sets up the new WebSocket by defining what to do when events are received on the WebSocket
	setup_websocket(ws: wst.WebSocket)
	{
		let ws_id: ws_identifier = {socket: null, open: true, ws_key: 0, game_key: 0, player_index: 0};

		if (this.open_websockets.size >= 2) // TMP, no more than 2 players at a time for now
		{
			ws.close();
			return ;
		}

		ws_id.socket = ws;
		ws_id.ws_key = this.created_websockets;
		this.created_websockets++;
		this.open_websockets.set(ws_id.ws_key, ws);
		console.log(`[WS] New client on ws at key ${ws_id.ws_key}`);
		if (this.waiting_game == null) // ther is no open game waiting for a player -> create a new game
		{
			ws_id.player_index = 0;
			ws_id.game_key = this.created_games;
			this.waiting_game = new Pong_game(this.created_games);
			this.waiting_game.add_player_websocket(ws_id);
			console.log(`[WS] -> created new Pong Game with key ${ws_id.game_key} for client, waiting for other player`);
		}
		else // there is an open game waiting for a player -> add player to this game (then the game starts)
		{
			ws_id.player_index = 1;
			ws_id.game_key = this.created_games;
			this.created_games++;
			this.games.set(ws_id.game_key, this.waiting_game);
			this.waiting_game.add_player_websocket(ws_id);
			this.waiting_game = null;
			console.log(`[WS] -> added client to game ${ws_id.game_key}, game will start`);
		}
		
		console.log(`[WS] Full ws_id of new websocket : wskey ${ws_id.ws_key}, gkey ${ws_id.game_key}, pind ${ws_id.player_index}`);
		ws.on("message", (msg: wst.RawData) => {this.ws_handle_data(msg, ws_id)});
		ws.on("close", () => {this.ws_handle_conn_closed(ws_id)});
	}

	// Transfers the data received on a websocket to the <Pong_game> instance responsible for that client
	ws_handle_data(msg: wst.RawData, ws_id: ws_identifier)
	{
		let game: Pong_game | undefined;

        console.log(`[WS] Received data on ws ${ws_id.ws_key} : ${msg.toString()}`);
		game = this.games.get(ws_id.game_key);
		if (game === undefined) // TMP (errors)
			return ;
		game.new_player_input(msg, ws_id.player_index);
	}

	// Handles a client disconnecting from a game (for example by closing the tab in the browser) :
	// if the game to which the client participated is not already terminated, terminate it by
	// 		(1) getting the websocket of the other participating client
	// 		(2) closing both websockets and deleting them (and the game) from memory
	ws_handle_conn_closed(ws_id: ws_identifier)
	{
		let counterpart_ws_id: ws_identifier;
		let game: Pong_game | undefined;

		console.log(`[WS] Connection closed on ws ${ws_id.ws_key}, notifying Pong_game instance`);
		ws_id.open = false;
		game = this.games.get(ws_id.game_key);
		if (game === undefined)
			return ; // TMP (errors)
		else if (game.status != "terminated")
		{
			counterpart_ws_id = game.end_by_disconnect(ws_id.player_index);
			if (counterpart_ws_id.socket != null && counterpart_ws_id.open)
				counterpart_ws_id.socket.close();
			this.open_websockets.delete(ws_id.ws_key);
			this.open_websockets.delete(counterpart_ws_id.ws_key);
			this.games.delete(ws_id.game_key);
		}
	}
}

let pong_server = new Pong_server('127.0.0.1', 8080);

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
o standardiser la communication client-serveur
	\ puisque plusieurs types de data envoyee du serveur au client, utiliser une union discriminante
	\ etendre principe de l'union discriminante aux messages envoyes par le client
	\ trouver une solution pas trop moche au probleme de l'envoi de messages differents aux deux joueurs (player_index)
- reflechir a une structure propre de creation et de fermeture des parties
	\ quand Pong_server rencontre une erreur
	\ quand Pong_game rencontre une erreur
	\ quand un client quitte precocement (websocket closed)
	\ quand un client concede (fait partie des commandes du client)
	\ quand la partie se termine (premier a 10 points) -> comment prevenir le serveur qu'on peut deco ?

+ revoir les contenus des fichiers :
	+ placer les constantes et interfaces dans un fichier JSON qui sera partage par serveur et client
	+ enlever (dans le code server et le code client) les references hard-codees aux constantes
	\ faire une vraie structure avec des repertoires Pong [Serveur, Client, Partage]
- utilisation d'outils externes :
	\ utiliser un bundler (Vite) pour creer un seul fichier avec toutes les dependances a envoyer au client
	\ utiliser des bibliotheques pour valider la forme de l'objet parse (zod)
- securisation du site
	\ servir les pages en HTTPS plutot que HTTP (utiliser une bibliotheque comme Express pour abstraire ?)
	\ utiliser des websockets securises 'wss'
	\ verifier que les clients ne sont pas mal intentionnes (messages trop longs / frequents)
- revoir la gestion des erreurs et ce qu'elles impliquent au niveau serveur / game / client

ONGOING : appliquer au client
	- les unions discriminantes des messages serveur
	- le message serveur de close pour terminer la partie

*/

