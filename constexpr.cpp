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
constexpr auto cons(const array<N1>& a, const array<N2>& b,
                     std::index_sequence<I1...>,
                     std::index_sequence<I2...>) ->
	decltype(array<N1 + N2> {})
{
	return {{a[I1]...,b[I2]...}};
}


template <std::size_t N1, std::size_t N2>
constexpr auto cons(const array<N1>& a, const array<N2>& b) ->
	decltype(array<N1+N2>{})
{
	return cons(a,b,
	             std::make_index_sequence<N1>{},
	             std::make_index_sequence<N2>{});
}

template <std::size_t N2>
constexpr auto cons(const std::size_t a, const array<N2>& b) ->
	decltype(array<N2+1>{})
{
	return cons(array<1>{{a}},
	             b,
	             std::make_index_sequence<1>{},
	             std::make_index_sequence<N2>{});
}

template <std::size_t N1>
constexpr auto cons(const array<N1>& a, std::size_t b) ->
	decltype(array<N1+1>{})
{
	return cons(a,
	             array<1>{b},
	             std::make_index_sequence<N1>{},
	             std::make_index_sequence<1>{});
}

constexpr auto cons(std::size_t a, std::size_t b) ->
	decltype(array<2>{})
{
	return cons(array<1>{a},
	             array<1>{b},
	             std::make_index_sequence<1>{},
	             std::make_index_sequence<1>{});
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
	decltype(auto)
{return a[0];}

template<std::size_t N, std::size_t... Is>
constexpr auto tail(const array<N>& a, std::index_sequence<Is...>) ->
	decltype(array<N-1>{})
{return {{a[Is]...}};}
	
template<std::size_t N>
constexpr auto tail(const array<N>& a) ->
	decltype(array<N-1>{})
{return tail(a,make_index_range<1,N> {});}
	            
template <std::size_t N1, std::size_t N2>
struct merge_;

template <std::size_t N1, std::size_t N2>
constexpr auto merge(const array<N1>& a, const array<N2>& b)
	-> decltype(auto)
{
	return merge_<N1,N2>::call(a,b);
}

template <typename X, typename Y>
constexpr bool less(const X& x, const Y& y)
{return x < y;}

template <std::size_t N1, std::size_t N2>
struct merge_
{
   static constexpr auto
   call(const array<N1>& a, const array<N2>& b) -> decltype(auto){
	   return (less(head(a), head(b))) ?
	      cons(head(a),merge(tail(a),b)) :
	      cons(head(b),merge(a,tail(b)));
   }
};

template <std::size_t N2>
struct merge_<0,N2>
{
   static constexpr auto
   call(const array<0>& a, const array<N2>& b) -> decltype(auto){
	   return b;
   }
};

template <std::size_t N1>
struct merge_<N1,0>
{
   static constexpr auto
   call(const array<N1>& a, const array<0>& b) -> decltype(auto){
	   return a;
   }
};

template <std::size_t N>
struct sort_;

template <std::size_t N>
constexpr auto sort(const array<N>& l) -> decltype(auto)
{return sort_<N>::call(l);}

template <std::size_t N>
struct sort_
{
  static constexpr auto
  call(const array<N>& l) -> decltype(auto) {
	  return merge(sort(split(l).first),
	                    sort(split(l).second));
  } 

};

template <>
struct sort_<1>
{
  static constexpr auto
  call(const array<1>& l) -> decltype(auto) {
	  return l;
  } 
};

constexpr array<12> a1  {{10,5,2,6,10,5,2,6,10,5,2,6}};
constexpr array<12> a2  {{10,5,2,6,10,5,2,6,10,5,2,6}};
constexpr auto s = sort(a2);

template <typename T, std::size_t N>
void test(std::array<T,N>&a ) 
{std::cout << a[0] << std::endl;}

int main()
{
	std::array<double,4> a{{3,4,5,6}};
	test(a);
	//std::cout << t << std::endl;
	 for(auto iter : s) {
	 	std::cout << iter << std::endl;
	 }
	 
}

	
	
