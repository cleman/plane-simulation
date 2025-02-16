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
        lancer_simulation(etat_init, cmd_act, temps);

        // Répondre que la simulation est terminée
        crow::json::wvalue response;
        response["status"] = "Simulation terminée!";
        return crow::response(response);
    });

    // Autres routes ou configurations supplémentaires ici...
}

void Server::startServer() {
    crow::SimpleApp app;
    setupRoutes(app);
    app.port(5000).multithreaded().run();
}
