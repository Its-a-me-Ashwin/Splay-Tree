#include "splay_tree.hpp"

using namespace std;

struct node
{
	int key;
	node* left,*right;
};

class splay_tree_implementation : public splay_tree
{
private:
	int number_of_nodes;
	node* root;

public:
	splay_tree_implementation();
	node* getNewNode(int);
	node* rotateLeft(node*);
	node* rotateRight(node*);
	node* splay(node*,int);
	int find(int);
	void insert(int);
	void remove(int);
	node* delete_parent(node*,int);
	node* delete_root(node*);
	vector<int> post_order();
	vector<int> pre_order();
	vector<int> in_order();
	void post(node*,vector<int>&);
	void pre(node*,vector<int>&);
	void in(node*,vector<int>&);
	int get_num_nodes();
	~splay_tree_implementation();
};

int splay_tree_implementation::get_num_nodes()
{
	return number_of_nodes;
}


splay_tree_implementation::splay_tree_implementation()
{
	root = NULL;
	number_of_nodes = 0;
}

node* splay_tree_implementation::getNewNode(int data)
{
	node* newNode = (node*)malloc(sizeof(node)*1);
	newNode -> key = data;
	newNode -> left = NULL;
	newNode -> right = NULL;
	//printf("made");
	return newNode;	
}

node* splay_tree_implementation::rotateLeft(node* x)
{
	if(!x || !x->right) 
	{
		//printf("error");		
		return x;
	}
	else
	{
		node* temp = x -> right;
		x -> right = temp -> left;
		temp -> left = x;
		return temp;
	}
}

node* splay_tree_implementation::rotateRight(node* y)	
{
	if(!y || !y->left) 
	{
		//printf("error");		
		return y;
	}
	else
	{
		node* temp = y -> left;
		y -> left = temp -> right;
		temp -> right = y;
		return temp;
	}
}


node* splay_tree_implementation::splay(node* r, int key)
{
	if(r == NULL || r -> key == key) // already splayed
	{
		return r;
	}

	if(key < r -> key)		
	{
		if(!r->left) return r;      
		if(key < r->left->key)		//left left
		{
			r->left->left = splay(r->left->left, key);
			r = rotateRight(r);
		}
		else if(key > r->left->key)  //left right
		{
			r->left->right = splay(r->left->right,key);
			r->left = rotateLeft(r->left);
		}
		r = rotateRight(r);    

	}

	else							
	{
		if(!r->right) return r;

		if(key > r->right->key)		//right rihgt
		{
			r->right->right = splay(r->right->right,key);
			r = rotateLeft(r);
		}
		else if(key < r->right->key)	//right left
		{
			r->right->left = splay(r->right->left,key);
			r->right = rotateRight(r->right);
		}
		r = rotateLeft(r);			
	}
	//printf("splayed fully");
}

int splay_tree_implementation::find(int key)
{
	//if(root == NULL) return 0;							

	root = splay(root, key);

	if(root->key == key) 
	{
		return 1;
	}
	else
	{
		return 0;	
	}
}
void splay_tree_implementation::insert(int key)
{
	node* root_cpy = root;					

	if(root_cpy == NULL)// only one element					
	{
		root =  getNewNode(key);
		number_of_nodes += 1;
	}
	else
	{
		node* parent = NULL;				
		//parent = get_parent(root_cpy,key);
		while(root_cpy && root_cpy->key!=key)		
		{
			parent = root_cpy;
			if(key < root_cpy->key) root_cpy = root_cpy->left;
			else root_cpy = root_cpy->right;
		}		

		
		if(root_cpy == NULL)  
		{

			node* newNode = getNewNode(key);
			if(key < parent->key) parent->left = newNode;
			else parent->right = newNode;

			number_of_nodes++;
		}
		root = splay(root, key);  // to bring nenode / parent to root

	}
}

node* splay_tree_implementation::delete_parent(node* r, int key)
{
	if (r == NULL) return NULL;
	node* parent = NULL;
	while(r != NULL && r->key!=key)
	{
		parent = r;
		if(key > r->key) r = r->right;
		else r = r->left;
	}


	if(r == NULL) return parent;    

	if(!(r->left && r->right))		
	{
		node* temp;
		if(!r->left) temp = r->right;
		else temp = r->left;

		if(parent->left == r) parent->left = temp;
		else parent->right = temp;

		free(r); 
	}

	else		
	{
		node* p = NULL;    				//
		node* successor = r->right; 	//we know successor isn't NULL;

		while(successor->left)
		{
			p = successor;
			successor = successor->left;
		}

		r->key = successor->key;

		if(!p) r->right = successor->right;
		else p->left = successor->right;
		free(successor);	
	}
	number_of_nodes -= 1;
	return parent;
}

node* splay_tree_implementation::delete_root(node* r)
{
	if (root == NULL) return NULL;
	if(r->left == NULL)// go right
	{
		node* temp = r->right;
		free(r);
		return temp;
	}
	if(r->right == NULL)// go left
	{
		node* temp = r->left;
		free(r);
		return temp;
	}

	else // do rotations 2 childerens 
	{

	node* parent = NULL;
	node* successor = r->right; // get inorder successor
	//sucessor = in_order_scucessor(sucessor)
	while(successor->left)
	{
		parent = successor;
		successor = successor->left;
	}

	r->key = successor->key;

	if(parent != NULL)
	{ 
		parent->left = successor->right;
	}
	else
	{
		 r->right = successor->right;
	}
	free(successor);
	return r;
	}

}

void splay_tree_implementation::remove(int key)
{
	if(root == NULL) return; // empty
	//node * parent == NULL;
	//parent get_parent(root_cpy,key);
	if(root->key == key) // root has data
	{
		root = delete_root(root);
		number_of_nodes--;
	}
	else
	{
		node* parent = delete_parent(root,key); // get parents data
		if (parent == NULL) printf("error!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		else root = splay(root, parent->key);		
	}	
}



void splay_tree_implementation::post(node* r, vector<int> &result)
{
	if(r != NULL)
	{
		post(r->left, result);
		post(r->right, result);
		result.push_back(r->key);
		//printf(" %d ",r->key);
	}
	else;
}

vector<int> splay_tree_implementation::post_order()
{
	vector<int> result;
	if (root == NULL) return result;
	else{
	post(root,result);
	return result;}
}


void splay_tree_implementation::pre(node* r, vector<int> &result)
{
	if(r != NULL)
	{
		result.push_back(r->key);
		//printf(" %d ",r->key);
		pre(r->left, result);
		pre(r->right, result);
	}	
	else;
}

vector<int> splay_tree_implementation::pre_order()
{
	vector<int> result;
	if (root == NULL) return result;
	else{
	pre(root,result);
	return result;}
}

void splay_tree_implementation::in(node* r, vector<int> &result)
{
	if(r != NULL)
	{
		in(r->left, result);
		result.push_back(r->key);
		//printf(" %d ",r->key);
		in(r->right, result);
	}
	else;
}

vector<int> splay_tree_implementation::in_order()
{
	vector<int> result;
	if (root == NULL) return result;
	else{
	in(root,result);
	return result;}
}

void del (node *head)
{
	if (head == NULL) return;
	else
	{
		del(head->right);
		del(head->left);
		free(head);
	}
	return;
}

splay_tree_implementation::~splay_tree_implementation()
{
    if (root)
    {
        del(root);
    }
    number_of_nodes = 0;
}



int main ()
{
	splay_tree_implementation a;
	printf("\nNode:%d\n",a.get_num_nodes());
	a.insert(5);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	a.insert(6);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	a.insert(4);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	a.insert(3);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	a.insert(2);
	a.insert(1);
	a.insert(10);
	a.insert(9);
	a.insert(7);
	a.insert(8);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	printf("\n");
	a.pre_order();
	printf("\n removing\n\n");
	a.post_order();
	printf("\n");
	a.pre_order();



	a.remove(5);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();

	a.remove(3);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();

	a.remove(10);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();

	a.remove(1);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	
	a.remove(1);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	
	a.remove(2);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();

	a.remove(3);
	a.remove(4);
	a.remove(5);
	a.remove(6);
	a.remove(7);
	a.remove(8);
	a.remove(9);
	a.remove(10);

	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	
	a.remove(4);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	

	printf("\nNode:%d\n",a.get_num_nodes());
	a.insert(5);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	a.insert(6);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	a.insert(4);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	a.insert(3);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	a.insert(2);
	a.insert(1);
	a.insert(10);
	a.insert(9);
	a.insert(7);
	a.insert(8);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	printf("\n");
	a.pre_order();
	printf("\n removing\n\n");
	a.post_order();
	printf("\n");
	a.pre_order();

	a.remove(5);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();

	a.remove(3);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();

	a.remove(10);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();

	a.remove(1);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	
	a.remove(1);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();
	
	a.remove(2);
	printf("\nNode:%d\n",a.get_num_nodes());		
	a.post_order();

}
