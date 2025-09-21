#include <Geode/modify/PlayerObject.hpp>
#include "../globals/settings.hpp"
#include "../globals/particle.hpp"
#include "../util.hpp"

using namespace geode::prelude;

class $modify(PlayerObject) {
	void update(float p0) override {
		if (!isValid(this, true) || !g_isTrueValues) return PlayerObject::update(p0);
		PlayerObject::update(p0);

		float size = g_isLegacyScaling ? 1.f : this->m_vehicleSize;
		int upsideDown = this->m_isUpsideDown ? -1 : 1;
		int sideways = this->m_isSideways ? -1 : 1;
		int goingLeft = this->m_isGoingLeft ? -1 : 1;

		CCPoint gravity;
		if (!this->m_isSideways)
			gravity = CCPoint(g_sourceGravity.x * goingLeft, g_sourceGravity.y * upsideDown);
		else
			gravity = CCPoint(g_sourceGravity.y * upsideDown, g_sourceGravity.x * goingLeft);

		int tweak = this->m_isSideways ? 270 : 0;
		float angle = g_sourceAngle.x * (upsideDown * sideways) + (tweak * sideways);

		float angleVar = g_sourceAngle.y;
		auto groundParticles = this->m_playerGroundParticles;
		auto trailParticles = this->m_trailingParticles;
		auto shipParticles = this->m_shipClickParticles;

		int i = playerIndex(this);

		if (!this->m_isSideways) {
			groundParticles->setPosVar(g_drag[i].posVar * size);
			trailParticles->setPosVar(g_trail[i].posVar * size);
			shipParticles->setPosVar(g_shipClick[i].posVar * size);
		}
		else {
			CCPoint dragPosVar = CCPoint(
				g_drag[i].posVar.y * size,
				g_drag[i].posVar.x * size
			);
			CCPoint trailPosVar = CCPoint(
				g_trail[i].posVar.y * size,
				g_trail[i].posVar.x * size
			);
			CCPoint shipPosVar = CCPoint(
				g_shipClick[i].posVar.y * size,
				g_shipClick[i].posVar.x * size
			);

			groundParticles->setPosVar(dragPosVar);
			trailParticles->setPosVar(trailPosVar);
			shipParticles->setPosVar(shipPosVar);
		}

		groundParticles->setAngle(angle);
		groundParticles->setAngleVar(angleVar);
		groundParticles->setGravity(gravity);

		int mult = this->m_isUpsideDown ? -1 : 1;

		trailParticles->setAngle(angle * mult);
		trailParticles->setAngleVar(angleVar);
		trailParticles->setGravity(gravity * mult);

		shipParticles->setAngle(angle * mult);
		shipParticles->setAngleVar(angleVar);
		shipParticles->setGravity(gravity * mult);

		// ---------- legacy scaling affected ----------
		trailParticles->setSpeed(g_trail[i].speed * size);
		trailParticles->setSpeedVar(g_trail[i].speedVar * size);
		trailParticles->setStartSize(g_trail[i].startSize * size);
		trailParticles->setStartSizeVar(g_trail[i].startSizeVar * size);
		trailParticles->setEndSize(g_trail[i].endSize * size);
		trailParticles->setEndSizeVar(g_trail[i].endSizeVar * size);

		shipParticles->setSpeed(g_shipClick[i].speed * size);
		shipParticles->setSpeedVar(g_shipClick[i].speedVar * size);
		shipParticles->setStartSize(g_shipClick[i].startSize * size);
		shipParticles->setStartSizeVar(g_shipClick[i].startSizeVar * size);
		shipParticles->setEndSize(g_shipClick[i].endSize * size);
		shipParticles->setEndSizeVar(g_shipClick[i].endSizeVar * size);

		groundParticles->setSpeed(g_drag[i].speed * size);
		groundParticles->setSpeedVar(g_drag[i].speedVar * size);
		groundParticles->setStartSize(g_drag[i].startSize * size);
		groundParticles->setStartSizeVar(g_drag[i].startSizeVar * size);
		groundParticles->setEndSize(g_drag[i].endSize * size);
		groundParticles->setEndSizeVar(g_drag[i].endSizeVar * size);
	}
};