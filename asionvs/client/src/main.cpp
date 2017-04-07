#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <functional>

#include "client.h"
#include "asio.hpp"

int main() {

    std::srand(std::time(NULL));
    std::cout << "Shalom-Client \n" << std::endl;

    asio::io_service service;

    asio::ip::tcp::endpoint ep ( asio::ip::address::from_string("127.0.0.1"), 4711); // localhost, port 4711

    // connect several clients to server
    std::vector<std::string> names{ "Songoku", "Vegeta", "Rei", "Asuka", "Mari", "Pikachu"};

    std::vector<Client::clientptr> clients;

    for (std::string clientname : names) {
        Client::clientptr client = Client::clientptr(new Client(service, clientname.c_str()));
        client->do_connect(ep);
        clients.push_back(client);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //Client::start(service, ep, clientname.c_str());
    }

    service.run();
    std::cout << "Server END" << std::endl;



}
