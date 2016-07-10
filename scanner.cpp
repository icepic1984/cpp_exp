 #include <thread>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class Tqueue 
{
public:

   Tqueue() : m_next_ticket(0),
              m_counter(0) {}

   void push(const T& e){
       std::unique_lock<std::mutex> lock(m_mutex);
       m_queue.push(e);
       lock.unlock();
       m_cond.notify_all();
    };

   T wait_and_pop() {
       std::unique_lock<std::mutex> lock(m_mutex);
       int ticket = m_next_ticket++;
       m_cond.wait(lock,[=]{return (!m_queue.empty())
                  && (ticket == m_counter);});
       m_counter++;
       T data = m_queue.front();
       m_queue.pop();
       m_cond.notify_all();
       lock.unlock();
       return data;
   }

private:
   int m_next_ticket;
   int m_counter;
   std::queue<T> m_queue;
   std::mutex m_mutex;
   std::condition_variable m_cond;   
};

Tqueue<int> queue;

std::mutex mutex;

void test(int i) 
{
    auto bla = queue.wait_and_pop();
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Thread : "<<i << std::endl;
}

const int SIZE = 200;


class lispstring : public std::string
{
   using std::string::string;
   
   
};

int main(int argc, char *argv[])
{
	lispstring bla("bla");
	bla.length();
	
	
	
    std::vector<std::thread> threads;
    for(int i = 0; i < SIZE; ++i){
       threads.push_back(std::thread(test,i));
       std::this_thread::sleep_for(std::chrono::milliseconds(100));
       std::cout << "yeah" << std::endl;
    }


    for(int i = 0; i < SIZE; ++i){
	    std::this_thread::sleep_for(std::chrono::milliseconds(100));
	    queue.push(i);
    }
    for(int i = 0; i < SIZE; ++i)
       threads[i].join();
    return 0;
}

 

