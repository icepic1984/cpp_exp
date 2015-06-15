#include <vector>
#include <string>
#include <iostream>
 
struct bla 
{
   bla(std::string x):
	   h(x){
	   std::cout<<"bla: I am being constructed"<<std::endl;
   }
   bla(const bla& other):
	   h(other.h){
	   std::cout<<"bla: I am beging copied"<<std::endl;
   }
   bla(bla&& other):
	   h(std::move(other.h)){
	   std::cout<<"bla: I am being moved"<<std::endl;
   }
	   
   std::string h;
};

   
struct President
{
   std::string name;
   std::string country;
   int year;
   bla x_;
   
 
   President(std::string p_name, std::string p_country, int p_year, bla x)
	   : name(std::move(p_name)), country(std::move(p_country)), year(p_year),x_(std::move(x))
	  {
		  std::cout << "I am being constructed.\n";
	  }
   President(President&& other)
   	   : name(std::move(other.name)), country(std::move(other.country)), year(other.year),x_(std::move(other.x_))
   	  {
   		  std::cout << "I am being moved.\n";
   	  }
   
   President& operator=(const President& other) = default;

   President(const President& other):
	   name(other.name), country(other.country),year(other.year),x_(other.x_){
	   std::cout << "I am being copied"<<std::endl;
   }
	   
   
};
 
int main()
{
    std::vector<President> elections;
    std::cout << "emplace_back:\n";
    elections.emplace_back("Nelson Mandela", "South Africa", 1994,bla("hello"));
 
    std::vector<President> reElections;
    std::cout << "\npush_back:\n";
    reElections.push_back(President("Franklin Delano Roosevelt", "the USA", 1936,bla("hello")));
 
    std::cout << "\nContents:\n";
    for (President const& president: elections) {
        std::cout << president.name << " was elected president of "
                  << president.country << " in " << president.year << ".\n";
    }
    for (President const& president: reElections) {
        std::cout << president.name << " was re-elected president of "
                  << president.country << " in " << president.year << ".\n";
    }
}
