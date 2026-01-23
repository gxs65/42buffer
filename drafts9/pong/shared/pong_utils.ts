// --- A module for utility functions and classes for the Pong application,
// 	   shared between Pong client and server

import { Point } from "./pong_interfaces.js";
import type * as wst from "ws";

// GEOMETRY - VECTORS

// 2-dimensional vector class
export class Vec2
{
	x: number;
	y: number;

	constructor(x: number, y:number)
	{
		this.x = x;
		this.y = y;
	}

	get length(): number
	{
		return (Math.sqrt(this.x * this.x + this.y * this.y));
	}

	scale(scaler: number): Vec2
	{
		this.x = scaler * this.x;
		this.y = scaler * this.y;
		return (this);
	}

	scaled(scaler:number): Vec2
	{
		let vec_scaled = new Vec2(scaler * this.x, scaler * this.y);
		return (vec_scaled);
	}

	normalize(): Vec2
	{
		let length = this.length;
		if (length != 1)
			this.scale(1 / length);
		return (this);
	}
}

// Addition of two vectors
export function vec2_add(vecA: Vec2, vecB: Vec2): Vec2
{
	let vecRes = new Vec2(0, 0);

	vecRes.x = vecA.x + vecB.x;
	vecRes.y = vecA.y + vecB.y;
	return (vecRes);
}

// Substraction of <vecB> to <vecA>
export function vec2_substract(vecA: Vec2, vecB: Vec2): Vec2
{
	return (vec2_add(vecA, vecB.scaled(-1)));
}

// Scalar product of two vectors
export function vec2_sproduct(vecA: Vec2, vecB: Vec2): number
{
	return (vecA.x * vecB.x + vecA.y * vecB.y);
}

// Length of the segment between two points represented by vectors
export function vec2_distance_between_points(vecA: Vec2, vecB: Vec2): number
{
	let seg = vec2_substract(vecA, vecB);
	return (seg.length);
}

// Slope of the line between two points represented by vectors
export function vec2_slope_between_two_points(vecA: Vec2, vecB: Vec2): number
{
	return ((vecA.y - vecB.y) / (vecA.x - vecB.x));
}

// Symmetry of vector according to a normal vector
// (the <incident> vector crosses a line with normal vector <normal>,
//  this function returns the vector reflected by the surface,
//  ie. symmetric to <incident> as if <normal> were the axis of symmetry)
// (computed thanks to formula :
//  `reflected = incident - 2 * (normal . incident) * normal`)
export function vec2_symmetry(incident: Vec2, normal: Vec2): Vec2
{
	return (vec2_add(incident, normal.scale(vec2_sproduct(incident, normal) * -2)));
}

// GEOMETRY - OTHER

export function lerp(p1: Point, p2 : Point, proportion: number): Point
{
	let p3: Point = {x: 0, y: 0};

	p3.x = p1.x + (p2.x - p1.x) * proportion;
	p3.y = p1.y + (p2.y - p1.y) * proportion;
	return (p3);
}

// Returns the slope of the circular line with radius centered on (0, 0) defined by `y**2 + x**2 == 1`
// (the simplified expression is : `f(x) = (1 - x**2)**0.5`, therefore derivative is `f'(x) = -x / (1 - x**2)**0.5`)
// (<abcissa> should be between -1 and 1, the endpoints of the circular line)
export function slope_on_circle(abcissa: number): number
{
	return (-1 * abcissa / (1 - abcissa * abcissa)**0.5);
}

// WEBSOCKETS

// (Server version : uses type 'wst.Websocket' from the Node.js websocket module)
// Sends the <data> string through websocket <ws>, with exception catching
// Returns 0 if data sent successfully, 1 if data could not be sent
export function ws_safe_send_server(ws: wst.WebSocket, data: string) : number
{
	try
	{
		if (ws.readyState == ws.OPEN)
		{
			ws.send(data);
			return (0);
		}
		else
			return (1);		
	}
	catch
	{
		console.log("Error on sending data through a websocket");
		return (1);
	}
}

// (Client version : uses type 'Websocket' from the builtin websocket class in browser context)
// Sends the <data> string through websocket <ws>, with exception catching
// Returns 0 if data sent successfully, 1 if data could not be sent
export function ws_safe_send_client(ws: WebSocket, data: string) : number
{
	try
	{
		if (ws.readyState == ws.OPEN)
		{
			ws.send(data);
			return (0);
		}
		else
			return (1);		
	}
	catch
	{
		console.log("Error on sending data through a websocket");
		return (1);
	}
}
