#include <iostream>

template <size_t X, size_t K>
struct highest_bit_helper
{
   static const int value = 
      ((X >> K) % 2) ? K : highest_bit_helper<X,K-1>::value;
};

template <size_t X>
struct highest_bit_helper<X,0>
{
   static const int value = (X % 2) ? 0 : -1;
};

template <size_t X>
struct highest_bit : highest_bit_helper<X,8*sizeof(size_t) -1>
{};

int main(int argc, char *argv[])
{
	
	std::cout << highest_bit<190>::value<<std::endl;
	return 0;
}
