/* Adapted from ecs.js and treap.js in the Open Arras Project by szdytom, publish under the Apache License 2.0 */

// FHQ Treap impl a set

class TreapNode {
	constructor(val) {
		this.value = val;
		this.left = null;
		this.right = null;
		this.sz = 1;
		this.rd = Math.random();
	}

	update() {
		this.sz = (this.left?.sz || 0) + (this.right?.sz || 0) + 1;
	}

	static splitBySize(rt, k) {
		if (rt == null) {
			return [null, null];
		}

		let x, y;
		if ((rt.left?.sz || 0) + 1 <= k) {
			x = rt;
			[rt.right, y] = TreapNode.splitBySize(rt.right, k - (rt.left?.sz || 0) - 1);
		} else {
			y = rt;
			[x, rt.left] = TreapNode.splitBySize(rt.left, k);
		}
		rt.update();
		return [x, y];
	}

	static splitByValue(rt, v) {
		if (rt == null) {
			return [null, null];
		}

		let x, y;
		if (rt.value <= v) {
			x = rt;
			[rt.right, y] = TreapNode.splitByValue(rt.right, v);
		} else {
			y = rt;
			[x, rt.left] = TreapNode.splitByValue(rt.left, v);
		}
		rt.update();
		return [x, y];
	}

	static merge(x, y) {
		if (x == null) {
			return y;
		} else if (y == null) {
			return x;
		}

		if (x.rd < y.rd) {
			y.left = TreapNode.merge(x, y.left);
			y.update();
			return y;
		}

		x.right = TreapNode.merge(x.right, y);
		x.update();
		return x;
	}

	static forEach(rt, func) {
		if (rt == null) {
			return;
		}

		TreapNode.forEach(rt.left, func);
		func(rt.value);
		TreapNode.forEach(rt.right, func);
	}
};

class TreapSet {
	constructor() {
		this.root = null;
	}

	insertRaw(v) {
		let [x, y] = TreapNode.splitByValue(this.root, v);
		this.root = TreapNode.merge(TreapNode.merge(x, new TreapNode(v)), y);
		return this;
	}

	erase(v) {
		let [x, p] = TreapNode.splitByValue(this.root, v - 1);
		let [_, z] = TreapNode.splitByValue(p, v);
		this.root = TreapNode.merge(x, z);
		return this;
	}

	takeInstance() {
		return this.root == null ? null : this.root.value;
	}

	forEach(func) {
		TreapNode.forEach(this.root, func);
		return this;
	}
};

export class EntityRegistry {
	constructor() {
		this.free_ids = new TreapSet();
		this.signatures = [];
		this.components = [];
		this.component_map = new Map();
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
		let id = this.free_ids.takeInstance();
		if (id == null) {
			id = this.signatures.length;
		} else {
			this.free_ids.erase(id);
		}

		this.signatures[id] = [];
		return id;
	}

	destory(entity) {
		for (let i in this.signatures[entity]) {
			const p = this.signatures[entity][i];
			this.components[i].splice(p, 1);
		}

		this.signatures[entity] = undefined;
		this.free_ids.insertRaw(entity);
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

