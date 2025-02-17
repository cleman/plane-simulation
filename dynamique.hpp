#ifndef PHYSIQUE_HPP
#define PHYSIQUE_HPP

#include "plane.hpp"

#include <iostream>
#include <math.h>
#include <Eigen/Dense> 
using namespace std;
using namespace Eigen;

struct Forces {
    double weight;
    double thrust;
    double lift[4];
    double drag[4];

    void reset() {
        weight = 0;
        thrust = 0;
        for (int i = 0; i < 4; i++) {
            lift[i] = 0;
            drag[i] = 0;
        }
    }

    void show() {
        cout << "Weight: " << weight << endl;
        cout << "Thrust: " << thrust << endl;
        cout << "Lift: " << endl;
        for (int i = 0; i < 4; i++) {
            cout << lift[i] << " ";
        }
        cout << endl;
        cout << "drag: " << endl;
        for (int i = 0; i < 4; i++) {
            cout << drag[i] << " ";
        }
        cout << endl;
    }
};

struct Actionneur {
    double rpm_mot;
    double aileron_angle[2];
    double gouverne_angle[2];

    void reset() {
        rpm_mot = 0;
        for (int i = 0; i < 2; i++) {
            aileron_angle[i] = 0;
            gouverne_angle[i] = 0;
        }
    }

    void set(const double cmd[5]) {
        // Affecter les valeurs à pose_vec et twist_vec
        rpm_mot = cmd[0];
        for (int i = 0; i < 2; i++) {
            aileron_angle[i] = cmd[i+1];  // Remplir pose_vec avec les valeurs de l'état
            gouverne_angle[i] = cmd[i+3];  // Remplir twist_vec (ou peut-être une autre logique si twist_vec doit être différent de pose_vec)
        }
}

};

struct State {
    //double pose_vec[6];
    //double twist_vec[6];

    VectorXd pose_vec;
    VectorXd twist_vec;

    State() : pose_vec(6), twist_vec(6) {}
    
    Actionneur angular_position;

    void reset() {
        for (int i = 0; i < 6; i++) {
            //pose_vec[i] = 0;
            //twist_vec[i] = 0;

            pose_vec(i) = 0;
            twist_vec(i) = 0;
        }
    }

    void show() {
        cout << "Affichage de l'état" << endl << "pose_vec: " << endl;
        for (int i = 0; i < 6; i++) {
            cout << pose_vec[i] << " ";
        }
        cout << endl;
        cout << "twist_vec: " << endl;
        for (int i = 0; i < 6; i++) {
            cout << twist_vec[i] << " ";
        }
        cout << endl;
        cout << "Actionneur position: " << endl;
        cout << angular_position.rpm_mot << endl;
        for (int i = 0; i < 2; i++) {
            cout << angular_position.aileron_angle[i] << " ";
            cout << endl;
            cout << angular_position.gouverne_angle[i] << " ";
            cout << endl;
            }
        cout << endl;
    }

    void set(const double etat[6], const double cmd[5]) {
        // Affecter les valeurs à pose_vec et twist_vec
        for (int i = 0; i < 6; i++) {
            pose_vec(i) = 0;
            twist_vec(i) = etat[i];  // Remplir twist_vec (ou peut-être une autre logique si twist_vec doit être différent de pose_vec)
        }

        // Affecter les valeurs aux actionneurs
        angular_position.rpm_mot = cmd[0];
        for (int i = 0; i < 2; i++) {
            angular_position.aileron_angle[i] = cmd[i + 1];  // cmd[1] et cmd[2] pour aileron_angle
            angular_position.gouverne_angle[i] = cmd[i + 3];  // cmd[3] et cmd[4] pour gouverne_angle
        }
    }
};

struct Dynamique {
    //double acc_vec[3];       // X-Y-Z
    //double moment_vec[3];    // Roll-pitch-yaw

    VectorXd acc_vec;
    VectorXd moment_vec;

    Dynamique() : acc_vec(3), moment_vec(3) {}

    void reset() {
        for (int i = 0; i < 3; i++) {
            //acc_vec[i] = 0;
            //moment_vec[i] = 0;

            acc_vec(i) = 0;
            moment_vec(i) = 0;
        }
    }

    void show() {
        cout << "Affichage des accélérations" << endl << "acc_vec: " << endl;
        for (int i = 0; i < 3; i++) {
            cout << acc_vec(i) << " ";
        }
        cout << endl;
        cout << "moment_vec: " << endl;
        for (int i = 0; i < 3; i++) {
            cout << moment_vec(i) << " ";
        }
        cout << endl;

    }
};

struct CoefAile  {
    double C_L[4];
    double C_D[4];

    void reset() {
        for (int i = 0; i < 4; i++) {
            C_L[i] = C_L_0[i/2];
            C_D[i] = C_D_0[i/2];
        }
    }

    void compute_coef(double angle[4]) {
        for (int i = 0; i < 4; i++) {
            C_L[i] = C_L_0[i/2] + C_L_alpha[i/2]*alpha + C_L_delta[i/2]*angle[i];
            C_D[i] = C_D_0[i/2] + k_D[i/2]*pow(C_L[i],2) + C_D_delta[i/2]*angle[i];
        }
    }

    void show() {
        for (int i = 0; i < 4; i++) {
            cout << "C_L[" << i << "] = " << C_L[i] << " C_D[" << i << "] = " << C_D[i] << endl;
        }
    }
};

#include <vector>

struct Data {

    std::vector<double> accX;
    std::vector<double> accY;
    std::vector<double> accZ;

    std::vector<double> speedX;
    std::vector<double> speedY;
    std::vector<double> speedZ;

    std::vector<double> thrust;
    std::vector<double> weight;
    std::vector<std::vector<double>> lift;
    std::vector<std::vector<double>> drag;

    std::vector<double> time;
    
    void add_entry(State etat, double time_value, Forces forces, Dynamique dyn) {
        accX.push_back(dyn.acc_vec(0));
        accY.push_back(dyn.acc_vec(1));
        accZ.push_back(dyn.acc_vec(2));
        
        // Ajouter les composantes de la vitesse dans les bons vecteurs
        speedX.push_back(etat.twist_vec(0));
        speedY.push_back(etat.twist_vec(1));
        speedZ.push_back(etat.twist_vec(2));

        // Ajouter la poussée
        thrust.push_back(forces.thrust);
        weight.push_back(forces.weight);

        // Convertir lift et drag en std::vector<double> avant de les ajouter
        lift.push_back(std::vector<double>(forces.lift, forces.lift + 4));
        drag.push_back(std::vector<double>(forces.drag, forces.drag + 4));

        // Ajouter le temps
        time.push_back(time_value);
    }
};

Forces compute_forces(State *etat, Actionneur cmd);
Dynamique compute_acc_moment(State etat, Forces forces);
void ground_correction(State etat, Dynamique *dyn);
State update_state(State etat, Dynamique dynamique, double dt);

#endif