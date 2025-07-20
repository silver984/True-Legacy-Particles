#include <Geode/modify/PlayerObject.hpp>
#include "variance.hpp"
#include "../util.hpp"
#include "../globals/settings.hpp"
#include "../globals/particle.hpp"

using namespace geode::prelude;

void scale(PlayerObject* player, bool isMode) {
	if (!g_isTrueValues || g_isLegacyScaling || !isMode) return;
	int i = playerIndex(player);
	g_particle[i].isDeviated = true;
	g_drag[i].posVar = g_sourcePosVar;
	g_trail[i].posVar = g_sourcePosVar;
	g_trail[i].speed = g_sourceSpeed.x;
	g_trail[i].speedVar = g_sourceSpeed.y;
	g_shipClick[i].posVar = g_sourcePosVar;
	g_shipClick[i].speed = g_sourceSpeed.x;
	g_shipClick[i].speedVar = g_sourceSpeed.y;
	g_shipClick[i].startSize = g_sourceStartSize.x;
	g_shipClick[i].startSizeVar = g_sourceStartSize.y;
	g_shipClick[i].endSize = g_sourceEndSize.x;
	g_shipClick[i].endSizeVar = g_sourceEndSize.y;
}

void wide(PlayerObject* player, bool isMode) {
	if (!g_isTrueValues || !isMode) return;
	int i = playerIndex(player);
	g_drag[i].posVar = CCPoint(15, 0);
}