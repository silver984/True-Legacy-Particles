#include "particle.hpp"
#include "data.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

void legacyTrackingFunc(bool guard, int index, float delta, PlayerObject* p) {
	auto& prtcl = Particle::get();
	auto& x = Particle::Extra::get();

	if (!guard)
		return;

	if (!p->m_playerGroundParticles)
		return;

	if (p->m_isOnGround) {
		CCPoint ground = p->m_playerGroundParticles->m_obPosition;
		x[index].onGroundPos = CCPoint(ground);

		CCPoint air = x[index].onGroundPos - p->m_position;
		x[index].offGroundPos = CCPoint(air);
	}

	if (prtcl[index].framesBeforeSpiderDashed >= (2 * ((1.f / 60) / delta))) {
		if (p->m_isOnGround)
			prtcl[index].spiderDashed = false;
	}

	if (!prtcl[index].spiderDashed) {
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

void legacyScalingFunc(bool guard, PlayerObject* p) {
	if (!guard)
		return;

	for (auto system : CCArrayExt<CCParticleSystem*>(p->m_particleSystems)) {
		system->loadScaledDefaults(1.f);
		system->setScale(p->m_vehicleSize);
	}
}

void inconstValFunc(bool guard, bool scaling, int index, PlayerObject* p) {
	auto& drag = Particle::Drag::get();
	auto& trail = Particle::Trail::get();
	auto& shipClick = Particle::ShipClick::get();

	if (!guard)
		return;

	float m;
	if (scaling)
		m = 1.f;
	else
		m = p->m_vehicleSize;

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

void core(const CoreParams& c) {
	legacyTrackingFunc(c.legacyTracking, c.index, c.delta, c.p);
	legacyScalingFunc(c.legacyScaling, c.p);
	inconstValFunc(c.inconstVal, c.legacyScaling, c.index, c.p);
}

void scale(const ScaleParams& s) {
	auto& drag = Particle::Drag::get();
	auto& trail = Particle::Trail::get();
	auto& shipClick = Particle::ShipClick::get();

	bool guard = s.isInLevel || s.inconstVal;
	if (!guard)
		return;

	if (!s.isMini)
		return;
	
	if (s.legacyScaling)
		return;

	int index;
	if (!s.isSecondPlayer)
		index = 0;
	else
		index = 1;

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

void wide(const WideParams& w) {
	auto& drag = Particle::Drag::get();

	bool guard = w.isInLevel || w.inconstVal;
	if (!guard)
		return;

	if (!w.isMode)
		return;

	if (!w.isSecondPlayer)
		drag[0].posVar = CCPoint(15, 0);
	else
		drag[1].posVar = CCPoint(15, 0);
}

void spiderDash(bool isSecondPlayer) {
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

void reset(bool isInLevel) {
	auto& particle = Particle::get();
	auto& drag = Particle::Drag::get();
	auto& trail = Particle::Trail::get();
	auto& shipClick = Particle::ShipClick::get();

	if (!isInLevel)
		return;

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