#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP

#include "dynamique.hpp"

extern double sum_err_cont1;
extern double kp1;
extern double kd1;
extern double ki1;

double controller_pitch(State etat, double cons, double dt, Forces forces, Data *s);
void set_pid1(double* pid_values);

#endif