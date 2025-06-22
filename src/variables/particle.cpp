#include "particle.hpp"

std::array<Particle, 2>& particle = Particle::get();
std::array<Particle::Drag, 2>& drag = Particle::Drag::get();
std::array<Particle::Trail, 2>& trail = Particle::Trail::get();
std::array<Particle::ShipClick, 2>& shipClick = Particle::ShipClick::get();
std::array<Particle::Land, 2>& land = Particle::Land::get();
std::array<Particle::Extra, 2>& extra = Particle::Extra::get();
CCPoint sourceAngle = CCPoint(0, 0);
CCPoint sourceGravity = CCPoint(0, 0);
CCPoint landGravity = CCPoint(0, 0);
float landAngle = 0.f;
CCPoint sourceStartSize = CCPoint(0, 0);
CCPoint sourceSpeed = CCPoint(0, 0);
CCPoint sourcePosVar = CCPoint(0, 0);