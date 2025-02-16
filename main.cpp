#include "dynamique.hpp"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    State etat;
    etat.reset();
    etat.show();
    etat.twist_vec[0] = 15*0;
    etat.show();

    Actionneur cmd;

    cmd.aileron_angle[0] = 0;
    cmd.aileron_angle[1] = 0;
    cmd.aileron_angle[2] = 0.31;
    cmd.rpm_mot = 200;
    cout << "Commande aileron : ";
    cout << " " << cmd.rpm_mot << " ";
    for (int i = 0; i < 2; i++) cout << cmd.aileron_angle[i] << " " << cmd.gouverne_angle[i] << " ";
    cout << "main" << endl;

    Forces forces;
    Dynamique dyn;
    
    for (int i = 0; i < 50; i++) {
        forces = compute_forces(&etat, cmd);
        dyn = compute_acc_moment(etat, forces);
        ground_correction(etat, &dyn);
        etat = update_state(etat, dyn, 0.1);

    }
    forces.show();
    dyn.show();
    cout << "etat.angular_position.rpm_mot " << etat.angular_position.rpm_mot << endl;
    etat.show();

    return 0;
}