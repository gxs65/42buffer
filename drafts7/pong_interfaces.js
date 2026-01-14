// --- A module defining the structure of data-transmission objects for the Pong game,
// 	   shared between Pong client and server
// OTHER INTERFACES
// 2-dimensional point class
export class Point {
    constructor(x = 0, y = 0) {
        this.x = x;
        this.y = y;
    }
}
