#ifndef CONNECTION_H
#define CONNECTION_H
#include <iostream>
#include <asio.hpp>
#include <chrono>
#include <asio/steady_timer.hpp>


class Server;


class Connection : public std::enable_shared_from_this<Connection> {

public:
    typedef std::shared_ptr<Connection> connptr;

private:
    typedef std::chrono::high_resolution_clock::time_point timepoint;
    typedef std::chrono::high_resolution_clock highresclock;

    //noncopyable
    Connection(const Connection& other) = delete;
    Connection& operator=(const Connection&) = delete;

public:

    Connection(asio::io_service &ios, Server* server);


    static connptr new_connection(asio::io_service &ios, Server *server);

    void start() ;
    void stop();
    bool is_started() const;
    asio::ip::tcp::socket& get_sock();

    std::string get_name() const;
    void set_clients_changed();

private:

    void handle_read(const asio::error_code & error, size_t bytes);
    void handle_join(const std::string & msg);
    void handle_shalom();
    void handle_clients();
    void handle_check_shalom();
    void handle_write(const asio::error_code & error, size_t bytes);


    void do_check_shalom();
    void do_read();
    void do_write(const std::string & msg);

    size_t read_until_enter(const asio::error_code & error, size_t bytes);
    double elapsed_milliseconds(const timepoint& now, const timepoint& before);

private:
    asio::io_service& service;
    Server* svr;
    asio::ip::tcp::socket sock;
    enum { max_msg = 1024 };
    char rcv_buff[max_msg];
    char write_buff[max_msg];
    bool started;
    std::string name;
    asio::steady_timer timer;
    timepoint last_shalom;
    bool clients_changed;
};





#endif
