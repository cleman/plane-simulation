#include <iostream>
#include "math.h"
using namespace std;

#include "dynamique.hpp"

Forces compute_forces(State *etat, Actionneur cmd) {
    etat->angular_position = cmd;

    //cout << "Forces fonction" << endl; 

    // Calcul des coefficients de portance et de trainée en fonction des commandes
    double angle[4];
    for (int i = 0; i < 2; i++) {
        angle[i] = cmd.aileron_angle[i];
        angle[i+2] = cmd.gouverne_angle[i];
    }
    CoefAile coef_aile;
    coef_aile.reset();
    coef_aile.compute_coef(angle);
    //coef_aile.show();

    double J = etat->twist_vec[0]/cmd.rpm_mot/Diameter;
    double Ct = 0.12 - 0.213*pow(J, 2);

    Forces forces;
    forces.reset();
    forces.weight = m*g;
    forces.thrust = rho_air*pow(cmd.rpm_mot, 2)*pow(Diameter, 4)*Ct;

    for (int i = 0; i < 4; i++) {
        double temp = rho_air*S[i/2]*pow(etat->twist_vec[0], 2)/2;
        forces.lift[i] = temp*coef_aile.C_L[i];
        forces.drag[i] = temp*coef_aile.C_D[i];
    }

    return forces;
}

Dynamique compute_acc_moment(State etat, Forces forces) {
    //cout << "Acceleration et moment_vec fonction" << endl;

    Dynamique dyn;
    dyn.reset();

    double theta = etat.pose_vec(4);
    
    dyn.acc_vec(0) = forces.thrust - (forces.lift[0] + forces.lift[1])*sin(alpha) - (forces.drag[0] + forces.drag[1])*cos(alpha) - forces.drag[2] - forces.drag[3] + forces.weight*sin(theta);
    dyn.acc_vec(1) = -forces.lift[3];
    dyn.acc_vec(2) = (forces.lift[0] + forces.lift[1])*cos(alpha) + forces.lift[3] - forces.weight*cos(theta);
    dyn.acc_vec /= m;

    //cout << "Accélération linéaire" << endl;
    //cout << dyn.acc_vec << endl;

    double Ixx = 2*pow(f_dim, 2)*m1 + pow(c_dim, 2)*m4;
    double Iyy = pow((g_dim - a_dim/2), 2)*m0 + 2*pow(g_dim-h_dim, 2)*m1 + pow(g_dim - a_dim + j_dim, 2)*(m3 + m4) + pow(c_dim, 2)*m4 + pow(g_dim - l_dim, 2)*m5;
    double Izz = pow(g_dim - a_dim/2, 2)*m0 + 2*(pow(g_dim-h_dim, 2) + pow(f_dim, 2))*m1 + pow(g_dim-a_dim+j_dim, 2)*(m3 + m4) + pow(g_dim-l_dim, 2)*m5;

    //cout << "Matrice d'inertie" << endl;
    //cout << "Ixx = " << Ixx << endl;
    //cout << "Iyy = " << Iyy << endl;
    //cout << "Izz = " << Izz << endl;

    Matrix3d I = Matrix3d::Zero();
    I(0, 0) = Ixx;
    I(1, 1) = Iyy;
    I(2, 2) = Izz;
    
    VectorXd sum_tau(3);

    sum_tau(0) = ( (forces.lift[0]-forces.lift[1])*cos(alpha) - (forces.drag[0]-forces.drag[1])*sin(alpha) )*f_dim + (forces.lift[3] - forces.drag[3])*c_dim;
    sum_tau(1) = ( (forces.drag[0]+forces.drag[1])*sin(alpha) - (forces.lift[0]+forces.lift[1])*cos(alpha) )*(g_dim-h_dim) - forces.lift[2]*(g-a_dim+j_dim);
    sum_tau(2) = ( (forces.lift[0] -forces.lift[1])*sin(alpha) + (forces.drag[0]-forces.drag[1])*cos(alpha) )*f_dim - forces.lift[3]*(g-a_dim+j_dim);

    //cout << "sum_tau[0] = " << sum_tau(0) << endl;
    //cout << "sum_tau[1] = " << sum_tau(1) << endl;
    //cout << "sum_tau[2] = " << sum_tau(2) << endl;

    dyn.moment_vec = I.colPivHouseholderQr().solve(sum_tau);

    return dyn;
}

void ground_correction(State etat, Dynamique *dyn) {
    if (etat.pose_vec(2) == 0 && dyn->acc_vec(2) < 0) dyn->acc_vec(2) = 0;
}

State update_state(State etat, Dynamique dynamique, double dt) {
    //cout << "update state fonction" << endl;

    etat.twist_vec.segment(0, 3) += dynamique.acc_vec * dt;
    etat.twist_vec.segment(3, 3) += dynamique.moment_vec * dt;
    etat.pose_vec += etat.twist_vec * dt;

    return etat;
}