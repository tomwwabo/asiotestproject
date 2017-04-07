#include "connection.h"
#include "server.h"

using namespace asio;
using namespace std::placeholders;



Connection::Connection(asio::io_service &ios, Server *server) :
    service(ios),
    sock(service),
    svr(server),
    started(false),
    timer(service)
{

}

Connection::connptr Connection::new_connection(asio::io_service &ios,
                                               Server* server)
{
    connptr conn(new Connection(ios, server));
    return conn;
}

void Connection::start()
{
    started = true;
    svr->clients.push_back(shared_from_this());
    last_shalom = highresclock::now();
    // wait for the client to join
    do_read();
}

void Connection::stop() {
    if(!started) return;
    started = false;
    sock.close();

    std::cout <<  name << " stopped. " << name
               << " went home." << std::endl;
    connptr self = shared_from_this();
    Server::connvec::iterator it = std::find(svr->clients.begin(),
                                             svr->clients.end(), self);
    svr->clients.erase(it);
    svr->update_clients_changed();
}

bool Connection::is_started() const {
    return started;
}

asio::ip::tcp::socket &Connection::get_sock()
{
    return sock;
}

std::string Connection::get_name() const
{
    return name;
}

void Connection::set_clients_changed()
{
    clients_changed = true;
}

void Connection::handle_read(const asio::error_code &error, size_t bytes)
{
    if(error) stop();
    if(!is_started()) return;
    // process the msg
    std::string msg(rcv_buff, bytes);
    if(msg.find("join ") != std::string::npos) handle_join(msg);
    else if(msg.find("shalom") != std::string::npos) handle_shalom();
    else if(msg.find("ask_for_clients") != std::string::npos) handle_clients();
    else std::cerr << "ERROR: INVALID MESSAGE" << std::endl;
}

void Connection::handle_join(const std::string &msg)
{
    std::istringstream in(msg);
    in >> name;
    in >> name;
    std::cout << name << " joined" << std::endl;
    do_write("join ok\n");
    svr->update_clients_changed();
}

void Connection::handle_shalom()
{
    do_write(clients_changed ? "shalom client_constellation_changed\n" :
                               "shalom ok\n");
    clients_changed = false;
}

void Connection::handle_clients()
{
    std::string msg;
    for( Server::connvec::const_iterator b = svr->clients.begin(),
         e = svr->clients.end() ;
         b != e; ++b) {
        msg += (*b)->get_name() + " ";
    }
    do_write("clients " + msg + "\n");
}

void Connection::handle_check_shalom()
{
    timepoint now = highresclock::now();
    double elapsed_time = (int)elapsed_milliseconds(now, last_shalom);
    if(elapsed_time > 500) {
        stop();
    }
    last_shalom = highresclock::now();
}

void Connection::handle_write(const asio::error_code &error, size_t bytes)
{
    do_read();
}



void Connection::do_check_shalom()
{
    timer.expires_from_now(std::chrono::milliseconds(500));
    timer.async_wait( std::bind(&Connection::handle_check_shalom,
                                shared_from_this()));
}

void Connection::do_read()
{
    async_read(sock, buffer(rcv_buff),
               std::bind(&Connection::read_until_enter,
                         shared_from_this(),_1,_2),
               std::bind(&Connection::handle_read,
                         shared_from_this(),_1,_2));
    do_check_shalom();
}

void Connection::do_write(const std::string &msg)
{
    if(!is_started() ) return;
    std::copy(msg.begin(), msg.end(), write_buff);
    sock.async_write_some( buffer(write_buff, msg.size()),
                           std::bind(&Connection::handle_write,
                                     shared_from_this(),_1,_2));
}

size_t Connection::read_until_enter(const asio::error_code &error,
                                    size_t bytes)
{
    if(error) return 0;
    bool found = std::find(rcv_buff, rcv_buff + bytes, '\n') < rcv_buff + bytes;
    return !found;
}

double Connection::elapsed_milliseconds(const Connection::timepoint &now,
                                        const Connection::timepoint &before)
{
    std::chrono::duration<double, std::milli> elapsed = now - before;
    double elapsed_milliseconds = elapsed.count();
    return elapsed_milliseconds;
}
