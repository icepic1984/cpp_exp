#include <iostream>
#include <typeinfo>
#include <string>
#include <functional>
#include <type_traits>

template <typename... Ts> 
class tuple
{};

template<size_t, typename> struct tuple_element;

template<typename T, typename... Ts>
struct tuple_element<0, tuple<T, Ts...>> 
{
	typedef T type;
};

template <size_t k, typename T, typename... Ts>
struct tuple_element<k, tuple<T, Ts...>> 
{
	typedef 
	   typename tuple_element<k-1,tuple<Ts...>>::type type;
};

template<size_t k, typename... Ts>
typename std::enable_if<k == 0, 
                        typename tuple_element<0,tuple<Ts...>>::type&>::type
   get(tuple<Ts...>& t)
{return t.head_;}

template<size_t k, typename T, typename... Ts>
typename std::enable_if<k != 0,
                        typename tuple_element<k,tuple<T,Ts...>>::type&>::type
   get(tuple<T,Ts...>& t)
{
	tuple<Ts...> & super = t;
	return get<k-1>(super);
}

template <typename T, typename... Ts>
class tuple<T,Ts...> : private tuple<Ts...> 
{
public:
   template<size_t k, typename... T2s>
   friend typename std::enable_if<k == 0, 
                                  typename tuple_element<0,tuple<T2s...>>::type&>::type
	  get(tuple<T2s...>& t);

   template<size_t k, typename T1, typename... T2s>
   friend typename std::enable_if<k != 0,
                                  typename tuple_element<k,tuple<T1,T2s...>>::type&>::type
      get(tuple<T1,T2s...>& t);
private:
   T head_;
};


int main(int argc, char *argv[])
{
	tuple<int,std::string> t;
	get<0>(t) = 10;
	get<1>(t) = std::string("hallo");
	std::cout << get<1>(t)<< std::endl;
    return 0;
}
