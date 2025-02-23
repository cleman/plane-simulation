#include "simulation.hpp"
#include "controlleurs.hpp"
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
        //std::cout << "Valeur c1_acc : " << dyn.moment_vec(2) << std::endl;

        forces = compute_forces(&etat, cmd);
        std::cout << "controlleur1_value avant : " << cmd.gouverne_angle[0] << std::endl;
        cmd.gouverne_angle[0] = controller_pitch(etat, 0, dt, forces, &simulation_data);
        std::cout << "controlleur1_value : " << cmd.gouverne_angle[0] << std::endl;

        forces.show();
        dyn.show();
        etat.show();
    }

    forces.show();
    dyn.show();
    etat.show();

    return simulation_data;
}