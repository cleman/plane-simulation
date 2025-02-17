#include "simulation.hpp"
#include <iostream>
#include <vector>

Data lancer_simulation(State &etat, Actionneur &cmd, double temps) {
    std::cout << "Lancement de la simulation" << std::endl;
    Forces forces;
    Dynamique dyn;
    double dt = 0.1;
    Data simulation_data;

    for (double i = 0; i < temps; i += dt) {
        forces = compute_forces(&etat, cmd);
        dyn = compute_acc_moment(etat, forces);
        ground_correction(etat, &dyn);
        etat = update_state(etat, dyn, dt);

        // Stocker les résultats à chaque itération
        simulation_data.add_entry(etat, i, forces, dyn);
    }

    forces.show();
    dyn.show();
    etat.show();

    return simulation_data;
}