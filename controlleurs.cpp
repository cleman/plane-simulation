#include "controlleurs.hpp"
#include "simulation.hpp"

double sum_err_cont1 = 0;
double kp1 = 10;
double kd1 = 7;
double ki1 = 0.4;

double controller_pitch(State etat, double cons, double dt, Forces forces, Data *s) {
    double err = cons - etat.pose_vec(4);
    sum_err_cont1 += err*dt;
    double Acc_cons = kp1*err - kd1*etat.twist_vec(4) + ki1*sum_err_cont1;
    double couple_y = Iyy*Acc_cons;


    double delta = (forces.drag[0] + forces.drag[1])*sin(alpha) - (forces.lift[0]+forces.lift[1])*cos(alpha);
    cout << "delta : " << delta << endl;
    delta = -delta*(g_dim-h_dim) - couple_y;
    cout << "delta : " << delta << ", g-h : " << g_dim-h_dim << ", coupley : " << couple_y << endl;
    delta /= g_dim-a_dim+j_dim;
    cout << "delta : " << delta << endl;
    delta -= rho_air*S[0]/4.75*pow(etat.twist_vec(0),2)/4;
    cout << "delta : " << delta << ", rho_air : " << rho_air << ", S[1] : " << S[1] << ", V*V : " << pow(etat.twist_vec(0),2) << endl;
    delta /= 0.1*rho_air*S[1]*pow(etat.twist_vec(0),2);
    cout << "delta : " << delta << "\n" << endl;
    
    cout << "Iyy : " << Iyy << endl;
    cout << "err : " << err << endl;
    cout << "Acc_cons : " << Acc_cons << endl;
    cout << "couple_y : " << couple_y << endl;
    cout << "sum_err_cont1 : " << sum_err_cont1 << endl;
    cout << "delta : " << delta << endl;

    s->add_entry_c1(etat.pose_vec(4), etat.twist_vec(4), Acc_cons);

    if (delta > 0.28) delta = 0.28;
    if (delta < -0.28) delta = -0.28;

    return delta;
}

// set_pid function to set kp, ki and kd values, take an array of double as argument
void set_pid1(double* pid_values) {
    kp1 = pid_values[0];
    kd1 = pid_values[1];
    ki1 = pid_values[2];
}