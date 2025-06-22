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
	
	if (!player->m_isSideways) {
		groundParticles->setPosVar(drag[index(player)].posVar * size);
		trailParticles->setPosVar(trail[index(player)].posVar * size);
		shipParticles->setPosVar(shipClick[index(player)].posVar * size);
	}
	else {
		CCPoint dragPosVar = CCPoint(
			drag[index(player)].posVar.y * size,
			drag[index(player)].posVar.x * size
		);
		CCPoint trailPosVar = CCPoint(
			trail[index(player)].posVar.y * size,
			trail[index(player)].posVar.x * size
		);
		CCPoint shipPosVar = CCPoint(
			shipClick[index(player)].posVar.y * size,
			shipClick[index(player)].posVar.x * size
		);

		groundParticles->setPosVar(dragPosVar);
		trailParticles->setPosVar(trailPosVar);
		shipParticles->setPosVar(shipPosVar);
	}

	groundParticles->setAngle(angle);
	groundParticles->setAngleVar(angleVar);
	groundParticles->setGravity(gravity);

	trailParticles->setSpeed(trail[index(player)].speed * size);
	trailParticles->setSpeedVar(trail[index(player)].speedVar * size);
	trailParticles->setAngle(angle);
	trailParticles->setAngleVar(angleVar);
	trailParticles->setGravity(gravity);

	shipParticles->setSpeed(shipClick[index(player)].speed * size);
	shipParticles->setSpeedVar(shipClick[index(player)].speedVar * size);
	shipParticles->setStartSize(shipClick[index(player)].startSize * size);
	shipParticles->setStartSizeVar(shipClick[index(player)].startSizeVar * size);
	shipParticles->setAngle(angle);
	shipParticles->setAngleVar(angleVar);
	shipParticles->setGravity(gravity);
}