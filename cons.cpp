#include <iostream>
#include <functional>
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
constexpr std::size_t size(List const& l) { return 0; }
 
template <class H, class T>
constexpr std::size_t size(cons_<H,T> const& l) { return 1 + size(l.tail); }

template <typename List>
struct size_
{
   static constexpr std::size_t value = 1 + size_<typename List::tail_>::value;
};

template <>
struct size_<nil_>
{
   static constexpr std::size_t value = 0;
};

// Helper to create list
constexpr auto list() -> decltype(auto)
{return nil;}

template <typename T>
constexpr auto list(T x) -> decltype(auto)
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
constexpr auto list(H h, T ...args) -> decltype(auto)
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
constexpr auto split(const List& x) -> decltype(auto)
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
   call(const List& l) -> decltype(auto){
	   return cons(nil,l);
   }
};


template <typename X, typename Y>
constexpr bool less(const X& x, const Y& y) 
{return x < y;}

template <typename L1, typename L2> struct merge_;

template <typename L1, typename L2>
constexpr auto merge(const L1& a, const L2& b) -> decltype(auto)
{return merge_<L1,L2>::call(a,b);}

template <typename L1, typename L2>
struct merge_
{
  static constexpr auto
  call(const L1& a, const L2& b) -> decltype(auto){
	  return (less(head(a),head(b))) ?
	     cons(head(a),merge(tail(a),b)) :
	     cons(head(b),merge(a, tail(b)));
  }
};

template <typename L1>
struct merge_<L1,nil_> 
{
   static constexpr auto
   call(const L1& a, const nil_& n) -> decltype(auto){
	   return a;
   }
};

template <typename L2>
struct merge_<nil_,L2> 
{
   static constexpr auto
   call(const nil_& n, const L2& b) -> decltype(auto){
	   return b;
   }
};

template <>
struct merge_<nil_,nil_> 
{
   static constexpr auto
   call(const nil_& n1, const nil_& n2) -> decltype(auto){
	   return n1;
   }
};


template <typename List, bool stop = (size_<List>::value <=1)>
struct sort_;

template <typename List>
constexpr auto sort(const List& l) -> decltype(auto)
{return sort_<List>::call(l);}

template <typename List>
constexpr auto sort2(const List& l) -> decltype(auto)
{
	return (size(l) <= 1) ?  l :
	   merge(sort2(head(split<(size_<List>::value / 2)>(l))),
	         sort2(tail(split<(size_<List>::value / 2)>(l))));
}

template <typename List, bool stop>
struct sort_
{
  static constexpr auto
  call(const List& l) -> decltype(auto) {
	  return merge(sort(head(split<(size_<List>::value / 2)>(l))),
	               sort(tail(split<(size_<List>::value / 2)>(l))));
  } 

};

template <typename List>
struct sort_<List,true>
{
  static constexpr auto
  call(const List& l) -> decltype(auto) {
	  return l;
  } 

};

template <std::size_t N,typename List>
struct get_;

template <std::size_t N,typename List>
constexpr auto get(const List& l) -> decltype(auto)
{return get_<N,List>::call(l);} 

template<std::size_t N, typename List>
struct get_
{
   static constexpr auto
   call(const List& l) -> decltype(auto) {
	   return get<(N-1)>(tail(l));
   }
};

template<typename List>
struct get_<1,List>
{
   static constexpr auto
   call(const List& l) -> decltype(auto){
	   return head(l);
   }
};


template <typename F, typename List>
struct apply_;

template <typename F, typename List>
constexpr auto apply(const List& l, F f) -> decltype(auto)
{
	return apply_<F,List>::call(l,f);
}

template <typename F, typename List>
struct apply_ 
{
   static constexpr auto
   call(const List& l, F f){
	   return cons(f(head(l)),apply(tail(l),f));
   }
};

template <typename F>
struct apply_<F,nil_>
{
   static constexpr auto
   call(const nil_& n, F f){
	   return n;
   }
};

constexpr double add(double x)
{return x+100;}

constexpr auto l1 = sort(list(12.4,12.3,302.0,10.0,99.0));
constexpr auto l2 = apply(l1,add);

int main(int argc, char *argv[])
{
	std::cout << l2 << std::endl;
	return 0;
	
}
