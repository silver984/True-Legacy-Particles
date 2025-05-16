#include "particle.hpp"
#include "data.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

void legacyTrackingFunc(PlayerObject* p, int index, float delta) {
	if (!Data::get().legacyTracking)
		return;

	auto& particle = Particle::get();
	auto& x = Particle::Extra::get();

	if (p->m_isOnGround) {
		CCPoint ground = p->m_playerGroundParticles->m_obPosition;
		x[index].onGroundPos = CCPoint(ground);

		CCPoint air = x[index].onGroundPos - p->m_position;
		x[index].offGroundPos = CCPoint(air);
	}

	if (particle[index].framesBeforeSpiderDashed >= (2 * ((1.f / 60) / delta))) {
		if (p->m_isOnGround)
			particle[index].spiderDashed = false;
	}

	if (!particle[index].spiderDashed) {
		if (!(p->m_isShip || p->m_isBird || p->m_isDart || p->m_isSwing)) {
			CCPoint newPos = CCPoint(p->m_position + x[index].offGroundPos);
			p->m_playerGroundParticles->setPosition(newPos);
		}
		else
			p->m_playerGroundParticles->setPosition(CCPoint(0, 0));
	}
	else
		p->m_playerGroundParticles->setPosition(CCPoint(0, 0));
}

void legacyScalingFunc(PlayerObject* p) {
	if (!Data::get().legacyScaling)
		return;

	for (auto system : CCArrayExt<CCParticleSystem*>(p->m_particleSystems)) {
		if (p->m_vehicleSize != 1.f)
			system->loadScaledDefaults(1.f);

		system->setScale(p->m_vehicleSize);
	}
}

void inconstValFunc(PlayerObject* p, int index) {
	if (!Data::get().inconstVal)
		return;

	float m; // multiplier
	if (Data::get().legacyScaling)
		m = 1.f;
	else
		m = p->m_vehicleSize;

	auto& drag = Particle::Drag::get();
	auto& trail = Particle::Trail::get();
	auto& shipClick = Particle::ShipClick::get();

	p->m_playerGroundParticles->setPosVar(drag[index].posVar * m);
	p->m_trailingParticles->setPosVar(trail[index].posVar * m);
	p->m_trailingParticles->setSpeed(trail[index].speed * m);
	p->m_trailingParticles->setSpeedVar(trail[index].speedVar * m);
	p->m_shipClickParticles->setPosVar(shipClick[index].posVar * m);
	p->m_shipClickParticles->setSpeed(shipClick[index].speed * m);
	p->m_shipClickParticles->setSpeedVar(shipClick[index].speedVar * m);
	p->m_shipClickParticles->setStartSize(shipClick[index].startSize * m);
	p->m_shipClickParticles->setStartSizeVar(shipClick[index].startSizeVar * m);
}

void core(PlayerObject* player, int index, float delta) {
	auto& d = Data::get();
	if (!d.modSwitch)
		return;

	if (!d.player1) {
		if (!player->m_isSecondPlayer)
			return;
	}

	if (!d.player2) {
		if (player->m_isSecondPlayer)
			return;
	}

	legacyTrackingFunc(player, index, delta);
	legacyScalingFunc(player);
	inconstValFunc(player, index);
}

void scale(bool isMini, bool isSecondPlayer) {
	if (!Data::get().modSwitch)
		return;

	auto& d = Data::get();

	if (!d.inconstVal)
		return;
	if (!isMini)
		return;
	if (d.legacyScaling)
		return;

	int index;
	if (!isSecondPlayer)
		index = 0;
	else
		index = 1;

	auto& drag = Particle::Drag::get();
	auto& trail = Particle::Trail::get();
	auto& shipClick = Particle::ShipClick::get();

	drag[index].posVar = drag[index].srcPV;
	trail[index].posVar = drag[index].srcPV;
	trail[index].speed = drag[index].speed;
	trail[index].speedVar = drag[index].speedVar;
	shipClick[index].posVar = drag[index].srcPV;
	shipClick[index].speed = drag[index].speed;
	shipClick[index].speedVar = drag[index].speedVar;
	shipClick[index].startSize = drag[index].startSize;
	shipClick[index].startSizeVar = drag[index].startSizeVar;
}

void wide(bool isMode, bool isSecondPlayer) {
	if (!Data::get().modSwitch)
		return;

	auto& d = Data::get();

	if (!d.inconstVal)
		return;
	if (!isMode)
		return;

	auto& drag = Particle::Drag::get();
	if (!isSecondPlayer)
		drag[0].posVar = CCPoint(15, 0);
	else
		drag[1].posVar = CCPoint(15, 0);
}

void spiderDash(bool isSecondPlayer) {
	if (!Data::get().modSwitch)
		return;

	auto& particle = Particle::get();

	if (isSecondPlayer) {
		particle[0].spiderDashed = true;
		particle[0].framesBeforeSpiderDashed = 0;
	}
	else {
		particle[1].spiderDashed = true;
		particle[1].framesBeforeSpiderDashed = 0;
	}
}

void reset() {
	if (!Data::get().modSwitch)
		return;

	auto& particle = Particle::get();
	auto& drag = Particle::Drag::get();
	auto& trail = Particle::Trail::get();
	auto& shipClick = Particle::ShipClick::get();

	for (int i = 0; i < 2; i++) {
		trail[i].posVar = CCPoint(0, 2);
		trail[i].speed = 15;
		trail[i].speedVar = 4;

		shipClick[i].posVar = CCPoint(0, 2);
		shipClick[i].speed = 150;
		shipClick[i].speedVar = 40;
		shipClick[i].startSize = 6;
		shipClick[i].startSizeVar = 4.5;

		drag[i].posVar = drag[i].srcPV;
	}
}