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

	int framesBeforeSpiderDashed = 0;
	bool spiderDashed = false;

	class Drag {
	private:
		CCDictionary* d = CCDictionary::createWithContentsOfFile("dragEffect.plist");
		CCString* posVarXStr = (CCString*)d->objectForKey("sourcePositionVariancex");
		CCString* posVarYStr = (CCString*)d->objectForKey("sourcePositionVariancey");
		CCString* speedStr = (CCString*)d->objectForKey("speed");
		CCString* speedVarStr = (CCString*)d->objectForKey("speedVariance");
		CCString* sStr = (CCString*)d->objectForKey("startParticleSize");
		CCString* sVarStr = (CCString*)d->objectForKey("startParticleSizeVariance");
	public:
		static std::array<Drag, 2>& get() {
			static std::array<Drag, 2> instance;
			return instance;
		}

		CCPoint srcPV = CCPoint(posVarXStr->floatValue(), posVarYStr->floatValue());
		float speed = speedStr->floatValue();
		float speedVar = speedVarStr->floatValue();
		float startSize = sStr->floatValue();
		float startSizeVar = sVarStr->floatValue();
		CCPoint posVar = srcPV;
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

struct ScaleParams {
	bool isSecondPlayer;
	bool isInLevel;
	bool legacyTracking;
	bool inconstVal;
	bool legacyScaling;
	bool isMini;
};

struct CoreParams {
	PlayerObject* p;
	int index;
	bool legacyTracking;
	bool inconstVal;
	bool legacyScaling;
	float delta;
};

struct WideParams {
	bool isSecondPlayer;
	bool isInLevel;
	bool inconstVal;
	bool isMode;
};

void core(const CoreParams& c);
void scale(const ScaleParams& s);
void wide(const WideParams& w);
void spiderDash(bool isSecondPlayer);
void reset(bool isInLevel);