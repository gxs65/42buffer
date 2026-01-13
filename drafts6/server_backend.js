"use strict";
/////////////
// IMPORTS //
/////////////
Object.defineProperty(exports, "__esModule", { value: true });
// 'require' syntax from CommonJS used to import modules
// (this is the old Node.JS system to import modules,
//  since then JS has developed another system using keyword 'import')
const http = require('node:http');
const fs = require('node:fs');
const ws = require('ws');
// CONSTANTS
// (TMP, should be replaced by a json file)
const canvas_width = 300;
const canvas_height = 150;
const paddle_margin = 10;
const paddle_width = 10;
const paddle_height = 40;
const paddle_speed = 4;
const paddle_colors = ["rgb(200 0 0)", "rgb(0 0 200)"];
const ball_radius = 4;
const ball_initial_speed = 0.8;
const ball_acceleration = 0.0001;
const max_direction_horiz = 1.5; // maximal vertical slope
const keys_by_player = [["KeyW", "KeyS"], ["ArrowUp", "ArrowDown"]];
const paddle_normal_vectors = [[-0.5, -0.5], [0, -1], [0.5, -0.5],
    [-1, 0], [0, 0], [1, 0],
    [-0.5, 0.5], [0, 1], [0.5, 0.5]];
const wall_normal_vectors = { top: [0, 1], bottom: [0, -1] };
const simulation_tick_interval = 10; // ms between each game state update by server
const broadcast_tick_interval = 50; // ms between each game state broadcast to clients by server
// UTILITY FUNCTIONS
// Returns the slope of the circular line with radius centered on (0, 0) defined by `y**2 + x**2 == 1`
// (the simplified expression is : `f(x) = (1 - x**2)**0.5`, therefore derivative is `f'(x) = -x / (1 - x**2)**0.5`)
// (<abcissa> should be between -1 and 1, the endpoints of the circular line)
function slope_on_circle(abcissa) {
    return (-1 * abcissa / (1 - abcissa * abcissa) ** 0.5);
}
// VECTOR MANIPULATION (CLASSES + FUNCTIONS)
// 2-dimensional point class
class Point {
    constructor(x = 0, y = 0) {
        this.x = x;
        this.y = y;
    }
}
// 2-dimensional vector class
class Vec2 {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }
    get length() {
        return (Math.sqrt(this.x * this.x + this.y * this.y));
    }
    scale(scaler) {
        this.x = scaler * this.x;
        this.y = scaler * this.y;
        return (this);
    }
    scaled(scaler) {
        let vec_scaled = new Vec2(scaler * this.x, scaler * this.y);
        return (vec_scaled);
    }
    normalize() {
        let length = this.length;
        if (length != 1)
            this.scale(1 / length);
        return (this);
    }
}
// Addition of two vectors
function vec2_add(vecA, vecB) {
    let vecRes = new Vec2(0, 0);
    vecRes.x = vecA.x + vecB.x;
    vecRes.y = vecA.y + vecB.y;
    return (vecRes);
}
// Substraction of <vecB> to <vecA>
function vec2_substract(vecA, vecB) {
    return (vec2_add(vecA, vecB.scaled(-1)));
}
// Scalar product of two vectors
function vec2_sproduct(vecA, vecB) {
    return (vecA.x * vecB.x + vecA.y * vecB.y);
}
// Length of the segment between two points represented by vectors
function vec2_distance_between_points(vecA, vecB) {
    let seg = vec2_substract(vecA, vecB);
    return (seg.length);
}
// Slope of the line between two points represented by vectors
function vec2_slope_between_two_points(vecA, vecB) {
    return ((vecA.y - vecB.y) / (vecA.x - vecB.x));
}
// Symmetry of vector according to a normal vector
// (the <incident> vector crosses a line with normal vector <normal>,
//  this function returns the vector reflected by the surface,
//  ie. symmetric to <incident> as if <normal> were the axis of symmetry)
// (computed thanks to formula :
//  `reflected = incident - 2 * (normal . incident) * normal`)
function vec2_symmetry(incident, normal) {
    return (vec2_add(incident, normal.scale(vec2_sproduct(incident, normal) * -2)));
}
////////////////
// GAME LOGIC //
////////////////
// A rectangular paddle that can be moved by one player and block the ball
class Paddle {
    constructor(game, index, initial_x, initial_y, width, height, speed) {
        this.position_upperleft_corner = new Vec2(0, 0);
        this.position_upperright_corner = new Vec2(0, 0);
        this.diag_slope = 0;
        this.game = game;
        this.width = width;
        this.height = height;
        this.position = new Vec2(initial_x, initial_y); // position of center of paddle rectangle
        this.compute_side_attributes();
        this.index = index; // which player the paddle belongs to
        this.speed = speed; // number of pixels the paddle can cross in 1 simulation tick (#f)
        this.moving = 0; // -1 for moving up, 0 for unmoving, 1 for moving down
        this.testify();
    }
    // Computes unimportant constant attributes
    compute_side_attributes() {
        this.position_upperleft_corner = vec2_add(this.position, new Vec2(-(this.width / 2), -(this.height / 2)));
        this.position_upperright_corner = vec2_add(this.position, new Vec2((this.width / 2), -(this.height / 2)));
        this.diag_slope = this.height / this.width;
    }
    // Log function
    testify() {
        console.log(`Paddle dim ${this.width},${this.height}, pos ${this.position.x},${this.position.y}, moving ${this.moving}`);
        //console.log(`with diag slope ${this.diag_slope}, UL corner ${this.position_upperleft_corner.x},${this.position_upperleft_corner.y},
        //	UR corner ${this.position_upperright_corner.x},${this.position_upperright_corner.y}`)
    }
    // Determines if paddle must be moving, according to keys pressed by player as given by <input>
    // 		- no key pressed / both 'up' and 'down' keys pressed : no move
    // 		- only one of 'up' and 'down' keys pressed : move in that direction
    process_keys(input) {
        this.moving = 0;
        if (input.up && !input.down)
            this.moving = -1;
        if (!input.up && input.down)
            this.moving = 1;
    }
    // Returns 1 if ball collides with the paddle, 0 if not
    // Does not take orientation into account
    // (ie. this method can't determine with which corner / edge of the paddle the ball collided)
    // Uses a succession of conditions explained here :
    // https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
    check_collision_with_ball_unoriented(ball) {
        let abs_ball_pos = new Vec2(0, 0); // position of ball center in an orthonormal basis centered on paddle center
        let distance_to_corner;
        abs_ball_pos.x = Math.abs(ball.position.x - this.position.x);
        abs_ball_pos.y = Math.abs(ball.position.y - this.position.y);
        //console.log(`abs ball position : ${abs_ball_pos.x}, ${abs_ball_pos.y}`);
        // Excludes cases where ball center is too far
        if (abs_ball_pos.x > (this.width / 2 + ball.radius))
            return (false);
        if (abs_ball_pos.y > (this.height / 2 + ball.radius))
            return (false);
        // Includes cases where ball center is close enough
        if (abs_ball_pos.x < this.width / 2)
            return (true);
        if (abs_ball_pos.y < this.height / 2)
            return (true);
        // Manages last remaining cases : ball center is near paddle corner,
        // so the distance to the corner must be less than ball radius for collision to occur
        distance_to_corner = Math.pow(abs_ball_pos.x - this.width / 2, 2) + Math.pow(abs_ball_pos.y - this.height / 2, 2);
        if (distance_to_corner <= Math.pow(ball.radius, 2))
            return (true);
        else
            return (false);
    }
    // Checks that there is a collision between ball and paddle,
    // and determine at which edge or corner of the paddle it occurred
    // Return value is an integer indicating where the collision occurred,
    // supposing the paddle is the rectangle in the center (area numbered 4)
    // 0 | 1 | 2	(-1 means no collision)
    // - + - + -	(0/2/6/8 means collision with corner, 1/3/5/7 means collision with edge)
    // 3 | 4 | 5	(4 never happens, it is rounded to a collision with edges 1/3/5/7)
    // - + - + -
    // 6 | 7 | 8
    // Subdivisions of area 4 (corresponding to zones 1/3/5/7) :
    // + - - - +	(10 and 12 are under diag2, 10 and 14 are under diag1)
    // | \ 10/ |
    // | 12X 14|
    // | / 16\ |
    // + - - - +	(-> return valuw of 10/12/14/16 indicates special case where ball is INSIDE paddles)
    check_collision_with_ball(ball) {
        let x_zone;
        let y_zone;
        let full_zone;
        //console.log(`computing collision between ball and paddle :`);
        //this.testify();
        //ball.testify();
        if (!this.check_collision_with_ball_unoriented(ball))
            return (-1);
        x_zone = +!!(ball.position.x > this.position.x - this.width / 2)
            + +!!(ball.position.x > this.position.x + this.width / 2); // `+!!` syntax transforms boolean into number
        y_zone = +!!(ball.position.y > this.position.y - this.height / 2)
            + +!!(ball.position.y > this.position.y + this.height / 2);
        full_zone = y_zone * 3 + x_zone;
        //console.log(`collision found : x zone ${x_zone}, y zone ${y_zone} -> full zone ${full_zone}`);
        if (full_zone == 4) // if ball center is INSIDE paddle, return 1/3/5/7 according to closest edge, computed using diagonals
         {
            let under_diag1 = vec2_slope_between_two_points(ball.position, this.position_upperleft_corner) < this.diag_slope;
            let under_diag2 = vec2_slope_between_two_points(ball.position, this.position_upperright_corner) * -1 < this.diag_slope;
            full_zone = 9 + 7 - 2 * +!!under_diag1 - 4 * +!!under_diag2;
            console.log(`collision within paddle (ud1 ${under_diag1}, ud2 ${under_diag2} -> full zone ${full_zone})`);
        }
        return (full_zone);
    }
    // Checks if the given <ball>'s center is within the abcissa of the paddle
    ball_in_range(ball) {
        return (ball.position.x >= (this.position.x - this.width / 2)
            && ball.position.x <= (this.position.x + this.width / 2));
    }
    // Moves the paddle if its <moving> property is -1 (moving up) or 1 (moving down)
    // (using <delta_time> to adjust the distance moved according to the time elapsed since last tick
    //  because sometimes the <delta_time> between two <simulation_tick> won't be exactly <sim_tick_interval>)
    // Checks before moving :
    // 		- that the paddle is not going beyond top wall / below bottom wall
    // 		- that paddle is not going "into" the ball if ball is between paddle and top/bottom wall
    update(delta_time, ball) {
        let time_modifier = delta_time / simulation_tick_interval;
        let ball_in_range = this.ball_in_range(ball);
        let next_paddle_topy = this.position.y - this.speed * time_modifier - this.height / 2;
        let next_paddle_bottomy = this.position.y + this.speed * time_modifier + this.height / 2;
        if (this.moving == -1 && next_paddle_topy > 0) // check : not beyond top wall
         {
            if (!(ball_in_range && ball.position.y < this.position.y // check : ball not between paddle and top wall
                && next_paddle_topy <= 2 * ball.radius))
                this.position.y -= 1 * this.speed * time_modifier; // moving up
        }
        else if (this.moving == 1 && next_paddle_bottomy < this.game.height) // check : not beyond bottom wall
         {
            if (!(ball_in_range && ball.position.y > this.position.y // check : ball not between paddle and bottom wall
                && this.game.height - next_paddle_bottomy <= 2 * ball.radius))
                this.position.y += 1 * this.speed * time_modifier; // moving down
        }
    }
}
// A ball that moves and collides walls and paddles until it goes through left or right wall
class Ball {
    constructor(game, initial_x, initial_y, radius, initial_speed) {
        this.position = new Vec2(0, 0);
        this.direction = new Vec2(0, 0); // a unit vector
        this.speed = 0; // a scaler on the direction vector
        this.recursion_counter = 0;
        this.game = game;
        this.radius = radius;
        this.initial_position = new Vec2(initial_x, initial_y); // initial position stored to be able to reset
        this.initial_speed = initial_speed;
        this.drawn = false;
        this.reset(0);
        this.testify();
    }
    // Log function
    testify() {
        console.log(`Ball at pos ${this.position.x},${this.position.y} with direction ${this.direction.x},${this.direction.y} and speed ${this.speed}`);
    }
    // Puts ball in initial position and sets its direction to a random vector
    // so that he ball is launched towards a player side,
    // determined by parameter <launch_direction> (0 for left, 1 for right)
    reset(launch_direction) {
        let angle_radians;
        this.position.x = this.initial_position.x;
        this.position.y = this.initial_position.y;
        angle_radians = Math.PI / 4 + Math.random() * Math.PI / 2; // angle between 45deg and 135deg
        this.direction.x = Math.sin(angle_radians); // always positive
        this.direction.y = Math.cos(angle_radians); // may be negative
        this.direction.normalize();
        if (launch_direction == 0) // if ball should be launched towards player 0's side
            this.direction.x = -1 * this.direction.x;
        /* FOR TESTS
        this.position.x = 150;
        this.position.y = 75;
        this.direction.x = -1;
        this.direction.y = 0.5;
        this.direction.normalize();
        */
        this.speed = this.initial_speed;
        this.recursion_counter = 0;
    }
    // Slightly adjusts the direction towards the horizontal (vectors [1,0] or [-1,0])
    // if the direction is too vertical (which would slow the gameplay)
    adjust_direction() {
        let side;
        if (Math.abs(this.direction.y / this.direction.x) < max_direction_horiz)
            return;
        if (this.direction.x < 0)
            side = -1;
        else
            side = 1;
        this.direction = vec2_add(this.direction.scale(9), new Vec2(side, 0));
        this.direction.normalize();
    }
    // Teleports ball outside paddle when its center is inside the paddle rectangle
    teleport_outside_paddle(collision_zone, paddle_index) {
        let paddle = this.game.paddles[paddle_index];
        if (collision_zone == 1)
            this.position.y = paddle.position.y - paddle.height / 2 - 1;
        if (collision_zone == 3)
            this.position.x = paddle.position.x - paddle.width / 2 - 1;
        if (collision_zone == 5)
            this.position.y = paddle.position.y + paddle.height / 2 + 1;
        if (collision_zone == 7)
            this.position.x = paddle.position.x + paddle.width / 2 + 1;
        console.log(`updating ball : ball teleported from outside paddle`);
    }
    // Checks if ball's next position would intersect a wall or a paddle,
    // if so transforms the ball's direction vector and returns value indicating collision :
    // 		- 0 : collision with left wall (player 0 loses point)
    // 		- 1 : collision with right wall (player 1 loses point)
    // 		- 2 : no collision
    // 		- 3 : collision with top wall / bottom wall
    // 		- 4 : collision with left / right paddle
    determine_collision() {
        let normal_vector = null;
        // Collision with walls left and right
        if (this.position.x <= 0) // left wall
            return (0);
        else if (this.position.x >= this.game.width) // right wall
            return (1);
        // Collision with walls top and bottom
        if (this.position.y - this.radius - 1 <= 0) // top wall
            normal_vector = new Vec2(wall_normal_vectors.top[0], wall_normal_vectors.top[1]);
        if (this.position.y + this.radius + 1 >= this.game.height) // bottom wall
            normal_vector = new Vec2(wall_normal_vectors.bottom[0], wall_normal_vectors.bottom[1]);
        if (normal_vector != null && vec2_sproduct(normal_vector, this.direction) < 0) // ensure that ball is going TOWARDS wall
         {
            this.direction = vec2_symmetry(this.direction, normal_vector);
            this.direction.normalize();
            console.log(`updating ball : collision with top/bottom wall, new direction : ${this.direction.x},${this.direction.y}`);
            return (3);
        }
        // Collision with paddles
        return (this.determine_collision_with_paddles());
    }
    // Checks if ball collides with one of the paddles,
    // if so, computes the normal of the collided surface and modifies ball's direction accordingly
    determine_collision_with_paddles() {
        let normal_vector = new Vec2(0, 0);
        let collision_zone = -1;
        let paddle;
        // Go through paddles to check if one of them collides with the ball
        for (paddle = 0; paddle < 2; paddle++) {
            collision_zone = this.game.paddles[paddle].check_collision_with_ball(this);
            if (collision_zone >= 9) // special case where ball is INSIDE paddle -> should be teleported
             {
                this.teleport_outside_paddle(collision_zone - 9, paddle);
                collision_zone -= 9;
            }
            if (collision_zone != -1)
                break;
        }
        if (collision_zone == -1)
            return (2);
        // Manage case where a paddle collides with the ball : modify ball's direction correctly
        normal_vector = this.compute_normal_at_collision_point(collision_zone, paddle);
        console.log(`updating ball : ball collision with paddle ${paddle} in zone ${collision_zone}, \
			normal vector : ${normal_vector.x}, ${normal_vector.y}, \
			current direction : ${this.direction.x},${this.direction.y}`);
        if (vec2_sproduct(this.direction, normal_vector) > 0) // case where the ball is going AWAY FROM the paddle
         {
            console.log(`   -> direction not modified`);
            return (2);
        }
        this.direction = vec2_symmetry(this.direction, normal_vector);
        this.direction.normalize();
        console.log(`   -> new direction : ${this.direction.x},${this.direction.y}`);
        return (4);
    }
    // Computes the normal vector of the paddle at collision point :
    // 	 - in cases of collision outside of expected edge of paddle,
    // 	   normal is simply a vertical or horizontal vector
    // 	 - only if collision on the expected edge of paddles (edge 3 for paddle 1, edge 5 for paddle 0),
    // 	   then normal is modified to emulate a curvy surface, allowing control over ball trajectory
    // 			(calculation of normal uses the slope of the curve defined by circle equation : `(x+y)**0.5==1`)
    compute_normal_at_collision_point(collision_zone, paddle) {
        let normal_vector = new Vec2(0, 0);
        if ((collision_zone == 5 && paddle == 0) || (collision_zone == 3 && paddle == 1)) {
            let paddle_y = this.game.paddles[paddle].position.y - this.game.paddles[paddle].height / 2;
            let pos_on_paddle = (this.position.y - paddle_y) / this.game.paddles[paddle].height; // should be between 0 and 1
            pos_on_paddle = pos_on_paddle * (1 / 2) ** 0.5 * 2 - (1 / 2) ** 0.5; // should be between -0.707 and +0.707
            let slope_at_pos = slope_on_circle(pos_on_paddle);
            if (paddle == 0) {
                normal_vector.x = 1;
                normal_vector.y = -1 * slope_at_pos;
            }
            else {
                normal_vector.x = -1;
                normal_vector.y = -1 * slope_at_pos;
            }
            console.log(`collision on curvy surface, normal at collision point : ${normal_vector.x}, ${normal_vector.y} \
						with pos_on_paddle ${pos_on_paddle}, slope_at_pos ${slope_at_pos}`);
        }
        else {
            normal_vector.x = paddle_normal_vectors[collision_zone][0];
            normal_vector.y = paddle_normal_vectors[collision_zone][1];
        }
        normal_vector.normalize();
        return (normal_vector);
    }
    // Moves the ball according to its speed and direction vector
    // (using <delta_time> to adjust the distance moved according to the time elapsed since last tick
    //  because sometimes the <delta_time> between two <simulation_tick> won't be exactly <sim_tick_interval>)
    // 	 - if the ball does not collide anything,
    // 	   moves the ball and draws it at its new position, then returns 2
    // 	 - if the ball collides the left or right wall, meaning a player lost a point,
    // 	   returns 0 (left) or 1 (right) without redrawing the ball
    // 	 - if the ball collides a paddle or the top or bottom wall,
    // 	   lets <Ball.determine_collision> transform the direction vector
    // 			and recurs (returning the return value of the recursion)
    // (The function should only be able to recur twice/thrice,
    //  eg. when ball hits top wall, then hits paddle, then possibly wall again,
    //  then should have a normal vector such that it goes away from both paddle and wall)
    update(delta_time) {
        let collision;
        let new_pos = new Vec2(0, 0);
        let time_modifier = delta_time / simulation_tick_interval;
        //console.log("updating ball");
        //this.testify();
        collision = this.determine_collision();
        if (collision == 2) // if no collision, redraw ball at new position
         {
            new_pos = vec2_add(this.position, this.direction.scaled(this.speed * time_modifier));
            this.position.x = new_pos.x;
            this.position.y = new_pos.y;
            this.speed += ball_acceleration; // ball accelerates a small amount
            this.recursion_counter = 0;
            return (2);
        }
        else if (collision == 0 || collision == 1) // if gone through left/right wall, end the point
         {
            //console.log(`updating ball : ball hit left/right wall`);
            this.recursion_counter = 0;
            return (collision);
        }
        else // if collision with paddle or top/bottom wall, recur with new direction
         {
            if (this.recursion_counter <= 4) {
                this.recursion_counter++;
                console.log(`updating ball : ball hit paddle or top/bottom wall, recurring with direction ${this.direction.x},${this.direction.y}`);
                return (this.update(delta_time));
            }
            else // infinite recursion should not happen, this is for precaution 
             {
                console.log("Max recursion count reached, ending point");
                this.recursion_counter = 0;
                if (this.position.x > this.game.width / 2)
                    return (1);
                else
                    return (0);
            }
        }
    }
}
// The pong game holding the instances of <Paddle> and <Ball> and running the game loop
class Pong_game {
    // GAME SETUP
    // Constructor does not affect game state variable :
    // they will be defined when there are 2 players in the game and it must start
    constructor() {
        this.player_ws_ids = [];
        this.players_input = []; // 2 objects storing input received from players
        // Game state
        this.active = false; // whether game is temporarily stopped (paused)
        this.concession = -1; // index of a conceding player (-1 means no player conceded)
        this.last_time = -1;
        this.points = [0, 0];
        this.ball = null;
        this.paddles = [];
        // Dimensions
        this.width = canvas_width;
        this.height = canvas_height;
        console.log("New Pong Game object created");
        this.full = false;
        this.terminated = false;
    }
    // Called when a client connected to websocket server to enter a game,
    // registers the client's websocket identity
    // and starts the game if there are now 2 players in the game
    add_player_websocket(ws_id) {
        if (this.full) // no player may be added if game is already full
            return;
        this.player_ws_ids.push(ws_id);
        if (this.player_ws_ids.length == 2) {
            this.full = true;
            this.active = true;
            this.setup_game();
            this.broadcast_to_players_init(); // send setup information to clients
            setTimeout(() => this.simulation_tick(), 1000); // 1s delay before game start
            setTimeout(() => this.broadcast_tick(), 1000);
        }
    }
    // Instantiates paddles and ball objects, which will make position and movement calculations,
    // and initializes all client commands to false for the two players
    setup_game() {
        this.ball = new Ball(this, canvas_width / 2, canvas_height / 2, ball_radius, ball_initial_speed);
        this.paddles.push(new Paddle(this, 0, paddle_margin, canvas_height / 2, paddle_width, paddle_height, paddle_speed));
        this.paddles.push(new Paddle(this, 1, canvas_width - paddle_margin, canvas_height / 2, paddle_width, paddle_height, paddle_speed));
        this.players_input.push({ up: false, down: false, play: false, pause: false, concede: false });
        this.players_input.push({ up: false, down: false, play: false, pause: false, concede: false });
    }
    // Called by <Pong_server> when the Websocket of a player in the game becomes closed,
    // returns the index of the websocket of the other player (the "counterpart")
    // and sets the game to state 'terminated'
    end_by_disconnect(disconnected_player_index) {
        let counterpart_index = 1 - disconnected_player_index;
        this.terminated = true;
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
        input = JSON.parse(msg.toString());
        if (this.concession == -1 && input.concede)
            this.concession = player;
        if (this.active && input.pause)
            this.active = false;
        if (!(this.active) && input.play)
            this.active = true;
        this.players_input[player] = input;
    }
    // Sends setup information to the players through websockets
    broadcast_to_players_init() {
        let player;
        let game_init_string;
        let gid = {
            player_index: -1,
            broadcast_interval: broadcast_tick_interval,
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
            active: this.active
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
        // (1) register current time
        if (this.last_time == -1) // case where this is the first call of <game_mainloop>
            delta_time = 10;
        else
            delta_time = current_time - this.last_time;
        console.log(`--- Simulation tick with delta ${delta_time}, game active : ${this.active}`);
        this.last_time = current_time;
        // (2) check if a player conceded
        if (this.concession != -1) {
            this.active = false;
            // #f : cleanly terminate game
        }
        // (3) Update positions if game is not paused
        if (this.active)
            this.update_ball_and_paddles(delta_time);
        // (4) call itself in 10ms
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
////////////////////////////////
// SERVER HANDLING WEBSOCKETS //
////////////////////////////////
// Server has 2 roles :
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
    // Handles incoming HTTP request : serves files whem request method is GET
    async request_handler(req, res) {
        console.log(`Received new request : ${req.method}, ${req.url}`);
        if (!("upgrade" in req.headers && req.headers["upgrade"] == "websocket") && req.method == "GET") {
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
            this.waiting_game = new Pong_game();
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
        else if (game.terminated) {
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
/*

+ souci de double close sur les sockets d'une partie qui se termine -> flag interne a Game pour partie terminee
+ impossible de splice-remove dans la liste des WS / des Games, puisque les index sont en dur dans les ws_id
    -> plutot utiliser une map avec des keys qu'on peut ajouter/retirer tranquillement
+ ameliorations souhaitables :
    + decouplage ticks de simulation / ticks de broadcast
    + laisser le client faire l'interpolation dans des animframes

- placer les constantes et interfaces dans un fichier JSON qui sera partage par serveur et client
- retester le redressement de trajectoire de la balle
- enlever (dans le code server et le code client) les references hard-codees aux constantes,
    les remplacer par des attributs de classe
- au lieu d'avoir plusieurs booleens, utiliser une seule variable de status de la partie :
    waiting | starting | active | paused | terminated

- pour le resume global :
    \ clarifier player/client et gestion du websocket par Websocketserver et ses instances de Pong_game
    \ bien distinguer ticks de simulation / ticks de broadcast / frames de browser
*/
