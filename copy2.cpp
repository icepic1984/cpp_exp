#include <iostream>

struct Noisy 
{
   Noisy() {
	   std::cout << "Constructed" << std::endl;
	   con++;
   }
   
   Noisy(const Noisy& other) {
	   std::cout << "Copied" << std::endl;
	   copy++;
   }

   void operator=(const Noisy& other){
	   std::cout << "Assign" << std::endl;
	   ass++;
   }

   void operator=(Noisy&& other){
	   std::cout << "Move Assign" << std::endl;
	   moveass++;
   }
   
   Noisy(Noisy&& other) {
	   std::cout << "Moved" << std::endl;
	   move++;
   }
   ~Noisy() {
	   std::cout << "Deconstructed" << std::endl;
	   dec++;
	   print();
   }

   void print() {std::cout << "Copy: " << copy
                           << " Ass: " << ass
                           << " Move: " << move
                           << " Movea: " << moveass 
                           << " Con: " << con
                           << " Dec: " << dec << std::endl;
   }
   static int con;
   static int ass;
   static int moveass;
   static int move;
   static int dec;
   static int copy;
};

int Noisy::con = 0;
int Noisy::ass = 0;
int Noisy::moveass = 0;
int Noisy::move = 0;
int Noisy::dec = 0;
int Noisy::copy = 0;

class Container 
{
public:
   void setNoisyM(Noisy a){
	   std::cout << "---Move" << std::endl;
	   a_ = std::move(a);
   }

   void setNoisyR(const Noisy& r){
	   std::cout << "---Copy ref" << std::endl;
	   a_ = r;
   }
   void setNoisyV(Noisy r){
	   std::cout << "---Copy value" << std::endl;
	   a_ = r;
   }

   void setNoisyMR(Noisy&& r){
	   std::cout << "---Copy mover" << std::endl;
	   a_ = std::move(r);
   }
   
private:
   Noisy a_;
};


Noisy f()
{
	Noisy g;
	return g;
}



int main()
{
	
	std::cout << "***Noisy***" << std::endl;
	//Noisy a;
	std::cout << "***Container***" << std::endl;
	Container c;
	std::cout << "***Set***" << std::endl;
	c.setNoisyMR(f());
	return 0;
}
