#include "Server.hpp"
#include <iostream>

int main() {
    Server server(8080);

    server.init();
    server.start();
    server.stop();
    return 0;
}