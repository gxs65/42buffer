//const { createServer } = require('node:http');
import http from 'node:http';
import fs from 'node:fs';

async function createResponse(path, mimeType, res)
{
	fs.readFile(path, 'utf8', (err, data) => {
		if (err) {
			console.error(err);
			return ;
  		}
		res.statusCode = 200;
		res.setHeader("Content-Type", mimeType);
		res.end(data);
	});
}

const hostname = '127.0.0.1';
const port = 8080;

const server = http.createServer((req, res) => {
	console.log("Received new request");
	if (req.headers.get("upgrade") !== "websocket")
	{
		const url = request.url;
		console.log(url);
		// switch (url.pathname) {
		// 	case "/client.js":
		// 		return await createResponse("./client.js", "text/javascript");
		// 	case "/client.css":
		// 		return await createResponse("./client.css", "text/css");
		// 	case "/":
		// 		return await createResponse("./index.html", "text/html");
		// 	default:
		// 		return new Response("Not found", {
		// 			status: 404,
		// 		});
		// }
	}
	// const { socket, response } = Deno.upgradeWebSocket(request);

	// socket.onopen = () => {
	// 	console.log("CONNECTED");
	// };
	// socket.onmessage = (event) => {
	// 	console.log(`RECEIVED: ${event.data}`);
	// 	socket.send("pong");
	// };
	// socket.onclose = () => console.log("DISCONNECTED");
	// socket.onerror = (error) => console.error("ERROR:", error);

	// return response;
});

server.listen(port, hostname, () => {
  console.log(`Node.JS server running at http://${hostname}:${port}/`);
});

