#include <iostream>
using namespace std;
template <typename Key, typename Info>
class Dictionary
{
	private:
		struct Node		//data in the tree
		{
			Node* left, *right;
			Key key;
			Info info;
			Node(Key k, Info i)	//constructor
			{
				left=NULL;
				right=NULL;
				key=k;
				info=i;
			}
		};
		Node* root;		//first element
		bool insert(Node* head, Key k, Info i)	//recursive function for insertion
		{
			if(head->key<k)						//not yet right place
			{
				if(!head->right)
				{
					head->right=new Node(k, i);		//insert here
					return true;
				}
				return insert(head->right, k, i);
			}
			else if(head->key>k)			//not yet right place
			{
				if(!head->left)
				{
					head->left=new Node(k, i);	//insert here
					return true;
				}
				return insert(head->left, k, i);
			}
			else return false;		//element exists
		}
		void printree(Node* head, int ttl, int space)		//"draw" the tree
		{
			if(!head)		//if empty element
			{
				int pow=1;
				for(int i=0; i<ttl; pow*=2, i++);	//count number of spaces between elements
				for(int j=0; j<pow; j++)
				{
					for(int i=0; i<space; i++, cout<<".");	//insert spaces
					cout<<"||";		//empty element
					for(int i=0; i<space; i++, cout<<".");
				}
				return;
			}
			if(!ttl)			//reached desired depth
			{
				for(int i=0; i<space; i++, cout<<".");	//insert spaces
				cout<<head->key;
				for(int i=0; i<space; i++, cout<<".");
				return;
			}
			printree(head->left, ttl-1, space);
			printree(head->right, ttl-1, space);
			return;
		}
		void rightrot(Node* &pole)		//right rotation
		{
			Node* nodeptr=pole;
			pole=pole->left;
			nodeptr->left=nodeptr->left->right;
			pole->right=nodeptr;
		}
		void leftrot(Node* &pole)		//left rotation
		{
			Node* nodeptr=pole;
			pole=pole->right;
			nodeptr->right=nodeptr->right->left;
			pole->left=nodeptr;
		}
		void balance(Node* &head)	//balance the tree
		{
			int h;
			if(!head) return;
			while(1)		//loop to check if still balanced
			{
				h=height(head->right)-height(head->left);	//balance factor
				if(h<-1)	//left too high
				{
					h=height(head->left->right)-height(head->left->left);
					if(h>=1)	//leftright rotation
					{
						leftrot(head->left);
						rightrot(head);
						continue;
					}
					rightrot(head);
					continue;
				}
				if(h>1)		//right too high
				{
					h=height(head->right->right)-height(head->right->left);	//balance factor
					if(h<=-1)	//rightleft rotation
					{
						rightrot(head->right);
						leftrot(head);
						continue;
					}
					leftrot(head);
					continue;
				}
				break;
			}
			balance(head->left);	//check the rest of the tree
			balance(head->right);
		}
		int height(Node* head)	//return the number of levels
		{
			if(!head) return 0;
			int l=height(head->left);
			int r=height(head->right);
			if(r>l) return r+1;
			return l+1;
		}
		void remove(Key k, Node* &head)		//remove a node with a key
		{
			if(!head) return;
			if(head->key<k) 	//not yet found
			{
				remove(k, head->right);
				return;
			}
			if(head->key>k) 	//not yet found
			{
				remove(k, head->left);
				return;
			}
			Node* nodeptr=head;		//helper
			if(!head->right)		//no right child
			{
				head=head->left;
				delete nodeptr;
				return;
			}
			if(!head->left)		//no left child
			{
				head=head->right;
				delete nodeptr;
				return;
			}
			if(!head->right->left)		//right child has no left child
			{
				head->right->left=head->left;
				head=head->right;
				delete nodeptr;
				return;
			}
			for(nodeptr=head->right; nodeptr->left->left!=NULL; nodeptr=nodeptr->left);	//look for leftmost child of right child
			nodeptr->left->left=head->left;		
			Node* help=head;
			head=nodeptr->left;
			nodeptr->left=nodeptr->left->right;
			head->right=help->right;	//replace with said child
			delete help;
			return;
		}
		Info furthest(Node* head, int ttl)		//value of the most distant leaf
		{
			if(!head) return 0;
			if(!ttl) return head->info;
			if(height(head->right)>height(head->left)) return furthest(head->right, ttl-1);
			return furthest(head->left, ttl-1);
		}
		void avgfurthest(Node* head, int ttl, Info &sum, int &num)	//average value of most distant leaves
		{
			if(!head) return;
			if(!ttl)
			{
				sum+=head->info;
				num++;
				return;
			}
			avgfurthest(head->right, ttl-1, sum, num);
			avgfurthest(head->left, ttl-1, sum, num);
			return;
		}
		void inorder(Node* head)	//print to cout in order
		{
			if(!head) return;
			inorder(head->left);
			cout<<"Key: "<<head->key<<"	Info: "<<head->info<<endl;
			inorder(head->right);
		}
		void postorder(Node* head)	//print to cout postorder
		{
			if(!head) return;
			inorder(head->left);
			inorder(head->right);
			cout<<"Key: "<<head->key<<"	Info: "<<head->info<<endl;
		}
		void preorder(Node* head)	//print to cout preorder
		{
			if(!head) return;
			cout<<"Key: "<<head->key<<"	Info: "<<head->info<<endl;
			inorder(head->left);
			inorder(head->right);
		}
		bool equals(Dictionary<Key, Info>::Node* head, Dictionary<Key, Info>::Node* other)	//check if 2 dicts are equal
		{
			if(head==other) return true;
			if(!head||!other) return false;
			return (head->key==other->key)&&(head->info==other->info)&&equals(head->left, other->left)&&equals(head->right, other->right);
		}
		void insertion(Node* head)	//insert all elements of a tree pointed by head to this tree
		{
			if(!head) return;
			insert(head->key, head->info, false);
			insertion(head->left);
			insertion(head->right);
		}
		void clear(Node* &head)		//clear the tree
		{
			if(!head->left&&!head->right)	//if a leaf, delete
			{
				Node* nodeptr=head;
				head=NULL;
				delete nodeptr;
				return;
			}
			if(head->left) clear(head->left);
			if(head->right) clear(head->right);
			clear(head);		//this become a leaf at one point
		}
		void search(Info i, Node* head, int &j)		//how many elements are there with Info i
		{
			if(!head) return;
			if(head->info==i) j++;
			search(i, head->left, j);
			search(i, head->right, j);
		}
		void max(Node* head, Info &i)	//maximum value of info
		{
			if(!head) return;
			if(head->info>i) i=head->info;
			max(head->left, i);
			max(head->right, i);
		}
		void min(Node* head, Info &i)	//minimum value of info
		{
			if(!head) return;
			if(head->info<i) i=head->info;
			min(head->left, i);
			min(head->right, i);
		}
	public:
		class Iterator				//tree iterator
		{
			private:
				Node* curr, **start;			//current state
				
			public:
				Iterator() {curr=NULL; start=NULL;}			//default constructor
				Iterator(Dictionary<Key, Info> &dict)
				{
					if(dict.isempty())
					{
						curr=NULL;
						start=NULL;
						return;
					}
					curr=dict.root;
					start=&dict.root;
					while(curr->left) curr=curr->left;
				}	//construct and assign to object
				~Iterator(){curr=NULL; start=NULL;}
				Iterator operator++()	//next element
				{
					if(finished()||!*start) return *this;
					if(curr->right)
					{
						for(curr=curr->right; curr->left; curr=curr->left);
						return *this;
					}
					Node* nodeptr=*start;
					while(1)
					{
						if(curr==nodeptr->right)
						{
							curr=nodeptr;
							nodeptr=*start;
							continue;
						}
						if(curr==nodeptr->left)
						{
							curr=nodeptr;
							return *this;
						}
						if(curr->key>nodeptr->key)
						{
							nodeptr=nodeptr->right;
							continue;
						}
						if(curr->key<nodeptr->key)
						{
							nodeptr=nodeptr->left;
							continue;
						}
						curr=NULL;
						return *this;
					}
				}
				Iterator operator--()	//prev element
				{
					if(finished()||!*start) return *this;
					if(curr->left)
					{
						for(curr=curr->left; curr->right; curr=curr->right);
						return *this;
					}
					Node* nodeptr=*start;
					while(1)
					{
						if(curr==nodeptr->left)
						{
							curr=nodeptr;
							nodeptr=*start;
							continue;
						}
						if(curr==nodeptr->right)
						{
							curr=nodeptr;
							return *this;
						}
						if(curr->key>nodeptr->key)
						{
							nodeptr=nodeptr->right;
							continue;
						}
						if(curr->key<nodeptr->key)
						{
							nodeptr=nodeptr->left;
							continue;
						}
						curr=NULL;
						return *this;
					}
				}
				bool operator==(Iterator it){if(!start) return it.start; if((*start==*it.start)&&(curr==it.curr)) return true; return false;}
				Iterator operator=(Dictionary<Key, Info>::Iterator &it){start=it.start; curr=it.curr; return *this;}	//assignment
				Iterator(Dictionary<Key, Info>::Iterator &it){start=it.start; curr=it.curr;}	
				Iterator begin()		//lowest el in tree
				{
					if(!start||!curr) return *this;
					curr=*start;
					while(curr->left) curr=curr->left; 
				}
				Iterator end()		//highest el in tree
				{
					if(!start||!curr) return *this;
					curr=*start;
					while(curr->right) curr=curr->right;
				}
				Key operator!(){if(curr) return curr->key; else return 0;}	//access key
				Info operator*(){if(curr) return curr->info; else return 0;}	//access info
				void reset(){if(!start) return; curr=*start;}	//back to origin
				bool finished(){return !curr;}	//nowhere else to go
		};
		Dictionary()	//default constructor
		{
			root=NULL;	
		}
		Dictionary(Dictionary<Key, Info> &dict)		//copy contructor, uses "insertion"
		{
			root=NULL;
			if(dict.isempty()) return;
			insertion(dict.root);
		}	
		Dictionary(Key arrk[], Info arri[], int i)	//constructor from arrays
		{
			if(i<=0)
			{
				root=NULL;
				return;
			}
			root=new Node(arrk[0], arri[0]);
			for(int j=1; j<i; j++)	//iterate through array
			{
				insert(root, arrk[j], arri[j]);
				balance(root);
			}
		}
		Dictionary<Key, Info> operator= (Dictionary<Key, Info> dict)	//assignment operator
		{
			if(dict==*this) return *this;	//if assigning referance
			clear();
			if(dict.isempty())
			{
				return *this;
			}
			insertion(dict.root);
			return *this;
		}
		~Dictionary()
		{
			clear();
		}
		bool operator== (Dictionary<Key, Info> dict)	//equals operator, call "equals" function
		{
			if(isempty()) return dict.isempty();
			if(height()!=dict.height()) return false;	//simple cases
			equals(root, dict.root);
		}
		Info operator[] (Key k)
		{
			if(isempty()) return 0;
			Iterator it(*this);
			while(!it.finished())	//while iterator can still iterate
			{
				if(!it>k) --it;
				else if(!it<k) ++it;
				else return *it;
			}
			if(!it==k) return *it;	//final chance
			return 0;
		}
		void remove(Key k)	//call "remove" function
		{
			if(isempty()) return;
			remove(k, root);
			balance(root);	//rebalance
		}
		bool insert(Key k, Info i, bool f=true)		//insert elements, if f then rebalance while inserting
		{
			if(isempty())
			{
				root=new Node(k, i);	//simple case
				return true;
			}
			bool b=insert(root, k, i);	//call recursive function
			if(f) balance(root);
			return b;
		}
		int height()	//call recursive function "height"
		{
			if(isempty()) return 0;
			return height(root);
		}
		void printree()	//"draw" the tree
		{
			if(isempty()) return;
			int h=height(root);	//maximum time to live of recursion
			int space;	//number of spaces
			for(int i=1; i<=h; i++)		//iterate through levels
			{
				space=1;
				for(int j=0; j<=(h-i); space*=2, j++); //calculate number of spaces
				printree(root, i-1, space-1);
				cout<<endl<<"--------------------------------------------------------------------------------------------------------------------------------"<<endl;
			}
		}
		void clear()		//clear the tree
		{
			if(isempty()) return;
			clear(root);
			root=NULL;
		}
		bool seek(Key k)	//look for key
		{
			if(isempty()) return false;
			Iterator it(*this);
			while(!it.finished())	//while iterator can still iterate
			{
				if(!it==k) return true;
				if(!it<k) ++it;
				else break;
			}
			return false;
		}
		int search(Info i)	//return number of elements with Info i
		{
			if(isempty()) return 0;
			int j=0;
			search(i, root, j);	//call recursive function
			return j;
		}
		Info furthest()		//return info of furthest leaf
		{
			if(isempty()) return 0;
			return furthest(root, height(root)-1);
		}
		Info avgfurthest()		//return average of furthest leaves
		{
			if(isempty()) return 0;
			Info sum=0;
			int num=0;
			avgfurthest(root, height(root)-1, sum, num);	//call recursive function
			return sum/num;
		}
		Key rightmost()		//return righmost (maximum) key
		{
			if(isempty()) return 0;
			Node* nodeptr;
			for(nodeptr=root; nodeptr->right; nodeptr=nodeptr->right);
			return nodeptr->key;
		}
		Key leftmost()		//return leftmost (minimum) key
		{
			if(isempty()) return 0;
			Node* nodeptr;
			for(nodeptr=root; nodeptr->left; nodeptr=nodeptr->left);
			return nodeptr->key;
		}
		Info max()		//look for max info
		{
			if(isempty()) return 0;
			Info i=root->info;
			max(root, i);	//call recursive function 
			return i;
		}
		Info min()		//look for min info
		{
			if(isempty()) return 0;
			Info i=root->info;
			min(root, i);	//call recursive function 
			return i;
		}
		bool isempty(){return !root;}	//if empty return true, else false
		Info rootinf(){if(isempty()) return 0; return root->info;}	//info at root
		Key rootkey(){if(isempty()) return 0; return root->key;}	//key at root
		void inorder(){inorder(root);}	//traversals
		void postorder(){postorder(root);}
		void preorder(){preorder(root);}
};
void key()	//"Press enter to continue"
{
    cout<<"Press enter to continue"<<endl;
    cin.get();
    fseek(stdin,0,SEEK_END);
    return;
}
template<typename Key, typename Info>
void stresstest(Key arr1[], Info arr2[], int j, Key r1, Key r2, Key r3, Key k, Info i)	//this function tests all the methods
{																					//r1, r2, r3 - keys to be removed, k - such nodes will be inserted																		
	Dictionary<Key, Info> dict(arr1, arr2, j);
	typename Dictionary<Key, Info>::Iterator iterator(dict);
	cout<<"empty: "<<dict.isempty()<<" height: "<<dict.height()<<endl;
	cout<<"tree:"<<endl;
	dict.printree();
	key();
	cout<<"Inorder:"<<endl;
	dict.inorder();
	key();
	cout<<"Postorder:"<<endl;
	dict.postorder();
	key();
	cout<<"Preorder:"<<endl;
	dict.preorder();
	key();
	cout<<"Copy constructor:"<<endl;
	Dictionary<Key, Info> copy(dict);
	if(!(copy==dict))
	{
		cout<<"Copy constructor or equality fault, aborting"<<endl;
		key();
		return;
	}
	copy.printree();
	key();
	cout<<"Assignment operator:"<<endl;
	copy=dict;
	if(!(copy==dict))
	{
		cout<<"Assignment fault, aborting"<<endl;
		key();
		return;
	}
	copy.printree();
	key();
	cout<<"Clear the copy:"<<endl;
	copy.clear();
	if(!(copy.isempty()))
	{
		cout<<"Clearing fault, aborting"<<endl;
		key();
		return;
	}
	copy.printree();
	key();
	Info info=dict.max();
	cout<<"Maximum value of Info: "<<info<<endl;
	for(int l=0; l<j; l++)
	{
		if(arr2[l]>info)
		{
			cout<<"Maximum fault, aborting"<<endl;
			key();
			return;
		}
	}
	info=dict.min();
	cout<<"Minimum value of Info: "<<info<<endl;
	for(int l=0; l<j; l++)
	{
		if(arr2[l]<info)
		{
			cout<<"Minimum fault, aborting"<<endl;
			key();
			return;
		}
	}
	Key val=dict.rightmost();
	cout<<"Maximum value of Key: "<<val<<endl;
	for(int l=0; l<j; l++)
	{
		if(arr1[l]>val)
		{
			cout<<"Rightmost fault, aborting"<<endl;
			key();
			return;
		}
	}
	val=dict.leftmost();
	cout<<"Minimum value of Key: "<<val<<endl;
	for(int l=0; l<j; l++)
	{
		if(arr1[l]<val)
		{
			cout<<"Leftmost fault, aborting"<<endl;
			key();
			return;
		}
	}
	key();
	cout<<"Elements of Key arr1:"<<endl;
	for(int l=0; l<j; l++)
	{
		if(!dict.seek(arr1[l]))
		{
			cout<<"Seeking fault, aborting"<<endl;
			key();
			return;
		}
		cout<<arr1[l]<<" ";
	}
	cout<<endl<<"Are all found in the tree"<<endl;
	key();
	cout<<"Elements of Info arr2:"<<endl;
	for(int l=0; l<j; l++)
	{
		if(!dict.search(arr2[l]))
		{
			cout<<"Search fault, aborting"<<endl;
			key();
			return;
		}
		cout<<arr2[l]<<" ";
	}
	cout<<endl<<"Are all found in the tree"<<endl;
	key();
	cout<<"Info at "<<r1<<" "<<r2<<" "<<r3<<endl;
	cout<<dict[r1]<<" "<<dict[r2]<<" "<<dict[r3]<<endl;
	cout<<"Copy constructor: "<<endl;
	key();
	cout<<"Test iterator, traverse upwards then downwards"<<endl;
	dict.printree();
	if(dict.leftmost()!=!iterator)
	{
		cout<<"Iterator fault, aborting"<<endl;
		key();
		return;
	}
	Key prev=!iterator;
	while(!iterator.finished())
	{
		if(!dict.search(*iterator))
		{
			cout<<"Iterator fault, aborting"<<endl;
			key();
			return;
		}
		if(dict[!iterator]!=*iterator)
		{
			cout<<"Iterator fault, aborting"<<endl;
			key();
			return;
		}
		if(!dict.seek(!iterator))
		{
			cout<<"Iterator fault, aborting"<<endl;
			key();
			return;
		}
		prev=!iterator;
		++iterator;
		if((!iterator.finished())&&!iterator<=prev)
		{
			cout<<"Iterator fault, aborting"<<endl;
			key();
			return;
		}
	}
	iterator.end();
	while(!iterator.finished())
	{
		if(!dict.search(*iterator))
		{
			cout<<"Iterator fault, aborting"<<endl;
			key();
			return;
		}
		if(dict[!iterator]!=*iterator)
		{
			cout<<"Iterator fault, aborting"<<endl;
			key();
			return;
		}
		if(!dict.seek(!iterator))
		{
			cout<<"Iterator fault, aborting"<<endl;
			key();
			return;
		}
		prev=!iterator;
		--iterator;
		if((!iterator.finished())&&!iterator>=prev)
		{
			cout<<"Iterator fault, aborting"<<endl;
			key();
			return;
		}
	}
	cout<<"Iterator ok"<<endl;
	key();
	cout<<"Removed elements with key: "<<r1<<" "<<r2<<" "<<r3<<endl;
	dict.remove(r1);
	dict.remove(r2);
	dict.remove(r3);
	if((dict.seek(r1))||(dict.seek(r2))||(dict.seek(r3)))
	{
		cout<<"Removal fault, aborting"<<endl;
		key();
		return;
	}
	dict.printree();
	key();	
	cout<<"Insert element with key "<<k<<" and info "<<i<<endl;
	dict.insert(k, i);
	if(!dict.seek(k)||!dict.search(i))
	{
		cout<<"Insertion fault, aborting"<<endl;
		key();
		return;
	}
	dict.printree();
	key();
	cout<<"End of test"<<endl;
	key();
}
int main()
{
	int arr1[]={18,19,82,84,22,29,91,21,11,12,69,54,32,15,23,65,43,92,33,};
	int arr2[]={ 8, 9, 2, 3, 2, 2, 1, 1, 1,12,69, 4,32,11, 3, 6,43, 2,33};
	int arr3[]={11, 23, 34};
	int arr4[]={69,45,32,43,67,87,34,54,12,44,65,37,18,33,30,36,20,50,60,80,49,40,42,98,99,96,23,55,38,76,47,56,77,21,22,25,57,75,51,11,29,95};
	int arr5[]={8, 9, 2, 3, 2, 2, 1, 1, 1,12,69, 4,32,11, 3, 6,43, 2,33,8, 9, 2, 3, 2, 2, 1, 1, 1,12,69, 4,32,11, 3, 6,43, 2,33,8, 9, 2, 3, 2, 2, 1, 1, 1,12,69, 4,32,11, 3, 6,43, 2,33};
	stresstest(arr1, arr2, 18, 22, 11, 54, 59, 69);
	stresstest(arr3, arr2, 3, 11, 23, 34, 59, 90);
	stresstest(arr2, arr3, 0, 11, 23, 1, 2, 4);
	stresstest(arr4, arr5, 42, 76, 22, 43, 27, 420);
}

