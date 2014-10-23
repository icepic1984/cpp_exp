#include <type_traits>
#include <iostream>
#include <utility>
#include <typeinfo>

class Widget 
{
public:
   Widget():a(100){}
	   
   int get() const {
	   return a;
   }
private:
   int a;
};

class Gadget 
{
public:
   Gadget():b(50){}
   
   int get() const{
	   return b;
   }
private:
   int b;
};

class Failget
{
public:
   Failget():c(100){}
private:
   int c;
};

template<typename T>
typename std::enable_if<std::is_fundamental<T>::value, void>::type
print(const T& t)
{std::cout << "Print: "<<t << std::endl;}


template<typename... T>
struct make_void
{
    typedef void type;
};

template<typename... T>
using void_t = typename make_void<T...>::type;

template<typename T, typename = void>
struct has_get : std::false_type{};

template<typename T>
struct has_get<T,void_t<decltype(std::declval<T&>().get())>>:std::true_type{};

template<typename T>
typename std::enable_if<has_get<T>::value,void>::type 
print(const T& t)
{std::cout << t.get() << std::endl;}

void test(int (&t)[3])
{std::cout << "sdsd" << std::endl;}

template<int N> 
void meow(const char (&array)[N])
{std::cout << "Size: "<<N << std::endl;}


int main(int argc, char *argv[])
{
	Widget w;
	Gadget g;
	Failget f;
	void_t<decltype(std::declval<Gadget>().get())> *a;
	
	//static_assert(has_get<Failget>::value,"Failget does not provide get");
	static_assert(has_get<Widget>::value,"Widget does not proivde get");
	
	meow("fuck");
	int b[3] = 
	   {
		   1,2,3
	   };
	std::cout <<"Add:"<< (&b)[0]<< std::endl;
	std::cout <<"Addr 2: "<< (&b[0])<< std::endl;
	std::cout <<"Val: "<< *(&b[0])<< std::endl;
	std::cout <<"Val 2: "<< *((&b)[0])<< std::endl;


	test(b);
	
	print(1000);
	print(g);
	print(w);
	//print(f);
	return 0;
}
