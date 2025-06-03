#include <Geode/modify/PlayerObject.hpp>
#include "variance.hpp"
#include "util.hpp"
#include "../variables/settings.hpp"
#include "../variables/particle.hpp"

void scale(PlayerObject* player, bool isMode) {
	if (!inconstVal || !isMode || legacyScaling)
		return;

	int i = index(player);
	drag[i].posVar = drag[i].sourcePosVar;
	trail[i].posVar = drag[i].posVar;
	trail[i].speed = drag[i].speed;
	trail[i].speedVar = drag[i].speedVar;
	shipClick[i].posVar = drag[i].posVar;
	shipClick[i].speed = drag[i].speed;
	shipClick[i].speedVar = drag[i].speedVar;
	shipClick[i].startSize = drag[i].startSize;
	shipClick[i].startSizeVar = drag[i].startSizeVar;
}

void wide(PlayerObject* player, bool isMode) {
	if (!inconstVal || !isMode)
		return;

	int i = index(player);
	drag[i].posVar = CCPoint(15, 0);
}