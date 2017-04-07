#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <asio.hpp>
#include <chrono>
#include <vector>
#include <asio/steady_timer.hpp>


class Connection;


class Server : public std::enable_shared_from_this<Server> {

public:
    typedef std::vector<std::shared_ptr<Connection>> connvec;

public:
    Server(int port);

    void run();
    void update_clients_changed();
    void handle_accept(std::shared_ptr<Connection> conn,
                        const asio::error_code &error);

public:
    connvec clients;
    bool clients_changed;

private:
    asio::io_service service;
    asio::ip::tcp::endpoint ep;
    std::shared_ptr<asio::ip::tcp::acceptor> acceptor;
};


#endif
