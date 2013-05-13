// Copyright © 2012, Jakob Bornecrantz.  All rights reserved.
// See copyright notice in src/charge/charge.d (GPLv2 only).
module miners.classic.otherplayer;

import charge.charge;

import std.math;
import miners.world;
import miners.importer.network;
static import miners.actors.otherplayer;


class OtherPlayer : miners.actors.otherplayer.OtherPlayer, GameTicker
{
public:
	char[] name;
	GfxDynamicTexture text;
	Point3d curPos;
	int ticks;

    const double animSpeed = 4.000000059604645;
    const int HEAD = 0;
    const int BODY = 1;
    const int ARML = 2;
    const int ARMR = 3;
    const int LEGL = 4;
    const int LEGR = 5;

	/// fCraft likes to hide players far away.
	bool isHidden;


public:
	this(World w, int id, char[] name, Point3d pos, double heading, double pitch)
	{
		this.name = name;
		this.curPos = pos;
		super(w, id, pos, heading, pitch);

		w.addTicker(this);

		if (text is null) {
			text = new GfxDynamicTexture(null);
			w.opts.classicFont().render(text, name);
		}

		if (name !is null) {
			auto t = w.opts.getSkin(removeColorTags(name));
			skel.getMaterial().setTexture("tex", t);
		}
	}

	~this()
	{
		w.remTicker(this);
		sysReference(&text, null);
	}

	void update(Point3d pos, double heading, double pitch)
	{
		this.heading = heading;
		this.pitch = pitch;
		this.pos = pos;

        skel.bones[ARML].rot = Quatd(sin(animSpeed * 0.6662 + PI) * 2.0, 0.0,
                                     sin(animSpeed * 0.2312) + 1.0);

        skel.bones[ARMR].rot = Quatd(sin(animSpeed * 0.6662) * 2.0, 0.0,
                                     sin(animSpeed * 0.2812) - 1.0);

        skel.bones[LEGL].rot = Quatd();
        skel.bones[LEGR].rot = Quatd();

		vel = (this.pos - curPos);
		vel.scale(0.2);
		ticks = 4;
	}

	void tick()
	{
		if (ticks > -1) {
			auto vel = this.vel;
			vel.scale(ticks);

			auto tmp = this.pos;

			curPos = this.pos - vel;
			super.update(curPos, heading, pitch);

			this.pos = tmp;

			ticks--;
		}
	}
}
