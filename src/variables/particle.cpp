#include "particle.hpp"

std::array<Particle, 2>& particle = Particle::get();
std::array<Particle::Drag, 2>& drag = Particle::Drag::get();
std::array<Particle::Trail, 2>& trail = Particle::Trail::get();
std::array<Particle::ShipClick, 2>& shipClick = Particle::ShipClick::get();
std::array<Particle::Extra, 2>& extra = Particle::Extra::get();

bool isInLevel = false;