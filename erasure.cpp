#include <iostream>
#include <memory>
#include <vector>

class object;
using document = std::vector<object>;
void draw(const document& obj);

struct test 
{
   test(): data_("test"){}
   std::string data_;
};
   
void draw(const test& t)
{std::cout << t.data_ << std::endl;}

void draw(int i) 
{std::cout << i << std::endl;}

void draw(const std::string& str)
{std::cout << str << std::endl;}

struct concept 
{
   virtual ~concept(){}
   virtual void draw() const = 0;
   virtual conectp* copy() const  = 0;
};

template <typename T>
struct model : concept 
{
   model(T t) : data_(std::move(t)){}

   void draw() const override {
	   ::draw(data_);
   }
   T data_;
};

class object 
{
public:
   template <typename T>
   object(T t) : self_(std::make_shared<model<T>>(std::move(t))){};

   auto use_count() const{
	   return self_.use_count();
   } 
   
   friend void draw(const object& o){
	   o.self_->draw();
   }
private:
   std::shared_ptr<const concept> self_;
};

class object_un 
{
public:
   template<typename T>
   object_un(T t) : self_(std::make_unique<model<T>>(t)){}

   object_un(const object_un& other): {
	   
   friend void draw(const object_un& o) {
	   o.self_->draw();
   }
private:
   std::unique_ptr<concept> self_;
};


using document = std::vector<object>;
using history = std::vector<document>;

void commit(history& h)
{h.push_back(h.back());}

document& current(history& h)
{return h.back();} 

void undo(history& h)
{h.pop_back();}

void draw(const document& doc)
{
	std::cout << "<doc>" << std::endl;
	for(auto& iter : doc){
		std::cout <<  iter.use_count()<< " ---->";
		draw(iter);
	}
	std::cout << "</doc>" << std::endl;
}

int main(int argc, char *argv[])
{
	object_un o1("test");
	object_un o2(o1);
	
	history h(1);
	current(h).emplace_back(test{});
	current(h).emplace_back("string");
	current(h).emplace_back(0);
	draw(current(h));
	commit(h);
	std::cout << "__________" << std::endl;
	draw(current(h));
	current(h)[0] = "fuck";
	draw(current(h));
	undo(h);
	std::cout << "__________" << std::endl;
	draw(current(h));
	
}
