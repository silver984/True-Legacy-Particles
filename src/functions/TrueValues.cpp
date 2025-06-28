#include <Geode/modify/PlayerObject.hpp>
#include "../variables/settings.hpp"
#include "../variables/particle.hpp"
#include "TrueValues.hpp"
#include "util.hpp"

using namespace geode::prelude;

void trueVals(PlayerObject* player) {
	if (!isTrueVals)
		return;

	float size = legacyScaling ? 1.f : player->m_vehicleSize;
	int upsideDown = player->m_isUpsideDown ? -1 : 1;
	int sideways = player->m_isSideways ? -1 : 1;
	int goingLeft = player->m_isGoingLeft ? -1 : 1;

	CCPoint gravity;
	if (!player->m_isSideways)
		gravity = CCPoint(sourceGravity.x * goingLeft, sourceGravity.y * upsideDown);
	else
		gravity = CCPoint(sourceGravity.y * upsideDown, sourceGravity.x * goingLeft);

	int tweak = player->m_isSideways ? 270 : 0;
	float angle = sourceAngle.x * (upsideDown * sideways) + (tweak * sideways);

	float angleVar = sourceAngle.y;
	auto groundParticles = player->m_playerGroundParticles;
	auto trailParticles = player->m_trailingParticles;
	auto shipParticles = player->m_shipClickParticles;
	
	int i = index(player);

	if (!player->m_isSideways) {
		groundParticles->setPosVar(drag[i].posVar * size);
		trailParticles->setPosVar(trail[i].posVar * size);
		shipParticles->setPosVar(shipClick[i].posVar * size);
	}
	else {
		CCPoint dragPosVar = CCPoint(
			drag[i].posVar.y * size,
			drag[i].posVar.x * size
		);
		CCPoint trailPosVar = CCPoint(
			trail[i].posVar.y * size,
			trail[i].posVar.x * size
		);
		CCPoint shipPosVar = CCPoint(
			shipClick[i].posVar.y * size,
			shipClick[i].posVar.x * size
		);

		groundParticles->setPosVar(dragPosVar);
		trailParticles->setPosVar(trailPosVar);
		shipParticles->setPosVar(shipPosVar);
	}

	groundParticles->setAngle(angle);
	groundParticles->setAngleVar(angleVar);
	groundParticles->setGravity(gravity);

	trailParticles->setSpeed(trail[i].speed * size);
	trailParticles->setSpeedVar(trail[i].speedVar * size);
	trailParticles->setAngle(angle);
	trailParticles->setAngleVar(angleVar);
	trailParticles->setGravity(gravity);

	shipParticles->setSpeed(shipClick[i].speed * size);
	shipParticles->setSpeedVar(shipClick[i].speedVar * size);
	shipParticles->setStartSize(shipClick[i].startSize * size);
	shipParticles->setStartSizeVar(shipClick[i].startSizeVar * size);
	shipParticles->setAngle(angle);
	shipParticles->setAngleVar(angleVar);
	shipParticles->setGravity(gravity);
}