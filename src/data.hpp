#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Data {
public:
	static Data& get() {
		static Data instance;
		return instance;
	}

	bool isInLevel;
	bool legacyTracking;
	bool legacyScaling;
	bool legacyLand;
	bool inconstVal;
	bool centerDash;
};