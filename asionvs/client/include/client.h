#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <functional>


#include <asio.hpp>
#include <asio/steady_timer.hpp>



class Client : public std::enable_shared_from_this<Client> {

public:
    typedef asio::error_code  error_code;
    typedef std::shared_ptr<Client> clientptr;


    Client(asio::io_service& service, const std::string& name);

    //noncopyable
    Client(const Client& other) = delete;
    Client& operator=(const Client&) = delete;




public:


    void do_connect(asio::ip::tcp::endpoint ep);
    void stop();
    bool is_started();

private:


    void handle_connect(const asio::error_code& error) ;
    void handle_read(const asio::error_code& error, size_t bytes);
    void handle_join();
    void handle_shalom(const std::string& msg);
    void handle_clients(const std::string& msg);


    void do_shalom();
    void defer_shalom();
    void do_ask_for_clients();
    void handle_write(const asio::error_code& error, size_t bytes);
    void do_read();
    void do_write(const std::string& msg);


private:

    asio::ip::tcp::socket sock;
    enum { max_msg = 1024 };
    char rcv_buff[max_msg];
    char write_buff[max_msg];
    bool started;
    std::string name;
    asio::steady_timer timer;
};















