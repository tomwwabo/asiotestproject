#include "server.h"
#include "connection.h"

using namespace asio;
using namespace std::placeholders;

Server::Server(int port) :
    ep(ip::tcp::endpoint(ip::tcp::v4(), port))
{
    try {
    acceptor = std::shared_ptr<ip::tcp::acceptor>(
                new ip::tcp::acceptor(service,ep));
    } catch (std::exception& e) {
        std::cerr << "An Error occured!" << std::endl;
        std::cerr << "Exception " << e.what() << std::endl;
        std::exit(1);
    }
}


void Server::run()
{
    std::cout << "Shalom-Server" << std::endl;

    std::shared_ptr<Connection> connection =
            Connection::new_connection(service,this);
    std::cout << "Waiting..." << std::endl;
    acceptor->async_accept(connection->get_sock(),
                           std::bind(&Server::handle_accept,
                                     shared_from_this(),connection,_1));

    service.run();
}


void Server::update_clients_changed()
{
    for(connvec::iterator b = clients.begin(),
        e = clients.end(); b != e; ++b)
        (*b)->set_clients_changed();
}


void Server::handle_accept(std::shared_ptr<Connection> conn,
                           const error_code &error)
{

    conn->start();


    std::shared_ptr<Connection> newconn =
            Connection::new_connection(service,this);
    acceptor->async_accept(newconn->get_sock(),
                           std::bind(&Server::handle_accept,
                                     shared_from_this(),
                                     newconn, _1));
}
