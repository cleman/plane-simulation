#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP

#include "dynamique.hpp"

// Pitch controller
extern double sum_err_cont1;
extern double kp1;
extern double kd1;
extern double ki1;

// SpeedX controller
extern double sum_err_cont2;
extern double kp2;
extern double kd2;
extern double ki2;

double controller_pitch(double cons, double dt, State etat, Forces forces, Data *s);
double controller_rpm(double cons, double dt, State etat, Forces forces, Dynamique dyn, Data *s);
void set_pid1(double* pid_values);
void set_pid2(double* pid_values);

#endif