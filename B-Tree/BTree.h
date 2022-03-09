#include "Linked-List.h"
template<typename midtype,typename type> 
class Triple
{
public:
    midtype middle ;
    type firstPart, secondPart;
};
template<typename type>
class BTreeNode
{
public:
	bool isLeaf = true ;
	LinkedList<type> keys ;
	LinkedList<BTreeNode<type>*> child ;
	BTreeNode<type> *parent = NULL ;
};
template<typename type>
class BTree
{
typedef LinkedList<type> KeyList ;
typedef LinkedList<BTreeNode<type>*> ChildrenList ;
public:	
    BTree()
	{
		maxDegree = 4 ; //it works for eny maxDegree
	} 
	void insert(type key)
	{
		if(root == NULL)
			insert_into_empty_tree(key) ;
		else
		{
			auto curr = find_leaf_where_new_key_insert(key) ;
			
			int suitableIndexForKey = get_index_of_suitable_child(curr,key) ;
			curr->keys.push_using_index(key,suitableIndexForKey) ;
			curr->child.push_back(NULL) ;
			balance_overflow(curr, key) ;
		}
	}
    void print (BTreeNode<type>* node)
    {
		if(node != NULL)
		{
			node->keys.display() ;
			if(node->isLeaf)
			{
                cout<<"is leaf"<<endl ;
				node->child.display() ;
			}
			else
			{
                cout<<"is not leaf"<<endl ;
            	for (auto child = node->child.begin(); child != NULL; child = child->next)
						print(child->data) ;		
			}
		}
    }
	BTreeNode<type> *get_root()
	{
		return root ;
	}
    BTreeNode<type> * get_min(BTreeNode<type> * subtreeRoot)
    {
		while(!subtreeRoot->isLeaf)
			subtreeRoot = subtreeRoot->child[0]->data ;

		return subtreeRoot ;
    }
    Triple<int, BTreeNode<type>*> search (BTreeNode<type>* const subtreeRoot, const type data)
    {
		int  i = 0 ;
		Triple<int, BTreeNode<type>*> pair ;
		for(auto node = subtreeRoot->keys.begin() ;node != NULL; node = node->next)
		{
			if(node->data == data)
			{
				pair.firstPart = subtreeRoot ;
				pair.middle = i ;
				return pair ;
			}
			if(node->data > data)
			{
				if(subtreeRoot->isLeaf)
				{
					cerr<<"not found1"<<endl ;
					exit(0) ;
				}
				return search(subtreeRoot->child[i]->data, data) ;
			}
			i++ ;
    	}
		if(subtreeRoot->isLeaf)
		{
			cerr<<"not found2"<<endl ;
			exit(0) ;
		}
		return search(subtreeRoot->child.end()->data, data ) ;
	}
	void deletion(const type& key)
    {
		auto includingNode = search (root,key) ;
		if(includingNode.firstPart->isLeaf)
        {
			if(includingNode.firstPart == root && root->keys.get_len() == 1 )
			{
				delete root ;
				root = new BTreeNode<type> ;
				root = NULL ;
				return ;
			}
            includingNode.firstPart->keys.pop_using_index(includingNode.middle ) ;
            includingNode.firstPart->child.pop_back();
			
			balance_underflow(includingNode.firstPart);
		}
        else
        {   
        	auto Successor = successor(includingNode);
				//firstpart: node 
				//middle: index of key in node
				//second part: null
			includingNode.firstPart->keys[includingNode.middle]->data = 
				Successor.firstPart->keys[Successor.middle]->data ;
            Successor.firstPart->keys.pop_using_index(Successor.middle) ;
			Successor.firstPart->child.pop_front() ;
        	balance_underflow (Successor.firstPart) ;
        }
	}
	bool is_empty() //dont call it while deletion and insertion
	{
		return root == NULL ;
	}
private:
	BTreeNode<type> *root = NULL ;
	int maxDegree ;
	void balance_underflow(BTreeNode<type>* node)
	{
		if(node->child.get_len() > maxDegree/2 ||  (node == root && root->keys.get_len() != 0)  )
			return ;
		else if (node == root) 
        {
            balance_underflow_root();
            return; 
        }
        auto brothersWhichCanLend = get_neighbors(node) ;
		
		if(brothersWhichCanLend.firstPart)
        	if (brothersWhichCanLend.firstPart->child.get_len() > (int)(maxDegree/2 + 1) )
			{
			    get_from_left_brother (node, brothersWhichCanLend.firstPart);
				return ;
			}
		if (brothersWhichCanLend.secondPart)
			if (brothersWhichCanLend.secondPart->child.get_len() > (int)(maxDegree/2 + 1) )
        	{    
				get_from_right_brother (node, brothersWhichCanLend.secondPart);
				return ;
        	}
		if (brothersWhichCanLend.firstPart)
		    merge (brothersWhichCanLend.firstPart, node ) ;
		else if (brothersWhichCanLend.secondPart)
			merge (node, brothersWhichCanLend.secondPart);		
	}
	void balance_underflow_root()
    {
        if (root->child.begin() == NULL)
        {
            delete root;
            root = new BTreeNode<type> ;
        }
		else
		{
        	root = root->child.begin()->data ;
        	delete root->parent; 
			root->parent = NULL;
    	}
	}
	void balance_overflow(BTreeNode<type>* curr, type key)
	{
		while(is_overflow(curr))
		{
			auto triple = split(curr) ;
			if(curr != root)
			{
				auto parent = curr->parent ;
				int indexOfMidInpatrent = get_index_of_suitable_child(curr->parent,key) ;
				fix_btreeNode_which_overflowed_and_is_not_root(triple, curr, indexOfMidInpatrent) ;
				delete curr ;
				curr = parent ;
				}
			else
			{
				fix_btreeNode_which_overflowed_and_is_root(triple) ;
				break;
			}
		}
	}
    void get_from_left_brother (BTreeNode<type>* node, BTreeNode<type>* leftBrother)
    {
	    int parentKeyIndex = node->parent->child.get_index(leftBrother) ;

        auto brotherMovingChild = leftBrother->child.end()->data ;
        leftBrother->child.pop_back() ;
        auto brotherMovingKey = leftBrother->keys.end()->data ;
        leftBrother->keys.pop_back();
		if (brotherMovingChild != NULL)
            brotherMovingChild->parent = node ;
        node->child.push_front(brotherMovingChild) ;
        node->keys.push_front(node->parent->keys[parentKeyIndex]->data);
        node->parent->keys[parentKeyIndex]->data = brotherMovingKey ;
	}
    void get_from_right_brother (BTreeNode<type>* node, BTreeNode<type>* rightBrother)
    {
        int parentKeyIndex = node->parent->child.get_index(node) ;
        
		auto brotherMovingChild = rightBrother->child.begin()->data ;
        rightBrother->child.pop_front();
        auto brotherMovingKey = rightBrother->keys.begin()->data ;
        rightBrother->keys.pop_front();
        if (brotherMovingChild != NULL)
            brotherMovingChild->parent = node;
        node->child.push_back(brotherMovingChild) ;
        node->keys.push_back(node->parent->keys[parentKeyIndex]->data) ;
        node->parent->keys[parentKeyIndex]->data = brotherMovingKey ;
    }
 	void merge (BTreeNode<type>* node, BTreeNode<type>* rightBrother) 
    {
		int indexParentKeyNodeDownComing = node->parent->child.get_index(node) ;
		auto parentKeyNodeDownComing = 
                node->parent->keys[indexParentKeyNodeDownComing]->data ;
		node->keys.push_back(parentKeyNodeDownComing) ; 
		node->parent->keys.pop_using_index(indexParentKeyNodeDownComing) ;
		for(auto rightKey = rightBrother->keys[0] ; rightKey != NULL ;rightKey = rightKey->next)
			node->keys.push_back(rightKey->data) ;
		for(auto rightChild = rightBrother->child[0] ; rightChild != NULL ; rightChild = rightChild->next)
		{
			node->child.push_back(rightChild->data) ;
        }
		if (rightBrother->child.begin()->data != NULL)
			for (auto left = rightBrother->child.begin(); left != NULL; left = left->next)
            	left->data->parent = node ;
		node->parent->child.pop_using_LinkedListNode(node->parent->child.findd(rightBrother)) ; 

		balance_underflow(node->parent) ;
	}
	Triple<char,BTreeNode<type>*> get_neighbors(BTreeNode<type>* node)
	{
		Triple<char,BTreeNode<type>*> pair ;
		if(node->parent->child.findd(node)->prev)
			pair.firstPart = node->parent->child.findd(node)->prev->data ;
		else
			pair.firstPart = NULL ;
		if(node->parent->child.findd(node)->next)
			pair.secondPart = node->parent->child.findd(node)->next->data ;
		else
			pair.secondPart = NULL ;
		return pair ;
	}
	Triple<int, BTreeNode<type>*> successor ( Triple<int, BTreeNode<type>*> node)
    {
		//firstpart: node 
		//middle: index of key in node
		//second part: null
		Triple<int, BTreeNode<type>*> Successor ;
		Successor.firstPart = get_min(node.firstPart->child[node.middle + 1]->data) ;
		Successor.middle = 0 ;
		return Successor ;
    }
	BTreeNode<type>* find_leaf_where_new_key_insert(type key)
	{
		BTreeNode<type> *curr = root ;
		while(!curr->isLeaf)
		{
			int childIndex = get_index_of_suitable_child(curr,key) ;
			curr = curr->child.get_node_using_index(childIndex)->data ;
		}
		return curr ;
	}
	void insert_into_empty_tree(type key)
	{
		root = new BTreeNode<type> ;
		root->keys.push_back(key) ;
		root->child.push_back(NULL) ;
		root->child.push_back(NULL) ;
		root->parent = NULL ;
	}
	void fix_btreeNode_which_overflowed_and_is_not_root(Triple<type, BTreeNode<type>*> triple,BTreeNode<type>* curr , int indexOfMidInpatrent)
	{
		curr->parent->keys.push_using_index(triple.middle,indexOfMidInpatrent) ;
		auto currNode = curr->parent->child.findd(curr) ;
		curr->parent->child.push_after(currNode,triple.firstPart) ;
		curr->parent->child.pop_using_LinkedListNode(currNode) ;
		
		auto N = curr->parent->child.findd(triple.firstPart) ;
		curr->parent->child.push_after(N, triple.secondPart) ;
	}
	void fix_btreeNode_which_overflowed_and_is_root(Triple<type, BTreeNode<type>*> triple)
	{
		KeyList rootJustBiuldKeys ;
		rootJustBiuldKeys.push_back(triple.middle) ;

		ChildrenList rootJustBiuldChild ;
		rootJustBiuldChild.push_back(triple.firstPart) ;
		rootJustBiuldChild.push_back(triple.secondPart) ;
		root = build_BTreeNode(false, rootJustBiuldKeys, rootJustBiuldChild, NULL) ;
		root->child[0]->data->parent = root ;
		root->child[1]->data->parent = root ;
	}
	BTreeNode<type>* build_BTreeNode(bool isLeaf, KeyList keys, ChildrenList child, BTreeNode<type> *parent )
	{
		BTreeNode<type>* bTreeNode = new BTreeNode<type> ;
		bTreeNode->isLeaf = isLeaf ;
		bTreeNode->child = child ;
		bTreeNode->parent = parent ;
		bTreeNode->keys = keys ;
	
		return bTreeNode ;
	}
	bool is_overflow(BTreeNode<type>* curr)
	{
		return curr->keys.get_len() > maxDegree ;
	}
	Triple<type, BTreeNode<type>*> split(BTreeNode<type> *curr)
	{
		int midIndex = maxDegree/2 ;
		Triple<type, BTreeNode<type>*> triple ;
		triple.firstPart = new BTreeNode<type> ;
		triple.secondPart = new BTreeNode<type> ;
		int i = 0 ;
		//key:
		for(i ; i<midIndex ; i++)
			triple.firstPart->keys.push_back(curr->keys[i]->data) ;
		triple.middle = curr->keys[i++]->data ;
		while (i<curr->child.get_len()-1 )
			triple.secondPart->keys.push_back(curr->keys[i++]->data) ;

		//children:
		for(i = 0 ; i < midIndex+1 ; i++)
		{
			triple.firstPart->child.push_back(curr->child[i]->data) ;
				if(!curr->isLeaf)
					triple.firstPart->child[i]->data->parent = triple.firstPart ;
		}
		
		while (i<curr->child.get_len() )
		{
			triple.secondPart->child.push_back(curr->child[i]->data) ;
				if(!curr->isLeaf)
					curr->child[i]->data->parent = triple.secondPart ;
			i++ ;
		}
		triple.firstPart->parent = triple.secondPart->parent = curr->parent ;
		triple.firstPart->isLeaf = triple.secondPart->isLeaf = curr->isLeaf ;
	
		return triple ;
	}
	int get_index_of_suitable_child(BTreeNode<type> *curr, type &key)
	{
		if(curr == NULL)
			return 0 ;
		LinkedListNode<type>* temp = curr->keys.begin() ;
		int index = 0 ;
		while(temp->data < key)
		{
			temp = temp->next ;
			index++ ;
			if(temp == NULL )
				return index ;
		}
		return index ; 
	}
};
