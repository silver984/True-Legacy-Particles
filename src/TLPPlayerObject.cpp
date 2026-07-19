#include <Geode/binding/GJBaseGameLayer.hpp>
#include <Settings.hpp>
#include <TLPPlayerObject.hpp>

namespace {
std::vector<std::shared_ptr<geode::Hook>> s_hooks;
}

TLPPlayerObject::Fields::Fields()
    : m_groundParticles(nullptr), m_lastVehicleSize(1.f),
      m_wasUpsideDown(false) {}

void TLPPlayerObject::onModify(Self& self) {
    for (auto const& [name, hook] : self.m_hooks) {
        if (name == "PlayerObject::init")
            continue;
        s_hooks.push_back(hook);
    }
}

bool TLPPlayerObject::init(int player, int ship, GJBaseGameLayer* gameLayer,
                           cocos2d::CCLayer* layer, bool playLayer) {
    if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer))
        return false;

    if (gameLayer) {
        // is this fine?
        for (auto& hook : s_hooks)
            (void) hook->toggle(!gameLayer->m_isEditor);
        if (gameLayer->m_isEditor)
            return true;
    }

    using enum cocos2d::tCCPositionType;
    auto& groundParticles = m_fields->m_groundParticles;
    groundParticles =
        cocos2d::CCParticleSystemQuad::create("dragEffect.plist", false);
    groundParticles->setPositionType(kCCPositionTypeRelative);
    groundParticles->setID("ground-particles"_spr);
    // remove `m_playerGroundParticles` from `m_particleSystems` and replace
    // it with our `m_fields->m_groundParticles`
    m_particleSystems->removeObject(m_playerGroundParticles, false);
    m_particleSystems->addObject(groundParticles);

    return true;
}

void TLPPlayerObject::addAllParticles() {
    PlayerObject::addAllParticles();
    auto& groundParticles = m_fields->m_groundParticles;
    m_parentLayer->addChild(groundParticles);
    // remove the original ground particles from its parent, assuming that
    // it has already been added by `PlayerObject::addAllParticles`
    m_playerGroundParticles->removeFromParentAndCleanup(true);
}

void TLPPlayerObject::stopParticles() {
    PlayerObject::stopParticles();
    m_fields->m_groundParticles->stopSystem();
}

void TLPPlayerObject::flipGravity(bool flip, bool noEffects) {
    PlayerObject::flipGravity(flip, noEffects);
    if (m_fields->m_wasUpsideDown != m_isUpsideDown) {
        onGravityFlip();
        m_fields->m_wasUpsideDown = m_isUpsideDown;
    }
}

// `PlayerObject::togglePlayerScale` and `PlayerObject::flipGravity` are
// called numerous times during initialization for some reason. because of
// this, i decided to just add state guards and new fixed functions for them

void TLPPlayerObject::togglePlayerScale(bool enable, bool noEffects) {
    PlayerObject::togglePlayerScale(enable, noEffects);
    if (m_fields->m_lastVehicleSize != m_vehicleSize) {
        onSizeChange(m_vehicleSize);
        m_fields->m_lastVehicleSize = m_vehicleSize;
    }
}

void TLPPlayerObject::toggleRobotMode(bool enable, bool noEffects) {
    PlayerObject::toggleRobotMode(enable, noEffects);
    if (enable)
        m_fields->m_groundParticles->setPosVar(ccp(15.f, 0.f));
}

void TLPPlayerObject::toggleSpiderMode(bool enable, bool noEffects) {
    PlayerObject::toggleSpiderMode(enable, noEffects);
    if (enable)
        m_fields->m_groundParticles->setPosVar(ccp(15.f, 0.f));
}

void TLPPlayerObject::onSizeChange(float size) {
    auto& groundParticles = m_fields->m_groundParticles;
    if (settings::is2p1ScalingEnabled()) {
        groundParticles->loadScaledDefaults(size);
        return;
    }
    groundParticles->setScale(size);
}

void TLPPlayerObject::onGravityFlip() {
    auto& groundParticles                = m_fields->m_groundParticles;
    cocos2d::CCPoint groundParticlesGrav = groundParticles->getGravity();
    groundParticlesGrav.y                = -groundParticlesGrav.y;
    groundParticles->setGravity(groundParticlesGrav);
    groundParticles->setAngle(-groundParticles->getAngle());
}

void TLPPlayerObject::updateParticles() {
    updateGroundParticlesPos();
    updateGroundParticlesEmission();
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
    if (m_isBall && !m_isOnGround3)
        factor.y = -factor.y;
    factor *= m_vehicleSize;
    m_fields->m_groundParticles->setPosition(this->getPosition() - factor);
}

void TLPPlayerObject::updateGroundParticlesEmission() {
    auto& obj = m_fields->m_groundParticles;
    if (m_hasGroundParticles && !m_isHidden) {
        if (!obj->isActive())
            obj->resumeSystem();
        return;
    }
    if (obj->isActive())
        obj->stopSystem();
}