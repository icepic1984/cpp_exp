#include <iostream>
#include <utility>
#include <tuple>


template <class F, class... Args>
void for_each_argument(F f, Args&&... args) {
	[](...){}((f(std::forward<Args>(args)), 0)...);
}

template <size_t... Indices>
struct index_sequence
{
   using type = index_sequence<Indices...>;
};

template <std::size_t I, typename T>
struct tuple_element {
   T value_;
};

template<typename Sequence, typename... Types>
struct tuple_impl;

template<size_t... Indices, typename... Types>
struct tuple_impl<index_sequence<Indices...>, Types...> :
	tuple_element<Indices,Types>...
{};


template <size_t I, typename Sequence>
struct cat_index_sequence;

template <size_t I, size_t... Indices>
struct cat_index_sequence<I, index_sequence<Indices...>> :
	index_sequence<Indices..., I>
{};

template <size_t N>
struct make_index_sequence :
	cat_index_sequence<N-1, typename make_index_sequence<N-1>::type>::type
{};

template <>
struct make_index_sequence<1>: index_sequence<0>
{};

template <typename... Types>
class tuple : tuple_impl<typename make_index_sequence<sizeof... (Types)>::type,
                         Types...>
{};



void f(size_t s) 
{std::cout << s << std::endl;}

template<size_t... Indices>
void g(index_sequence<Indices...>)
{
	int ignore[] {(f(Indices),3)...};
}


template <typename T1, typename T2>
struct A 
{
   void print() {std::cout << "Generic" << std::endl;}
};

template <typename T1, typename T2>
struct A<std::pair<T1,T2>,T2>
{
	void print() {std::cout << "Pair" << std::endl;}
};


template <typename F, typename T, std::size_t... IS>
decltype(auto) call_impl(F&& f, T&& tuple, std::index_sequence<IS...>)
{
	return f(std::get<IS>(tuple)...);
}

template <typename F, typename T>
decltype(auto) call(F&& f, T&& t)
{
	constexpr auto SIZE = std::tuple_size<std::decay_t<T>>::value;
	return call_impl(std::forward<F>(f), std::forward<T>(t),
	                 std::make_index_sequence<SIZE> {});
}

double sum(double a, double b)
{return a+b;}




template < std::size_t IS, typename T>
void fill(T& t)
{
	std::cin >> std::get<IS>(t);
}

template<std::size_t... IS, typename T>
void fillAll(std::index_sequence<IS...>, T& t)
{
	int ignore[] {(fill<IS>(t),0)...};
}


template <typename... Types>
decltype(auto) getCsv()
{
	using Tuple = std::tuple<Types...>;
	Tuple t;
	constexpr auto TUPLE_SIZE = std::tuple_size<Tuple>::value;
	fillAll(std::make_index_sequence<TUPLE_SIZE> {},t);
	return t;
};


int main(int argc, char *argv[])
{

	auto test = getCsv<int,std::string,double>();
	
	std::cout << std::get<0>(test) << std::endl;
	std::cout << std::get<1>(test) << std::endl;
	// std::cout << call(sum,std::tuple<double,double>(3.0,2.0))<< std::endl;
	
	// A<std::pair<int,double>,double> spec2;
	// //A<double> spec2;
	
	// spec2.print();
	
	// tuple<int,double,int> bla;
	
	
	// typename cat_index_sequence<4,index_sequence<1,2>>::type a;
	// typename make_index_sequence<10>::type b;
	// g(a);
	
	// for_each_argument([](auto a){std::cout << a << std::endl;},4, "hallo");
	return 0;
}
