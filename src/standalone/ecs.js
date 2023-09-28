/* Adapted from ecs.js and treap.js in the Open Arras Project by szdytom, publish under the Apache License 2.0 */

import { PriorityQueue } from './priority_queue.js';

export class EntityRegistry extends EventTarget {
	constructor() {
		this.free_ids = new PriorityQueue();
		this.signatures = [];
		this.components = [];
		this.component_map = new Map();
		this.systems = [];
		this.destroy_queue = [];
	}

	registerSystem(sys) {
		this.systems.push(sys);
	}

	step(dt) {
		for (const s of this.systems) {
			s.call(this, dt);
		}
		this.commitDestory();
	}

	requestDestory(entity) {
		this.destory_queue.push(entity);
	}

	commitDestory() {
		for (const e of this.destory_queue) {
			this.destory(e);
		}
		this.destory_queue = [];
	}

	requireComponentList(cname) {
		if (!this.component_map.has(cname)) {
			this.component_map.set(cname, this.components.length);
			this.components.push([]);
		}
		return this.components[this.component_map.get(cname)];
	}

	componentListLowerBound(cl, entity) {
		if (cl.length === 0) {
			return 0;
		} else if (cl[cl.length - 1].__owner_entity < entity) {
			return cl.length;
		}

		let l = 0, r = cl.length - 1, p = 0;
		while (l <= r) {
			let mid = (l + r) >>> 1;
			if (cl[mid].__owner_entity < entity) {
				l = mid + 1;
			} else {
				p = mid;
				r = mid - 1;
			}
		}
		return p;
	}

	assign(entity, cv) {
		const cname = cv.constructor.name;
		const cl = this.requireComponentList(cname);

		cv.__owner_entity = entity;
		const p = this.componentListLowerBound(cl, entity);
		
		if (cl[p]?.__owner_entity === entity) {
			cl[p] = cv;
			return this;
		}

		cl.splice(p, 0, cv);
		this.signatures[entity][this.component_map.get(cname)] = p;
		return this;
	}

	create() {
		let id = this.free_ids.empty() ? this.signatures.length : -this.free_ids.pop()[1];
		this.signatures[id] = [];
		return id;
	}

	destory(entity) {
		for (let i in this.signatures[entity]) {
			const p = this.signatures[entity][i];
			this.components[i].splice(p, 1);
		}

		this.signatures[entity] = undefined;
		this.free_ids.push([null, -entity]);
		return this;
	}

	getRaw(entity, cid) {
		return this.components[cid][this.signatures[entity][cid]];
	}

	get(entity, ctype) {
		if (!this.component_map.has(ctype.name)) {
			return null;
		}

		const cid = this.component_map.get(ctype.name);
		if (this.signatures[entity][cid] == null) {
			return null;
		}
		return this.getRaw(entity, cid);
	}

	forEach(rc, func) {
		if (rc.length === 0) {
			for (let i = 0; i < this.signatures.length; ++i) {
				if (this.signatures[i]) {
					func.call(this, i);
				}
			}
			return this;
		}

		const rcid = rc.map(ctype => this.component_map.get(ctype.name));

		let p, v = Infinity;
		for (let cid of rcid) {
			const cl = this.components[cid];
			if (cl.length < v) {
				v = cl.length;
				p = cid;
			}
		}

		for (let c of this.components[p]) {
			const e = c.__owner_entity;
			if (rcid.every(cid => this.signatures[e][cid] != null)) {
				func.apply(this, [e].concat(rcid.map(cid => this.getRaw(e, cid))));
			}
		}
		return this;
	}
};

