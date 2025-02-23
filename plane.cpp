#include "plane.hpp"

#include <math.h>

//double S[] = {0.13, 0.13/4.75};
double S[] = {0.13, 0.027};
double b[] = {0.8, 0.27};

double Cd = 0.07;
double k = 1.1;
double m = 2;
double C_L = 0.7;
double C_D = 0.01;
double kpu = 1.1;
double Diameter = 0.2;

double alpha = 0;
double e = 0.75;
double AR[] = {pow(b[0], 2)/S[0], pow(b[1], 2)/S[1]};

double k_D[] = {1/3.143/e/AR[0], 1/3.143/e/AR[1]};

double C_L_0[] = {0.5, 0.5, -0.5, 0};
double C_D_0[] = {0.01, 0.01, 0.01, 0};
double C_L_alpha[] = {2*3.143, 2*3.143};
double C_L_delta[] = {0.2, 0.2};
double C_D_delta[] = {0.012, 0.012};

// Caractéristique moteur
double n_mot = 0.7;
double kv = 1000;
double kt = 0.01;
double C = 0.1;

double I = 10;

double h = 0.01;

double rho_air = 1.225;     // Masse volumique de l'air
double g = 9.81;            // Constante d'accélération gravitationnelle

// Dimesions avions
//double alpha = 0;    // Angle d'incidence
double a_dim = 1.5;        // Longueur de l'avion
double b_dim = 1.2;        // Largeur de l'avion
double c_dim = 0.05;        // Diamètre du corps
double d_dim = 0.1;        // Hauteur gouverne direction
double e_dim = 0.3;        // Largeur gourverne direction
double f_dim = 0.3;        // Distance CDG-centre de portance L1 et L2 selon y
double g_dim = 0.5;        // Distance avant-CDG selon x
double h_dim = 0.6;        // Distance avant-centre de portance L1 et L2 selon x
double i_dim = 0.03;        // Epaisseur de l'aile
double j_dim = 0.05;        // Distance arrière-Centre de gouverne portance selon x
double k_dim = 0.1;        // Longueur gouverne
double l_dim = 0.1;        // Distance contre_poids-avant

// Masses
double m0 = 0.2;       // Fuselage
double m1 = 0.2;       // Aile gauche
double m2 = 0.2;       // Aile droite
double m3 = 0.06;       // Gouverne direction
double m4 = 0.06;       // Gouverne profondeur
double m5 = 1.215;       // Contre-poids

// Moment d'inertie
double Ixx = 2*pow(f_dim, 2)*m1 + pow(c_dim, 2)*m4;
double Iyy = pow((g_dim - a_dim/2), 2)*m0 + 2*pow(g_dim-h_dim, 2)*m1 + pow(g_dim - a_dim + j_dim, 2)*(m3 + m4) + pow(c_dim, 2)*m4 + pow(g_dim - l_dim, 2)*m5;
double Izz = pow(g_dim - a_dim/2, 2)*m0 + 2*(pow(g_dim-h_dim, 2) + pow(f_dim, 2))*m1 + pow(g_dim-a_dim+j_dim, 2)*(m3 + m4) + pow(g_dim-l_dim, 2)*m5;