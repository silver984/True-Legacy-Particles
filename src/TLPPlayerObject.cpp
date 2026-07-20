#include <Geode/binding/GJBaseGameLayer.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
#include <Settings.hpp>
#include <TLPPlayerObject.hpp>

struct TLPCCParticleSystem final
    : geode::Modify<TLPCCParticleSystem, cocos2d::CCParticleSystem> {
    using Self = geode::modifier::ModifyBase<geode::modifier::ModifyDerive<
        TLPCCParticleSystem, cocos2d::CCParticleSystem>>;
    static void onModify(Self& self) {
        (void) self.setHookPriority(
            "cocos2d::CCParticleSystem::loadScaledDefaults",
            geode::Priority::Last);
    }

    void loadScaledDefaults(float) {
        // intentionally do nothing
    }

    // @note True Legacy Particles Addition
    void loadScaledDefaults2(float scale) {
        setStartSize(m_fDefaultStartSize * scale);
        setStartSizeVar(m_fDefaultStartSizeVar * scale);
        setEndSize(m_fDefaultEndSize * scale);
        setAngle(m_fDefaultAngle * scale);
        setSpeed(m_fDefaultModeASpeed * scale);
        setSpeedVar(m_fDefaultModeASpeedVar * scale);
        setPosVar(m_tDefaultPosVar * scale);
    }
};

TLPPlayerObject::Fields::Fields()
    : m_lastVehicleSize(1.f), m_wasUpsideDown(false) {}

void TLPPlayerObject::onModify(Self& self) {
    (void) self.setHookPriority("PlayerObject::updatePlayerArt",
                                geode::Priority::Last);
}

void TLPPlayerObject::updatePlayerArt() {
    // todo: walking on walls (this implementation is not faithful enough)
    if (m_isShip || m_isBird || m_isDart || m_isRobot || m_isSpider)
        m_mainLayer->setScaleY(m_isUpsideDown ? -1.f : 1.f);
    else m_mainLayer->setScaleY(1.f);
}

void TLPPlayerObject::togglePlayerScale(bool enable, bool noEffects) {
    PlayerObject::togglePlayerScale(enable, noEffects);
    if (m_fields->m_lastVehicleSize != m_vehicleSize) {
        onSizeChange();
        m_fields->m_lastVehicleSize = m_vehicleSize;
    }
}

void TLPPlayerObject::flipGravity(bool flip, bool noEffects) {
    PlayerObject::flipGravity(flip, noEffects);
    if (m_fields->m_wasUpsideDown != m_isUpsideDown) {
        onGravityFlip();
        m_fields->m_wasUpsideDown = m_isUpsideDown;
    }
}

void TLPPlayerObject::onSizeChange() {
    if (settings::is2p1ScalingEnabled()) {
        for (cocos2d::CCObject* obj :
             geode::cocos::CCArrayExt(m_particleSystems))
            static_cast<TLPCCParticleSystem*>(obj)->loadScaledDefaults2(
                m_vehicleSize);
        return;
    }
    for (cocos2d::CCObject* obj : geode::cocos::CCArrayExt(m_particleSystems))
        static_cast<cocos2d::CCParticleSystemQuad*>(obj)->setScale(
            m_vehicleSize);
}

void TLPPlayerObject::onGravityFlip() {
    cocos2d::CCPoint grav = m_playerGroundParticles->getGravity();
    grav.y                = -grav.y;
    m_playerGroundParticles->setGravity(grav);
    m_playerGroundParticles->setAngle(-m_playerGroundParticles->getAngle());
}

void TLPPlayerObject::updateGroundParticles() {
    cocos2d::CCPoint factor(10.f, m_isUpsideDown ? -13.f : 13.f);
    if (m_isBall && !m_isOnGround3) factor.y = -factor.y;
    factor *= m_vehicleSize;
    m_playerGroundParticles->setPosition(this->getPosition() - factor);
}