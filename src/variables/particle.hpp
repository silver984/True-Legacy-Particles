#pragma once

#include <Geode/modify/PlayerObject.hpp>
#include <array>
using namespace geode::prelude;

class Particle {
public:
	static std::array<Particle, 2>& get() {
		static std::array<Particle, 2> instance;
		return instance;
	}
	
	bool isGrounded = false;
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
	};

	class Land {
	public:
		static std::array<Land, 2>& get() {
			static std::array<Land, 2> instance;
			return instance;
		}

		CCPoint gravity = CCPoint(0, 0); // y axis gravity
		float angle = 0.f;
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

extern std::array<Particle, 2>& particle;
extern std::array<Particle::Drag, 2>& drag;
extern std::array<Particle::Trail, 2>& trail;
extern std::array<Particle::ShipClick, 2>& shipClick;
extern std::array<Particle::Land, 2>& land;
extern std::array<Particle::Extra, 2>& extra;
extern CCPoint sourceAngle;
extern CCPoint sourceGravity;
extern CCPoint landGravity;
extern float landAngle;
extern CCPoint sourceStartSize;
extern CCPoint sourceSpeed;
extern CCPoint sourcePosVar;