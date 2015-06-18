#include <iostream>
#include <memory>
#include <vector>

namespace tree {

class node;
using node_ptr = std::unique_ptr<node>;

struct node 
{
   node(int i) : data_(i),
                 left_(nullptr),
                 right_(nullptr){}
   int data_;
   node_ptr left_;
   node_ptr right_;
};

namespace tree_impl {

node_ptr copy(const node_ptr& node) 
{
	if(node){
		node_ptr c = std::make_unique<tree::node>(node->data_);
		c->left_ = copy(node->left_);
		c->right_ = copy(node->right_);
		return c;
	} else {
		return nullptr;
	}
}

void add_impl(node_ptr& node, int i) {
	if(!node){
		node = std::make_unique<tree::node>(i);
	} else {
		if(node->data_ > i){
			add_impl(node->left_,i);
		} else {
			add_impl(node->right_,i);
		}
	}
}

void pre_order_print_impl(node_ptr& node)
{
	if(node) {
		std::cout << node->data_ << std::endl;
		pre_order_print_impl(node->left_);
		pre_order_print_impl(node->right_);
	}
}

void inverse_impl(node_ptr& node) 
{
	if(node) {
		node->left_.swap(node->right_);
		inverse_impl(node->left_);
		inverse_impl(node->right_);
	}
}

}
   
class tree 
{
public:
   tree() : root_(nullptr){};
   tree(const tree& other) : root_(tree_impl::copy(other.root_)){};
   
   tree& operator=(const tree& other) {
	   tree tmp(other);
	   *this = std::move(tmp);
	   return *this;
   }

   tree(tree&& other) = default;
   tree& operator=(tree&& other) = default; 


   void add(int i) {
	   using namespace tree_impl;
	   add_impl(root_,i);
   }

   void add(const std::vector<int> data){
	   for(auto iter: data){
		   add(iter);
	   }
   }

   void pre_order_print(){
	   using namespace tree_impl;
	   pre_order_print_impl(root_);
   }
   void inverse() {
	   using namespace tree_impl;
	   inverse_impl(root_);
   }
	   
private:
   node_ptr root_;
};


}

int main(int argc, char *argv[])
{
	tree::tree t;
	t.add({10,4,1,4,7,3,11,34,1});
	t.pre_order_print();
	std::cout << "Copy:" << std::endl;
	tree::tree t2 = t;
	t = t2;
	t2.pre_order_print();
}
