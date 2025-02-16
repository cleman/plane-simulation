#ifndef SERVER_H
#define SERVER_H

#include "crow.h"

class Server {
public:
    void setupRoutes(crow::SimpleApp& app);
    void startServer();
};

#endif // SERVER_H
