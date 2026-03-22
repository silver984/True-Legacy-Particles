#pragma once

namespace tlp
{

class Settings
{
private:
	Settings() = default;
	~Settings() = default;

public:
	static inline Settings& get()
	{
		static Settings instance;
		return instance;
	}

	Settings(const Settings&) = delete;
	Settings& operator=(const Settings&) = delete;
	Settings(Settings&&) = delete;
	Settings& operator=(Settings&&) = delete;

	void update();

	inline bool isModEnabled() const
	{
		return m_isModEnabled;
	}

private:
	bool m_isModEnabled = true;
};

} // tlp