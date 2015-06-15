#include <string>
#include <vector>
#include <iostream>
#include <chrono>

const std::string stringValue("This string has 29 characters");

class Widget 
{
public:
   Widget(): s(stringValue){}

   Widget(Widget&& w)  :
   	   s(std::move(w.s)){
   	   // std::cout << "Move" << std::endl;
   }
   Widget& operator=(Widget&& w) noexcept{
   	   std::cout << "Assign" << std::endl;
   	   s = std::move(w.s);
   	   return *this;
   }

   //  Widget(Widget&& w) = delete;
   //Widget& operator=(Widget&& w) =delete;
   
   Widget(const  Widget& w) :
	   s(w.s){
	   //std::cout << "Copy" << std::endl;
   }
private:
   std::string s;
};

int main(int argc, char *argv[])
{
	
	std::vector<Widget> vw;
	Widget w;
	int n = 1000000; 
	//int n = 2;
	 typedef std::chrono::high_resolution_clock clock;
	 typedef std::chrono::milliseconds milliseconds;
 
	 clock::time_point t0 = clock::now();
 
	for(std::size_t i = 0; i <n; ++i){
		vw.push_back(w);
	}
	clock::time_point t1 = clock::now();
	auto ms = std::chrono::duration_cast<milliseconds>(t1 - t0).count();
	std::cout << ms << std::endl;
	std::cout << &vw[100] << std::endl;
	return 0;


}
