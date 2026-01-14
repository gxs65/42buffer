// --- A Node.JS server organising Pong games through Websockets
/////////////
// IMPORTS //
/////////////
// Import external modules
import * as http from 'node:http'; // HTTP networking
import * as fs from 'node:fs'; // filesystem manipulation
import * as ws from 'ws'; // Websocket networking
// Import classes for computations on game components
import { Ball, Paddle } from "./pong_game_classes.js";
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
//////////////////
// GAME OF PONG //
//////////////////
// Class managing the Pong game by
// 		- creating the Ball and Paddle instances whose methods do the calculations
// 		- running the game loop
// 		- sending and receiving messages from clients through websockets
export class Pong_game {
    // GAME SETUP
    // Constructor does not start the game (game becomes "starting" once there are 2 players in it)
    constructor(id) {
        this.player_ws_ids = [];
        this.players_input = []; // 2 objects storing input received from players
        // Game state
        this.concession = -1; // index of a conceding player (-1 means no player conceded)
        this.last_time = -1;
        this.points = [0, 0];
        this.ball = null;
        this.paddles = [];
        // Dimensions of playground
        this.playground_width = playground_width;
        this.playground_height = playground_height;
        console.log(`New Pong Game with id ${id} created`);
        this.status = "waiting";
        this.id = id;
    }
    // Called when a client connected to websocket server to enter a game,
    // registers the client's websocket identity
    // and starts the game if there are now 2 players in the game
    add_player_websocket(ws_id) {
        if (this.status != "waiting") // no player may be added if game is already full
            return;
        this.player_ws_ids.push(ws_id);
        if (this.player_ws_ids.length == 2) {
            this.status = "starting";
            this.setup_game();
            this.broadcast_to_players_init(); // send setup information to clients
            setTimeout(() => this.simulation_tick(), 1000); // 1s delay before game start
            setTimeout(() => this.broadcast_tick(), 1000);
        }
    }
    // Instantiates paddles and ball objects, which will make position and movement calculations,
    // and initializes all client commands to false for the two players
    setup_game() {
        this.ball = new Ball(this, simulation_tick_interval, this.playground_width / 2, this.playground_height / 2, ball_radius, ball_initial_speed);
        this.paddles.push(new Paddle(this, 0, simulation_tick_interval, paddle_margin, this.playground_height / 2, paddle_width, paddle_height, paddle_speed));
        this.paddles.push(new Paddle(this, 1, simulation_tick_interval, this.playground_width - paddle_margin, this.playground_height / 2, paddle_width, paddle_height, paddle_speed));
        this.players_input.push({ up: false, down: false, play: false, pause: false, concede: false });
        this.players_input.push({ up: false, down: false, play: false, pause: false, concede: false });
    }
    // Called by <Pong_server> when the Websocket of a player in the game becomes closed,
    // returns the index of the websocket of the other player (the "counterpart")
    // and sets the game to state 'terminated'
    end_by_disconnect(disconnected_player_index) {
        let counterpart_index = 1 - disconnected_player_index;
        this.status = "terminated";
        return (this.player_ws_ids[counterpart_index].ws_key);
    }
    // COMMUNICATION WITH CLIENTS
    // Receives the user commands as a JSON string sent by a player through websockets
    // 		- checks if the user wants to pause / resume / concede the game
    // 			(because this can be done immediately)
    // 		- stores the input about pressed keys ('up' or 'down')
    // 			(because this must wait to be processed in <simulation_tick>)
    new_player_input(msg, player) {
        let input;
        try {
            input = JSON.parse(msg.toString());
            if (this.concession == -1 && input.concede)
                this.concession = player;
            if (this.status == "active" && input.pause)
                this.status = "paused";
            if (this.status == "paused" && input.play)
                this.status = "active";
            this.players_input[player] = input;
        }
        catch {
            console.log(`Pong game ${this.player_ws_ids[0].game_key} - error on validation, disregarding message`);
            // #f : actions to take in case of validation failure
        }
    }
    // Sends setup information to the players through websockets
    broadcast_to_players_init() {
        let player;
        let game_init_string;
        let gid = {
            player_index: -1,
            broadcast_interval: broadcast_tick_interval,
            dims: {
                cwidth: this.playground_width, cheight: this.playground_height,
                pwidth: this.paddles[0].width, pheight: this.paddles[0].height,
                ball_radius: this.ball.radius
            }
        };
        for (player = 0; player < 2; player++) {
            gid.player_index = player;
            game_init_string = JSON.stringify(gid);
            this.player_ws_ids[player].socket?.send(game_init_string);
        }
    }
    // Sends the current state of the game as a JSON string to the players through websockets
    broadcast_to_players() {
        let player;
        let game_state_string;
        let gsd = {
            ball_pos: { x: this.ball.position.x, y: this.ball.position.y },
            paddles_pos: [
                { x: this.paddles[0].position.x, y: this.paddles[0].position.y },
                { x: this.paddles[1].position.x, y: this.paddles[1].position.y },
            ],
            points: [this.points[0], this.points[1]],
            active: (this.status == "active")
        };
        for (player = 0; player < 2; player++) {
            game_state_string = JSON.stringify(gsd);
            this.player_ws_ids[player].socket?.send(game_state_string);
        }
    }
    // LOOPS
    // interval 50ms
    broadcast_tick() {
        console.log(`+++ Broadcast tick +++`);
        this.broadcast_to_players();
        setTimeout(() => this.broadcast_tick(), broadcast_tick_interval);
    }
    // interval 10ms
    simulation_tick() {
        let delta_time;
        let current_time = performance.now();
        if (this.status == "starting")
            this.status = "active";
        // (0) register current time
        if (this.last_time == -1) // case where this is the first call of <game_mainloop>
            delta_time = 10;
        else
            delta_time = current_time - this.last_time;
        console.log(`--- Simulation tick with delta ${delta_time}, game status ${this.status}`);
        this.last_time = current_time;
        // (1) check if a player conceded
        if (this.concession != -1) {
            this.status = "terminated";
            // #f : cleanly terminate game
        }
        // (2) Update positions if game is not paused
        if (this.status == "active")
            this.update_ball_and_paddles(delta_time);
        // (3) call itself in 10ms
        setTimeout(() => this.simulation_tick(), simulation_tick_interval);
    }
    update_ball_and_paddles(delta_time) {
        let ball_out_of_bounds;
        let losing_player_index;
        // Let paddle instances register if they must move (up or down)
        if (!this.ball || !this.paddles[0] || !this.paddles[1]) // TMP (errors)
            return;
        this.paddles[0].process_keys(this.players_input[0]);
        this.paddles[1].process_keys(this.players_input[1]);
        // Let paddle and ball instances move
        this.paddles[0].update(delta_time, this.ball);
        this.paddles[1].update(delta_time, this.ball);
        ball_out_of_bounds = this.ball.update(delta_time);
        // Check if ball went went through left/right wall <=> a player loses a point
        if (ball_out_of_bounds == 0 || ball_out_of_bounds == 1) {
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
class Pong_server {
    constructor(hostname, port) {
        this.waiting_game = null; // the Pong game currently waiting for additional players
        this.games = new Map(); // map containing all ongoing games
        this.open_websockets = new Map(); // map containing all currently active websockets
        this.created_games = 0; // counter used to make unique keys for <games>
        this.created_websockets = 0; // counter used to make unique keys for <open_websockets>
        this.hostname = hostname;
        this.port = port;
        this.server = http.createServer((req, res) => this.request_handler(req, res));
        this.ws_server = new ws.WebSocketServer({ server: this.server }); // new WebSocket Server "attached" to general HTTP server
        this.ws_server.on("connection", (ws) => this.setup_websocket(ws));
        this.server.listen(port, hostname, () => { console.log(`Node.JS server at http://${hostname}:${port}/`); });
    }
    // HTTP SERVER FEATURES
    // Handles incoming HTTP request :
    // 		- serves files whem request method is GET
    // 		- ignores websocket requests (with an 'upgrade' header) to be managed by the WS server
    async request_handler(req, res) {
        console.log(`Received new request : ${req.method}, ${req.url}`);
        if (!("upgrade" in req.headers && req.headers["upgrade"] == "websocket") && req.method == "GET") {
            console.log("-> serving site content");
            switch (req.url) {
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
                        console.log("	Failed to find requested content");
                        res.statusCode = 404;
                        res.end("Not Found");
                    }
            }
        }
        else {
            console.log("-> websocket request, handled by websocket server");
        }
    }
    // Prepares an HTTP response containing the content of file at <path> in its body
    async create_response(path, mime_type, res) {
        fs.readFile(path, 'utf8', (err, data) => {
            if (err) {
                console.error(err);
                res.statusCode = 500;
                res.end("Internal server error");
            }
            else {
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
    setup_websocket(ws) {
        let ws_id = { socket: null, ws_key: 0, game_key: 0, player_index: 0 };
        if (this.open_websockets.size >= 2) // TMP, no more than 2 players at a time for now
         {
            ws.close();
            return;
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
        ws.on("message", (msg) => { this.ws_handle_data(msg, ws_id); });
        ws.on("close", () => { this.ws_handle_conn_closed(ws_id); });
    }
    // Transfers the data received on a websocket to the <Pong_game> instance responsible for that client
    ws_handle_data(msg, ws_id) {
        let game;
        console.log(`[WS] Received data on ws ${ws_id.ws_key} : ${msg.toString()}`);
        game = this.games.get(ws_id.game_key);
        if (game === undefined) // TMP (errors)
            return;
        game.new_player_input(msg, ws_id.player_index);
    }
    // Handles a client disconnecting from a game (for example by closing the tab in the browser) :
    // if the game to which the client participated is not already terminated, terminate it by
    // 		(1) finding the websocket of the other participating client
    // 		(2) closing both websockets and deleting them (and the game) from memory
    ws_handle_conn_closed(ws_id) {
        let counterpart_ws_key;
        let counterpart_ws;
        let game;
        console.log(`[WS] Connection closed on ws ${ws_id.ws_key}`);
        game = this.games.get(ws_id.game_key);
        if (game === undefined)
            return; // TMP (errors)
        else if (game.status == "terminated") {
        }
        else {
            counterpart_ws_key = game.end_by_disconnect(ws_id.player_index);
            console.log(`[WS] -> Game ${ws_id.game_key} terminated, closing counterpart websocket at ${counterpart_ws_key}`);
            counterpart_ws = this.open_websockets.get(counterpart_ws_key);
            if (counterpart_ws === undefined)
                return; // TMP (errors)
            counterpart_ws.close();
            this.open_websockets.delete(ws_id.ws_key);
            this.open_websockets.delete(counterpart_ws_key);
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
- au lieu d'avoir plusieurs booleens, utiliser une seule variable de status de la partie :
    waiting | starting | active | paused | terminated
- reflechir a une structure propre de creation et de fermeture des parties
    \ quand Pong_server rencontre une erreur
    \ quand Pong_game rencontre une erreur
    \ quand un client quitte precocement (websocket closed)
    \ quand un client concede (fait partie des commandes du client)
    \ quand la partie se termine (premier a 10 points)

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

- pour le resume global :
    \ clarifier player/client et gestion du websocket par Websocketserver et ses instances de Pong_game
    \ bien distinguer ticks de simulation / ticks de broadcast / frames de browser
*/
