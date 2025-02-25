#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "dynamique.hpp"
#include "server.h"

// Fonction pour lancer la simulation
Data lancer_simulation(State &etat, Actionneur &cmd, double temps, double altitude_u, double vitesse_u);

#endif // SIMULATION_HPP