#include <Geode/modify/PlayerObject.hpp>
#include "globals/settings.hpp"
#include "globals/particle.hpp"
#include "util.hpp"

bool isValid(PlayerObject* player, bool detectPlayer = false) {
    if (!g_isModEnabled)
        return false;

    if (!detectPlayer)
        return true;

    if (!g_affectP1 && !player->m_isSecondPlayer)
        return false;

    if (!g_affectP2 && player->m_isSecondPlayer)
        return false;

    return true;
}

int playerIndex(PlayerObject* player) {
	if (player->m_isSecondPlayer)
		return 1;
	else
		return 0;

	return 0;
}