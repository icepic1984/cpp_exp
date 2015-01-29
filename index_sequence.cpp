#include <cstddef>
#include <iostream>

// Define index_sequence data type T
template <std::size_t... index>
struct index_sequence
{
   using type = index_sequence<index...>;
};

// Get head of index sequence T
template <typename T>
struct head;

template <std::size_t h, std::size_t... tail>
struct head<index_sequence<h,tail...>> : index_sequence<h>
{};

template <typename T>
using head_t = typename head<T>::type;

// Get tail of index sequence T
template <typename T>
struct tail;

template <std::size_t h, std::size_t... tail>
struct tail<index_sequence<h,tail...>> : index_sequence<tail...>
{};

template <typename T>
using tail_t = typename tail<T>::type;

// Get number of elements of index sequence T
template <typename T>
struct size;

template <std::size_t... index>
struct size<index_sequence<index...>>
{
	static constexpr int value = 1 +
	   size<tail_t<index_sequence<index...>>>::value;
};

template <std::size_t i>
struct size<index_sequence<i>>
{
	static constexpr int value = 1;
};

template <typename T>
using size_v =  typename size<T>::value;

// Add element i to the beginning of index sequence T
template <std::size_t i, typename T>
struct add;


template <std::size_t i, std::size_t... index>
struct add<i,index_sequence<index...>> : index_sequence<i,index...>
{};

template <std::size_t i, typename T>
using add_t = typename add<i,T>::type;

// Append element i to the end of index sequence 
template <std::size_t i, typename T>
struct append;

template <std::size_t i, std::size_t... index>
struct append<i,index_sequence<index...>> : index_sequence<index...,i>
{};

template <std::size_t i, typename T>
using append_t = typename append<i,T>::type;

// Create index sequence with 0..N elements
template <std::size_t n>
struct make_index : append_t<n-1,typename make_index<n-1>::type>
{};

template <>
struct make_index<1> : index_sequence<0>
{};

template <std::size_t N>
using make_index_t = typename make_index<N>::type;

// Append index sequence B to index sequence A
template <typename A, typename B>
struct merge;

template <std::size_t... i1, std::size_t... i2>
struct merge<index_sequence<i1...>,index_sequence<i2...>> :
	index_sequence<i1...,i2...>
{};

// Get element i from index sequence T
template <std::size_t i, typename T>
struct get : get<i-1,tail_t<T>>::type
{
	static_assert(size<T>::value > i, "Index out of range");
};

template <typename T>
struct get<0,T> : head_t<T> 
{};

// Set element i from index sequence T to j
template <std::size_t i, std::size_t j, typename T>
struct set : merge<head_t<T>,
                   typename set<i-1,j, tail_t<T>>::type>::type
{
	static_assert(size<T>::value > i, "Index out of range");
};

template <std::size_t j, typename T>
struct set<0,j,T> : merge<index_sequence<j>,tail_t<T>>::type
{
};

template <std::size_t i, std::size_t j, typename T>
using set_t = typename set<i,j,T>::type;

template <typename F, std::size_t... index>
void apply(F f, index_sequence<index...>)
{
	int ignore[] = {(f(index),0)...};
}

int main()
{
	set_t<0,123,add_t<10,append_t<99,make_index_t<10>>>> h;
	
	apply([] (std::size_t index)
	      { std::cout << "New Index: "<<index << std::endl;},h);	
	return 0;
}
