#include <Geode/binding/GJBaseGameLayer.hpp>
#include <Settings.hpp>
#include <TLPPlayerObject.hpp>

namespace {
std::vector<std::shared_ptr<geode::Hook>> s_hooks;
}

TLPPlayerObject::Fields::Fields()
    : m_groundParticles(nullptr), m_trailParticles(nullptr),
      m_lastVehicleSize(1.f), m_wasUpsideDown(false) {}

void TLPPlayerObject::onModify(Self& self) {
    for (auto const& [name, hook] : self.m_hooks) {
        if (name == "PlayerObject::init") continue;
        s_hooks.push_back(hook);
    }
    auto setHookPrio = [](Self& a_self, std::string_view displayName) -> void {
        if (!a_self.setHookPriority(displayName, geode::Priority::VeryLatePost))
            geode::log::warn("Failed to set hook priority for {}", displayName);
    };
    setHookPrio(self, "PlayerObject::addAllParticles");
    setHookPrio(self, "PlayerObject::stopParticles");
}

bool TLPPlayerObject::init(int player, int ship, GJBaseGameLayer* gameLayer,
                           cocos2d::CCLayer* layer, bool playLayer) {
    if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
        return false;

    if (gameLayer) {
        // is this fine?
        for (auto& hook : s_hooks)
            (void) hook->toggle(!gameLayer->m_isEditor);
        if (gameLayer->m_isEditor) return true;
    }

    initGroundParticles();
    initTrailParticles();

    auto& f = m_fields;
    m_particleSystems->removeObject(m_playerGroundParticles, false);
    m_particleSystems->removeObject(m_trailingParticles, false);
    m_particleSystems->addObject(f->m_groundParticles);
    m_particleSystems->addObject(f->m_trailParticles);

    return true;
}

void TLPPlayerObject::initGroundParticles() {
    auto& obj = m_fields->m_groundParticles;
    obj = cocos2d::CCParticleSystemQuad::create("dragEffect.plist", false);
    obj->setPositionType(cocos2d::tCCPositionType::kCCPositionTypeRelative);
    obj->setID("ground-particles"_spr);
}

void TLPPlayerObject::initTrailParticles() {
    auto& obj = m_fields->m_trailParticles;
    obj = cocos2d::CCParticleSystemQuad::create("dragEffect.plist", false);
    obj->setPosVar(ccp(0, 2));
    obj->setSpeed(obj->getSpeed() * 0.2f);
    obj->setPositionType(cocos2d::tCCPositionType::kCCPositionTypeRelative);
    obj->setID("trail-particles"_spr);
}

void TLPPlayerObject::addAllParticles() {
    for (cocos2d::CCObject* obj : geode::cocos::CCArrayExt(m_particleSystems)) {
        auto particle = static_cast<cocos2d::CCParticleSystemQuad*>(obj);
        m_parentLayer->addChild(particle);
    }
}

void TLPPlayerObject::stopParticles() {
    for (cocos2d::CCObject* obj : geode::cocos::CCArrayExt(m_particleSystems))
        static_cast<cocos2d::CCParticleSystemQuad*>(obj)->stopSystem();
}

// `PlayerObject::flipGravity` and `PlayerObject::togglePlayerScale` are
// called numerous times during initialization. because of this, i decided to
// add state guards and new fixed functions

void TLPPlayerObject::flipGravity(bool flip, bool noEffects) {
    PlayerObject::flipGravity(flip, noEffects);
    if (m_fields->m_wasUpsideDown != m_isUpsideDown) {
        onGravityFlip();
        m_fields->m_wasUpsideDown = m_isUpsideDown;
    }
}

void TLPPlayerObject::togglePlayerScale(bool enable, bool noEffects) {
    PlayerObject::togglePlayerScale(enable, noEffects);
    if (m_fields->m_lastVehicleSize != m_vehicleSize) {
        onSizeChange(m_vehicleSize);
        m_fields->m_lastVehicleSize = m_vehicleSize;
    }
}

void TLPPlayerObject::toggleRobotMode(bool enable, bool noEffects) {
    PlayerObject::toggleRobotMode(enable, noEffects);
    if (enable) m_fields->m_groundParticles->setPosVar(ccp(15, 0));
}

void TLPPlayerObject::toggleSpiderMode(bool enable, bool noEffects) {
    PlayerObject::toggleSpiderMode(enable, noEffects);
    if (enable) m_fields->m_groundParticles->setPosVar(ccp(15, 0));
}

void TLPPlayerObject::onSizeChange(float size) {
    if (settings::is2p1ScalingEnabled()) {
        for (cocos2d::CCObject* obj :
             geode::cocos::CCArrayExt(m_particleSystems))
            static_cast<cocos2d::CCParticleSystemQuad*>(obj)
                ->loadScaledDefaults(size);
        return;
    }
    for (cocos2d::CCObject* obj : geode::cocos::CCArrayExt(m_particleSystems)) {
        auto particle = static_cast<cocos2d::CCParticleSystemQuad*>(obj);
        std::string_view particleID = particle->getID();
        if (particleID.empty() ||
            !particleID.starts_with(geode::Mod::get()->getID()))
            particle->loadScaledDefaults(1.f);
        particle->setScale(size);
    }
}

void TLPPlayerObject::onGravityFlip() {
    auto& groundParticles = m_fields->m_groundParticles;
    cocos2d::CCPoint grav = groundParticles->getGravity();
    grav.y                = -grav.y;
    groundParticles->setGravity(grav);
    groundParticles->setAngle(-groundParticles->getAngle());
}

void TLPPlayerObject::updateParticles() {
    updateGroundParticlesPos();
    updateGroundParticlesEmission();
    // ship: 12, 9
    // ufo: 0, 10
    auto& trailParticles = m_fields->m_trailParticles;
    if (isFlying() && !m_isDart) {
        if (!trailParticles->isActive()) trailParticles->resumeSystem();
        cocos2d::CCPoint factor;
        if (m_isShip) factor = ccp(12, m_isUpsideDown ? -9 : 9);
        if (m_isBird) factor = ccp(0, m_isUpsideDown ? -10 : 10);
        factor *= m_vehicleSize;
        trailParticles->setPosition(this->getPosition() - factor);
    } else if (trailParticles->isActive()) trailParticles->stopSystem();
}

void TLPPlayerObject::updateParticleColors() {
    auto ubtof    = [](GLubyte val) -> float { return (float) val / 255.f; };
    auto c3btoc4f = [ubtof](cocos2d::ccColor3B val) -> cocos2d::ccColor4F {
        return {ubtof(val.r), ubtof(val.g), ubtof(val.b), 1.f};
    };
    m_fields->m_groundParticles->setStartColor(
        c3btoc4f(m_iconSprite->getColor()));
}

void TLPPlayerObject::updateGroundParticlesPos() {
    cocos2d::CCPoint factor(10.f, m_isUpsideDown ? -13.f : 13.f);
    if (m_isBall && !m_isOnGround3) factor.y = -factor.y;
    factor *= m_vehicleSize;
    m_fields->m_groundParticles->setPosition(this->getPosition() - factor);
}

void TLPPlayerObject::updateGroundParticlesEmission() {
    auto& obj = m_fields->m_groundParticles;
    if (m_hasGroundParticles && !m_isHidden) {
        if (!obj->isActive()) obj->resumeSystem();
        return;
    }
    if (obj->isActive()) obj->stopSystem();
}