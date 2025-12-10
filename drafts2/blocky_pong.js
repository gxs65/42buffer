"use strict";
// Configuration constants
const canvas_width = 300;
const canvas_height = 150;
const paddle_margin = 10;
const paddle_width = 10;
const paddle_height = 40;
const paddle_speed = 4;
const paddle_colors = ["rgb(200 0 0)", "rgb(0 0 200)"];
const ball_radius = 4;
const ball_initial_speed = 2;
const ball_acceleration = 0.0001;
const max_direction_horiz = 5;
const keys_by_player = [["KeyW", "KeyS"], ["ArrowUp", "ArrowDown"]];
const paddle_normal_vectors = [[-0.5, -0.5], [0, -1], [0.5, -0.5],
    [-1, 0], [0, 0], [1, 0],
    [-0.5, 0.5], [0, 1], [0.5, 0.5]];
const wall_normal_vectors = { top: [0, 1], bottom: [0, -1] };
// Utility functions
// Returns the slope of the circular line with radius centered on (0, 0) defined by `y**2 + x**2 == 1`
// (the simplified expression is : `f(x) = (1 - x**2)**0.5`, therefore derivative is `f'(x) = -x / (1 - x**2)**0.5`)
// (<abcissa> should be between -1 and 1, the endpoints of the circular line)
function slope_on_circle(abcissa) {
    return (-1 * abcissa / (1 - abcissa * abcissa) ** 0.5);
}
// Vector manipulation (class + functions for operations, not operator overload)
class Point {
    x;
    y;
    constructor(x = 0, y = 0) {
        this.x = x;
        this.y = y;
    }
}
// 2-dimensional vector class
class Vec2 {
    x;
    y;
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
// Paddle class
class Paddle {
    game;
    position;
    position_upperleft_corner = new Vec2(0, 0);
    position_upperright_corner = new Vec2(0, 0);
    index;
    width;
    height;
    speed;
    moving;
    diag_slope = 0;
    constructor(game, index, initial_x, initial_y, width, height, speed) {
        this.game = game;
        this.width = width;
        this.height = height;
        this.position = new Vec2(initial_x, initial_y); // position of center of paddle rectangle
        this.compute_side_attributes();
        this.index = index; // which player the paddle belongs to
        this.speed = speed; // number of pixels the paddle can cross in 1 frame (#f)
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
    // Processes a keyboard event indicating that player wants to affect their paddle :
    // 		- event 'ArrowUpd' <=> 'ArrowUp' pressed <=> paddle of player starts moving
    // 		- event 'ArrowUpu' <=> 'ArrowUp' released <=> paddle of player stops moving
    process_keyevent(code) {
        let move_direction;
        let key_released; // whether the key was released, or pressed
        key_released = (code.slice(code.length - 1)) == "r";
        // Ignore cases where key released while no key pressed / key pressed while key already pressed
        if (key_released && this.moving == 0)
            return;
        if (!key_released && this.moving != 0)
            return;
        // Retrieve movement direction given by key (up or down)
        if (code.slice(0, code.length - 1) == keys_by_player[this.index][0])
            move_direction = -1;
        else
            move_direction = 1;
        // Ignore cases where key released does not match key pressed
        if (key_released && this.moving != move_direction)
            return;
        // Modify paddle's <moving> property
        if (key_released)
            this.moving = 0;
        else
            this.moving = move_direction;
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
    // (using <delta_time> to adjust the distance moved according to the time elapsed since last frame)
    // then redraws the paddle at the new position
    // Checks before moving :
    // 		- that the paddle is not going beyond top wall / below bottom wall
    // 		- that paddle is not going "into" the ball if ball is between paddle and top/bottom wall
    update(delta_time, ball) {
        let ctx = this.game.ctx;
        let time_modifier = delta_time / 16; // on a monitor with 60 frames/seconds rate, this should be circa 1
        let ball_in_range = this.ball_in_range(ball);
        let next_paddle_topy = this.position.y - this.speed * time_modifier - this.height / 2;
        let next_paddle_bottomy = this.position.y + this.speed * time_modifier + this.height / 2;
        if (this.moving == -1 && next_paddle_topy > 0) // check : not beyond top wall
         {
            if (!(ball_in_range && ball.position.y < this.position.y // check : ball not between paddle and top wall
                && next_paddle_topy <= 2 * ball.radius))
                this.position.y -= 1 * this.speed * time_modifier; // moving up
        }
        else if (this.moving == 1 && next_paddle_bottomy < this.game.canvas.height) // check : not beyond bottom wall
         {
            if (!(ball_in_range && ball.position.y > this.position.y // check : ball not between paddle and bottom wall
                && this.game.canvas.height - next_paddle_bottomy <= 2 * ball.radius))
                this.position.y += 1 * this.speed * time_modifier; // moving down
        }
        ctx.fillStyle = paddle_colors[this.index];
        ctx.fillRect(this.position.x - this.width / 2, this.position.y - this.height / 2, this.width, this.height);
    }
}
// Ball class
class Ball {
    game;
    radius;
    initial_position;
    initial_speed;
    position = new Vec2(0, 0);
    direction = new Vec2(0, 0); // a unit vector
    speed = 0; // a scaler on the direction vector
    recursion_counter = 0;
    drawn;
    constructor(game, initial_x, initial_y, radius, initial_speed) {
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
    // if the direction is too vertical (which would slow the game)
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
        else if (this.position.x >= this.game.canvas.width) // right wall
            return (1);
        // Collision with walls top and bottom
        if (this.position.y - this.radius - 1 <= 0) // top wall
            normal_vector = new Vec2(wall_normal_vectors.top[0], wall_normal_vectors.top[1]);
        if (this.position.y + this.radius + 1 >= this.game.canvas.height) // bottom wall
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
    // 	 - in general case, normal is simply a vertical or horizontal vector
    // 	 - only if collision on the expected edge of paddles (edge 3 for paddle 1, edge 5 for paddle 0),
    // 	   then normal is modified to emulate a curvy surface, allowing control over ball trajectory
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
    // (using <delta_time> to adjust the distance moved according to the time elapsed since last frame)
    // - if the ball does not collide anything,
    // 	 moves the ball and draws it at its new position, then returns 2
    // - if the ball collides the left or right wall,
    // 	 returns 0 (left) or 1 (right) without redrawing the ball
    // - if the ball collides a paddle or the top or bottom wall,
    // 	 lets <Ball.determine_collision> transform the direction vector
    // 		and recurs (returning the return value of the recursion)
    // (The function should only be able to recur twice/thrice,
    //  eg. when ball hits top wall, then hits paddle, then possibly wall again,
    //  then should have a normal vector such that it goes away from both paddle and wall)
    update(delta_time) {
        let collision;
        let new_pos = new Vec2(0, 0);
        let time_modifier = delta_time / 16; // on a monitor with 60 frames/seconds rate, this should be circa 1
        //console.log("updating ball");
        //this.testify();
        collision = this.determine_collision();
        if (collision == 2) // if no collision, redraw ball at new position
         {
            new_pos = vec2_add(this.position, this.direction.scaled(this.speed * time_modifier));
            this.position.x = new_pos.x;
            this.position.y = new_pos.y;
            this.draw();
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
                this.game.running = false; // FOR TESTS
                if (this.position.x > this.game.canvas.width / 2)
                    return (1);
                else
                    return (0);
            }
        }
    }
    // Draws the black ball at the current position
    draw() {
        let ctx = this.game.ctx;
        ctx.strokeStyle = "rgb(0 0 0)";
        ctx.beginPath();
        ctx.arc(this.position.x, this.position.y, this.radius, 0, Math.PI * 2, false);
        ctx.stroke();
    }
}
// Pong Game class
class Pong_game {
    // Game state
    running;
    points;
    ball = null;
    paddles = [];
    animation_events = [];
    // Canvas graphics
    last_time;
    frame_counter;
    canvas = null;
    ctx = null;
    // HTML page elements
    points_info = null;
    play_button = null;
    stop_button = null;
    tick_button = null;
    // SETUP
    // Constructor : Game starts NOT running
    constructor() {
        this.running = false;
        this.points = [0, 0];
        this.extract_HTML_elements();
        this.prepare_canvas();
        this.create_event_listeners();
        this.last_time = -1;
        this.frame_counter = 0;
        this.animation_events = [];
    }
    // Extract elements from HTML DOM
    extract_HTML_elements() {
        this.canvas = document.querySelector("#canvas");
        if (!this.canvas)
            throw new Error("Invalid Canvas");
        this.ctx = this.canvas.getContext("2d");
        if (!this.ctx)
            throw new Error("Invalid Context");
        this.points_info = document.querySelector("#pointsInfo");
        this.play_button = document.querySelector("#play");
        this.stop_button = document.querySelector("#stop");
        this.tick_button = document.querySelector("#tick");
        if (!this.points_info || !this.play_button || !this.stop_button || !this.tick_button)
            throw new Error("Invalid HTML");
        this.play_button.addEventListener("click", () => { this.launch(); });
        this.stop_button.addEventListener("click", () => { this.pause(); });
        this.tick_button.addEventListener("click", () => { this.update_by_tick(); });
    }
    // Instantiate 2 paddles and 1 ball (not drawn yet)
    prepare_canvas() {
        this.ball = new Ball(this, canvas_width / 2, canvas_height / 2, ball_radius, ball_initial_speed);
        this.paddles.push(new Paddle(this, 0, paddle_margin, canvas_height / 2, paddle_width, paddle_height, paddle_speed));
        this.paddles.push(new Paddle(this, 1, canvas_width - paddle_margin, canvas_height / 2, paddle_width, paddle_height, paddle_speed));
    }
    // Listen to keyboard events for player keys
    create_event_listeners() {
        document.addEventListener('keydown', (event) => { this.store_key_event(event, false); });
        document.addEventListener('keyup', (event) => { this.store_key_event(event, true); });
    }
    // Launch game by calling a first animation update
    launch() {
        if (this.running == false) {
            this.running = true;
            requestAnimationFrame(this.update_game_state);
        }
    }
    // Pause game (can be restarted by pressing "launch" button)
    pause() {
        if (this.running == true) {
            this.running = false;
            this.last_time = -1;
        }
    }
    // Performs 1 game tick when tick button is clicked (while the game is not running)
    update_by_tick() {
        if (!this.running) {
            console.log("Tick update");
            this.update_game_state(1);
        }
    }
    // GAME MAINLOOP
    // Stores keyboard events (players trying to move their paddle)
    // to be analyzed when generating next frame
    store_key_event(event, key_released) {
        var code = event.code;
        if (!this.running)
            return;
        if (keys_by_player[0].includes(code) || keys_by_player[1].includes(code)) {
            event.preventDefault();
            if (key_released)
                this.animation_events.push(code + "r"); // "r" for "released"
            else
                this.animation_events.push(code + "p"); // "p" for "pressed"
        }
    }
    // Prepares a new game frame :
    // 		- sends keyboard events that were received since last frame to be processed by paddle instances
    // 		- erases all drawings on the canvas
    // 		- updates ball and paddles
    // 		- calls itself again with <requestAnimationFrame> (as long as <running> is true)
    // (Method declaration with the syntax `funcname = (paraname) => {funcbody}`,
    //  because it seems to be the only way to avoid issues with call by `requestAnimationFrame`)
    update_game_state = (current_time) => {
        let delta_time;
        let ball_out_of_bounds;
        let losing_player_index;
        // Check if "pause" button was pressed since last frame
        if (!this.running || this.ball == null || this.canvas == null)
            return;
        // Register current time
        this.frame_counter++;
        if (this.last_time == -1) // case where this frame is first frame after "launch" button was pressed
            delta_time = 16; // 16 milliseconds, delta between 2 frames on the usual 60 frames/second setup
        else
            delta_time = current_time - this.last_time;
        console.log(`--- Frame ${this.frame_counter}, current time ${current_time}, delta ${delta_time}`);
        this.last_time = current_time;
        // Go through keyboard events received since last frame
        for (let i = 0; i < this.animation_events.length; i++) {
            let code = this.animation_events[i];
            //console.log("- processing event : " + code);
            if (keys_by_player[0].includes(code.slice(0, code.length - 1)))
                this.paddles[0].process_keyevent(code);
            else if (keys_by_player[1].includes(code.slice(0, code.length - 1)))
                this.paddles[1].process_keyevent(code);
        }
        this.animation_events = []; // emptying event list
        // Update paddles and ball
        this.ctx?.clearRect(0, 0, this.canvas.width, this.canvas.height);
        this.paddles[0].update(delta_time, this.ball);
        this.paddles[1].update(delta_time, this.ball);
        ball_out_of_bounds = this.ball.update(delta_time);
        if (ball_out_of_bounds == 0 || ball_out_of_bounds == 1) // when ball went through left/right wall
         {
            losing_player_index = ball_out_of_bounds; // index of player who lost point
            this.points[1 - losing_player_index] += 1; // other player gains 1 point
            this.points_info.innerHTML = "Points : " + this.points[0] + "-" + this.points[1];
            this.ball.reset(losing_player_index);
        }
        // Slightly adjust ball direction if it is too vertical (and away from paddles)
        if (!this.paddles[0].ball_in_range(this.ball) && !this.paddles[1].ball_in_range(this.ball))
            this.ball.adjust_direction();
        if (this.running)
            requestAnimationFrame(this.update_game_state);
    };
    // OTHER
    // Function for ad-hoc tests
    tests() {
        this.paddles[0].position.x = 50;
        this.paddles[0].position.y = 50;
        this.paddles[0].compute_side_attributes();
        this.ball.position.x = 53;
        this.ball.position.y = 52;
        this.ball.radius = 8;
        let res = this.paddles[0].check_collision_with_ball(this.ball);
        console.log(`Result of collision test : ${res}`);
    }
}
function setup() {
    let game = new Pong_game();
    //game.tests();
}
window.onload = setup;
/*


+ tenir compte du temps passe depuis la derniere frame dans update_game_state
+ transformer la surface des paddles en demi-cercles
+ creer un petit mecanisme de rectification de la direction de la balle quand elle est trop verticale
- creer un speed bump artificiel de 1 frame pour que la balle puisse s'extraire du paddle quand elle le touche
    (pas forcement seulement en cas de collision modifiant la direction de la balle)
*/
/*
I am experimenting with typescript by creating a small game using an HTML canvas. I define a small <Point> class at the beginning of my typescript program :

Unexpectedly, when I try to run my program in my browser, I get an error "Uncaught SyntaxError: bad class member definition" on the line `x:number`.
I am very surprised, since I did not find any reference to that error online, and my Point class is exactly the same as in the typescript documentation example.
*/
