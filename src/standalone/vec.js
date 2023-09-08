/* Adapted from 2d.js in the Open Arras Project by szdytom and ez_lcw, pushlished under the Apache License 2.0 */

function sqr(x) {
	return x * x;
}

export class Vec2 {
	constructor(x, y) {
		this.x = x || 0;
		this.y = y || 0;
	}

	adjust(len) {
		return this.mul(len / this.abs());
	}

	distanceTo(b) {
		return this.sub(b).abs();
	}

	add(y) {
		return new Vec2(this.x + y.x, this.y + y.y);
	}

	sub(y) {
		return new Vec2(this.x - y.x, this.y - y.y);
	}

	mul(k) {
		return new Vec2(this.x * k, this.y * k);
	}

	conj() {
		return new Vec2(this.x, -this.y);
	}

	yx() {
		return new Vec2(this.y, this.x);
	}

	complexMul(y) {
		return new Vec2(this.x * y.x - this.y * y.y, this.x * y.y + this.y * y.x);
	}

	complexInv() {
		return this.conj().mul(1 / this.norm());
	}

	complexDiv(y) {
		return this.complexMul(y.complexInv());
	}

	addTo(y) {
		this.x += y.x;
		this.y += y.y;
	}

	subTo(y) {
		this.x -= y.x;
		this.y -= y.y;
	}

	mulTo(k) {
		this.x *= k;
		this.y *= k;
	}

	equal(y) {
		return this.x == y.x && this.y == y.y;
	}

	arg() {
		return Math.atan2(this.y, this.x);
	}

	abs() {
		return Math.sqrt(this.norm());
	}

	norm() {
		return sqr(this.x) + sqr(this.y);
	}

	clone() {
		return new Vec2(this.x, this.y);
	}

	dot(y) {
		return this.x * y.x + this.y * y.y;
	}

	cross(y) {
		return this.x * y.y - this.y * y.x;
	}
};
