#include <vector>
#include <thread>
#include <memory>
#include <chrono>
#include<iostream>
#include <iomanip>
#include <zmq.hpp>

int add(int a, int b) {
    return a + b;
}

class server_worker {
public:
    server_worker(zmq::context_t& ctx, int sock_type)
        : ctx_(ctx), worker_(ctx_, sock_type) {
    }

    void work() {
            worker_.connect("inproc://backend");
        try {
            while (true) {
                zmq::message_t identity;
                zmq::message_t msg;
                zmq::message_t copied_id;
                zmq::message_t copied_msg;
                worker_.recv(&identity);
                worker_.recv(&msg);
                std::string identity_str(static_cast<char*>(identity.data()), identity.size());
                std::string msg_str(static_cast<char*>(msg.data()), msg.size());
                std::cout << "Message:" << msg_str << std::endl;
                std::cout << "Identity: " << identity_str << std::endl;
                
                for (int reply = 0; reply < 5; ++reply) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    copied_id.copy(&identity);
                    copied_msg.copy(&msg);
                    worker_.send(copied_id, ZMQ_SNDMORE);
                    worker_.send(copied_msg);
                }
            }
        }
        catch (std::exception &e) {}
    }

private:
    zmq::context_t &ctx_;
    zmq::socket_t worker_;
};

class server_task {
public:
    server_task()
        : ctx_(1),
          frontend_(ctx_, ZMQ_ROUTER),
          backend_(ctx_, ZMQ_DEALER)
    {}

    void run() {
        frontend_.bind("tcp://*:5570");
        backend_.bind("inproc://backend");

        std::vector<std::thread> worker_thread;
        for (int i = 0; i < 5; ++i) {

            worker_thread.emplace_back(std::thread(&server_worker::work,server_worker{ctx_, ZMQ_DEALER}));
                    
        }

        try {
            zmq::proxy(frontend_, backend_, nullptr);
        }
        catch (std::exception &e) {}

    }

private:
    zmq::context_t ctx_;
    zmq::socket_t frontend_;
    zmq::socket_t backend_;
};

//  The main thread simply starts several clients and a server, and then
//  waits for the server to finish.

int main (void)
{
    server_task st;

    std::thread t4(&server_task::run, &st);
    t4.join();

    return 0;
}
