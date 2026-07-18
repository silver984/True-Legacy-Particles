#include <TLPGJBaseGameLayer.hpp>
#include <TLPPlayerObject.hpp>

void TLPGJBaseGameLayer::update(float dt) {
    GJBaseGameLayer::update(dt);
    if (m_isEditor)
        return;
    // particles are updated here instead since `PlayerObject::update` runs in
    // fixed timesteps
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