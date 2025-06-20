#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "LegacyTracking.hpp"
#include "LegacyScaling.hpp"
#include "AlwaysSpawn.hpp"
#include "TrueValues.hpp"
#include "util.hpp"
#include "../variables/settings.hpp"

using namespace geode::prelude;

// this function basically just calls other functions
void core(PlayerObject* player) {
	tracking(player);
	scaling(player);
	trueVals(player);
	alwaysSpawnFunc(player);
}