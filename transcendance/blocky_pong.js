// Configuration constants

const canvas_width = 300;
const canvas_height = 150;
const paddle_margin = 10;
const paddle_width = 10;
const paddle_height = 20;
const paddle_speed = 4;
const paddle_colors = ["rgb(200 0 0)", "rgb(0 0 200)"];
const ball_radius = 4;
const ball_initial_speed = 2;
const keys_by_player = [["KeyW", "KeyS"],
					["ArrowUp","ArrowDown"]];

// Vector manipulation
// (a class, and also functions for operations since JS does not support operator overload)

// 2-dimensional vector class
class Vec2
{
	constructor(x, y)
	{
		this.x = x;
		this.y = y;
	}

	get length()
	{
		return (Math.sqrt(this.x * this.x + this.y * this.y));
	}

	scale(scaler)
	{
		this.x = scaler * this.x;
		this.y = scaler * this.y;
		return (this);
	}

	scaled(scaler)
	{
		let vec_scaled = new Vec2(scaler * this.x, scaler * this.y);
		return (vec_scaled);
	}

	normalize()
	{
		let length = this.length;
		if (length != 1)
			this.scale(1 / length);
		return (this);
	}
}

// Addition of two vectors
function vec2_add(vecA, vecB)
{
	let vecRes = new Vec2(0, 0);

	vecRes.x = vecA.x + vecB.x;
	vecRes.y = vecA.y + vecB.y;
	return (vecRes);
}

// Substraction of <vecB> to <vecA>
function vec2_substract(vecA, vecB)
{
	return (vec2_add(vecA, vecB.scale(-1)));
}

// Scalar product of two vectors
function vec2_sproduct(vecA, vecB)
{
	return (vecA.x * vecB.x + vecA.y * vecB.y);
}

// Length of the segment between two points represented by vectors
function vec2_distance_between_points(vecA, vecB)
{
	let seg = new Vec2(vecA.x - vecB.x, vecA.y - vecB.y);
	return (seg.length);
}

// Symmetry of vector according to a normal vector
// (the incident vector crosses a line with normal vector <normal>,
//  thisfunction returns the vector reflected by the surface, ie. symmetric to <incident>
// 	as if <normal> were the axis of symmetry)
// (computed thanks to formula :
//  `reflected = incident - 2 * (normal . incident) * normal`)
function	vec2_symmetry(incident, normal)
{
	return (vec2_add(incident, normal.scale(vec2_sproduct(incident, normal) * -2)));
}

// Paddle class (2 instances in the game)

class Paddle
{
	constructor(game, index, initial_x, initial_y, width, height, speed)
	{
		this.game = game;
		this.position = new Vec2(initial_x, initial_y); // position of center of paddle rectangle
		this.width = width;
		this.height = height;
		this.radius = (new Vec2(width, height)).length / 2; // radius of a circle that contains the whole paddle
		this.angle = Math.atan(width / height);
		this.index = index; // which player the paddle belongs to
		this.speed = speed; // #f
		this.moving = 0; // -1 for moving up, 0 for unmoving, 1 for moving down
		this.drawn = 0; // whether the paddle is currently drawn on the canvas
		this.testify();
	}

	testify()
	{
		console.log(`Paddle dim ${this.width},${this.height} pos ${this.position.x},${this.position.y} radius ${this.radius} angle ${this.angle}`);
	}
 
	// Returns 1 if the given <y> is between the top and bottom of the paddle
	y_contains(y)
	{
		if (this.y - this.height / 2 <= y && y <= this.y + this.height / 2)
			return (1);
		return (0);
	}

	check_collision_with_ball(ball)
	{
		let distance; // distance between center of paddle and center of ball
		let vec_vert = new Vec2(0, -1);
		let vec_paddle_to_ball; // vector from center of paddle to center of ball
		let angle; // angle between vector [0, -1] and <vec_paddle_to_ball>

		this.testify();
		ball.testify();

		// Initial check : the ball intersects the circle that contains the whole paddle
		vec_paddle_to_ball = vec2_substract(ball.position, this.position);
		distance = vec_paddle_to_ball.length;
		console.log(`[collision ball-paddle] distance between object : ${distance}, compared with paddle radius : ${this.radius}`);
		if (distance > this.radius + ball.radius)
			return (0);

		// Determine incidence angle
		angle = Math.acos(vec2_sproduct(vec_vert, vec_paddle_to_ball) / (vec_vert.length * distance));
		console.log(`[collision ball-paddle] incidence angle : ${angle}, compared with paddle angle : ${this.angle}`);
		if (angle < this.angle || angle > Math.PI * 2 - this.angle) // sector : upper side
		{
			console.log("[collision ball-paddle] check in sector : upper side");
			if (ball.position.y >= this.position.y)
				return (1);
		}
		else if (angle < Math.PI - this.angle) // sector : right side
		{
			console.log("[collision ball-paddle] check in sector : right side");
			if (ball.position.x <= this.position.x)
				return (2);
		}
		else if (angle < Math.PI + this.angle) // sector : lower side
		{
			console.log("[collision ball-paddle] check in sector : lower side");
			if (ball.position.y <= this.position.y)
				return (3);
		}
		else // sector : left side
		{
			console.log("[collision ball-paddle] check in sector : left side");
			if (ball.position.x >= this.position.x)
				return (4);
		}
		return (0);

	}

	// Moves the paddle if its <moving> property is -1 (up) or 1 (down)
	// (checks that the paddle does not go beyonf top wall or bottom wall)
	update() // #f : add elapsed time parameter
	{
		let ctx = this.game.ctx;
	
		if (this.drawn && this.moving == 0) // #f : add condition to redraw if ball is close
			return ;
		//console.log("<move> called for paddle : " + this.index + " while motion is " + this.moving);
		ctx.clearRect(this.position.x - this.width / 2,
					 this.position.y - this.height / 2, this.width, this.height); // erase previous rectangle
		if (this.moving == -1 && this.position.y - this.speed - this.height / 2 > 0)
			this.position.y += this.speed; // moving up
		else if (this.moving == 1 && this.position.y + this.speed + this.height / 2 < canvas.height)
			this.position.y -= this.speed; // moving down
		ctx.fillStyle = paddle_colors[this.index];
		ctx.fillRect(this.position.x - this.width / 2,
					this.position.y - this.height / 2, this.width, this.height);
		this.drawn = 1;
	}
}

// Ball class (1 instance in the game)

class Ball
{
	constructor(game, initial_x, initial_y, radius, initial_speed, initial_direction)
	{
		this.game = game;
		this.initial_position = new Vec2(initial_x, initial_y); // initial position is stored to be able to reset
		this.position = new Vec2(initial_x, initial_y);
		this.radius = radius;
		this.speed = initial_speed; // #f
		this.drawn = 0;
		this.direction = initial_direction; // normalized direction vector
		this.direction.normalize();
		this.testify();
	}

	testify()
	{
		console.log(`Ball dim ${this.radius} pos ${this.position.x},${this.position.y}`);
	}

	// Puts ball in initial position and sets its direction to a random vector
	// (vector does not have uniform probability on the unit circle, but it is tolerable)
	reset()
	{
		this.position.x = this.initial_position.x;
		this.position.y = this.initial_position.y;
		this.direction.x = Math.random() - 0.5;
		this.direction.y = Math.random() - 0.5;
		this.direction.normalize();
	}

	// Checks if ball's next position would intersect a wall or a paddle,
	// if so transforms the ball's direction vector and returns value indicating object :
	// 		- 1 : collision with top wall / bottom wall
	// 		- 2 : collision with left wall (player 0 loses point)
	// 		- 3 : collision with right wall (player 1 loses point)
	// 		- 4 : collision with left paddle (player 0)
	// 		- 5 : collision with right paddle (player 1)
	// 		- 0 : no collisions
	determine_collision(new_pos)
	{
		let nx = new_pos.x;
		let ny = new_pos.y;

		if (nx <= 0) // past left wall
			return (2);
		else if (nx >= canvas.width) // past right wall
			return (3);
		if (ny - this.radius - 1 <= 0) // top wall
		{
			this.direction = vec2_symmetry(this.direction, new Vec2(0, 1));
			return (1);
		}
		if (ny + this.radius + 1 >= canvas_height) // bottom wall
		{
			this.direction = vec2_symmetry(this.direction, new Vec2(0, -1));
			return (1);
		}
		if (this.game.paddles[0].y_contains(ny) // paddle player 0
			&& nx - this.game.paddles[0].position.x - this.game.paddles[0].width / 2 < this.radius)
		{
			this.direction = vec2_symmetry(this.direction, new Vec2(1, 0));
			return (4);
		}
		if (this.game.paddles[1].y_contains(ny) // paddle player 1
			&& this.game.paddles[1].position.x - this.game.paddles[1].width / 2 - nx < this.radius)
		{
			this.direction = vec2_symmetry(this.direction, new Vec2(-1, 0));
			return (5);
		}
		return (0);
	}

	// Moves the ball according to its speed and direction vector,
	// if the ball would collide a wall or a paddle, does not move it and instead
	// 		- lets <Ball.determine_collision> transform the direction vector
	// 		- transmits the collision information to <Game> by return value
	update()
	{
		let ctx = this.game.ctx;
		let collision;
		let new_pos = new Vec2(0, 0);

		console.log(`updating ball : current position is ${this.position.x},${this.position.y}, direction is ${this.direction.x},${this.direction.y}`);
		new_pos = vec2_add(this.position, this.direction.scaled(this.speed));
		console.log(`updating ball : next position is ${new_pos.x},${new_pos.y}`);
		collision = this.determine_collision(new_pos);
		if (collision != 0)
			console.log(`updating ball : collision with ${collision}`);
		if (collision == 0 || collision == 2 || collision == 3)
		{
			console.log("updating ball : erasing last ball");
			ctx.clearRect(this.position.x - this.radius - 2, this.position.y - this.radius - 2,
				this.radius * 2 + 4, this.radius * 2 + 4);
		}
		if (collision == 0) // if no collision, go to new position : redraw ball
		{
			this.position.x = new_pos.x;
			this.position.y = new_pos.y;
			ctx.strokeStyle = "rgb(0 0 0)"; // ball is a black circle
			ctx.beginPath();
			ctx.arc(this.position.x, this.position.y, this.radius, 0, Math.PI * 2, 0);
			ctx.stroke();
			this.drawn = 1;
		}
		return (collision);
	}
}

// Game class, organizing 1 Pong game

class Pong_game
{
	// Game initialization

	// Fetch page elements outside of canvas
	constructor()
	{
		this.running = 0;
		this.points_info = document.querySelector("#pointsInfo");
		this.points = [0, 0];
		this.play_button = document.querySelector("#play");
		this.play_button.addEventListener("click", () => {this.launch();});
		this.stop_button = document.querySelector("#stop");
		this.stop_button.addEventListener("click", () => {this.running = 0;});
		this.tick_button = document.querySelector("#tick");
		this.tick_button.addEventListener("click", () => {this.update_by_tick();});
		this.create_canvas();
		this.create_event_listeners();
		this.last_time = 0;
		this.animation_events = [];
		this.frame_counter = 0;
	}

	// Create canvas, and instantiate 2 paddles and 1 ball (not drawn yet)
	create_canvas()
	{
		this.canvas = document.querySelector("#canvas");
		this.ctx = canvas.getContext("2d");
		this.ball = new Ball(this, canvas_width / 2, canvas_height / 2, ball_radius, ball_initial_speed, new Vec2(1, 0));
		this.paddles = [];
		this.paddles.push(new Paddle(this, 0, paddle_margin, canvas_height / 2,
			paddle_width, paddle_height, paddle_speed));
		this.paddles.push(new Paddle(this, 1, canvas_width - paddle_margin, canvas_height / 2,
			paddle_width, paddle_height, paddle_speed));
		console.log("created ball and paddles");
	}

	// Listen to keyboard events for player keys
	create_event_listeners()
	{
		document.addEventListener('keydown', (event) => {this.store_key_event(event, 0)});
		document.addEventListener('keyup', (event) => {this.store_key_event(event, 1)});
	}

	// Launch game by calling a first animation update
	launch()
	{
		this.running = 1;
		this.update_game_state(0);
	}

	// Function for ad-hoc tests
	tests()
	{
		console.log(`Game of Pong with state : running ${this.running}`);
		this.paddles[0].position.x = 50;
		this.paddles[0].position.y = 50;
		this.ball.position.x = 40;
		this.ball.position.y = 50;
		let res = this.paddles[0].check_collision_with_ball(this.ball);
		console.log(`Result : ${res}`);
	}

	// Game mainloop

	// Stores keyboard events (players trying to move their paddle)
	// to be analyzed when generating next frame
	store_key_event(event, key_released)
	{
		var code = event.code;

		if (this.running == 0)
			return ;
		if(keys_by_player[0].includes(code) || keys_by_player[1].includes(code))
		{
			event.preventDefault();
			if (key_released)
				this.animation_events.push(code + "u");
			else
				this.animation_events.push(code + "d");
		}
	}

	// When receiving a keyboard event indicating that player with index <player> wants to affect its paddle :
	// 		- event 'ArrowUpd' <=> 'ArrowUp' pressed <=> paddle of player gets moving
	// 		- event 'ArrowUpu' <=> 'ArrowUp' released <=> paddle of playe stops moving
	process_key_player(code, player)
	{
		let key_direction = 0;
		let key_released;
		
		key_released = (code.slice(code.length - 1)) == "u";
		console.log("processing key for player " + player + ", key_rel " + key_released);
		if (key_released && this.paddles[player].moving == 0)
			return ;
		if (!key_released && this.paddles[player].moving != 0)
			return ;
		if (code.slice(0, code.length - 1) == keys_by_player[player][0])
			key_direction = 1;
		else
			key_direction = -1;
		console.log("direction determined : " + key_direction);
		if (key_released && this.paddles[player].moving != key_direction)
			return ;
		console.log("modifying paddles's 'moving' property");
		if (key_released)
			this.paddles[player].moving = 0;
		else
			this.paddles[player].moving = key_direction;
	}

	// When tick button is clicked, calls <update_game_state> if <running> is 0
	update_by_tick()
	{
		console.log("update by tick");
		if (this.running == 0)
		{
			this.update_game_state(1);
		}
	}

	// Prepares a new game frame :
	// 		- processes incoming keyboard events to see if players want to move their paddle
	// 		- updates ball (checking if it goes out of bounds) and paddles
	// 		- calls itself again with <requestAnimationFrame> (as long as <running> is true)
	update_game_state(current_time)
	{
		let delta_time;
		let ball_advance;

		console.log(`Frame ${this.frame_counter}`);
		this.frame_counter++;
		if (this.last_time == 0)
		{
			this.last_time = 1;
			console.log("initial tick");
			if (this.running)
				requestAnimationFrame(() => {this.update_game_state();});
			return ;
		}

		delta_time = current_time - this.last_time;
		this.last_time = current_time;
		for (let i = 0; i < this.animation_events.length; i++) // going through events
		{
			let code = this.animation_events[i];
			console.log("- processing event : " + code.slice(0, code.length - 1));
			if (keys_by_player[0].includes(code.slice(0, code.length - 1)))
				this.process_key_player(code, 0);
			else if (keys_by_player[1].includes(code.slice(0, code.length - 1)))
				this.process_key_player(code, 1);
		}
		this.animation_events = []; // emptying event list
		ball_advance = this.ball.update();
		if (ball_advance == 2 || ball_advance == 3) // when ball went through left/right wall
		{
			this.points[1 - (ball_advance - 2)] += 1; // point goes to the other player
			this.points_info.innerHTML = "Points : " + this.points[0] + "-" + this.points[1];
			this.ball.reset();
		}
		this.paddles[0].update();
		this.paddles[1].update();

		if (this.running)
			requestAnimationFrame(() => {this.update_game_state();});
	}
}

function setup()
{
	let game = new Pong_game();
	game.tests();
}

window.onload = setup;

/*

- journal
- spectacle avec Fanny
- compte Discord
- utilisation cartes

- tenir compte du temps passe depuis la derniere frame dans update_game_state
- passer au typescript
- passer en camelcase
- definir une classe de vecteurs
- nettoyer les classes et utiliser des getters pour les attributs
- check la distance a la balle dans le update de paddle pour eviter troncature



*/
