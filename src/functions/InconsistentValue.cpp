#include <Geode/modify/PlayerObject.hpp>
#include "../variables/settings.hpp"
#include "../variables/particle.hpp"
#include "InconsistentValue.hpp"
#include "util.hpp"

using namespace geode::prelude;

void inconsistentVal(PlayerObject* player) {
	if (!inconstVal)
		return;

	float multiplier;
	if (legacyScaling)
		multiplier = 1.f;
	else
		multiplier = player->m_vehicleSize;
	auto& drag = Particle::Drag::get();
	auto& trail = Particle::Trail::get();
	auto& shipClick = Particle::ShipClick::get();
	auto groundParticles = player->m_playerGroundParticles;
	auto trailParticles = player->m_trailingParticles;
	auto shipParticles = player->m_shipClickParticles;
	int i = index(player);

	groundParticles->setPosVar(drag[i].posVar * multiplier);
	trailParticles->setPosVar(trail[i].posVar * multiplier);
	trailParticles->setSpeed(trail[i].speed * multiplier);
	trailParticles->setSpeedVar(trail[i].speedVar * multiplier);
	shipParticles->setPosVar(shipClick[i].posVar * multiplier);
	shipParticles->setSpeed(shipClick[i].speed * multiplier);
	shipParticles->setSpeedVar(shipClick[i].speedVar * multiplier);
	shipParticles->setStartSize(shipClick[i].startSize * multiplier);
	shipParticles->setStartSizeVar(shipClick[i].startSizeVar * multiplier);
}