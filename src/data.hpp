#pragma once

class Data {
public:
	static Data& get() {
		static Data instance;
		return instance;
	}

	bool legacyTracking;
	bool legacyScaling;
	bool legacyLand;
	bool inconstVal;
	bool centerDash;
};