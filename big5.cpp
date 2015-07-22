#include <iostream>
//https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom <-- Copy/Swap
//https://stackoverflow.com/questions/5976459/how-to-actually-implement-the-rule-of-five/5976829#5976829 <-- Big5
//https://stackoverflow.com/questions/17118256/implementing-move-constructor-by-calling-move-assignment-operator
//http://scottmeyers.blogspot.de/2014/06/the-drawbacks-of-implementing-move.html
//Example 1: Big3 c++98 style
class array_one 
{
public:
   array_one(std::size_t n = 0):
	   size_(n),
	   pointer_(new int[n]){
	   std::cout << "Constructed" << std::endl;
	   for(int i = 0; i < size_; ++i){
		   pointer_[i] = i;
	   }
   }

   ~array_one(){
	   std::cout << "Destructed" << std::endl;
	   delete [] pointer_;
   }

   //copy:
   array_one(const array_one& other) :
	   size_(other.size_),
	   pointer_(new int[other.size_]){
	   std::cout << "Copy" << std::endl;
	   std::copy(other.pointer_,other.pointer_+other.size_,pointer_);
   }
   //assignment
   array_one& operator=(array_one a){
	   std::cout << "Assigment" << std::endl;
	   swap(*this,a);
	   return *this;
   }

   //Swap
   friend void swap(array_one& a1, array_one& a2){
	   using std::swap;
	   std::cout << "Swap" << std::endl;
	   swap(a1.size_,a2.size_);
	   swap(a1.pointer_,a2.pointer_);
   }
   int& operator[](int i){
	   return(pointer_[i]);
   }

   void print(){
	   for(int i = 0; i < size_; ++i){
		   std::cout << pointer_[i] << std::endl;
	   }
   }

private:
   std::size_t size_;
   int* pointer_;
};

//Big5 Variant one: no move assigment, instead copy/assigment by value
class array_two 
{
public:
   array_two(std::size_t n = 0):
	   size_(n),
	   pointer_(new int[n]){
	   std::cout << "Constructed" << std::endl;
	   for(int i = 0; i < size_; ++i){
		   pointer_[i] = i;
	   }
   }

   ~array_two(){
	   std::cout << "Destructed" << std::endl;
	   delete [] pointer_;
   }

   //copy:
   array_two(const array_two& other) :
	   size_(other.size_),
	   pointer_(new int[other.size_]){
	   std::cout << "Copy" << std::endl;
	   std::copy(other.pointer_,other.pointer_+other.size_,pointer_);
   }
   //move constructor
   array_two(array_two&& other) :
	   size_(std::move(other.size_)),
	   pointer_(std::move(other.pointer_)){
	   std::cout << "Move Constructed" << std::endl;
	   other.size_ = 0;
	   other.pointer_ = nullptr;
   }

   //assignment
   array_two& operator=(array_two a){
	   std::cout << "Assigment" << std::endl;
	   swap(*this,a);
	   return *this;
   }

   //Swap
   friend void swap(array_two& a1, array_two& a2){
	   using std::swap;
	   std::cout << "Swap" << std::endl;
	   swap(a1.size_,a2.size_);
	   swap(a1.pointer_,a2.pointer_);
   }
   int& operator[](int i){
	   return(pointer_[i]);
   }

   void print(){
	   for(int i = 0; i < size_; ++i){
		   std::cout << pointer_[i] << std::endl;
	   }
   }

private:
   std::size_t size_;
   int* pointer_;
};


//Big5: Scott Meyer Variant
class array_three 
{
public:
   array_three(std::size_t n = 0):
	   size_(n),
	   pointer_(new int[n]){
	   std::cout << "Constructed" << std::endl;
	   for(int i = 0; i < size_; ++i){
		   pointer_[i] = i;
	   }
   }

   ~array_three(){
	   std::cout << "Destructed" << std::endl;
	   delete [] pointer_;
   }

   //copy:
   array_three(const array_three& other) :
	   size_(other.size_),
	   pointer_(new int[other.size_]){
	   std::cout << "Copy" << std::endl;
	   std::copy(other.pointer_,other.pointer_+other.size_,pointer_);
   }
   //move constructor
   array_three(array_three&& other) noexcept :
	   size_(std::move(other.size_)),
	   pointer_(std::move(other.pointer_)){
	   std::cout << "Move Constructed" << std::endl;
	   other.size_ = 0;
	   other.pointer_ = nullptr;
   }

   //move assigment
   array_three& operator=(array_three&& other)noexcept {
	   std::cout << "Move Assigment" << std::endl;
	   delete [] pointer_;
	   size_ = other.size_;
	   pointer_ = other.pointer_;
	   other.pointer_ = nullptr;
	   other.size_ = 0;
   }
	   
   //assignment
   array_three& operator=(const array_three& a){
	   std::cout << "Assigment" << std::endl;
	   array_three tmp(a);
	   swap(*this,tmp);
	   return *this;
   }

   //Swap
   friend void swap(array_three& a1, array_three& a2) noexcept{
	   using std::swap;
	   std::cout << "Swap" << std::endl;
	   swap(a1.size_,a2.size_);
	   swap(a1.pointer_,a2.pointer_);
   }
   int& operator[](int i){
	   return(pointer_[i]);
   }

   void print(){
	   for(int i = 0; i < size_; ++i){
		   std::cout << pointer_[i] << std::endl;
	   }
   }

private:
   std::size_t size_;
   int* pointer_;
};


int main()
{
	std::cout << "++++++++Array Big5++++++++++" << std::endl;
	array_one a(10);
	array_one b(20);
	std::cout << "Test1:" << std::endl;
	a = b;
	std::cout << "Test2:" << std::endl;
	std::cout << "DONE" << std::endl;
	std::cout << "++++++++Array Big5++++++++++" << std::endl;
	array_two c(10);
	array_two e;
	e = std::move(c);
	std::cout << "DONE" << std::endl;
	std::cout << "++++++++Array Big5 Variant2++++++++++" << std::endl;
	array_three f(20);
	array_three g(15);
	g = std::move(f);
	std::cout << "DONE" << std::endl;


	return 0;
}

