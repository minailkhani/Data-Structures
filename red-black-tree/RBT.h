#include <iostream>
#include <string>
using namespace std;
class Node
{
public:
    int data ;
    Node *rightChild = NULL , *leftChild = NULL, *parent = NULL;
    char color ;
};
class RedBlackTree
{
public:
     void insert(int &data) 
     {
          numOfNode++ ;
          Node *rootHelp = root, *curr = new Node ;
          curr->data=data;
          curr->leftChild=NULL;
          curr->rightChild=NULL;
          curr->color='R';
          if(is_empty())
          {
              root=curr;
              curr->parent=NULL;
              curr->color = 'B' ;
          }
          else
          {
               Node*temp = NULL ;
               while(rootHelp!=NULL)
               {
                    temp=rootHelp;
                    if(rootHelp->data<curr->data)
                         rootHelp=rootHelp->rightChild;
                    else
                         rootHelp=rootHelp->leftChild;
               }
               curr->parent=temp;
               if(temp->data<curr->data)
                   temp->rightChild=curr;
               else
                   temp->leftChild=curr;
          }
          validate_insert(curr) ; 
     }
     bool deletion(int &data)
     {
          Node* curr = pre_search(data) ;
          if(curr == NULL)
          {
               cout<<"INVALID\n" ;
               return false ;
          }
          numOfNode-- ;
          Node *currAfterDeletion ;
          char primaryColor = curr->color ;
          if(curr->leftChild == NULL) //it can be leaf
          {
               currAfterDeletion = curr->rightChild ;
               delete_condistion1(curr, curr->rightChild) ;
          }
          else if(curr->rightChild == NULL)
          {
               currAfterDeletion = curr->leftChild ;
               delete_condistion1(curr, curr->leftChild) ;
          }
          else
          {
               Node* minNodeOfRightChild = get_min_Node_in_sub_tree(curr->rightChild) ;
               primaryColor = minNodeOfRightChild->color ;
               currAfterDeletion = minNodeOfRightChild->rightChild;
			if (minNodeOfRightChild->parent == curr) 
				currAfterDeletion->parent = minNodeOfRightChild;
                else 
               {
				delete_condistion1(minNodeOfRightChild, minNodeOfRightChild->rightChild);
				minNodeOfRightChild->rightChild = curr->rightChild;
				minNodeOfRightChild->rightChild->parent = minNodeOfRightChild;
			}

			delete_condistion1(curr, minNodeOfRightChild);
			minNodeOfRightChild->leftChild = curr->leftChild;
			minNodeOfRightChild->leftChild->parent = minNodeOfRightChild;
			minNodeOfRightChild->color = curr->color;
          }

          if (primaryColor == 'B')
          {
               Node* sibling ;
               if(currAfterDeletion != NULL )
               {
                    while (currAfterDeletion != root && currAfterDeletion->color == 'B') 
                    {
                         if (currAfterDeletion == currAfterDeletion->parent->leftChild) 
                         {
                              sibling = currAfterDeletion->parent->rightChild;
                              if (sibling->color == 'R')
                              {
                                   sibling->color = 'B';
                                   currAfterDeletion->parent->color = 'R';
                                   left_rotation(currAfterDeletion->parent);
                                   sibling = currAfterDeletion->parent->rightChild;
                              }

                              if (sibling->leftChild->color == 'B' && sibling->rightChild->color == 'B') 
                              {
                                   sibling->color = 'R';
                                   currAfterDeletion = currAfterDeletion->parent;
                              }
                              else 
                              {
                                   if (sibling->rightChild->color == 'B') 
                                   {
                                        sibling->leftChild->color = 'B';
                                        sibling->color = 'R';
                                        right_rotation(sibling);
                                        sibling = currAfterDeletion->parent->rightChild;
                                   } 
                                   sibling->color = currAfterDeletion->parent->color;
                                   currAfterDeletion->parent->color = 'B';
                                   sibling->rightChild->color = 'B';
                                   left_rotation(currAfterDeletion->parent);
                                   currAfterDeletion = root;
                              }
                         } 
                         else 
                         {
                              sibling = currAfterDeletion->parent->leftChild;
                              if (sibling->color == 'R') 
                              {
                                   sibling->color = 'B';
                                   currAfterDeletion->parent->color = 'R';
                                   right_rotation(currAfterDeletion->parent);
                                   sibling = currAfterDeletion->parent->leftChild;
                              }

                              if (sibling->rightChild->color == 'B' && sibling->rightChild->color == 'B') 
                              {
                                   sibling->color = 'R';
                                   currAfterDeletion = currAfterDeletion->parent;
                              }
                              else 
                              {
                                   if (sibling->leftChild->color == 'B') 
                                   {
                                        sibling->rightChild->color = 'B';
                                        sibling->color = 'R';
                                        left_rotation(sibling);
                                        sibling = currAfterDeletion->parent->leftChild;
                                   } 
                                   sibling->color = currAfterDeletion->parent->color;
                                   currAfterDeletion->parent->color = 'B';
                                   sibling->leftChild->color = 'B';
                                   right_rotation(currAfterDeletion->parent);
                                   currAfterDeletion = root;
                              }
                    } 
                    }
                    currAfterDeletion->color = 'B';
               }
          
          }
               return true ;
     }
     void BST(int searched = -100000)
     {
          int height = maxDepth(root)+2 ;
          string *arr = new string[height] ;
          for (int i = 0; i < height ; i++)
          {    
		     arr[i] = print_given_level(root, i, searched) ; 
               arr[i]+="\n" ;
          }
          for (int i = 1; i < height ; i++) 
          {
               int count = 0 ;
               cout<<arr[i] ;
               for(int j = 0 ; j<arr[i].length(); j ++)
               {
                    if(arr[i][j] == ' ')
                         if(arr[i][j-1] != 'N')
                         count++ ;
               }
               if(i<height-1)
                    
                    for(int k = 0 ; k<count ;k++)
                         cout<<"/"<<"\\   " ;
               cout<<endl ;
          }
     }
     void search(int &data)
     {
          Node *node = pre_search(data) ;
          if(node == NULL)
               cout<<"INVALID\n" ;
          else
               BST(data) ;
     }
private:
     int maxDepth(Node* node) 
     { 
     	if (node == NULL) 
     		return 0; 
     	else
     	{ 
     		int lDepth = maxDepth(node->leftChild); 
     		int rDepth = maxDepth(node->rightChild); 
     
     		if (lDepth > rDepth) 
     			return(lDepth + 1); 
     		else return(rDepth + 1); 
     	} 
     } 
     string print_given_level(Node* root, int level, int searched )
     {
          string ans = "" ;
          if (root == NULL)
		     return "N "; 
          if (level == 1)
          {
               ans=to_string(root->data) ;
               ans+=root->color ;
               if(searched == root->data)
                    ans+="*" ;
               ans+=" " ;
          }
          else if (level > 1) 
	     {               
	     	ans += print_given_level(root->leftChild, level-1, searched) ;
	     	ans += print_given_level(root->rightChild, level-1, searched) ;
          }
          return ans ;
     }
     void left_rotation(Node* curr) 
     {
		Node* child = curr->rightChild;
		curr->rightChild = child->leftChild;
		if (child->leftChild != NULL) 
			child->leftChild->parent = curr;
		child->parent = curr->parent;
		if (curr->parent == nullptr)
			this->root = child;
		else if (curr == curr->parent->leftChild)
			curr->parent->leftChild = child;
		else 
			curr->parent->rightChild = child;
		child->leftChild = curr;
		curr->parent = child;
	}
     void right_rotation(Node *curr)
     {
          Node* child = curr->leftChild;
		curr->leftChild = child->rightChild;
		if (child->rightChild != NULL)
			child->rightChild->parent = curr;
		child->parent = curr->parent;
		if (curr->parent == nullptr)
			this->root = child;
		 else if (curr == curr->parent->rightChild)
			curr->parent->rightChild = child;
		else
			curr->parent->leftChild = child;
		child->rightChild = curr;
		curr->parent = child;
     }
     int numOfNode = 0 ;
     Node* root = NULL ;
     Node* get_min_Node_in_sub_tree(Node*curr)
     {
          while (curr->leftChild != NULL)
			curr = curr->leftChild;
		return curr;
     }
     void delete_condistion1(Node *curr, Node *child)
     {
          if (curr->parent == NULL)
	     	root = child;
          else if (curr == curr->parent->leftChild)
	     	curr->parent->leftChild = child;
          else if(curr == curr->parent->rightChild)
	     	curr->parent->rightChild = child;
          
          if(child != NULL)
               child->parent = curr->parent;
     }
     bool is_empty()
    {
        return (root == NULL) ;
    }
     void validate_insert(Node* curr)
     {
          if(curr->parent == NULL)
          {    
               curr->color = 'B' ; 
               return ;
          }
          else if(curr->parent->color == 'B')
               return ;
          else
          {
               Node * uncle = get_uncle(curr) ;
               if(uncle == NULL)
               {
                    if (curr->parent == curr->parent->parent->leftChild && curr == curr->parent->leftChild)
                         left_left_rotation(curr) ;
                    else if(curr->parent == curr->parent->parent->leftChild && curr == curr->parent->rightChild)
                         left_right_rotation(curr) ;
                    else if(curr->parent == curr->parent->parent->rightChild && curr == curr->parent->rightChild)
                         right_right_rotation(curr) ;
                    
                    else if (curr->parent == curr->parent->parent->rightChild && curr == curr->parent->leftChild)
                         right_left_ratation(curr) ;
                    else
                         exit(0) ;
               }
               else if(uncle->color == 'B')
               {
                    if     (curr->parent == curr->parent->parent->leftChild && curr == curr->parent->leftChild)
                          left_left_rotation(curr) ;
                         else if(curr->parent == curr->parent->parent->leftChild && curr == curr->parent->rightChild)
                          left_right_rotation(curr) ;
                    else if(curr->parent == curr->parent->parent->rightChild && curr == curr->parent->rightChild)
                          right_right_rotation(curr) ;
                    
                    else if (curr->parent == curr->parent->parent->rightChild && curr == curr->parent->leftChild)
                          right_left_ratation(curr) ;

                    else
                         exit(0) ;
   
               }
               else if(uncle->color == 'R')
                    condition1_insert(curr, uncle) ;
          }
     }
     void right_left_ratation(Node*curr)
     {
          Node*parent = curr->parent, * grandParent = curr->parent->parent ;
          grandParent->rightChild = curr->leftChild ;  
          if(curr->leftChild != NULL)                  
               curr->leftChild->parent = grandParent ;
          parent->leftChild = curr->rightChild      ; 
          if(curr->rightChild!= NULL)
               curr->rightChild->parent = parent ;
          curr->leftChild = grandParent ;
          if(grandParent->parent != NULL)                   
          {                                                  
               if(grandParent->parent->rightChild == grandParent)
                    grandParent->parent->rightChild = curr ;
               else if(grandParent->parent->leftChild == grandParent)
                    grandParent->parent->leftChild = curr ;
          }                                                  
          else                         
          {                            
               curr->parent = NULL ;   
               root = curr ;
          }                            
          grandParent->parent = curr ; 
          curr->rightChild = parent         ; 
          parent->parent = curr ;
          curr->leftChild = grandParent   ;

          curr->color = 'B' ;
          grandParent->color = 'R' ;

     }
     void right_right_rotation(Node*curr)
     {
          Node*parent = curr->parent, * grandParent = curr->parent->parent ;
          if(grandParent->parent != NULL)
          {
               parent->parent = grandParent->parent ;
               if(grandParent == grandParent->parent->rightChild)
                    grandParent->parent->rightChild = parent ;
               else 
                    grandParent->parent->leftChild = parent ;
               grandParent->rightChild = parent->leftChild ;
               if(grandParent->rightChild != NULL)
                    grandParent->rightChild->parent = grandParent ;
          }
          else
          {
               grandParent->rightChild = parent->leftChild ;
               if(parent->leftChild != NULL)
                    parent->leftChild->parent = grandParent ;
               parent->parent = NULL ;
               root = parent ;
          }
          parent->leftChild = grandParent ;
          grandParent->parent = parent ;
          grandParent->color = 'R' ;
          parent->color = 'B' ;
          
     }
     void left_right_rotation(Node*curr)
     {
          Node*parent = curr->parent, * grandParent = curr->parent->parent ;
          grandParent->leftChild = curr->rightChild ;  
          if(curr->rightChild != NULL)                  
               curr->rightChild->parent = grandParent ;
          parent->rightChild = curr->leftChild      ; 
          if(curr->leftChild != NULL)
               curr->leftChild->parent = parent ;
          curr->rightChild = grandParent ;
          if(grandParent->parent != NULL)                   
          {                                                  
               if(grandParent->parent->leftChild == grandParent)
                    grandParent->parent->leftChild = curr ;
               else if(grandParent->parent->rightChild == grandParent)
                    grandParent->parent->rightChild = curr ;
          }                                                  
          else                         
          {                            
               curr->parent = NULL ;   
               root = curr ;
          }                            
          grandParent->parent = curr ; 
          curr->leftChild = parent         ; 
          parent->parent = curr ;
          curr->rightChild = grandParent   ;

          curr->color = 'B' ;
          grandParent->color = 'R' ;
     }
     void left_left_rotation(Node*curr)
     {
          Node*parent = curr->parent, * grandParent = curr->parent->parent ;
          if(grandParent->parent != NULL)
          {
               parent->parent = grandParent->parent ;
               if(grandParent == grandParent->parent->leftChild)
                    grandParent->parent->leftChild = parent ;
               else 
                    grandParent->parent->rightChild = parent ;
               grandParent->leftChild = parent->rightChild ;
               if(grandParent->leftChild != NULL)
                    grandParent->leftChild->parent = grandParent ;
          }
          else
          {
               grandParent->leftChild = parent->rightChild ;
               if(parent->rightChild != NULL)
                    parent->rightChild->parent = grandParent ;
               parent->parent = NULL ;
               root = parent ;
          }
               parent->rightChild = grandParent ;
               grandParent->parent = parent ;
               grandParent->color = 'R' ;
               parent->color = 'B' ;
     }
     Node * get_uncle(Node* curr)
     {
          if(curr->parent->parent == NULL)
               return NULL ;
          if(curr->parent->parent->leftChild == NULL)
               return NULL ;
          if(curr->parent->parent->rightChild == NULL)
               return NULL ;
          if(curr->parent->parent->leftChild == curr->parent)
               return curr->parent->parent->rightChild ;
          else if(curr->parent->parent->rightChild == curr->parent)
               return curr->parent->parent->leftChild ;
          else
          {
               cout<<"mistake in get_uncle\n" ;
               exit(0) ;
          }
     }
     void condition1_insert(Node *curr, Node *uncle)
     {
          uncle->color = 'B' ;
          curr->parent->color = 'B' ;
          curr->parent->parent->color = 'R' ;
          validate_insert(curr->parent->parent) ;
     }
     Node* pre_search(int &data)
     {
          Node * curr = root, *ansPtr = NULL ;
          while (curr != NULL && ansPtr == NULL)
          {
               if (curr->data == data)
				ansPtr = curr;
			if (curr->data < data)
				curr = curr->rightChild ;
			else 
				curr = curr->leftChild ;
          }
          return ansPtr ;
     }
};
