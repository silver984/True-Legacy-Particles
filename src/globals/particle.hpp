#pragma once

#include <array>
using namespace geode::prelude;

class Particle {
public:
	static std::array<Particle, 2>& get() {
		static std::array<Particle, 2> instance;
		return instance;
	}
	
	bool isGrounded = false;
	bool isDeviated = false;
	bool spiderDashed = false;

	class Drag {
	public:
		static std::array<Drag, 2>& get() {
			static std::array<Drag, 2> instance;
			return instance;
		}
		float speed = 0.f;
		float speedVar = 0.f;
		float startSize = 0.f;
		float startSizeVar = 0.f;
		float endSize = 0.f;
		float endSizeVar = 0.f;
		CCPoint posVar = CCPoint(0, 0);
	};

	class Trail {
	public:
		static std::array<Trail, 2>& get() {
			static std::array<Trail, 2> instance;
			return instance;
		}

		CCPoint posVar = CCPoint(0, 0);
		float speed = 0.f;
		float speedVar = 0.f;
		float startSize = 0.f;
		float startSizeVar = 0.f;
		float endSize = 0.f;
		float endSizeVar = 0.f;
	};

	class ShipClick {
	public:
		static std::array<ShipClick, 2>& get() {
			static std::array<ShipClick, 2> instance;
			return instance;
		}

		CCPoint posVar = CCPoint(0, 0);
		float speed = 0.f;
		float speedVar = 0.f;
		float startSize = 0.f;
		float startSizeVar = 0.f;
		float endSize = 0.f;
		float endSizeVar = 0.f;
	};

	class Land {
	public:
		static std::array<Land, 2>& get() {
			static std::array<Land, 2> instance;
			return instance;
		}

		CCPoint gravity = CCPoint(0, 0); // y axis gravity
		float angle = 0.f;
		bool gate = false;
		int landSwitch = 0;
	};

	class Extra {
	public:
		static std::array<Extra, 2>& get() {
			static std::array<Extra, 2> instance;
			return instance;
		}

		CCPoint onGroundPos = CCPoint(0, 0);
		CCPoint offGroundPos = CCPoint(0, 0);
	};
};

extern std::array<Particle, 2>& g_particle;
extern std::array<Particle::Drag, 2>& g_drag;
extern std::array<Particle::Trail, 2>& g_trail;
extern std::array<Particle::ShipClick, 2>& g_shipClick;
extern std::array<Particle::Land, 2>& g_land;
extern std::array<Particle::Extra, 2>& g_extra;
extern CCPoint g_sourceAngle;
extern CCPoint g_sourceGravity;
extern CCPoint g_landGravity;
extern float g_landAngle;
extern CCPoint g_sourceStartSize;
extern CCPoint g_sourceEndSize;
extern CCPoint g_sourceSpeed;
extern CCPoint g_sourcePosVar;