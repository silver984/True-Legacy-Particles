#include <Geode/modify/PlayerObject.hpp>
#include "../variables/settings.hpp"
#include "../variables/particle.hpp"
#include "TrueValues.hpp"
#include "util.hpp"

using namespace geode::prelude;

void trueVals(PlayerObject* player) {
	if (!isTrueVals)
		return;

	float multiplier;
	int multiplier2;
	int multiplier3;
	int multiplier4;
	multiplier = legacyScaling ? 1.f : player->m_vehicleSize;
	multiplier2 = player->m_isGoingLeft ? -1 : 1;
	multiplier3 = player->m_isUpsideDown ? -1 : 1;
	multiplier4 = player->m_isSideways ? -1 : 1;

	CCPoint gravity;
	if (!player->m_isSideways)
		gravity = CCPoint(sourceGravity.x * multiplier2, sourceGravity.y * multiplier3);
	else
		gravity = CCPoint(sourceGravity.y * multiplier3, sourceGravity.x * multiplier2);

	int i = index(player);

	float angle;
	if (player->m_isSideways)
		angle = sourceAngle.x - (angleTweak[i] * multiplier3);
	else
		angle = sourceAngle.x * multiplier3;

	float angleVar = sourceAngle.y;
	auto groundParticles = player->m_playerGroundParticles;
	auto trailParticles = player->m_trailingParticles;
	auto shipParticles = player->m_shipClickParticles;
	
	if (!player->m_isSideways) {
		groundParticles->setPosVar(drag[i].posVar * multiplier);
		trailParticles->setPosVar(trail[i].posVar * multiplier);
		shipParticles->setPosVar(shipClick[i].posVar * multiplier);
	}
	else {
		CCPoint dragPosVar = CCPoint(
			drag[i].posVar.y * multiplier,
			drag[i].posVar.x * multiplier
		);
		CCPoint trailPosVar = CCPoint(
			trail[i].posVar.y * multiplier,
			trail[i].posVar.x * multiplier
		);
		CCPoint shipPosVar = CCPoint(
			shipClick[i].posVar.y * multiplier,
			shipClick[i].posVar.x * multiplier
		);

		groundParticles->setPosVar(dragPosVar);
		trailParticles->setPosVar(trailPosVar);
		shipParticles->setPosVar(shipPosVar);
	}

	groundParticles->setAngle(angle);
	groundParticles->setAngleVar(angleVar);
	groundParticles->setGravity(gravity);

	trailParticles->setSpeed(trail[i].speed * multiplier);
	trailParticles->setSpeedVar(trail[i].speedVar * multiplier);
	trailParticles->setAngle(angle);
	trailParticles->setAngleVar(angleVar);
	trailParticles->setGravity(gravity);

	shipParticles->setSpeed(shipClick[i].speed * multiplier);
	shipParticles->setSpeedVar(shipClick[i].speedVar * multiplier);
	shipParticles->setStartSize(shipClick[i].startSize * multiplier);
	shipParticles->setStartSizeVar(shipClick[i].startSizeVar * multiplier);
	shipParticles->setAngle(angle);
	shipParticles->setAngleVar(angleVar);
	shipParticles->setGravity(gravity);
}