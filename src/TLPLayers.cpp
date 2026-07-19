#include <TLPLayers.hpp>
#include <TLPPlayerObject.hpp>

// particles are updated in these layers

// `GJBaseGameLayer::update` and `MenuGameLayer::update` are used since
// `PlayerObject::update` runs in fixed timesteps (there is no need updating
// particles in the same frequency as physics calculations)

void TLPGJBaseGameLayer::update(float dt) {
    GJBaseGameLayer::update(dt);
    if (m_isEditor)
        return;
    static_cast<TLPPlayerObject*>(m_player1)->updateParticles();
    if (TLPPlayerObject* player2 = static_cast<TLPPlayerObject*>(m_player2);
        player2->getParent() != nullptr) {
        player2->updateParticles();
    }
}

void TLPGJBaseGameLayer::createPlayer() {
    GJBaseGameLayer::createPlayer();
    if (m_isEditor)
        return;
    // update particle colors here since theyre not set during
    // `PlayerObject::init`
    static_cast<TLPPlayerObject*>(m_player1)->updateParticleColors();
    static_cast<TLPPlayerObject*>(m_player2)->updateParticleColors();
}

void TLPMenuGameLayer::update(float dt) {
    MenuGameLayer::update(dt);
    static_cast<TLPPlayerObject*>(m_playerObject)->updateParticles();
}

void TLPMenuGameLayer::resetPlayer() {
    MenuGameLayer::resetPlayer();
    static_cast<TLPPlayerObject*>(m_playerObject)->updateParticleColors();
}