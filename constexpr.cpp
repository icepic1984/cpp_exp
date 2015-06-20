#include <utility>
#include <vector>
#include <iostream>
#include <array>
#include <typeinfo>

template <std::size_t N>
using array = std::array<std::size_t,N>;

template <typename Seq, std::size_t Begin>
struct make_index_range_impl;

template <std::size_t... Is, std::size_t Begin>
struct make_index_range_impl<std::index_sequence<Is...>,Begin>
	:  std::common_type<std::index_sequence<Begin+Is...>>
{};

template <std::size_t Begin, std::size_t End>
using make_index_range = typename make_index_range_impl<
	std::make_index_sequence<End-Begin>,Begin>::type;

template<std::size_t... Index>
constexpr auto generate(std::index_sequence<Index...>) ->
	decltype(array<sizeof...(Index)>{})
{
	return {{Index...}};
}
	
template <std::size_t N1, std::size_t N2, std::size_t... I1, std::size_t... I2>
constexpr auto merge(const array<N1>& a, const array<N2>& b,
                     std::index_sequence<I1...>,
                     std::index_sequence<I2...>) ->
	decltype(array<N1 + N2> {})
{
	return {{a[I1]...,b[I2]...}};
}


template <std::size_t N1, std::size_t N2>
constexpr auto merge(const array<N1>& a, const array<N2>& b) ->
	decltype(array<N1+N2>{})
{
	return merge(a,b,std::make_index_sequence<N1>{},
	             std::make_index_sequence<N2>{});
}

template <std::size_t N, std::size_t... I1, std::size_t... I2>
constexpr auto split(const array<N>& a,
                    std::index_sequence<I1...>,
                    std::index_sequence<I2...>)->
	decltype(std::pair<array<N/2>,
	         array<N-N/2>>())
{
	return {{{a[I1]...}},{{a[I2]...}}};
}

template <std::size_t N>
constexpr auto split(const array<N>& a) ->
	decltype(std::pair<array<N/2>,
	         array<N-N/2>>())
{
	return split(a,make_index_range<0,N/2>{},
	             make_index_range<N/2,N>{});
}

template<std::size_t N>
constexpr auto head(const array<N>& a) ->
	decltype(array<1> {})
{return {{a[0]}};}

template<std::size_t N, std::size_t... Is>
constexpr auto tail(const array<N>& a, std::index_sequence<Is...>) ->
	decltype(array<N-1>{})
{return {{a[Is]...}};}
	
template<std::size_t N>
constexpr auto tail(const array<N>& a) ->
	decltype(array<N-1>{})
{return tail(a,make_index_range<1,N> {});}
	            

constexpr array<4> a1  {{10,5,2,6}};
constexpr array<4> a2  {{8,3,5,2}};	
constexpr auto m1 = merge(a1,a2);


int main()
{
	for(auto iter : m1) {
		std::cout << iter << std::endl;
	}
	 
}

	
	
