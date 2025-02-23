#ifndef PLANE_HPP
#define PLANE_HPP

extern double S[];
extern double b[2];

extern double Cd;
extern double k;
extern double m;
extern double C_L;
extern double C_D;
extern double kpu;
extern double Diameter;

extern double alpha;
extern double e;
extern double AR[2];

extern double k_D[2];

extern double C_L_0[4];
extern double C_D_0[4];
extern double C_L_alpha[2];
extern double C_L_delta[2];
extern double C_D_delta[2];

extern double n_mot;
extern double kv;
extern double kt;
extern double C;

extern double I;

extern double h;

extern double rho_air;
extern double g;

// Dimensions avions
//extern double alpha;    // Angle d'incidence
extern double a_dim;        // Longueur de l'avion
extern double b_dim;        // Largeur de l'avion
extern double c_dim;        // Diamètre du corps
extern double d_dim;        // Hauteur gouverne direction
extern double e_dim;        // Largeur gourverne direction
extern double f_dim;        // Distance CDG-centre de portance L1 et L2 selon y
extern double g_dim;        // Distance avant-CDG selon x
extern double h_dim;        // Distance avant-centre de portance L1 et L2 selon x
extern double i_dim;        // Epaisseur de l'aile
extern double j_dim;        // Distance arrière-Centre de gouverne portance selon x
extern double k_dim;        // Longueur gouverne
extern double l_dim;        // Distance contre_poids-avant

// MAsses
extern double m0;       // Fuselage
extern double m1;       // Aile gauche
extern double m2;       // Aile droite
extern double m3;       // Gouverne direction
extern double m4;       // Gouverne profondeur
extern double m5;       // Contre-poids

// Composantes de la matrices d'inertie
extern double Ixx;      // Moment d'inertie autour de l'axe x
extern double Iyy;      // Moment d'inertie autour de l'axe y
extern double Izz;      // Moment d'inertie autour de l'axe z

#endif