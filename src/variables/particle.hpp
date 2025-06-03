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

	bool spiderDashed = false;

	class Drag {
	private:
		CCDictionary* dict =
			CCDictionary::createWithContentsOfFile("dragEffect.plist");

		CCPoint sourceStartSize = CCPoint(
			((CCString*)dict->objectForKey("startParticleSize"))->floatValue(),
			((CCString*)dict->objectForKey("startParticleSizeVariance"))->floatValue()
		);

		CCPoint sourceSpeed = CCPoint(
			((CCString*)dict->objectForKey("speed"))->floatValue(),
			((CCString*)dict->objectForKey("speedVariance"))->floatValue()
		);
	public:
		static std::array<Drag, 2>& get() {
			static std::array<Drag, 2> instance;
			return instance;
		}

		float speed = sourceSpeed.x;
		float speedVar = sourceSpeed.y;
		float startSize = sourceStartSize.x;
		float startSizeVar = sourceStartSize.y;

		CCPoint sourcePosVar = CCPoint(
			((CCString*)dict->objectForKey("sourcePositionVariancex"))->floatValue(),
			((CCString*)dict->objectForKey("sourcePositionVariancey"))->floatValue()
		);

		CCPoint posVar = sourcePosVar;
	};

	class Trail {
	public:
		static std::array<Trail, 2>& get() {
			static std::array<Trail, 2> instance;
			return instance;
		}

		CCPoint posVar = CCPoint(0, 2);
		float speed = 15.f;
		float speedVar = 4.f;
	};

	class ShipClick {
	public:
		static std::array<ShipClick, 2>& get() {
			static std::array<ShipClick, 2> instance;
			return instance;
		}

		CCPoint posVar = CCPoint(0, 2);
		float speed = 150.f;
		float speedVar = 40.f;
		float startSize = 6.f;
		float startSizeVar = 4.5f;
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
extern std::array<Particle::Extra, 2>& extra;

extern bool isInLevel;