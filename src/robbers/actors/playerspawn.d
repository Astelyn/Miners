// Copyright © 2011, Jakob Bornecrantz.  All rights reserved.
// See copyright notice in src/charge/charge.d (GPLv2 only).
module robbers.actors.playerspawn;

import std.random : rand;

import charge.charge;
import robbers.world;


class PlayerSpawn : GameActor
{
private:
	static Vector!(PlayerSpawn) spawns;

public:
	this(World w, Point3d pos, Quatd rot)
	{
		super(w);
		spawns ~= this;
		this.pos = pos;
		this.rot = rot;
	}

	static PlayerSpawn random() {
		return spawns[rand() % spawns.length];
	}

}
