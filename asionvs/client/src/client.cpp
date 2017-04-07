#include "client.h"

using namespace asio;
using namespace std::placeholders;



Client::Client(io_service &service, const std::string &name) :
    sock(service),
    started(true),
    name(name),
    timer(service)
{ }

void Client::do_connect(ip::tcp::endpoint ep)
{
    sock.async_connect(ep, std::bind(&Client::handle_connect, shared_from_this(),_1));
}

void Client::stop()
{
    if ( !is_started()) return;
    std::cout << std::endl <<  "stopping " << name << ". " << name
              << " went home \n" << std::endl;
    started = false;
    sock.close();
}

bool Client::is_started() { return started; }


void Client::handle_connect(const error_code &error)
{
    if ( !error) {
        do_write("join " + name + "\n");
    } else{
        std::cout << name << " could not connect" << std::endl;
        stop();
    }
}


void Client::handle_read(const error_code &error, size_t bytes)
{
    if (error)
        stop();
    if (!is_started())
        stop();//return;
    // process the msg

    std::string msg(rcv_buff, bytes);

    if ( msg.find("join ") != std::string::npos)
        handle_join();
    else if ( msg.find("shalom") != std::string::npos) {
        //std::cout <<"shalom | Mesage| " << msg <<"|" << std::endl;
        handle_shalom(msg);
    }
    else if ( msg.find("clients") != std::string::npos) {
        //std::cout <<"CLIENTS | Mesage| " << msg <<"|" << std::endl;
        handle_clients(msg);
    }
    else{
        //std::cout << "ERROR:"<<name<<"  invalid msg: |" << msg << "| "
        //<< std::endl;
    }
}

void Client::handle_join()
{
    std::cout << name << " joined " << std::endl;
    do_ask_for_clients();
}

void Client::handle_shalom(const std::string &msg)
{
    //std::cout <<"HANDLE shalom" << std::endl;
    std::istringstream in(msg);
    std::string answer;
    //first token shalom
    in >> answer;
    //second token is either client_constellation_changed or ok
    in >> answer;
    //std::cout << "\n ANSWER: " << answer << std::endl;
    if ( answer == "client_constellation_changed")
        do_ask_for_clients();
    else if ( answer == "ok")
        defer_shalom();
    else {
        std::cerr << "ERROR: INVALID MESSAGE" << std::endl;
        stop();
    }
}

void Client::handle_clients(const std::string &msg)
{
    std::string clients = msg.substr(8);
    std::cout << std::endl << name << ", new client list: " << clients
              << std::endl;
    defer_shalom();
}

void Client::do_shalom()
{
    do_write("shalom\n");
}

void Client::defer_shalom()
{
    //wait a random time, so the server is gonna disconnect the client
    int millis = rand() % 700;
    std::cout << name << " will say shalom in  " << millis
              << " millis" << std::endl;
    timer.expires_from_now(std::chrono::milliseconds(millis));
    timer.async_wait( std::bind(&Client::do_shalom, shared_from_this()));

}

void Client::do_ask_for_clients()
{
    do_write("ask_for_clients\n");
}

void Client::handle_write(const error_code &error, size_t bytes)
{
    auto self = shared_from_this();
    // here i wanted to try out the completion function of async_read
    // and practice lambdas
    // as async_read_until also returns data that comes after
    //the specified delimitter
    async_read( sock,
                buffer(rcv_buff),
                [this, self](const asio::error_code& error, size_t bytes) ->
                size_t
    {
        if ( error)
            return 0;
        // proceeds with reading while \n is not met. If it's met, the return
        // value  is nonequal to 0 and async_read will be completet stop reading
        bool found = (std::find(rcv_buff, rcv_buff + bytes, '\n')
                      < rcv_buff + bytes);
        return !found;
    },
    std::bind(&Client::handle_read, shared_from_this(),_1,_2));
}

void Client::do_read()
{
    auto self = shared_from_this();
    // here i wanted to try out the completion function of async_read
    // and practice lambdas
    // as async_read_until also returns data that comes after
    //the specified delimitter
    async_read( sock,
                buffer(rcv_buff),
                [this, self](const asio::error_code& error, size_t bytes) ->
                size_t
    {
        if ( error)
            return 0;
        // proceeds with reading while \n is not met. If it's met, the return
        // value is nonequal to 0 and async_read will be completet stop reading
        bool found = (std::find(rcv_buff, rcv_buff + bytes, '\n') <
                      rcv_buff + bytes);
        return !found;
    },
    std::bind(&Client::handle_read, shared_from_this(),_1,_2));
}

void Client::do_write(const std::string &msg)
{
    if (!is_started()) {
        return;
    }
    //copy message to buffer, overwrite buffer while doing so
    std::copy(msg.begin(), msg.end(), write_buff);
    sock.async_write_some( buffer(write_buff, msg.size()),
                           std::bind(&Client::handle_write,
                                     shared_from_this(),_1,_2));

}
