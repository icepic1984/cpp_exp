#include <thread>
#include <iostream>
#include <mutex>
#include <memory>
#include <queue>
#include <condition_variable>

namespace messaging {

struct message_base {
	virtual ~message_base()
	{}
};

template <typename Msg> struct wrapped_message : message_base {
	Msg contents;

	explicit wrapped_message(const Msg& contents_) : contents(contents_)
	{}
};

class queue {
public:
	std::shared_ptr<message_base> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mutex);
		cond.wait(lk, [&] { return !q.empty(); });
		auto res = q.front();
		q.pop();
		return res;
	}

	template <typename T> void push(const T& msg)
	{
		std::lock_guard<std::mutex> lk(mutex);
		q.push(std::make_shared<wrapped_message<T>>(msg));
		cond.notify_all();
	}

private:
	std::mutex mutex;
	std::condition_variable cond;
	std::queue<std::shared_ptr<message_base>> q;
};

struct close_queue
{};

template<typename Prev, typename Msg, typename Func>
class TemplateDispatcher {
public:
	TemplateDispatcher(TemplateDispatcher&& other)
	    : q(other.q), prev(other.prev), f(std::move(other.f)),
	      chained(other.chained)
	{
		other.chained = true;
	}

	TemplateDispatcher(queue* q_, Prev* prev_, Func&& f_)
	    : q(q_), prev(prev_), f(std::forward<Func>(f_)), chained(false)
	{
		prev_->chained = true;
	}

	template <typename OtherMsg, typename OtherFunc>
	TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>
	handle(OtherFunc&& of)
	{
		return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>(
		    q, this, std::forward<OtherFunc>(of));
	}

	~TemplateDispatcher() noexcept(false)
	{
		if (!chained) {
			wait_and_dispatch();
		}
	}

private:
	TemplateDispatcher(const TemplateDispatcher&) = delete;

    TemplateDispatcher& operator=(const TemplateDispatcher&) = delete;

    template<typename Dispatcher, typename OtherMsg, typename OtherFunc>
    friend class TemplateDispatcher;

	void wait_and_dispatch()
	{
		for (;;) {
			auto msg = q->wait_and_pop();
			if (dispatch(msg))
				break;
		}
	}

	bool dispatch(const std::shared_ptr<message_base>& msg)
	{
		if (wrapped_message<Msg>* wrapper =
		        dynamic_cast<wrapped_message<Msg>*>(msg.get())) {
			f(wrapper->contents);
			return true;
		}
		else {
			return prev->dispatch(msg);
		}
	}

	queue* q;
    Prev* prev;
    Func f;
    bool chained;
};

class dispatcher {
public:
	dispatcher(dispatcher&& other) : q(other.q), chained(other.chained)
	{
		other.chained = true;
	}

	explicit dispatcher(queue* q_) : q(q_), chained(false)
	{}

	template <typename Message, typename Func>
	TemplateDispatcher<dispatcher, Message, Func> handle(Func&& f)
	{
		return TemplateDispatcher<dispatcher, Message, Func>(
		    q, this, std::forward<Func>(f));
	}

	~dispatcher() noexcept(false)
	{
		if (!chained) {
			wait_and_dispatch();
		}
	}

private:
	dispatcher(const dispatcher&) = delete;

	dispatcher& operator=(const dispatcher&) = delete;

	template <typename Dispatcher, typename Msg, typename Func>
	friend class TemplateDispatcher;

	void wait_and_dispatch()
	{
		for (;;) {
			auto msg = q->wait_and_pop();
			dispatch(msg);
		}
	}

	bool dispatch(const std::shared_ptr<message_base>& msg)
	{
		if (dynamic_cast<wrapped_message<close_queue>*>(msg.get())) {
			throw close_queue();
		}
		return false;
	}

	queue* q;
    bool chained;
};

class sender
{
public:
    sender() : q(nullptr)
	{}

	explicit sender(queue* q_) : q(q_)
	{}

	template <typename Message> void send(const Message& msg)
	{
		if (q) {
			q->push(msg);
		}
	}

private:
	queue *q;
};

class receiver
{
public:
	operator sender()
	{
		return sender(&q);
	}
	dispatcher wait()
	{
		return dispatcher(&q);
	}

private:
	queue q;
    
};

} // end namespace

struct bla{};

struct blup{};

class foo {
public:
	void run() try {
		for (;;) {
			incoming.wait()
			    .handle<bla>(
			        [&](const bla& msg) { std::cout << "bla" << std::endl; })
			    .handle<blup>(
                    [&](const blup& msg) { std::cout << "blup" << std::endl;});
		}
	}
	catch (messaging::close_queue&) {
	}

	messaging::sender get_sender()
	{
		return incoming;
    }
private:
    messaging::receiver incoming;
    
};

int main(int argc, char* argv[])
{
    foo f;

    std::thread thread(&foo::run,&f);
    f.get_sender().send(bla());
    f.get_sender().send(blup());
    f.get_sender().send(messaging::close_queue());
    thread.join();

	return 0;
}
