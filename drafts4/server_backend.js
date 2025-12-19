"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const http = require('node:http');
const fs = require('node:fs');
const ws = require('ws');
//import {createServer, Server} from "node:http";
class Pong_server {
    constructor(hostname, port) {
        this.game_states = [];
        this.open_websockets = [];
        this.counter = 0;
        this.hostname = hostname;
        this.port = port;
        this.server = http.createServer((req, res) => this.request_handler(req, res));
        let s = this.server;
        this.ws_server = new ws.WebSocketServer({ server: this.server });
        this.ws_server.on("connection", (ws) => this.setup_websocket(ws));
        this.server.listen(port, hostname, () => { console.log(`Node.JS server at http://${hostname}:${port}/`); });
    }
    async request_handler(req, res) {
        console.log(`Received new request : ${req.method}, ${req.url}`);
        if (!("upgrade" in req.headers && req.headers["upgrade"] == "websocket")) {
            console.log("-> serving site content");
            switch (req.url) {
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
        else {
            console.log("-> websocket request, handled by websocket server");
        }
    }
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
    setup_websocket(ws) {
        console.log("New client connected");
        this.open_websockets.push(ws);
        ws.on("message", msg => {
            console.log("Received:", msg.toString(), this.counter);
            this.counter++;
            if (msg.toString() === "ping") {
                ws.send("pong");
            }
        });
        ws.on("close", () => console.log("Client disconnected"));
    }
}
let pong_server = new Pong_server('127.0.0.1', 8080);
