#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <TLPPlayerObject.hpp>

// particles are updated in these layers

// `GJBaseGameLayer::update` and `MenuGameLayer::update` are used since
// `PlayerObject::update` runs in fixed timesteps (there is no need updating
// particles in the same frequency as physics calculations)

struct TLPGJBaseGameLayer final
    : geode::Modify<TLPGJBaseGameLayer, GJBaseGameLayer> {
    void update(float dt) override {
        GJBaseGameLayer::update(dt);
        if (m_isEditor) return;
        auto player1 = static_cast<TLPPlayerObject*>(m_player1);
        auto player2 = static_cast<TLPPlayerObject*>(m_player2);
        if (!player1->m_isDead) player1->updateParticles();
        if (player2->getParent() != nullptr && !player2->m_isDead)
            player2->updateParticles();
    }

    void createPlayer() {
        GJBaseGameLayer::createPlayer();
        if (m_isEditor) return;
        // update particle colors here since theyre not set during
        // `PlayerObject::init`
        static_cast<TLPPlayerObject*>(m_player1)->updateParticleColors();
        static_cast<TLPPlayerObject*>(m_player2)->updateParticleColors();
    }
};

struct TLPMenuGameLayer final : geode::Modify<TLPMenuGameLayer, MenuGameLayer> {
    void update(float dt) override {
        MenuGameLayer::update(dt);
        auto playerObject = static_cast<TLPPlayerObject*>(m_playerObject);
        if (!playerObject->m_isDead) playerObject->updateParticles();
    }

    void resetPlayer() {
        MenuGameLayer::resetPlayer();
        static_cast<TLPPlayerObject*>(m_playerObject)->updateParticleColors();
    }
};