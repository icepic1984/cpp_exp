#include <cstddef>
#include <iostream>

// Identity
template <typename T>
struct type_is
{
   using type = T;
};

template <typename T>
using type_is_t = typename type_is<T>::type;

// Enable type T if bool is true
template <bool, typename T = void>
struct enable_if : type_is<T>
{};

template <typename T>
struct enable_if<false,T>
{};

template <bool p, typename T>
using enable_if_t = typename enable_if<p,T>::type;

// If p is true then T else F
template <bool p, typename T, typename F>
struct conditional : type_is<T> 
{};

template <typename T, typename F>
struct conditional<false,T,F> : type_is<F> 
{};

template <bool p, typename T, typename F>
using conditional_t = typename conditional<p,T,F>::type;

// Integral constant of type T with value v
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

// Definition of true and false type
template <bool b>
using bool_constant = integral_constant<bool,b>;
using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

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

template <>
struct size<index_sequence<>>
{
	static constexpr int value = 0;
};

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

template <typename A, typename B>
using merge_t = typename merge<A,B>::type;


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

// Split index sequence at position i and return left half
template <std::size_t i, typename T>
struct left_split : merge_t<head_t<T>,
                            typename left_split<i-1,tail_t<T>>::type>
{};

template <typename T>
struct left_split<0,T> : head_t<T>
{};

template <std::size_t i, typename T>
using left_split_t = typename left_split<i,T>::type;

// Split index sequence at position i and return right half
template <std::size_t i, typename T>
struct right_split : right_split<i-1,tail_t<T>>::type
{};

template <typename T>
struct right_split<0,T> : tail_t<T>
{};
	
template <std::size_t i, typename T>
using right_split_t = typename right_split<i,T>::type;

// Combine left and right split
template <std::size_t i, typename T>
struct split 
{
   static_assert( size<T>::value > i, "Index out of range");
   using left_t = left_split_t<i,T>;
   using right_t = right_split<i,T>;
};

template <typename A>
using split_half_left_t =  left_split_t<(size<A>::value / 2) - 1,A>;


template <typename A>
using split_half_right_t =  right_split_t<(size<A>::value / 2) - 1,A>;

// Check if index_sequence is empty
template <typename T>
struct is_empty : conditional_t<size<T>::value == 0,true_type,false_type> 
{};

// Return true if first element of index sequence A is greater then
// first element of index sequence B
template <typename A, typename B>
struct greater;

template <size_t i, size_t j>
struct greater<index_sequence<i>,index_sequence<j>>
	: conditional_t< (i > j),true_type,false_type>
{};

// Return true if first element of index sequence A is smaller or
// equal then first element of index sequence B
template <typename A, typename B>
struct smaller;

template <size_t i, size_t j>
struct smaller<index_sequence<i>, index_sequence<j>> :
	conditional_t< (i <= j), true_type,false_type>
{};


// Merge index sequence A and index sequence B element by element
template <typename A, typename B>
struct merge_small : conditional_t<
	smaller<head_t<A>,head_t<B>>::value,
	merge_t<head_t<A>,typename merge_small<tail_t<A>,B>::type>,
	merge_t<head_t<B>,typename merge_small<A,tail_t<B>>::type>>
{};

template <typename B>
struct merge_small<index_sequence<>,B> : B
{
};

template <typename A>
struct merge_small<A,index_sequence<>>  : A
{};

template <typename A, typename B>
using merge_small_t = typename merge_small<A,B>::type;

// Merge sort
template <typename A>
struct merge_sort : merge_small_t<
	typename merge_sort<split_half_left_t<A>>::type,
	typename merge_sort<split_half_right_t<A>>::type>
{};

template <size_t i>
struct merge_sort<index_sequence<i>> : index_sequence<i>
{};

// Map
template <template<class> class F, typename A>
struct map : merge_t<typename F<head_t<A>>::type,
                     typename map<F,tail_t<A>>::type>
{};

template <template<class> class F,std::size_t i>
struct map<F, index_sequence<i>> : F<index_sequence<i>>::type
{};

// Filter
template <template<class> class F, typename A>
struct filter : conditional_t<F<head_t<A>>::value,
                              merge_t<head_t<A>,
                                      typename filter<F,tail_t<A>>::type>,
                              typename filter<F,tail_t<A>>::type>
{};

template <template<class> class F,std::size_t i>
struct filter<F, index_sequence<i>> :  conditional_t<F<index_sequence<i>>::value,
                                                     index_sequence<i>,
                                                     index_sequence<>>
{};



template <typename A>
struct sum;

template <std::size_t i>
struct sum<index_sequence<i>> : index_sequence<i + 2>
{};

template <typename A>
using sum_t = typename sum<A>::type;

template <typename A>
struct isless;

template <std::size_t i>
struct isless<index_sequence<i>> : conditional_t<(i < 2),
	               true_type, false_type>
{};



template <typename F, std::size_t... index>
void apply(F f, index_sequence<index...>)
{
	int ignore[] = {(f(index),0)...};
}

int main()
{
	map<sum,index_sequence<3,3,1,2,3>> h1;
	filter<isless,index_sequence<10,2,45,3,5,7,3,4>> h4;
	
	//merge_small<index_sequence<6,5,4>,index_sequence<1,8>>::type h3;
	merge_sort<index_sequence<10,12,10,1,2,7,3,1,5,7,9,7,5,4,3,2,6,8,1,3>>::type h3;
	// apply([] (std::size_t index)
	//       { std::cout << "L New Index: "<<index << std::endl;},h1);	
	apply([] (std::size_t index)
	      { std::cout << "R New Index: "<<index << std::endl;},h4);	
	return 0;
}
