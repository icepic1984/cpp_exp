#include <iostream>
#include <exception>
#include <functional>
#include <stdexcept>

template <typename T>
struct ScannerFactory 
{
   static T* init(){
	   return new T();
   }
   typedef T ScannerType_t;
};

template <typename T> 
struct Observer 
{
   void registerObserver(T* ob) {
	   m_ob = ob;
   }

   T* m_ob;
};

template <typename T>
struct ScanController
{
   ScanController(T* sc) : m_sc(sc) {}

   T* m_sc;
};


struct TestInterface : Observer<ScanController<TestInterface>> 
{};

struct ScannerInterface : Observer<ScanController<ScannerInterface>>
{};

      
typedef ScannerFactory<ScannerInterface> ScannerInterface_t;


template <typename T>
struct Monad
{
   T value;
};

using func_t = std::function<void()> ;
struct Chain
{
   Chain(func_t v) : value(v),
                     exception(nullptr){}
   Chain(func_t v, std::exception_ptr exp) :
	   value(v), exception(exp){}

   func_t value;
   std::exception_ptr exception;
};

template <typename T>
Chain unit(T t)
{return Chain{t};}

template <typename A>
Chain bind(Chain m, Chain(*func)(A))
 {
	 if(m.exception){
		 auto tmp = unit([]() {});
		 tmp.exception = m.exception;
		 return tmp;
	 } else {
		 try {
			 return func(m.value);
		 } catch(...) {
			 auto tmp = unit([]() {});
			 tmp.exception = std::current_exception();
			 return tmp;
		 }
	 }
 }

   
int main(int argc, char *argv[])
{
	ScannerInterface_t::ScannerType_t* test = ScannerInterface_t::init();
	ScanController<ScannerInterface_t::ScannerType_t> sc(test);
	test->registerObserver(&sc);

	auto t = unit([](){ std::cout<<"hall"<<std::endl;});
	auto t2 = bind(t,unit<func_t>);
   
	
	auto bla = bind(t,+[](func_t t){
			return Chain([=]{t();
					throw std::invalid_argument("fuc");
					std::cout<<"yeah"<<std::endl;});})
;
	
		
	
 
	t.value();
	t2.value();
	
	// if(bla.exception){
	// 	std::rethrow_exception(bla.exception);
	// }
	bla.value();
	
	
	return 0;
}

