#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <TLPPlayerObject.hpp>

using namespace geode::cast;

// `GJBaseGameLayer::update` and `MenuGameLayer::update` are used since
// `PlayerObject::update` runs in fixed timesteps (there is no need updating
// particles in the same frequency as physics calculations)

struct TLPGJBaseGameLayer final
    : geode::Modify<TLPGJBaseGameLayer, GJBaseGameLayer> {
    $override void update(float dt) {
        GJBaseGameLayer::update(dt);
        if (m_isEditor) return;
        TLPPlayerObject* player1 = modify_cast<TLPPlayerObject*>(m_player1);
        TLPPlayerObject* player2 = modify_cast<TLPPlayerObject*>(m_player2);
        if (!player1->m_isDead) player1->updateGroundParticles();
        if (player2->getParent() != nullptr && !player2->m_isDead)
            player2->updateGroundParticles();
    }
};

struct TLPMenuGameLayer final : geode::Modify<TLPMenuGameLayer, MenuGameLayer> {
    $override void update(float dt) {
        MenuGameLayer::update(dt);
        TLPPlayerObject* playerObject =
            modify_cast<TLPPlayerObject*>(m_playerObject);
        if (!playerObject->m_isDead) playerObject->updateGroundParticles();
    }
};