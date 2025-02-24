#include "controlleurs.hpp"
#include "simulation.hpp"

// Pitch controller
double sum_err_cont1 = 0;
double kp1 = 10;
double kd1 = 2;
double ki1 = 10;

double controller_pitch(double cons, double dt, State etat, Forces forces, Data *s) {
    double err = cons - etat.pose_vec(4);
    sum_err_cont1 += err*dt;
    if (sum_err_cont1 > 0.5) sum_err_cont1 = 0.5;
    double Acc_cons = kp1*err - kd1*etat.twist_vec(4) + ki1*sum_err_cont1;
    double couple_y = Iyy*Acc_cons;

    double delta = (forces.drag[0] + forces.drag[1])*sin(alpha) - (forces.lift[0]+forces.lift[1])*cos(alpha);
    delta = -delta*(g_dim-h_dim)*0 - couple_y;
    delta /= g_dim-a_dim+j_dim;
    delta /= 0.1*rho_air*S[0]/4.75*pow(etat.twist_vec(0),2);
    
    /*
        cout << "Iyy : " << Iyy << endl;
        cout << "err : " << err << endl;
        cout << "Acc_cons : " << Acc_cons << endl;
        cout << "couple_y : " << couple_y << endl;
        cout << "sum_err_cont1 : " << sum_err_cont1 << endl;
    */
    
    //cout << "delta : " << delta << endl;
    
    if (delta > 0.28) delta = 0.28;
    if (delta < -0.28) delta = -0.28;
    
    s->add_entry_c1(delta, etat.pose_vec(4), etat.twist_vec(4), Acc_cons);

    return delta;
}

// set_pid function to set kp, ki and kd values, take an array of double as argument
void set_pid1(double* pid_values) {
    kp1 = pid_values[0];
    ki1 = pid_values[2];
    kd1 = pid_values[1];
    sum_err_cont1 = 0;
}

// Pitch controller
double sum_err_cont2 = 0;
double kp2 = 1;
double kd2 = 0;
double ki2 = 0;

double controller_rpm(double cons, double dt, State etat, Forces forces, Dynamique dyn, Data *s) {
    double theta = etat.pose_vec(4);

    double err = cons - etat.twist_vec(0);
    sum_err_cont2 += err*dt;
    double Acc_cons = kp2*err - kd2*dyn.acc_vec(0) + ki2*sum_err_cont2;
    double T = m*Acc_cons + (forces.lift[0] + forces.lift[1])*sin(alpha) + (forces.drag[0] + forces.drag[1])*cos(alpha) + forces.drag[2] + forces.drag[3] + forces.weight*sin(theta);

    double rpm = T + 0.213*rho_air*pow(etat.twist_vec(0),2)*pow(Diameter,2);
    rpm /= 0.12*rho_air*pow(Diameter, 4);
    rpm = pow(rpm, 0.5);

    cout << "rpm : " << rpm << endl;
    cout << "Acc_cons : " << Acc_cons << endl;
    cout << "T : " << T << endl;
    cout << "err : " << err << endl;
    cout << "sum_err_cont2 : " << sum_err_cont2 << endl;
    cout << "kp2 : " << kp2 << endl;
    cout << "kd2 : " << kd2 << endl;
    cout << "ki2 : " << ki2 << endl;
    cout << "kp2*err : " << kp2*err << endl;
    cout << "kd2*etat.acc_vec(0) : " << kd2*dyn.acc_vec(0) << endl;
    cout << "ki2*sum_err_cont2 : " << ki2*sum_err_cont2 << endl;

    if (rpm > 300) rpm = 300;

    return rpm;     // rad/s not rpm
}

void set_pid2(double* pid_values) {
    kp2 = pid_values[0];
    ki2 = pid_values[2];
    kd2 = pid_values[1];
    sum_err_cont2 = 0;
}