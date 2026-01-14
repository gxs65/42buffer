// --- A Javascript Pong client that connects to the Pong server through a websocket
import { lerp } from "./pong_utils.js";
///////////
// UTILS //
///////////
// Configuration constants
const keys_by_player = [["KeyW", "KeyS"], ["ArrowUp", "ArrowDown"]];
const paddle_colors = ["rgb(200 0 0)", "rgb(0 0 200)"];
////////////
// CLIENT //
////////////
class Pong_game {
    // SETUP
    // Constructor : Game starts NOT running (<active> is false)
    constructor() {
        this.player_index = -1; // index of this client in the game (will be 0 or 1 once game starts)
        this.server_broadcast_interval = -1; // interval at which server sends game data, in ms
        this.game_state_current = null; // current position of paddles/ball on canvas
        this.game_state_next = null; // positions given by server, to be reached
        this.commands_for_server = { up: false, down: false, pause: false, play: false, concede: false };
        this.pending_commands_for_server = false; // whether new commands must be sent to server
        this.canvas = null;
        this.ctx = null;
        this.dims = { cwidth: 0, cheight: 0, pwidth: 0, pheight: 0, ball_radius: 0 };
        // HTML page elements
        this.announcement = null;
        this.points_info = null;
        this.play_button = null;
        this.pause_button = null;
        this.concede_button = null;
        // GAME MAINLOOP
        // Update function called when browser is ready for a new frame (usually each 16ms)
        // (Method declaration with the syntax `funcname = (paraname) => {funcbody}`,
        //  because it seems to be the only way to avoid issues with call by `requestAnimationFrame`)
        this.update_animation = () => {
            let current_time = performance.now();
            let delta_time;
            let commands_for_server_string;
            let bot_checkbox = document.getElementById("testbot");
            if (this.terminated)
                return;
            // (0) registers current time
            this.frame_counter++;
            if (this.time_last_anim_update == -1) // case where this frame is first frame
                delta_time = 16; // 16ms, delta between 2 frames on the usual 60 frames/second setup
            else
                delta_time = current_time - this.time_last_anim_update;
            this.time_last_anim_update = current_time;
            console.log(`--- Frame ${this.frame_counter}, current time ${current_time}, delta ${delta_time}`);
            // (1) updates game state if game is active
            if (this.active && this.game_state_current != null) {
                // (1a) makes game state ('current') progress towards game state ('next') received from server
                this.lerp_game_state(current_time, delta_time);
                //this.log_game_state();
                // (1b) redraws paddles and ball on canvas, according to current game state
                this.draw_canvas(this.game_state_current);
                this.announcement.innerHTML = `[Player ${this.player_index}] Game active`;
                this.points_info.innerHTML = "Points : " + this.game_state_current.points[0] + "-"
                    + this.game_state_current.points[1];
            }
            if (!this.active)
                this.announcement.innerHTML = `[Player ${this.player_index}] Game paused`;
            // (2) sends to server the user commands received since last call
            // 		~ nothing sent if no new commands received
            // 			(eg. human user did not press/release any key, and did not press any button)
            // 		~ if this client is a bot, calls bot decision function
            // 			which will overwrite all commands from keyboard events if game is active
            if (bot_checkbox.checked)
                this.bot_decision_making();
            if (this.pending_commands_for_server) {
                commands_for_server_string = JSON.stringify(this.commands_for_server);
                this.ws.send(commands_for_server_string);
            }
            this.commands_for_server.play = false;
            this.commands_for_server.pause = false;
            this.commands_for_server.concede = false;
            this.pending_commands_for_server = false;
            // (3) calls itself again with <requestAnimationFrame>
            if (!this.terminated)
                requestAnimationFrame(this.update_animation);
        };
        this.active = false;
        this.terminated = false;
        this.time_last_anim_update = -1;
        this.time_last_data_received = -1;
        this.frame_counter = 0;
        this.ws = new WebSocket(`ws://${window.location.host}`);
        if (this.extract_HTML_elements()) {
            this.abort_game("invalidHTML");
            return;
        }
        this.create_event_listeners();
        this.setup_websocket();
    }
    // Extract elements from HTML DOM, returns <true> if an error occurred
    extract_HTML_elements() {
        this.canvas = document.querySelector("#canvas");
        if (!this.canvas)
            return (true);
        this.ctx = this.canvas.getContext("2d");
        if (!this.ctx)
            return (true);
        this.announcement = document.querySelector("#announcement");
        this.points_info = document.querySelector("#pointsInfo");
        this.play_button = document.querySelector("#play");
        this.pause_button = document.querySelector("#pause");
        this.concede_button = document.querySelector("#concede");
        if (!this.announcement || !this.points_info
            || !this.play_button || !this.pause_button || !this.concede_button)
            return (true);
        this.announcement.innerHTML = "Waiting for players...";
        this.play_button.addEventListener("click", () => { this.play(); });
        this.pause_button.addEventListener("click", () => { this.pause(); });
        this.concede_button.addEventListener("click", () => { this.concede(); });
        return (false);
    }
    // Called when the first message from server containing game setup data is received by the client
    // Adjusts canvas if canvas dimensions as defined by the HTML file are not equal to dimensions given by server
    adjust_canvas_dimensions() {
        if (this.canvas == null)
            return;
        if (this.dims.cwidth != this.canvas.width)
            this.canvas.width = this.dims.cwidth;
        if (this.dims.cheight != this.canvas.height)
            this.canvas.height = this.dims.cheight;
    }
    // WEBSOCKET EVENTS MANAGEMENT
    // Defines what to do on events on the websocket
    setup_websocket() {
        this.ws.onopen = () => { console.log("Client side websocket : opened"); };
        this.ws.onmessage = (event) => { this.receive_game_state_data(event.data); };
        this.ws.onclose = () => { this.end_game(); };
    }
    // Registers data sent by the server and updates <time_last_data_received>
    // 		- first package sent by server is setup information, which is read to :
    // 			~ get the <player_index> of this player (useful to know which keyboard keys to watch)
    // 			~ get the <server_broadcast_interval>, expected time between server broadcasts
    // 		  	~ start the game mainloop with <requestAnimationFrame>
    // 		- then all following messages are game state data
    // 			~ if <game_state_current> is not set, it becomes set to the new data
    // 			~ if <game_state_current> is set, the new data is registered in <game_state_next>
    // 				so that the <update_animation> loop can use it for interpolation
    // 					(except the 'points' data which is put immediately in <game_state_current>)
    receive_game_state_data(data) {
        let game_state_data;
        let game_init_data;
        console.log(`Client websocket : received data from server after ${performance.now() - this.time_last_data_received}`);
        if (this.time_last_data_received == -1) // first message received : setup info
         {
            try {
                game_init_data = JSON.parse(data);
                this.player_index = game_init_data.player_index;
                this.server_broadcast_interval = game_init_data.broadcast_interval;
                this.dims = game_init_data.dims;
                this.adjust_canvas_dimensions();
                console.log(`   -> first server message, assigned index ${this.player_index}, starting anim`);
                this.active = true;
                this.announcement.innerHTML = `[Player ${this.player_index}] Starting game...`;
                requestAnimationFrame(this.update_animation);
            }
            catch {
                console.log(`Error while validating initial data from server`);
                this.abort_game("validation");
            }
        }
        else // next messages received : game state
         {
            try {
                game_state_data = JSON.parse(data);
                this.game_state_next = game_state_data;
                this.active = game_state_data.active;
                if (this.game_state_current == null) // initialization of <game_state_current>
                    this.game_state_current = game_state_data;
                else
                    this.game_state_current.points = game_state_data.points;
            }
            catch {
                console.log(`Error while validating game state data from server`);
                this.abort_game("validation");
            }
        }
        this.time_last_data_received = performance.now();
    }
    // Ends game "cleanly" when server side closes the websocket
    end_game() {
        console.log("Client side websocket : closed");
        this.terminated = true;
        if (this.announcement != null && this.game_state_current != null) {
            if (this.game_state_current.points[0] > this.game_state_current.points[1])
                this.announcement.innerHTML = `[Player ${this.player_index}] Game ended, victory : player 0.`;
            else if (this.game_state_current.points[1] > this.game_state_current.points[0])
                this.announcement.innerHTML = `[Player ${this.player_index}] Game ended, victory : player 1.`;
            else
                this.announcement.innerHTML = `[Player ${this.player_index}] Game ended, tie.`;
        }
    }
    // Ends game "unexpectedly" when an error occurs and is caught
    abort_game(reason) {
        console.log(`Game aborted because of : ${reason}`);
        this.terminated = true;
        if (this.announcement != null)
            this.announcement.innerHTML = "Game aborted because of an unexpected error.";
    }
    // USER EVENTS MANAGEMENT
    // (user events stored in <commands_for_server> to be sent to server at next animation frame)
    // Listen to keyboard events for player keys
    create_event_listeners() {
        document.addEventListener('keydown', (event) => { this.store_key_event(event, false); });
        document.addEventListener('keyup', (event) => { this.store_key_event(event, true); });
    }
    // Associate keyboard event to a paddle move command according to player keys
    // (eg. if this client is player index 0, they use keys 'w' and 's')
    // /!\ checks if <player_index> is different from -1
    // 	   to avoid registering key events until game is really started
    store_key_event(event, key_released) {
        var code = event.code;
        if (this.player_index == -1 || !(keys_by_player[this.player_index].includes(code)))
            return;
        event.preventDefault();
        if (code == keys_by_player[this.player_index][0]) {
            if (key_released)
                this.commands_for_server.up = false;
            else
                this.commands_for_server.up = true;
        }
        else {
            if (key_released)
                this.commands_for_server.down = false;
            else
                this.commands_for_server.down = true;
        }
        this.pending_commands_for_server = true;
    }
    // Play button : request server to play game
    play() {
        if (this.active == false) {
            this.commands_for_server.play = true;
            this.pending_commands_for_server = true;
        }
    }
    // Pause button : request server to pause game
    pause() {
        if (this.active == true) {
            this.commands_for_server.pause = true;
            this.pending_commands_for_server = true;
        }
    }
    // Concede button : request server to concede
    concede() {
        this.commands_for_server.concede = true;
        this.pending_commands_for_server = true;
    }
    // Makes game state ('current') progress towards game state ('next') received from server
    // using interpolation : objects advance towards their expected position,
    // 		by a distance proportional to the distance between current pos and expected pos
    // The proportion is calculated by comparing the animation tick <delta_time>
    // to the expected time before the next broadcast
    // 		(eg. if they are equal, the animation should go immediately to next game state -> <proportion> = 1)
    lerp_game_state(current_time, delta_time) {
        let gsc = this.game_state_current; // for readability
        let gsn = this.game_state_next;
        let elapsed = current_time - this.time_last_data_received; // time elapsed since last broadcast
        let time_before_broadcast = (this.server_broadcast_interval - elapsed); // time before next broadcast
        let proportion = delta_time / (time_before_broadcast + delta_time);
        if (gsc == null || gsn == null)
            return;
        //console.log(`	lerping : time before next broadcast ${time_before_broadcast}, proportion ${proportion}`);
        if (proportion < 0) {
            proportion = 0;
        }
        if (proportion > 1) {
            proportion = 1;
        }
        gsc.ball_pos = lerp(gsc.ball_pos, gsn.ball_pos, proportion);
        gsc.paddles_pos[0] = lerp(gsc.paddles_pos[0], gsn.paddles_pos[0], proportion);
        gsc.paddles_pos[1] = lerp(gsc.paddles_pos[1], gsn.paddles_pos[1], proportion);
    }
    // Draw canvas figures (1 ball and 2 paddles), using the canvas context features
    draw_canvas(gsd) {
        let ctx = this.ctx;
        let paddle_index;
        ctx.clearRect(0, 0, this.dims.cwidth, this.dims.cheight); // erase previous drawings
        for (paddle_index = 0; paddle_index < 2; paddle_index++) {
            ctx.fillStyle = paddle_colors[paddle_index];
            ctx.fillRect(gsd.paddles_pos[paddle_index].x - this.dims.pwidth / 2, gsd.paddles_pos[paddle_index].y - this.dims.pheight / 2, this.dims.pwidth, this.dims.pheight);
        }
        ctx.strokeStyle = "rgb(0 0 0)";
        ctx.beginPath();
        ctx.arc(gsd.ball_pos.x, gsd.ball_pos.y, this.dims.ball_radius, 0, Math.PI * 2, false);
        ctx.stroke();
    }
    // BOT DECISIONS
    // Decides automatically whether the player's paddle should go up or down
    // (for now simplistically follows the ball's ordinate)
    bot_decision_making() {
        let gsn = this.game_state_next;
        let vertical_dist_to_ball;
        if (this.active == false || gsn == null)
            return;
        vertical_dist_to_ball = gsn.paddles_pos[this.player_index].y - gsn.ball_pos.y;
        if (vertical_dist_to_ball > this.dims.pheight / 4)
            this.commands_for_server.up = true;
        else
            this.commands_for_server.up = false;
        if (vertical_dist_to_ball < (this.dims.pheight / 4) * -1)
            this.commands_for_server.down = true;
        else
            this.commands_for_server.down = false;
        this.pending_commands_for_server = true;
        console.log(`	Bot decision : distance ${vertical_dist_to_ball} -> up ${this.commands_for_server.up} down ${this.commands_for_server.down}`);
    }
    // LOGS
    // Logs the position of game components (current position, and last position broadcasted by server)
    log_game_state() {
        let gsc = this.game_state_current;
        let gsn = this.game_state_next;
        console.log(`current : ball ${gsc?.ball_pos.x},${gsc?.ball_pos.y}`);
        console.log(`	paddle 0 ${gsc?.paddles_pos[0].x},${gsc?.paddles_pos[0].y}`);
        console.log(`	paddle 1 ${gsc?.paddles_pos[1].x},${gsc?.paddles_pos[1].y}`);
        console.log(`next : ball ${gsn?.ball_pos.x},${gsn?.ball_pos.y}`);
        console.log(`	paddle 0 ${gsn?.paddles_pos[0].x},${gsn?.paddles_pos[0].y}`);
        console.log(`	paddle 1 ${gsn?.paddles_pos[1].x},${gsn?.paddles_pos[1].y}`);
    }
}
function setup() {
    let game = new Pong_game();
}
window.onload = setup;
/* TODO

+ tenir compte du temps passe depuis la derniere frame dans update_animation
+ transformer la surface des paddles en demi-cercles
+ creer un petit mecanisme de rectification de la direction de la balle quand elle est trop verticale
+ teleporter la balle si son centre est a l'interieur d'un paddle

+ utiliser <time_last_data_received> pour interpoler la position des game components
+ creer une option pour jouer contre un bot primaire
+ trouver un moyen de n'envoyer les events au serveur que si au moins l'un des 4 events est true
    -> remplace par un flag qui indique s'il y a des commandes pending a envoyer au serveur

+ valider le game state recu du serveur quand on le JSON.parse
+ peaufiner le HTML : zone pour indiquer si le jeu est en attente / actif / paused / terminated

- construire une vraie methode de tests avec des bots lances par node.JS sans rendering
- rendre le bot un peu plus interessant (ca devient la partie IA)
*/
