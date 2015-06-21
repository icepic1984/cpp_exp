#include <utility>
#include <vector>
#include <iostream>
#include <array>
#include <typeinfo>

template <typename Seq, std::size_t Begin>
struct make_index_range_impl;

template <std::size_t... Is, std::size_t Begin>
struct make_index_range_impl<std::index_sequence<Is...>,Begin>
	:  std::common_type<std::index_sequence<Begin+Is...>>
{};

template <std::size_t Begin, std::size_t End>
using make_index_range = typename make_index_range_impl<
	std::make_index_sequence<End-Begin>,Begin>::type;

template<typename T,
         std::size_t... Index>
constexpr auto generate(std::index_sequence<Index...>) ->
	decltype(std::array<T,sizeof...(Index)>{})
{
	return {{Index...}};
}
	
template <typename T,
          std::size_t N1,
          std::size_t N2,
          std::size_t... I1,
          std::size_t... I2>
constexpr auto cons(const std::array<T,N1>& a, const std::array<T,N2>& b,
                     std::index_sequence<I1...>,
                     std::index_sequence<I2...>) ->
	decltype(std::array<T,N1 + N2> {})
{
	return {{a[I1]...,b[I2]...}};
}


template <typename T,
          std::size_t N1,
          std::size_t N2>
constexpr auto cons(const std::array<T,N1>& a, const std::array<T,N2>& b) ->
	decltype(std::array<T,N1+N2>{})
{
	return cons(a,b,
	             std::make_index_sequence<N1>{},
	             std::make_index_sequence<N2>{});
}

template <typename T,
          std::size_t N2>
constexpr auto cons(const T& a, const std::array<T,N2>& b) ->
	decltype(std::array<T,N2+1>{})
{
	return cons(std::array<T,1>{{a}},
	             b,
	             std::make_index_sequence<1>{},
	             std::make_index_sequence<N2>{});
}

template <typename T,
          std::size_t N1>
constexpr auto cons(const std::array<T,N1>& a, const T& b) ->
	decltype(std::array<T,N1+1>{})
{
	return cons(a,
	             std::array<T,1>{b},
	             std::make_index_sequence<N1>{},
	             std::make_index_sequence<1>{});
}

template <typename T>
constexpr auto cons(const T& a, const T& b) ->
	decltype(std::array<T,2>{})
{
	return cons(std::array<T,1>{a},
	             std::array<T,1>{b},
	             std::make_index_sequence<1>{},
	             std::make_index_sequence<1>{});
}

template <typename T,
          std::size_t N,
          std::size_t... I1,
          std::size_t... I2>
constexpr auto split(const std::array<T,N>& a,
                    std::index_sequence<I1...>,
                    std::index_sequence<I2...>)->
	decltype(std::pair< std::array<T,N/2>,
	         std::array<T,N-N/2>>())
{
	return {{{a[I1]...}},{{a[I2]...}}};
}

template <typename T,
          std::size_t N>
constexpr auto split(const std::array<T,N>& a) ->
	decltype(std::pair<std::array<T,N/2>,
	         std::array<T,N-N/2>>())
{
	return split(a,make_index_range<0,N/2>{},
	             make_index_range<N/2,N>{});
}

template<typename T,
         std::size_t N>
constexpr auto head(const std::array<T,N>& a) ->
	decltype(auto)
{return a[0];}

template<typename T,
         std::size_t N,
         std::size_t... Is>
constexpr auto tail(const std::array<T,N>& a, std::index_sequence<Is...>) ->
	decltype(std::array<T,N-1>{})
{return {{a[Is]...}};}
	
template<typename T,
         std::size_t N>
constexpr auto tail(const std::array<T,N>& a) ->
	decltype(std::array<T,N-1>{})
{return tail(a,make_index_range<1,N> {});}
	            
template <typename T,
          std::size_t N1,
          std::size_t N2>
struct merge_;

template <typename T,
          std::size_t N1,
          std::size_t N2>
constexpr auto merge(const std::array<T,N1>& a, const std::array<T,N2>& b)
	-> decltype(auto)
{
	return merge_<T,N1,N2>::call(a,b);
}

template <typename X, typename Y>
constexpr bool less(const X& x, const Y& y)
{return x < y;}

constexpr bool less(const char* x, const char* y)
{
	while (*x != '\0'){
		if(*y == '\0') return false;
		if(*x > *y) return false;
		if(*x < *y) return true;
		x++;
		y++;
	}
	if (*y != '\0') return false;
	return true;
}

template <typename T,
          std::size_t N1,
          std::size_t N2>
struct merge_
{
   static constexpr auto
   call(const std::array<T,N1>& a, const std::array<T,N2>& b) -> decltype(auto){
	   return (less(head(a), head(b))) ?
	      cons(head(a),merge(tail(a),b)) :
	      cons(head(b),merge(a,tail(b)));
   }
};

template <typename T,
          std::size_t N2>
struct merge_<T,0,N2>
{
   static constexpr auto
   call(const std::array<T,0>& a, const std::array<T,N2>& b) -> decltype(auto){
	   return b;
   }
};

template <typename T,
          std::size_t N1>
struct merge_<T,N1,0>
{
   static constexpr auto
   call(const std::array<T,N1>& a, const std::array<T,0>& b) -> decltype(auto){
	   return a;
   }
};

template <typename T,
          std::size_t N>
struct sort_;

template <typename T,
          std::size_t N>
constexpr auto sort(const std::array<T,N>& l) -> decltype(auto)
{return sort_<T,N>::call(l);}

template <typename T,
          std::size_t N>
struct sort_
{
  static constexpr auto
  call(const std::array<T,N>& l) -> decltype(auto) {
	  return merge(sort(split(l).first),
	               sort(split(l).second));
  } 

};

template <typename T>
struct sort_<T,1>
{
  static constexpr auto
  call(const std::array<T,1>& l) -> decltype(auto) {
	  return l;
  } 
};

template <typename F, typename T, std::size_t N>
struct apply_;

template <typename F, typename T, std::size_t N>
constexpr auto apply(const std::array<T,N>& a, F f) -> decltype(auto)
{
	return apply_<F,T,N>::call(a,f);
}

template <typename F, typename T, std::size_t N>
struct apply_
{
   static constexpr auto
   call(const std::array<T,N>& a, F f){
	   return cons(f(head(a)),apply(tail(a),f));
   }
};

template <typename F, typename T >
struct apply_<F,T,1>
{
   static constexpr auto
   call(const std::array<T,1>& a, F f){
	   return f(head(a));
   }
};

constexpr double mul(double x) {return x*10;}
constexpr std::array<std::size_t,12> a1  {{10,5,2,6,10,5,2,6,10,5,2,6}};
constexpr std::array<double,12> a2  {{10.0,5.0,2.10,
		   6.4,10.5,5.1,
		   2.1,6.5,10.6,
		   5.76,2.6,6.9}};
constexpr std::array<char,3> a3 {{'c','b','a'}};
constexpr std::array<const char*,4> a4 {{ "a","bb","abc","aaa"}};
		
constexpr auto s1 = apply(sort(a2),mul);
constexpr auto s2 = sort(a4);

int main()
{
	
	for(auto iter : s2) {
		std::cout << iter << std::endl;
	 }
	 
}

	
	
