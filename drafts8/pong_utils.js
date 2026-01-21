// --- A module for utility functions and classes for the Pong application,
// 	   shared between Pong client and server
// GEOMETRY - VECTORS
// 2-dimensional vector class
export class Vec2 {
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
export function vec2_add(vecA, vecB) {
    let vecRes = new Vec2(0, 0);
    vecRes.x = vecA.x + vecB.x;
    vecRes.y = vecA.y + vecB.y;
    return (vecRes);
}
// Substraction of <vecB> to <vecA>
export function vec2_substract(vecA, vecB) {
    return (vec2_add(vecA, vecB.scaled(-1)));
}
// Scalar product of two vectors
export function vec2_sproduct(vecA, vecB) {
    return (vecA.x * vecB.x + vecA.y * vecB.y);
}
// Length of the segment between two points represented by vectors
export function vec2_distance_between_points(vecA, vecB) {
    let seg = vec2_substract(vecA, vecB);
    return (seg.length);
}
// Slope of the line between two points represented by vectors
export function vec2_slope_between_two_points(vecA, vecB) {
    return ((vecA.y - vecB.y) / (vecA.x - vecB.x));
}
// Symmetry of vector according to a normal vector
// (the <incident> vector crosses a line with normal vector <normal>,
//  this function returns the vector reflected by the surface,
//  ie. symmetric to <incident> as if <normal> were the axis of symmetry)
// (computed thanks to formula :
//  `reflected = incident - 2 * (normal . incident) * normal`)
export function vec2_symmetry(incident, normal) {
    return (vec2_add(incident, normal.scale(vec2_sproduct(incident, normal) * -2)));
}
// GEOMETRY - OTHER
export function lerp(p1, p2, proportion) {
    let p3 = { x: 0, y: 0 };
    p3.x = p1.x + (p2.x - p1.x) * proportion;
    p3.y = p1.y + (p2.y - p1.y) * proportion;
    return (p3);
}
// Returns the slope of the circular line with radius centered on (0, 0) defined by `y**2 + x**2 == 1`
// (the simplified expression is : `f(x) = (1 - x**2)**0.5`, therefore derivative is `f'(x) = -x / (1 - x**2)**0.5`)
// (<abcissa> should be between -1 and 1, the endpoints of the circular line)
export function slope_on_circle(abcissa) {
    return (-1 * abcissa / (1 - abcissa * abcissa) ** 0.5);
}
