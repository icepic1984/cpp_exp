#include <iostream>
#include <typeinfo>

template <typename T1, typename T2>
struct typepair
{
   using head_t = T1;
   using tail_t = T2;
   
};

struct empty
{};

struct Error;

template <size_t N, typename CONTAINER, typename ERR = Error>
struct typeat;

template <size_t N, typename T1, typename T2,typename ERR>
struct typeat<N, typepair<T1,T2>,ERR>
{
   typedef typename typeat<N-1,T2,ERR>::type type;
};

template <typename T1, typename T2, typename ERR>
struct typeat<0,typepair<T1,T2>,ERR>
{
   typedef T1 type;
};

template <size_t N, typename ERR>
struct typeat<N,empty,ERR>
{
   typedef ERR type;
};

template <typename CONTAINER>
struct front;

template <typename CONTAINER>
struct back;

template <>
struct back<empty>;

template <>
struct front<empty>;

template <typename T1, typename T2>
struct front<typepair<T1,T2>>
{typedef T1 type;};

template <typename T1>
struct back<typepair<T1,empty>>
{typedef T1 type;};

template <typename T1, typename T2>
struct back<typepair<T1,T2>>:back<T2>
{
};

template <typename CONTAINER>
struct length;

template <typename T1, typename T2>
struct length<typepair<T1,T2>> 
{static const int value = 1 + length<T2>::value;};

template <typename T1>
struct length<typepair<T1,empty>>
{static const int value = 1;};

	
template<typename T, typename CONTAINER>
struct find;

template <typename T>
struct find<T,empty>
{static const int value = -1;};

template <typename T1, typename T2>
struct find<T1,typepair<T1,T2>>
{static const int value = 0;};

template <typename T1, typename T2, typename T3>
struct find<T1,typepair<T2,T3>>
{
	static const int aux = find<T1,T3>::value;
	static const int value = (aux == -1 ? -1 : aux+1);
};


template <typename CONTAINER, typename T1>
struct push_front;

template <typename T>
struct push_front<empty,T>
{
   typedef typepair<T,empty> type;
};

template <typename T1,typename T2, typename T>
struct push_front<typepair<T1,T2>,T>
{
   typedef typepair<T,typepair<T1,T2>> type;
};

template <typename CONTAINER>
struct pop_front;

template <>
struct pop_front<empty>;

template <typename T1, typename T2>
struct pop_front<typepair<T1,T2>>
{typedef T2 type;};

template <typename T1, typename T2, typename T3>
using typelist3_t = typepair<T1, 
                             typepair<T2,
                                      typepair<T3,empty>>>;

int main(int argc, char *argv[])
{
	
	using test_t = typelist3_t<std::string, float, double> ;
	test_t a;
	
	std::cout << typeid(typeat<1,test_t,Error>::type).name()
	          <<std::endl;
	
	std::cout << typeid(front<test_t>::type).name() << std::endl;
	std::cout << length<test_t>::value << std::endl;
	std::cout << find<std::string,test_t>::value << std::endl;
	std::cout << find<double,test_t>::value << std::endl;
	std::cout << find<float,test_t>::value << std::endl;
	std::cout << find<void,test_t>::value << std::endl;
	std::cout << length<pop_front<push_front<test_t,void>::type>::type>::value
	          <<std::endl;

	
	return 0;
}
