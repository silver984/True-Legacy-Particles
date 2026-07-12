#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/modify/PlayerObject.hpp>

$on_mod(Loaded) {
    auto toggleHooks = [](bool val) -> void {
        for (auto& hook : geode::Mod::get()->getHooks()) {
            (void) hook->toggle(val);
        }
    };
    toggleHooks(geode::Mod::get()->getSettingValue<bool>("switch"));
    (void) geode::listenForSettingChanges<bool>("switch", toggleHooks);
}

class $modify(TLPPlayerObject, PlayerObject) {
    struct Fields {
        Fields() : m_dragParticles(nullptr), m_elapsed(0.f) {}
        cocos2d::CCParticleSystemQuad* m_dragParticles;
        float m_elapsed;
    };

    bool init(int player, int ship, GJBaseGameLayer* gameLayer,
              cocos2d::CCLayer* layer, bool playLayer) {
        if (!PlayerObject::init(player, ship, gameLayer, layer, playLayer)) {
            return false;
        }

        m_playerGroundParticles->unscheduleUpdate();
        m_playerGroundParticles->setVisible(false);

        using PSysQuad = cocos2d::CCParticleSystemQuad;
        using enum cocos2d::tCCPositionType;
        auto& drag = m_fields->m_dragParticles;
        drag       = PSysQuad::create("dragEffect.plist", false);
        drag->setPositionType(kCCPositionTypeRelative);
        m_parentLayer->addChild(drag);

        return true;
    }

    void update(float dt) override {
        PlayerObject::update(dt);

        auto& drag = m_fields->m_dragParticles;
        cocos2d::CCPoint dragPosFactor;
        dragPosFactor.x = 10.f;
        dragPosFactor.y = m_isUpsideDown ? -13.f : 13.f;

        if (m_isBall && !m_isOnGround3) {
            dragPosFactor.y = -dragPosFactor.y;
        }

        dragPosFactor *= m_vehicleSize;
        drag->setPosition(this->getPosition() - dragPosFactor);

        if (!m_isOnGround2) {
            if (drag->isActive()) {
                m_fields->m_elapsed += dt;
                float delay = 3.75f;
                while (m_fields->m_elapsed >= delay) {
                    drag->stopSystem();
                    m_fields->m_elapsed -= delay;
                }
            }
        } else {
            if (!drag->isActive()) {
                drag->resumeSystem();
            }
        }
    }

    // TLP addition
    void toggleWideDragPosVar() {
        m_fields->m_dragParticles->setPosVar(cocos2d::CCPoint(15.f, 0.f));
    }

    void toggleRobotMode(bool enable, bool noEffects) {
        PlayerObject::toggleRobotMode(enable, noEffects);
        if (enable) {
            toggleWideDragPosVar();
        }
    }

    void toggleSpiderMode(bool enable, bool noEffects) {
        PlayerObject::toggleSpiderMode(enable, noEffects);
        if (enable) {
            toggleWideDragPosVar();
        }
    }
};
