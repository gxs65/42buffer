// --- A module defining classes for game objects Ball and Paddle in a Pong game
// 	   (classes serve to store data about objects and make move and collision computations)

import { Client_commands } from "./pong_interfaces.js";
import { Vec2, vec2_add, vec2_slope_between_two_points,
		 vec2_sproduct, vec2_symmetry,
		 slope_on_circle } from "./pong_utils.js";
import type { Pong_game } from "./pong_server.js";

// CONSTANTS

const ball_acceleration = 0.0001;
const max_direction_horiz = 2.5; // maximal vertical slope of ball's trajectory
const paddle_normal_vectors =
				[	[-0.5, -0.5], 	[0, -1], 	[0.5, -0.5],
					[-1, 0], 		[0, 0], 	[1, 0],
					[-0.5, 0.5], 	[0, 1], 	[0.5, 0.5]];
const wall_normal_vectors = {top: [0, 1], bottom: [0, -1]};

// PADDLE

// A rectangular paddle that can be moved by one player and block the ball
export class Paddle
{
	game: Pong_game;
	simulation_tick_interval: number;
	position: Vec2;
	position_upperleft_corner: Vec2 = new Vec2(0, 0);
	position_upperright_corner: Vec2 = new Vec2(0, 0);
	index: number;
	width: number;
	height: number;
	speed: number;
	moving: -1 | 0 | 1;
	diag_slope: number = 0;

	constructor(game: Pong_game, index: number, simulation_tick_interval: number,
				initial_x: number, initial_y: number, width: number, height: number, speed: number)
	{
		this.game = game;
		this.simulation_tick_interval = simulation_tick_interval;
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
	compute_side_attributes()
	{
		this.position_upperleft_corner = vec2_add(this.position, new Vec2(-(this.width / 2), -(this.height / 2)));
		this.position_upperright_corner = vec2_add(this.position, new Vec2((this.width / 2), -(this.height / 2)));
		this.diag_slope = this.height / this.width;
	}

	// Log function
	testify()
	{
		console.log(`Paddle dim ${this.width},${this.height}, pos ${this.position.x},${this.position.y}, moving ${this.moving}`);
		//console.log(`with diag slope ${this.diag_slope}, UL corner ${this.position_upperleft_corner.x},${this.position_upperleft_corner.y},
		//	UR corner ${this.position_upperright_corner.x},${this.position_upperright_corner.y}`)
	}

	// Determines if paddle must be moving, according to keys pressed by player as given by <input>
	// 		- no key pressed / both 'up' and 'down' keys pressed : no move
	// 		- only one of 'up' and 'down' keys pressed : move in that direction
	process_keys(input: Client_commands)
	{
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
	check_collision_with_ball_unoriented(ball: Ball) : boolean
	{
		let abs_ball_pos = new Vec2(0, 0); // position of ball center in an orthonormal basis centered on paddle center
		let distance_to_corner: number;

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
	check_collision_with_ball(ball: Ball): number
	{
		let x_zone: number;
		let y_zone: number;
		let full_zone: number;

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
			let under_diag1: boolean = vec2_slope_between_two_points(ball.position, this.position_upperleft_corner) < this.diag_slope;
			let under_diag2: boolean = vec2_slope_between_two_points(ball.position, this.position_upperright_corner) * -1 < this.diag_slope;
			full_zone = 9 + 7 - 2 * +!!under_diag1 - 4 * +!!under_diag2;
			console.log(`collision within paddle (ud1 ${under_diag1}, ud2 ${under_diag2} -> full zone ${full_zone})`);
		}
		return (full_zone);
	}

	// Checks if the given <ball>'s center is within the abcissa of the paddle
	ball_in_range(ball: Ball): boolean
	{
		return (ball.position.x >= (this.position.x - this.width / 2)
				&& ball.position.x <= (this.position.x + this.width / 2));
	}

	// Moves the paddle if its <moving> property is -1 (moving up) or 1 (moving down)
	// (using <delta_time> to adjust the distance moved according to the time elapsed since last tick
	//  because sometimes the <delta_time> between two <simulation_tick> won't be exactly <sim_tick_interval>)
	// Checks before moving :
	// 		- that the paddle is not going beyond top wall / below bottom wall
	// 		- that paddle is not going "into" the ball if ball is between paddle and top/bottom wall
	update(delta_time: number, ball: Ball)
	{
		let time_modifier = delta_time / this.simulation_tick_interval;
		let ball_in_range: boolean = this.ball_in_range(ball);
		let next_paddle_topy = this.position.y - this.speed * time_modifier - this.height / 2;
		let next_paddle_bottomy = this.position.y + this.speed * time_modifier + this.height / 2;
	
		if (this.moving == -1 && next_paddle_topy > 0) // check : not beyond top wall
		{
			if (!(ball_in_range && ball.position.y < this.position.y // check : ball not between paddle and top wall
				&& next_paddle_topy <= 2 * ball.radius))
				this.position.y -= 1 * this.speed * time_modifier; // moving up
		}
		else if (this.moving == 1 && next_paddle_bottomy < this.game.playground_height) // check : not beyond bottom wall
		{
			if (!(ball_in_range && ball.position.y > this.position.y // check : ball not between paddle and bottom wall
				&& this.game.playground_height - next_paddle_bottomy <= 2 * ball.radius))
				this.position.y += 1 * this.speed * time_modifier; // moving down
		}
	}
}

// BALL

// A ball that moves and collides walls and paddles until it goes through left or right wall
export class Ball
{
	game: Pong_game;
	simulation_tick_interval: number;
	radius: number;
	initial_position: Vec2;
	initial_speed: number;
	position: Vec2 = new Vec2(0, 0);
	direction: Vec2 = new Vec2(0, 0); // a unit vector
	speed: number = 0; // a scaler on the direction vector
	recursion_counter: number = 0;
	drawn: boolean;

	constructor(game: Pong_game, simulation_tick_interval: number,
				initial_x: number, initial_y: number, radius: number, initial_speed: number)
	{
		this.game = game;
		this.simulation_tick_interval = simulation_tick_interval;
		this.radius = radius;
		this.initial_position = new Vec2(initial_x, initial_y); // initial position stored to be able to reset
		this.initial_speed = initial_speed;
		this.drawn = false;
		this.reset(0);
		this.testify();
	}

	// Log function
	testify()
	{
		console.log(`Ball at pos ${this.position.x},${this.position.y} with direction ${this.direction.x},${this.direction.y} and speed ${this.speed}`);
	}

	// Puts ball in initial position and sets its direction to a random vector
	// so that he ball is launched towards a player side,
	// determined by parameter <launch_direction> (0 for left, 1 for right)
	reset(launch_direction: 0 | 1)
	{
		let angle_radians: number;
	
		this.position.x = this.initial_position.x;
		this.position.y = this.initial_position.y;
		angle_radians = Math.PI / 4 + Math.random() * Math.PI / 2; // angle between 45deg and 135deg
		this.direction.x = Math.sin(angle_radians); // always positive
		this.direction.y = Math.cos(angle_radians); // may be negative
		this.direction.normalize();
		if (launch_direction == 0) // if ball should be launched towards player 0's side
			this.direction.x = -1 * this.direction.x;
		this.speed = this.initial_speed;
		this.recursion_counter = 0;
	}

	// Slightly adjusts the direction towards the horizontal (vectors [1,0] or [-1,0])
	// if the direction is too vertical (which would slow the gameplay)
	adjust_direction()
	{
		let side: number;
	
		if (Math.abs(this.direction.y / this.direction.x) < max_direction_horiz)
			return ;
		if (this.direction.x < 0)
			side = -1;
		else
			side = 1;
		this.direction = vec2_add(this.direction.scale(9), new Vec2(side, 0));
		this.direction.normalize();
	}
	
	// Teleports ball outside paddle when its center is inside the paddle rectangle
	teleport_outside_paddle(collision_zone: number, paddle_index: number)
	{
		let paddle: Paddle = this.game.paddles[paddle_index];

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
	determine_collision(): 0 | 1 | 2 | 3 | 4
	{
		let normal_vector: Vec2 | null = null;
	
		// Collision with walls left and right
		if (this.position.x <= 0) // left wall
			return (0);
		else if (this.position.x >= this.game.playground_width) // right wall
			return (1);
		
		// Collision with walls top and bottom
		if (this.position.y - this.radius - 1 <= 0) // top wall
			normal_vector = new Vec2(wall_normal_vectors.top[0], wall_normal_vectors.top[1]);
		if (this.position.y + this.radius + 1 >= this.game.playground_height) // bottom wall
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
	determine_collision_with_paddles(): 2 | 4
	{
		let normal_vector = new Vec2(0, 0);
		let collision_zone: number = -1;
		let paddle: number;
	
		// Go through paddles to check if one of them collides with the ball
		for(paddle = 0; paddle < 2; paddle++)
		{
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
	compute_normal_at_collision_point(collision_zone: number, paddle: number): Vec2
	{
		let normal_vector = new Vec2(0, 0);

		if ((collision_zone == 5 && paddle == 0) || (collision_zone == 3 && paddle == 1))
		{
			let paddle_y = this.game.paddles[paddle].position.y - this.game.paddles[paddle].height / 2;
			let pos_on_paddle = (this.position.y -  paddle_y) / this.game.paddles[paddle].height; // should be between 0 and 1
			pos_on_paddle = pos_on_paddle * (1/2)**0.5 * 2 - (1/2)**0.5; // should be between -0.707 and +0.707
			let slope_at_pos = slope_on_circle(pos_on_paddle);
			if (paddle == 0)
			{
				normal_vector.x = 1;
				normal_vector.y = -1 * slope_at_pos;
			}
			else
			{
				normal_vector.x = -1;
				normal_vector.y =  -1 * slope_at_pos;
			}
			console.log(`collision on curvy surface, normal at collision point : ${normal_vector.x}, ${normal_vector.y} \
						with pos_on_paddle ${pos_on_paddle}, slope_at_pos ${slope_at_pos}`);
		}
		else
		{
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
	update(delta_time: number): 0 | 1 | 2
	{
		let collision: number;
		let new_pos = new Vec2(0, 0);
		let time_modifier = delta_time / this.simulation_tick_interval;

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
			if (this.recursion_counter <= 4)
			{
				this.recursion_counter++;
				console.log(`updating ball : ball hit paddle or top/bottom wall, recurring with direction ${this.direction.x},${this.direction.y}`);
				return (this.update(delta_time));
			}
			else // infinite recursion should not happen, this is for precaution 
			{
				console.log("Max recursion count reached, ending point");
				this.recursion_counter = 0;
				if (this.position.x > this.game.playground_width / 2)
					return (1);
				else
					return (0);
			}
		}
	}
}
