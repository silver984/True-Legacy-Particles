#include <Geode/modify/PlayerObject.hpp>
#include "variance.hpp"
#include "util.hpp"
#include "../variables/settings.hpp"
#include "../variables/particle.hpp"

void scale(PlayerObject* player, bool isMode) {
	if (!isTrueVals || !isMode || legacyScaling)
		return;

	int i = index(player);
	drag[i].posVar = sourcePosVar;
	trail[i].posVar = sourcePosVar;
	trail[i].speed = sourceSpeed.x;
	trail[i].speedVar = sourceSpeed.y;
	shipClick[i].posVar = sourcePosVar;
	shipClick[i].speed = sourceSpeed.x;
	shipClick[i].speedVar = sourceSpeed.y;
	shipClick[i].startSize = sourceStartSize.x;
	shipClick[i].startSizeVar = sourceStartSize.y;
}

void wide(PlayerObject* player, bool isMode) {
	if (!isTrueVals || !isMode)
		return;

	int i = index(player);
	drag[i].posVar = CCPoint(15, 0);
}