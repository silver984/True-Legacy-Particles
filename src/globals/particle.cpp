#include <Geode/Geode.hpp>
#include "particle.hpp"

std::array<Particle, 2>& g_particle = Particle::get();
std::array<Particle::Drag, 2>& g_drag = Particle::Drag::get();
std::array<Particle::Trail, 2>& g_trail = Particle::Trail::get();
std::array<Particle::ShipClick, 2>& g_shipClick = Particle::ShipClick::get();
std::array<Particle::Land, 2>& g_land = Particle::Land::get();
std::array<Particle::Extra, 2>& g_extra = Particle::Extra::get();
CCPoint g_sourceAngle = CCPoint(0, 0);
CCPoint g_sourceGravity = CCPoint(0, 0);
CCPoint g_landGravity = CCPoint(0, 0);
float g_landAngle = 0.f;
CCPoint g_sourceStartSize = CCPoint(0, 0);
CCPoint g_sourceSpeed = CCPoint(0, 0);
CCPoint g_sourcePosVar = CCPoint(0, 0);