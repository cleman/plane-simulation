#include "server.h"
#include "simulation.hpp"  // Inclure le fichier pour la simulation
#include <iostream>
#include <fstream>
#include <sstream>
#include <crow.h>  // Assurez-vous d'inclure la bibliothèque Crow pour les routes HTTP

using namespace std;

void Server::setupRoutes(crow::SimpleApp& app) {

    // Route pour servir l'interface HTML
    CROW_ROUTE(app, "/")([&]() {
        std::ifstream file("index.html");
        std::stringstream buffer;
        buffer << file.rdbuf();
        return crow::response(buffer.str());
    });

    // Route POST pour la simulation
    CROW_ROUTE(app, "/start_simulation").methods(crow::HTTPMethod::Post)([](const crow::request& req) {
        auto data = crow::json::load(req.body);
        if (!data) return crow::response(400, "Invalid JSON");

        // Extraire les données de l'input (6 champs pour l'état et 5 pour les commandes)
        double etat[6] = {
            data["etat1"].d(),
            data["etat2"].d(),
            data["etat3"].d(),
            data["etat4"].d(),
            data["etat5"].d(),
            data["etat6"].d()
        };

        double cmd[5] = {
            data["cmd1"].d(),
            data["cmd2"].d(),
            data["cmd3"].d(),
            data["cmd4"].d(),
            data["cmd5"].d()
        };

        double temps = data["temps"].d();  // Temps de simulation

        // Initialiser l'état et la commande
        State etat_init;
        etat_init.set(etat, cmd);  // Utiliser une méthode pour configurer l'état à partir de l'input

        Actionneur cmd_act;
        cmd_act.set(cmd);  // Méthode pour configurer l'actionneur à partir de l'input

        // Appeler la fonction de simulation
        Data data_simu = lancer_simulation(etat_init, cmd_act, temps);

        // Exemple : récupérer les résultats sous forme de vecteurs
        std::vector<double> time_series = data_simu.time;        // Fonction fictive pour récupérer le temps
        std::vector<double> speedX_series = data_simu.speedX;      // Exemple : vitesse en fonction du temps
        std::vector<double> speedY_series = data_simu.speedY;      // Exemple : vitesse en fonction du temps
        std::vector<double> speedZ_series = data_simu.speedZ;      // Exemple : vitesse en fonction du temps

        std::vector<double> accX_series = data_simu.accX;      // Exemple : vitesse en fonction du temps
        std::vector<double> accY_series = data_simu.accY;      // Exemple : vitesse en fonction du temps
        std::vector<double> accZ_series = data_simu.accZ;      // Exemple : vitesse en fonction du temps

        std::vector<double> thrust_series = data_simu.thrust;      // Exemple : vitesse en fonction du temps
        std::vector<double> weight_series = data_simu.weight;      // Exemple : vitesse en fonction du temps

        std::vector<double> lift1_series, lift2_series, lift3_series, lift4_series;
        std::vector<double> drag1_series, drag2_series, drag3_series, drag4_series;

        for (const auto& l : data_simu.lift) {
            lift1_series.push_back(l[0]);
            lift2_series.push_back(l[1]);
            lift3_series.push_back(l[2]);
            lift4_series.push_back(l[3]);
        }

        for (const auto& d : data_simu.drag) {
            drag1_series.push_back(d[0]);
            drag2_series.push_back(d[1]);
            drag3_series.push_back(d[2]);
            drag4_series.push_back(d[3]);
        }

        // Construire la réponse JSON
        crow::json::wvalue response;
        response["status"] = "Simulation terminée!";

        response["time"] = crow::json::wvalue::list(time_series.begin(), time_series.end());
        
        response["speedX"] = crow::json::wvalue::list(speedX_series.begin(), speedX_series.end());
        response["speedY"] = crow::json::wvalue::list(speedY_series.begin(), speedY_series.end());
        response["speedZ"] = crow::json::wvalue::list(speedZ_series.begin(), speedZ_series.end());

        response["accX"] = crow::json::wvalue::list(accX_series.begin(), accX_series.end());
        response["accY"] = crow::json::wvalue::list(accY_series.begin(), accY_series.end());
        response["accZ"] = crow::json::wvalue::list(accZ_series.begin(), accZ_series.end());

        response["thrust"] = crow::json::wvalue::list(thrust_series.begin(), thrust_series.end());
        response["weight"] = crow::json::wvalue::list(weight_series.begin(), weight_series.end());

        response["lift1"] = crow::json::wvalue::list(lift1_series.begin(), lift1_series.end());
        response["lift2"] = crow::json::wvalue::list(lift2_series.begin(), lift2_series.end());
        response["lift3"] = crow::json::wvalue::list(lift3_series.begin(), lift3_series.end());
        response["lift4"] = crow::json::wvalue::list(lift4_series.begin(), lift4_series.end());

        response["drag1"] = crow::json::wvalue::list(drag1_series.begin(), drag1_series.end());
        response["drag2"] = crow::json::wvalue::list(drag2_series.begin(), drag2_series.end());
        response["drag3"] = crow::json::wvalue::list(drag3_series.begin(), drag3_series.end());
        response["drag4"] = crow::json::wvalue::list(drag4_series.begin(), drag4_series.end());

        return crow::response(response);
    });

    // Autres routes ou configurations supplémentaires ici...
}

void Server::startServer() {
    crow::SimpleApp app;
    setupRoutes(app);
    app.port(5000).multithreaded().run();
}
