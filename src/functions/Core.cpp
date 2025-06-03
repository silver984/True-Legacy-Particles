#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "LegacyTracking.hpp"
#include "LegacyScaling.hpp"
#include "InconsistentValue.hpp"
#include "util.hpp"
#include "../variables/settings.hpp"

using namespace geode::prelude;

// this function basically just calls other functions
void core(PlayerObject* player) {
	tracking(player);
	scaling(player);
	inconsistentVal(player);
}