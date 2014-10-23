#include <iostream>
#include <memory>
#include <string>
#include <vector>

class test 
{
public:
   test(std::string name):
	   name_(std::move(name))
	  {
		  std::cout<<"Test ist constructed"<<std::endl;
	  }
   std::string getName() const {
	   return name_;
   }
private:
   std::string name_;
};

template <typename T>
void draw(const T& x, std::ostream& out, size_t position)
{out << std::string(position,' ') << x <<std::endl;}


void draw(const test& t, std::ostream& out, size_t position)
{out<<std::string(position,' ')<<t.getName()<<std::endl;}

class object_t;
void draw(const std::vector<object_t>, std::ostream&, size_t);

class object_t 
{
public:
   template<typename T>
   object_t(T x):self_(std::make_shared<model<T>>(std::move(x))){}

   void draw(std::ostream& out, size_t position) const {
	   self_->draw_(out,position);
   }
private:
   struct concept_t
   {
	  virtual ~concept_t(){};
	  virtual void draw_(std::ostream&, size_t) const = 0;
   };
   template<typename T>
   struct model : concept_t
   {
	  model(T x) : data_(std::move(x)){}
	  void draw_(std::ostream& out, size_t position) const{
		  ::draw(data_,out, position);
	  }
	  T data_;
   };
   std::shared_ptr<const concept_t> self_;
};

//using document_t = std::vector<object_t>
									 
void draw(const std::vector<object_t> docs, std::ostream& out, size_t position)
{
	for(auto& iter: docs){
		iter.draw(std::cout,0);
	}
}

typedef std::vector<object_t> document_t;

int main()
{
	document_t doc;
	doc.emplace_back(std::string("FUCK"));
	doc.emplace_back(test("fuck"));
	doc.emplace_back(doc);
	
	for(auto& iter: doc){
		iter.draw(std::cout, 0);
	}
}

	
