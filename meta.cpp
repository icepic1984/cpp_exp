#include <type_traits>
#include <iostream>

/* Workaround for void_t:
   template <typename...>
   using void_t = void
 */

template<typename... T>
struct make_void
{
   typedef void type;
};

template<typename... T>
using void_t = typename make_void<T...>::type;

//**************************************************
template <typename T, typename = void>
struct has_type_member : std::false_type
{};

template <typename T>
struct has_type_member<T,void_t<typename T::type>>:std::true_type
{};

//**************************************************
template <typename T>
using member_get_t = decltype(std::declval<T&>().get());

template <typename T, typename=void>
struct has_get_member : std::false_type
{};

template <typename T>
struct has_get_member<T,void_t<member_get_t<T>>>:std::true_type
{};

struct testHasType 
{
   using type = int;
};

struct testHasNotType
{
   using dummy = int;
};

struct testGet
{
   int get();
};

//**************************************************
template <typename T, typename U>
struct is_same:std::false_type
{};

template <typename T>
struct is_same<T,T>:std::true_type
{};

template <typename T, typename... Tail>
struct is_one_of;

template <typename T>
struct is_one_of<T>: std::false_type
{};

template <typename T, typename... Tail>
struct is_one_of<T,T,Tail...>:std::true_type
{};

template <typename T, typename U, typename... Tail>
struct is_one_of<T,U,Tail...>:is_one_of<T,Tail...>
{};


int main(int argc, char *argv[])
{
	std::cout<<is_one_of<bool,char,std::string,float,bool,double>::value<<std::endl;
	
	std::cout << "Has type member: "
	          << has_type_member<testHasType>::value <<std::endl;
	std::cout << "Has not type member: "
	          << has_type_member<testHasNotType>::value <<std::endl;
	std::cout << "Has get memfunc: " 
	          << has_get_member<testGet>::value <<std::endl;
	
	std::cout<<is_same<int,bool>::value<<std::endl;
	return 0;

}
