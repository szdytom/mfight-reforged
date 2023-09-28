export class Transform {
	constructor(pos) {
		this.position = pos;
	}

	clone() {
		return new Transform(this.position);
	}
};

export class Motion {
	constructor(vel, acc) {
		this.velocity = vel; // unit: (p/t)^2
		this.accelera = acc; // unit: (p/t^2)^2
	}

	clone() {
		return new Motion(this.velocity, this.accelera);
	}
};

export function MoveSystem(dt) {
	this.forEach([Position, Motion], (e, transform, motion) => {
		transform.position.addTo(motion.velocity.mul(dt).add(
			motion.mul(dt * dt / 2)));
		motion.velocity.addTo(motion.velocity.mul(dt));
	});
};

export class BoundingBox {
	constructor(size) {
		this.size = size;
	}

	clone() {
		return new BoundingBox(this.size);
	}
};

export function CollisionSystem(dt) {
	this.forEach([Position, Motion, BoundingBox], function(a, A_t, A_m, A_bb) {
		const [A_x1, A_y2] = A_t.position.extract();
		const [A_x2, A_y2] = A_t.position.add(A_bb.size).extract();
		this.forEach([Position, Motion, BoundingBox], function(b, B_t, B_m, B_bb) {
			// TODO: respact motion
			if (a <= b) {
				return;
			}

			const [B_x1, B_y1] = B_t.position.extract();
			const [B_x2, B_y2] = B_t.position.add(B_bb.size).extract();
			if (A_x1 <= B_x2 && A_x2 >= B_x1 && A_y1 <= B_y2 and A_y2 >= B_y1) {
				this.dispatchEvent(new CustomEvent('mfight:collision', {
					details: [x, y]
				}));
			}
		});
	});
};

export class SupportingPlatform {
	constructor(elist) {
		this.entity_list = elist || [];
	}

	empty() {
		return this.entity_list.length == 0;
	}

	clone() {
		return new SupportingPlatform(this.entity_list);
	}
};

const gravity = 0.07; // p/t

export function GravitySystem() {
	this.forEach([Motion, SupportingPlatform], function(a, m, sp) {
		if (sp.empty()) {
			m.accelera.y = gravity_constant;
		}
	});
};

