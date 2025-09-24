// Configuration constants

const canvas_width = 300;
const canvas_height = 150;
const paddle_margin = 10;
const paddle_width = 10;
const paddle_height = 20;
const paddle_speed = 4;
const paddle_colors = ["rgb(200 0 0)", "rgb(0 0 200)"];
const ball_radius = 4;
const ball_initial_speed = 3;
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
	let vecRes = new Vector(0, 0);

	vecRes.x = vecA.x + vecB.x;
	vecRes.y = vecA.y + vecB.y;
	return (vecRes);
}

// Substraction of <vecB> to <vecA>
function vec2_add(vecA, vecB)
{
	return (vec2_add(vecA, vecB.scale(-1)));
}

// Scalar product of two vectors
function vec2_sproduct(vecA, vecB)
{
	return (vecA.x * vecB.x + vecA.y * vecB.y);
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

// Paddle class (2 instanes in the game)

class Paddle
{
	constructor(index, initial_x, initial_y, width, height, speed)
	{
		this.position = new Vec2(initial_x, initial_y); // position of center of paddle rectangle
		this.width = width;
		this.height = height;
		this.index = index; // which player the paddle belongs to
		this.speed = speed; // #f
		this.moving = 0; // -1 for moving up, 0 for unmoving, 1 for moving down
		this.drawn = 0; // whether the paddle is currently drawn on the canvas
	}
 
	// Returns 1 if the given <y> is between the top and bottom of the paddle
	y_contains(y)
	{
		if (this.y - this.height / 2 <= y && y <= this.y + this.height / 2)
			return (1);
		return (0);
	}

	// Moves the paddle if its <moving> property requires it
	update() // #f : add elapsed time parameter
	{
		if (this.drawn && this.moving == 0) // #f : add condition to redraw if ball is close
			return ;
		console.log("<move> called for paddle : " + this.index + " while motion is " + this.moving);
		ctx.clearRect(this.x - this.width / 2, this.y - this.height / 2, this.width, this.height);
		if (this.moving == -1 && this.position.y - this.speed - this.height / 2 > 0)
			this.position.y -= this.speed; // moving up
		else if (this.moving == 1 && this.position.y + this.speed + this.height / 2 < canvas.height)
			this.position.y += this.speed; // moving down
		ctx.fillStyle = paddle_colors[this.index];
		ctx.fillRect(this.position.x - this.width / 2,
					this.position.y - this.height / 2, this.width, this.height);
		this.drawn = 1;
	}
}

// Ball class (1 instance in the game)

class Ball
{
	constructor(initial_x, initial_y, radius, initial_speed, initial_direction = [1, 0])
	{
		this.initial_x = initial_x;
		this.initial_y = initial_y;
		this.x = initial_x;
		this.y = initial_y;
		this.radius = radius;
		this.speed = initial_speed;
		this.moving = 0;
		this.drawn = 0;
		this.direction = vec2_normalize(initial_direction); // normalized direction vector
	}

	reset()
	{
		this.x = this.initial_x;
		this.y = this.initial_y;
		this.direction[0] = Math.random() - 0.5;
		this.direction[1] = Math.random() - 0.5;
		this.direction = vec2_normalize(this.direction);
	}

	determine_collision(new_pos)
	{
		let nx = new_pos[0];
		let ny = new_pos[1];
		if (ny - this.radius - 1 <= 0) // upper wall
		{
			this.direction = vec2_symmetry(this.direction, [0, 1]);
			return (1);
		}
		if (ny + this.radius + 1 >= canvas_height) // bottom wall
		{
			this.direction = vec2_symmetry(this.direction, [0, -1]);
			return (1);
		}
		if (paddles[0].y_contains(ny) && nx - paddles[0].x - paddles[0].width / 2 < this.radius)
		{ // paddle player 1
			this.direction = vec2_symmetry(this.direction, [1, 0]);
			return (1);
		}
		if (paddles[1].y_contains(ny) && paddles[1].x - paddles[1].width / 2 - nx < this.radius)
		{ // paddle player 2
			this.direction = vec2_symmetry(this.direction, [-1, 0]);
			return (1);
		}
		if (nx <= 0 || nx >= canvas.width) // ball outside borders
			return ((nx >= canvas.width) + 2); // eg. returns 2 in case of point lost by player 0
		return (0);
	}

	update()
	{
		if (this.drawn && this.moving == 0)
			return 0;
		//console.log("<move> called for ball while direction is :");
		//console.log(this.direction);
		if (this.drawn)
		{
			ctx.clearRect(this.x - this.radius - 2, this.y - this.radius - 2, this.radius * 2 + 4, this.radius * 2 + 4);
			this.drawn = 0;
		}
		if (this.moving)
		{
			let collision;
			let old_pos = [this.x, this.y];
			let new_pos = [0, 0];
			new_pos = vec2_add(old_pos, vec2_scale(this.direction, this.speed));
			collision = this.determine_collision(new_pos);
			if (collision >= 2) // ball outside borders
				return (collision - 2);
			if (collision == 0) // go to new position only if no collision
			{
				console.log("collision detected : " + collision);
				this.x = new_pos[0];
				this.y = new_pos[1];
			}
		}

		ctx.strokeStyle = "rgb(0 0 0)";
		ctx.beginPath();
		ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2, 0);
		ctx.stroke();
		this.drawn = 1;
		return (-1);
	}
}

function process_key_player(code, player)
{
	let key_direction = 0;
	let key_released;
	
	key_released = (code.slice(code.length - 1)) == "u";
	console.log("processing key for player " + player + ", key_rel " + key_released);
	if (key_released && paddles[player].moving == 0)
		return ;
	if (!key_released && paddles[player].moving != 0)
	{
		return ;
	}
	console.log("checking direction on slice : " + code.slice(0, code.length - 1))
	if (code.slice(0, code.length - 1) == keys_by_player[player][0])
		key_direction = 1;
	else
		key_direction = -1;
	console.log("direction determined : " + key_direction);
	if (key_released && paddles[player].moving != key_direction)
		return ;
	console.log("modifying 'moving' property");
	if (key_released)
		paddles[player].moving = 0;
	else
		paddles[player].moving = key_direction;
}

function update_game_state(current_time)
{
	let delta_time;

	if (running == 0)
		return ;
	if (last_time == 0)
	{
		last_time = 1;
		console.log("initial tick");
		paddles[0].update();
		paddles[1].update();
		ball.moving = 1;
		requestAnimationFrame(update_game_state);
		return ;
	}

	delta_time = current_time - last_time;
	last_time = current_time;
	if (animation_events.length > 0)
	{
		console.log("tick, events to process are :");
		console.log(animation_events);
	}
	for (let i = 0; i < animation_events.length; i++)
	{
		let code = animation_events[i];
		console.log("- on event : " + code.slice(0, code.length - 1));
		if (keys_by_player[0].includes(code.slice(0, code.length - 1)))
			process_key_player(code, 0);
		else if (keys_by_player[1].includes(code.slice(0, code.length - 1)))
			process_key_player(code, 1);
	}
	animation_events = [];
	let result = ball.update();
	console.log("result of ball advance : " + result);
	if (result != -1)
	{
		points[1 - result] += 1;
		points_info.innerHTML = "Points : " + points[0] + "-" + points[1];
		ball.reset();
	}
	paddles[0].update();
	paddles[1].update();

	requestAnimationFrame(update_game_state);
}

function stop_animation()
{
	running = 0;
}



function store_key_event(event, key_released)
{
	var code = event.code;

	if (running == 0)
			return ;
	console.log("key event :",code);
	if(keys_by_player[0].includes(code) || keys_by_player[1].includes(code))
	{
		event.preventDefault();
		if (key_released)
			animation_events.push(code + "u");
		else
			animation_events.push(code + "d");
	}
}

class Pong_game
{
	// Game initialization

	// Fetch page elements outside of canvas
	constructor()
	{
		this.points_info = document.querySelector("#pointsInfo");
		this.points = [0, 0];
		this.stop_button = document.querySelector("#stop");
		this.stop_button.addEventListener("click", stop_animation);
		this.create_canvas();
		this.create_event_listeners();
	}

	// Create canvas, and instantiate 2 paddles and 1 ball (not drawn yet)
	create_canvas()
	{
		this.canvas = document.querySelector("#canvas");
		this.ctx = canvas.getContext("2d");
		this.ball = new Ball(canvas_width / 2, canvas_height / 2, ball_radius, ball_initial_speed, [1, 0.25]);
		this.paddles = [];
		this.paddles.push(new Paddle(0, paddle_margin, canvas_height / 2,
			paddle_width, paddle_height, paddle_speed));
		this.paddles.push(new Paddle(1, canvas_width - paddle_margin, canvas_height / 2,
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
		this.running = 1
		this.counter = 0;
		this.last_time = 0;
		this.animation_events = [];
		update_game_state(0);
	}
}

function setup()
{
	let game = Pong_game();
	game.launch();
}

window.onload = setup;

/*

- journal
- spectacle avec Fanny
- compte Discord

- tenir compte du temps passe depuis la derniere frame dans update_game_state
- passer au typescript
- passer en camelcase
- definir une classe de vecteurs
- nettoyer les classes et utiliser des getters pour les attributs
- check la distance a la balle dans le update de paddle pour eviter troncature



*/
