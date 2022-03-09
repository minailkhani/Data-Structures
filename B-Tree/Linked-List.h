#include <iostream>
#include <bits/stdc++.h>
using namespace std;

template <typename type>
class LinkedListNode
{
public:
    LinkedListNode<type> *next = NULL, *prev = NULL ;
    type data ;
};
template <typename type>
class LinkedList
{
typedef LinkedListNode<type> ListNode ;
public:
	void push_back(type val)
    {
        if(is_empty())
        {
            head = new ListNode ;
            head->data = val ;
            last = head ;
        }
        else
        {
            ListNode *temp = new ListNode ;
            temp->data = val ;
            last->next = temp ;
            temp->prev = last ;
            temp->next = NULL ;
            last = temp ;
        }
		len++ ;
    }
    void push_front (type &val)
    {
        if (is_empty())
        {
            head = new ListNode ;
            head->data = val ;
            last = head ;
        }
        else 
        {
            ListNode *temp = new ListNode ;
            temp->data = val ;
            temp->prev = NULL ;
            temp->next = head ;
            head->prev = temp ;
            head = temp ;
        }
        len++ ;
    }
	void push_using_index(type val, int &index)
    {
        if(index == 0)
            push_front(val) ;
        else if(index == len)
            push_back(val) ;
        else if(index > len)
            cout<<"not found!"<<endl ;
        else
        {
            auto newNode = new ListNode ;
            auto temp = get_node_using_index(index) ;
            newNode->data = val ;
            newNode->prev = temp->prev ;
            newNode->next = temp ;
            temp->prev = newNode ;
            newNode->prev->next = newNode ;
            len++ ;
        }
    }
    void push_after(ListNode *curr, type NewData)
    {
        if(last == curr)
            push_back(NewData) ;
        else
        {
            auto NewNode = new ListNode ;
            NewNode->data = NewData ;
            NewNode->next = curr->next ;
            NewNode->prev = curr ;
            NewNode->next->prev = NewNode ;
            curr->next = NewNode ;
            len++ ;
        }
    }
    ListNode* findd(type curr)
    {
        for(auto Node = head ; Node != NULL ; Node = Node->next )
            if(Node->data == curr)
                return Node ;
        return NULL ;
    }
    void pop_back()
    {
        if(!is_empty())
        {
            if(last->prev != NULL)
            {
                last = last->prev ;
                delete last->next ;
                last->next = NULL ;
		    }
            else
            {
                delete head ;
                head = last = NULL ;
            }
			len-- ;
        }
		else
			cout<<"is empty :/\n" ;
    }
    void pop_front()
    {
        if(!is_empty())
        {
            if(head == last)
            {
                delete head ;
                head = last = NULL ;
            }
            else
            {
                head = head->next ;
                delete head->prev ;
                head->prev = NULL ;
            }
                len-- ;
        }
    }
	void pop_using_index(int index)
    {
        if(index == 0)
            pop_front() ;
        else if(index == get_len()-1 )
            pop_back() ;
        else
        {
            auto curr = get_node_using_index(index) ;
            pop_using_LinkedListNode(curr) ;
        }
    }
    void pop_using_LinkedListNode(ListNode* temp)
    {
        if(temp == head)
            pop_front() ;
        else if(temp == last )
            pop_back() ;
        else
        {
            temp->prev->next = temp->next ;
            temp->next->prev = temp->prev ;
            delete temp ;
            len-- ;
        }
    }
      ListNode* operator[] (int index)
    {
        auto node = head ;
        for (int i = 0; i < index ; i++)
        {
            if(node == NULL)
                cout<<"false index"<<endl ;
            node = node->next;
        }
        return node;
    }
    int get_len()
	{
		return len ;
	}
	int get_index(type nodeToFind)
    {
        auto temp = head ;
        int index = 0 ;
        while(temp)
        {
            if(temp->data == nodeToFind)
                return index ;
            temp = temp->next ;
            index++ ;
        }
        cout<<"not found3"<<endl ;
        return -1 ;
    }
    bool is_empty()
    {
        return head == NULL ;
    }
    ListNode* get_node_using_index(int index)
    {
        auto curr = head ;
        for(int i = 0 ; i<index ; i++ )
        {
            if(curr == NULL)
                cout<<"false index"<<endl ;
            curr = curr->next ;        
        }
        return curr ;
    }
    ListNode* begin()
    {
        return head ;
    }
    ListNode* end()
    {
        return last ;
    }
    void display()
    {
        int i = 0 ;
        curr = head ;
        while(curr != NULL)
        {
            i++ ;
            cout<<curr->data<<" " ;
            curr = curr->next ;
        }
        cout<<endl ;
        if(i != len)
        cout<<"mistake"<<endl ;
    }
private:
	ListNode *head = NULL, *last = NULL, *curr = NULL ; 
    int len = 0 ;
};
