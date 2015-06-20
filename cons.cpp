#include <iostream>
#include <string>

// Define nil
struct nil_ 
{
   constexpr nil_(){};
};
std::ostream& operator<<(std::ostream& s, const nil_&)
{return s<<"nil";}

// Instance of nil_
constexpr nil_ nil;

// Define cons cell helper
template <typename H, typename T = nil_>
struct cons_ 
{
   using head_ = H;
   using tail_ = T;

   constexpr cons_(){}
   constexpr cons_(const H& x, const T& y):
	   head(x),tail(y){}
   head_ head;
   tail_ tail;
};

// Create cons cell
template <typename H, class T = nil_>
constexpr cons_<H,T> cons(const H& h, const T& t = nil)
{return cons_<H,T>(h,t);}

// Get head
template <typename L>
using head_ = typename L::head_;
template <typename L>
constexpr const head_<L>& head(const L& l) 
{return l.head;}

// Get tail
template <typename L>
using tail_ = typename L::tail_;
template <typename L>
constexpr const tail_<L>& tail(const L& l)
{return l.tail;}

// Calculate size
template <class List>
constexpr unsigned size(List const& l) { return 0; }
 
template <class H, class T>
constexpr unsigned size(cons_<H,T> const& l) { return 1 + size(l.tail); }

// Helper to create list
constexpr auto list() -> decltype(nil)
{return nil;}

template <typename T>
constexpr auto list(T x) -> decltype(cons(x,nil))
{return cons(x,nil);}

// Create list with variadic elements
// Helper:
template <typename... T>
struct list_t;

template <typename H, typename... T>
struct list_t<H,T...>
{
   using type = cons_<H,typename list_t<T...>::type>;
};

template <typename H>
struct list_t<H>
{
   using type = cons_<H,nil_>;
};

// Implementation
template <typename H, typename ...T>
constexpr auto list(H h, T ...args) -> typename list_t<H,T...>::type
{return cons(h,list(args...));}

// Print
template <typename X>
void print(std::ostream& s, const X& h)
{s << ". " <<h;}

template <typename H>
void print(std::ostream& s, const cons_<H,nil_>& h)
{s << head(h);}

template <typename H, typename T>
void print(std::ostream& s, const cons_<H,T>& h)
{
	s << head(h) << " ";
	print(s,tail(h));
}

template <typename H, typename T>
std::ostream& operator<<(std::ostream& s, const cons_<H,T>& x)
{
	s << "( ";
	print(s,x);
	return s << " )";
}

template <std::size_t N, typename List> struct split_;

template <std::size_t N, typename List>
constexpr auto split(const List& x) -> decltype(split_<N,List>::call(x))
{return split_<N,List>::call(x);}

template <std::size_t N, typename List>
struct split_
{
   using next_t = decltype(split<N-1>(tail(std::declval<List>())));

   static constexpr auto
   call2(const List& l, const next_t& next) ->
	   decltype(cons(cons(head(l),head(next)),tail(next))){
	   return cons(cons(head(l),head(next)),tail(next));
   }
   static constexpr auto
   call(const List& l) -> decltype(call2(l,split<(N-1)>(tail(l)))){
	   return call2(l,split<(N-1)>(tail(l)));
   }
   
};

template <typename List>
struct split_<0,List>
{
   static constexpr auto
   call(const List& l) -> decltype(cons(nil,l)){
	   return cons(nil,l);
   }
};


int main(int argc, char *argv[])
{
	
	auto bla = list(list(10,std::string("erer"),10),10);
	auto blup = cons(10,cons(20,10));

	auto test = cons(10,cons(20,cons(40,cons(50,nil))));
	auto sp = split<1>(test);
	
	std::cout << test << std::endl;
	std::cout << sp << std::endl;
	std::cout << size(bla) << std::endl;
	std::cout << bla << std::endl;
	std::cout << blup << std::endl;
	
	return 0;
	
}
