#include <functional>
#include <iostream>

template<typename T>
struct identity 
{
   typedef T type;
};


template<typename T>
void meow(T t, 
		  typename identity<std::function<void (T)>>::type f)
{f(t);}

int main(int argc, char *argv[])
{
	
	std::function<void (int)> fxn = [](int a)
	   {
		   std::cout << a << std::endl;
	   };

	meow(3,fxn);
	meow(4, [](int a){std::cout<<a<<std::endl;});
}

