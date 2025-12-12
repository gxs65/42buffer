// 'require' syntax from CommonJS used to import modules
// (this is the old Node.JS system to import modules,
//  since then JS has developed another system using keyword 'import')
const http = require('node:http');
const fs = require('node:fs');
const ws = require('ws');
import { LargeNumberLike } from "node:crypto";
// Lines asking TS to load types for the modules,
// disappear in the JS code post transpilation with `tsc`
import type * as httpt from "node:http";
import type * as wst from "ws";

interface ws_identifier
{
	socket: wst.WebSocket | null;
	index: number;
	game_index: number;
	player_index: number;
}

class Pong_game
{
	player_websockets: Array<wst.WebSocket> = [];

	constructor()
	{
		console.log("New Pong Game object created");
	}

	add_player_websocket(ws_id: ws_identifier)
	{
		if (this.player_websockets.length >= 2)
			return ;
		this.player_websockets.push(ws_id.socket!);
		if (this.player_websockets.length == 2)
		{
			this.setup_game();
			setTimeout(() => this.game_mainloop(), 1000);
		}
	}

	setup_game()
	{
		;
	}

	end_by_disconnect(player: number)
	{
		;
	}

	new_player_input(msg: wst.RawData, player: number)
	{
		;
	}

	game_mainloop()
	{
		/*
		check jeu en pause
		si oui : parcourir input recent pour trouver reprise
		si non : parcourir input recent pour deplacer paddles
			update position de la balle
			envoyer la nouvelle position des deux paddles et de la balle aux websockets
			setTimeout du prochaine game_mainloop
		*/
	}
}

class Pong_server
{
	hostname: string;
	port: number;
	server: httpt.Server;
	ws_server: wst.WebSocketServer;
	games: Array<Pong_game> = [];
	open_websockets: Array<wst.WebSocket> = [];

	counter = 0;

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

	// Handles incoming HTTP request : serves files whem request method is GET
	async request_handler(req: httpt.IncomingMessage, res: httpt.ServerResponse)
	{
		console.log(`Received new request : ${req.method}, ${req.url}`);	

		if (!("upgrade" in req.headers && req.headers["upgrade"] == "websocket") && req.method == "GET")
		{
			console.log("-> serving site content");
			switch (req.url)
			{
				case "/blocky_pong.js":
					await this.create_response("./blocky_pong.js", "text/javascript", res);
					break;
				case "/blocky_pong.html":
					await this.create_response("./blocky_pong.html", "text/html", res);
					break;
				default:
				{
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
		fs.readFile(path, 'utf8', (err: Error, data: string) => {
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

	// Function called when a client sends an HTTP request asking for transition to WebSocket protocol
	// Sets up the new WebSocket by defining what to do when events are received on the WebSocket
	setup_websocket(ws: wst.WebSocket)
	{
		let ws_id: ws_identifier = {socket: null, index: 0, game_index: 0, player_index: 0};

		if (this.open_websockets.length >= 2)
		{
			ws.close();
			return ;
		}

		ws_id.socket = ws;
		ws_id.index = this.open_websockets.push(ws);
		console.log(`[WS] New client on ws ${ws_id.index}`);
		ws_id.game_index = 0;
		if (this.games.length == 0)
		{
			ws_id.player_index = 0;
			this.games.push(new Pong_game());
			this.games[ws_id.game_index].add_player_websocket(ws_id);
		}
		else
		{
			ws_id.player_index = 1;
			this.games[ws_id.game_index].add_player_websocket(ws_id);
		}
		ws.on("message", (msg: wst.RawData) => {this.ws_handle_data(msg, ws_id)});
		ws.on("close", () => {this.ws_handle_conn_closed(ws_id)});

	}

	ws_handle_data(msg: wst.RawData, ws_id: ws_identifier)
	{
        console.log(`[WS] Received data on ws ${ws_id.index} : ${msg.toString()}`);
        this.games[ws_id.game_index].new_player_input(msg, ws_id.player_index);
	}

	ws_handle_conn_closed(ws_id: ws_identifier)
	{
		let counterpart_ws_index: number;
		console.log(`[WS] Connection closed on ws ${ws_id.index}`);
		counterpart_ws_index = this.games[ws_id.game_index].end_by_disconnect(ws_id.player_index);
		this.games.splice(ws_id.game_index, 1);
		this.open_websockets[counterpart_ws_index].close();
		this.forget_websockets(ws_id.index, counterpart_ws_index);
	}

	forget_websockets(ws_index1: number, ws_index2: number)
	{
		this.open_websockets.splice(ws_index1, 1);
		this.open_websockets.splice(ws_index2 - +!!(ws_index1 < ws_index2), 1);
	}
	


}

let pong_server = new Pong_server('127.0.0.1', 8080);

// souci de double close sur les sockets d'une partie qui se termine
// comment Game fait-il comprendre a Server que la partie est terminee ?

