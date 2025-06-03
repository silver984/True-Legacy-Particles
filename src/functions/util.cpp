#include <Geode/modify/PlayerObject.hpp>
#include "../variables/settings.hpp"
#include "../variables/particle.hpp"
#include "util.hpp"

using namespace geode::prelude;

bool check(PlayerObject* player, bool detectPlayer = false) {
    if (!toggle)
        return false;

    if (!detectPlayer)
        return true;

    if (!player)
        return false;

    if (!player1 && !player->m_isSecondPlayer)
        return false;

    if (!player2 && player->m_isSecondPlayer)
        return false;

    return true;
}

int index(PlayerObject* player) {
	if (player->m_isSecondPlayer)
		return 1;
	else
		return 0;

	return 0;
}