#include <iostream>
using namespace std;
template <typename Key, typename Info>
class Dictionary
{
	private:
		struct Node
		{
			//friend Iterator(Node* head);
			Node* left, *right;
			Key key;
			Info info;
			Node(Key k, Info i)
			{
				left=NULL;
				right=NULL;
				key=k;
				info=i;
			}
		};
		Node* root;
		bool insert(Node* head, Key k, Info i)
		{
			if(head->key<k)
			{
				if(!head->right)
				{
					head->right=new Node(k, i);
					return true;
				}
				return insert(head->right, k, i);
			}
			else if(head->key>k)
			{
				if(!head->left)
				{
					head->left=new Node(k, i);
					return true;
				}
				return insert(head->left, k, i);
			}
			else return false;
		}
		void printree(Node* head, int ttl, int space)
		{
			if(!head)
			{
				int pow=1;
				for(int i=0; i<ttl; pow*=2, i++);
				for(int j=0; j<pow; j++)
				{
					for(int i=0; i<space; i++, cout<<".");
					cout<<"||";
					for(int i=0; i<space; i++, cout<<".");
				}
				return;
			}
			if(!ttl)
			{
				for(int i=0; i<space; i++, cout<<".");
				cout<<head->key;
				for(int i=0; i<space; i++, cout<<".");
				return;
			}
			printree(head->left, ttl-1, space);
			printree(head->right, ttl-1, space);
			return;
		}
		void rightrot(Node* &pole)
		{
			Node* nodeptr=pole;
			pole=pole->left;
			nodeptr->left=nodeptr->left->right;
			pole->right=nodeptr;
		}
		void leftrot(Node* &pole)
		{
			Node* nodeptr=pole;
			pole=pole->right;
			nodeptr->right=nodeptr->right->left;
			pole->left=nodeptr;
		}
		void balance()
		{
			int h;
			while(1)
			{
				h=height(root->right)-height(root->left);
				if(h<-1)
				{
					h=height(root->left->right)-height(root->left->left);
					if(h>=1)
					{
						leftrot(root->left);
						rightrot(root);
						continue;
					}
					rightrot(root);
					continue;
				}
				if(h>1)
				{
					h=height(root->right->right)-height(root->right->left);
					if(h<=-1)
					{
						rightrot(root->right);
						leftrot(root);
						continue;
					}
					leftrot(root);
					continue;
				}
				break;
			}
			return;
		}
		int height(Node* head)
		{
			if(!head) return 0;
			int l=height(head->left);
			int r=height(head->right);
			if(r>l) return r+1;
			return l+1;
		}
		void remove(Key k, Node* &head)
		{
			if(!head) return;
			if(head->key<k) 
			{
				remove(k, head->right);
				return;
			}
			if(head->key>k) 
			{
				remove(k, head->left);
				return;
			}
			Node* nodeptr=head;
			if(!head->right)
			{
				head=head->left;
				delete nodeptr;
				return;
			}
			if(!head->left)
			{
				head=head->right;
				delete nodeptr;
				return;
			}
			if(!head->right->left)
			{
				head->right->left=head->left;
				head=head->right;
				delete nodeptr;
				return;
			}
			for(nodeptr=head->right; nodeptr->left->left!=NULL; nodeptr=nodeptr->left);	//THIS BAD/////////////
			nodeptr->left->left=head->left;
			Node* help=head;
			head=nodeptr->left;
			nodeptr->left=nodeptr->left->right;
			head->right=help->right;
			delete help;
			return;
		}
		Info furthest(Node* head, int ttl)
		{
			if(!head) return 0;
			if(!ttl) return head->info;
			if(height(head->right)>height(head->left)) return furthest(head->right, ttl-1);
			return furthest(head->left, ttl-1);
		}
		void avgfurthest(Node* head, int ttl, Info &sum, int &num)
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
		void inorder(Node* head)
		{
			if(!head) return;
			inorder(head->left);
			cout<<"Key: "<<head->key<<"	Data: "<<head->info<<endl;
			inorder(head->right);
		}
		void postorder(Node* head)
		{
			if(!head) return;
			inorder(head->left);
			inorder(head->right);
			cout<<"Key: "<<head->key<<"	Data: "<<head->info<<endl;
		}
		void preorder(Node* head)
		{
			if(!head) return;
			cout<<"Key: "<<head->key<<"	Data: "<<head->info<<endl;
			inorder(head->left);
			inorder(head->right);
		}
		bool equals(Dictionary<Key, Info>::Node* head, Dictionary<Key, Info>::Node* other)
		{
			if(head==other) return true;
			if(!head||!other) return false;
			return (head->key==other->key)&&(head->info==other->info)&&equals(head->left, other->left)&&equals(head->right, other->right);
		}
		void insertion(Node* head)
		{
			if(!head) return;
			insert(head->key, head->info);
			balance();
			insertion(head->left);
			insertion(head->right);
		}
		void clear(Node* &head)
		{
			if(!head->left&&!head->right)
			{
				Node* nodeptr=head;
				head=NULL;
				delete nodeptr;
				return;
			}
			if(head->left) clear(head->left);
			if(head->right) clear(head->right);
			clear(head);
		}
		void search(Info i, Node* head, int &j)
		{
			if(!head) return;
			if(head->info==i) j++;
			search(i, head->left, j);
			search(i, head->right, j);
		}
		void max(Node* head, Info &i)
		{
			if(!head) return;
			if(head->info>i) i=head->info;
			max(head->left, i);
			max(head->right, i);
		}
		void min(Node* head, Info &i)
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
				Node* curr, *start;			//current state
			public:
				Iterator() {curr=NULL; start=NULL;}			//default constructor
				Iterator(Dictionary<Key, Info> dict){curr=dict.root; start=curr;}	//construct and assign to object
				~Iterator(){curr=NULL; start=NULL;}
				Iterator operator++(){if(curr) curr=curr->right; return *this;}	//next element
				Iterator operator--(){if(curr) curr=curr->left; return *this;}	//previous element
				Key operator!(){if(curr) return curr->key; else return 0;}	//access key
				Info operator*(){if(curr) return curr->info; else return 0;}	//access info
				void reset(){curr=start;}	//back to origin
				bool finished(){if((!curr->left&&!curr->right)||(!curr)) return true; return false;}	//nowhere else to go
		};
		Dictionary()
		{
			root=NULL;	
		}
		Dictionary(Dictionary<Key, Info> &dict)
		{
			root=NULL;
			if(dict.isempty()) return;
			insertion(dict.root);
		}	
		Dictionary(Key arrk[], Info arri[], int i)
		{
			if(i<=0)
			{
				root=NULL;
				return;
			}
			root=new Node(arrk[0], arri[0]);
			for(int j=1; j<i; j++)
			{
				insert(root, arrk[j], arri[j]);
				balance();
			}
		}
		Dictionary<Key, Info> operator= (Dictionary<Key, Info> dict)
		{
			if(dict==*this) return *this;
			if(dict.isempty())
			{
				root=NULL;
				return *this;
			}
			clear();
			insertion(dict.root);
			return *this;
		}
		bool operator== (Dictionary<Key, Info> dict)
		{
			if(isempty()) return dict.isempty();
			if(height()!=dict.height()) return false;
			equals(root, dict.root);
		}
		void remove(Key k)
		{
			if(isempty()) return;
			remove(k, root);
			balance();
		}
		bool insert(Key k, Info i)
		{
			if(isempty())
			{
				root=new Node(k, i);
				return true;
			}
			bool b=insert(root, k, i);
			balance();
			return b;
		}
		int height()
		{
			if(isempty()) return 0;
			return height(root);
		}
		void printree()
		{
			if(isempty()) return;
			int h=height(root);
			int space;
			for(int i=1; i<=h; i++)
			{
				space=1;
				for(int j=0; j<=(h-i); space*=2, j++);
				printree(root, i-1, space-1);
				cout<<endl<<"--------------------------------------------------------------------------------------------------------------------------------"<<endl;
			}
		}
		void clear()
		{
			if(isempty()) return;
			clear(root);
		}
		bool seek(Key k)
		{
			if(isempty()) return false;
			Iterator it(*this);
			while(!it.finished())
			{
				cout<<!it<<endl;
				if(!it>k) --it;
				else if(!it<k) ++it;
				else return true;
			}
			if(!it==k) return true;
			return false;
		}
		int search(Info i)
		{
			if(isempty()) return 0;
			int j=0;
			search(i, root, j);
			return j;
		}
		Info furthest()
		{
			if(isempty()) return 0;
			return furthest(root, height(root)-1);
		}
		Info avgfurthest()
		{
			if(isempty()) return 0;
			Info sum=0;
			int num=0;
			avgfurthest(root, height(root)-1, sum, num);
			return sum/num;
		}
		Key rightmost()
		{
			if(isempty()) return 0;
			Node* nodeptr;
			for(nodeptr=root; nodeptr->right; nodeptr=nodeptr->right);
			return nodeptr->key;
		}
		Key leftmost()
		{
			if(isempty()) return 0;
			Node* nodeptr;
			for(nodeptr=root; nodeptr->left; nodeptr=nodeptr->left);
			return nodeptr->key;
		}
		Info max()
		{
			Info i=root->info;
			max(root, i);
			return i;
		}
		Info min()
		{
			Info i=root->info;
			min(root, i);
			return i;
		}
		bool isempty(){return !root;}
		Info rootinf(){if(isempty()) return 0; return root->info;}
		Key rootkey(){if(isempty()) return 0; return root->key;}
		void inorder(){inorder(root);}
		void postorder(){postorder(root);}
		void preorder(){preorder(root);}
};

int main()
{
	int arr1[]={18,19,82,84,22,29,91,21,11,12,69,54,32,11,23,65,43,12,33};
	int arr2[]={ 8, 9, 2, 3, 2, 2, 1, 1, 1,12,69, 4,32,11, 3, 6,43, 2,33,2,22,1,2,1,24,6,87,54,2,1,1,23,34,5,45,563,54,34,5};
	Dictionary<int,int> dict1(arr1, arr2, 18);
	Dictionary<int,int> dict3(arr1, arr2, 18);
	cout<<"tri rdy"<<endl;
	dict1.insert(99, 7);
	dict1.insert(83, 6);
	dict1.insert(74, 6);
	dict1.printree();
	cout<<"_________________________________________________________________"<<endl;
	dict1.printree();
	Dictionary<int,int> dict2(dict1);
	cout<<"_________________________________________________________________"<<endl;
	dict2.printree();
	cout<<"_________________________________________________________________"<<endl;
	dict1.printree();
	dict2=dict1;
	cout<<(dict1==dict2)<<endl;
	dict2.printree();
	cout<<dict2.max()<<" "<<dict2.search(1)<<" "<<dict2.seek(99)<<endl;
	dict2.inorder();
}
