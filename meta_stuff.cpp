#include <iostream>
#include <utility>
#include <type_traits>

template <unsigned M, unsigned N>
struct gcd 
{
   static int const value = gcd<N, M % N>::value;
};

template <unsigned M>
struct gcd<M,0> 
{
   static_assert(M !=0,"Error: ");
   static int const value = M;
};


template <typename T, T v>
struct integral_constant 
{
   static constexpr T value = v;
   constexpr operator T() const noexcept {
	   return value;
   }
   constexpr T operator() () const noexcept {
	   return value;
   }
};

template <typename T>
struct rank : integral_constant<std::size_t,0u>
{};

template <typename U, size_t N>
struct rank <U[N]> : integral_constant<std::size_t, 1u + rank<U>::value>
{};

template <typename U>
struct rank<U[]> : integral_constant<std::size_t, 1u + rank<U>::value>
{};

template <typename T>
struct type_is
{
   using type = T;
};

template <typename T>
using type_is_t = typename type_is<T>::type;

template <typename T>
struct remove_const : type_is<T>
{};

template <typename U>
struct remove_const<const U> : type_is<U>
{};

template <typename T>
using remove_const_t = typename remove_const<T>::type;
 
template <bool p, typename T, typename F>
struct conditional : type_is<T> 
{};

template <typename T, typename F>
struct conditional<false,T,F> : type_is<F> 
{};

template <bool p, typename T, typename F>
using conditional_t = typename conditional<p,T,F>::type;

template <bool, typename T = void>
struct enable_if : type_is<T>
{};

template <typename T>
struct enable_if<false,T>
{};

template <bool p, typename T>
using enable_if_t = typename enable_if<p,T>::type;

template <typename T>
enable_if_t<std::is_integral<T>::value,int> f(T val)
{return val;}

template <bool b>
using bool_constant = integral_constant<bool,b>;
using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

template <typename T>
struct is_void : false_type
{};

template <>
struct is_void<void> : true_type
{};

template <typename T, typename U>
struct is_same : false_type
{};

template <typename T>
struct is_same<T,T> : true_type
{};

template <typename T, typename... List>
struct is_one_of;

template <typename T>
struct is_one_of<T> : false_type
{};

template <typename T, typename... List>
struct is_one_of<T,T,List...> : true_type
{};

template <typename T, typename P0, typename... List>
struct is_one_of<T,P0,List...> : is_one_of<T,List...>
{};


template<typename T>
struct is_fucking 
{
private:
   template <typename U, typename = decltype(std::declval<U&>().fuck(
	                                             std::declval<std::string>()))>
   static true_type try_fucking(U&&);
   static false_type try_fucking(...);
public:
   using type = decltype(try_fucking(std::declval<T>()));
};

template <typename...>
using void_t = void;

template <typename, typename = void>
struct has_typename : false_type
{};

template <typename T>
struct has_typename<T, void_t<typename T::type>> : true_type
{};

template <size_t... index>
struct index_sequence
{
   using type = index_sequence<index...>;
};

template <typename T>
struct head;

template <size_t h, size_t... tail>
struct head<index_sequence<h,tail...>> : index_sequence<h>
{};

template <typename T>
using head_t = typename head<T>::type;

template <typename T>
struct tail;

template <size_t h, size_t... tail>
struct tail<index_sequence<h,tail...>> : index_sequence<tail...>
{};

template <typename T>
using tail_t = typename tail<T>::type;

template <typename T>
struct size;

template <size_t... index>
struct size<index_sequence<index...>>
{
	static constexpr int value = 1 +
	   size<tail_t<index_sequence<index...>>>::value;
};

template <size_t i>
struct size<index_sequence<i>>
{
	static constexpr int value = 1;
};

template <typename T>
using size_v =  typename size<T>::value;


template <size_t i, typename T>
struct add;

template <size_t i, size_t... index>
struct add<i,index_sequence<index...>> : index_sequence<i,index...>
{};

template <size_t i, typename T>
using add_t = typename add<i,T>::type;

template <size_t i, typename T>
struct append;

template <size_t i, size_t... index>
struct append<i,index_sequence<index...>> : index_sequence<index...,i>
{};

template <size_t i, typename T>
using append_t = typename append<i,T>::type;

template <size_t n>
struct make_index : append<n-1,typename make_index<n-1>::type>::type
{};

template <>
struct make_index<1> : index_sequence<0>
{};

template <size_t N>
using make_index_t = typename make_index<N>::type;

template <typename A, typename B>
struct merge;

template <size_t... i1, size_t... i2>
struct merge<index_sequence<i1...>,index_sequence<i2...>> :
	index_sequence<i1...,i2...>
{};

template <size_t i, typename T>
struct get : get<i-1,tail_t<T>>::type
{
	static_assert(size<T>::value > i, "Index out of range");
};

template <typename T>
struct get<0,T> : head_t<T> 
{};

template <size_t i, size_t j, typename T>
struct set : merge<head_t<T>,
                   typename set<i-1,j, tail_t<T>>::type>::type
{
	static_assert(size<T>::value > i, "Index out of range");
};

template <size_t j, typename T>
struct set<0,j,T> : merge<index_sequence<j>,tail_t<T>>::type
{
};


template <typename F, size_t... index>
void apply(F f, index_sequence<index...>)
{
	int ignore[] = {(f(index),0)...};
}

// template <size_t h, size_t... tail>
// struct head : index_sequence<h>
// {};

// template <size_t h, size_t... tail>
// using head_t =  typename head<h,tail...>::type;


class A
{
public:
   void fuck() {};
   
   void fuck(double bla){
	   std::cout << bla << std::endl;
   }
};

int main()
{

	set<4,100,index_sequence<1,2,3,4,5>>::type h;
	
	std::cout << size<index_sequence<1,2,3,4,5>>::value <<std::endl;
	
	//get<9,make_index<10>::type>::type h;
	
	apply([] (size_t index) { std::cout << "New Index: "<<index << std::endl;},h);	
	
	// using array_t = int[10][20][30];
	// std::cout <<rank<array_t>::value<<std::endl;
	// std::cout << gcd<12,8>::value<<std::endl;
	// const int a = 4;
	// remove_const_t<const int> b = 4;
	// conditional_t<true,int,double> c = 4;

	
	// std::cout << is_same<double,double>::value<< std::endl;
	// std::cout << is_void<double>::value << std::endl;
	// int x = integral_constant<int,10>();

	
	// std::cout << is_fucking<A>::type::value << std::endl;
	
	
	return 0;
}
