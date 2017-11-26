#include <vector>
#include <thread>
#include <memory>
#include <iostream>
#include <iomanip>
#include <zmq.hpp>
#include <sstream>

class client_task {
public:
    client_task()
        : ctx_(1),
          client_socket_(ctx_, ZMQ_DEALER)
    {}

    void start() {
        std::stringstream identity;
        identity << id++;
        
        client_socket_.setsockopt(ZMQ_IDENTITY, identity.str().c_str(), identity.str().size());
        client_socket_.connect("tcp://localhost:5570");

        zmq::pollitem_t items[] = {client_socket_, 0, ZMQ_POLLIN, 0};

        try {
            while (true) {
                std::cout << "round " << std::endl;
                for (int i = 0; i < 100; ++i) {
                    // 10 milliseconds
                    zmq::poll(items, 1, 10);
                    if (items[0].revents & ZMQ_POLLIN) {
                    }
                }
                const std::string request = "request";
                client_socket_.send(request.c_str(), request.size());
            }
        } catch (std::exception& e) {
        }
    }

private:
    static int id;
    zmq::context_t ctx_;
    zmq::socket_t client_socket_;
};

int client_task::id = 0;

int main()
{
    client_task ct1;
    client_task ct2;
    client_task ct3;
    std::thread t1(&client_task::start,&ct1);
    std::thread t2(&client_task::start,&ct2);
    t1.join();
}
