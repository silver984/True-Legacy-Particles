#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>

#include <tlp/PlayerObject.hpp>

using namespace geode::prelude;

namespace gd {

using GJBaseGameLayer = ::GJBaseGameLayer;
using MenuGameLayer   = ::MenuGameLayer;

} // namespace gd

namespace tlp {

struct GJBaseGameLayer;
struct MenuGameLayer;

} // namespace tlp

struct tlp::GJBaseGameLayer : Modify<tlp::GJBaseGameLayer, gd::GJBaseGameLayer> {
    void update(float dt) $override {
        gd::GJBaseGameLayer::update(dt);

        if (m_isEditor) {
            return;
        }

        auto* player1 = modify_cast<tlp::PlayerObject*>(m_player1);
        auto* player2 = modify_cast<tlp::PlayerObject*>(m_player2);

        if (!player1->m_isDead) {
            player1->updateParticles();
        }

        if (player2->getParent() && !player2->m_isDead) {
            player2->updateParticles();
        }
    }
};

struct tlp::MenuGameLayer : Modify<tlp::MenuGameLayer, gd::MenuGameLayer> {
    void update(float dt) $override {
        gd::MenuGameLayer::update(dt);
        auto* player = modify_cast<tlp::PlayerObject*>(m_playerObject);
        if (!player->m_isDead) {
            player->updateParticles();
        }
    }
};
