#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <iostream>
#include <vector>

//create Node struct that represents each node in the tree
template <typename KeyType, typename ValueType>
struct Node
{
    Node(const KeyType& key, const ValueType& value)
    : m_rightChild(nullptr), m_leftChild(nullptr), m_key(key)
    {
       //constructor for a new node that is inserted
        m_values.push_back(value);
    }
    
    //tree's structure
    Node* m_rightChild;
    Node* m_leftChild;
    
    //data that acts as a multimap
    KeyType m_key;
    std::vector<ValueType> m_values;
};

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator() //default constructor
        {
            // Replace this line with correct code.
            m_ptrNode = nullptr;
            m_value = nullptr;
            m_size = 0;
            //m_ptrVecValues = nullptr;
        }
        
        Iterator(Node<KeyType, ValueType>& currNode) //points to currNode to process
        {
            //get size of the vector in the node
            m_size = currNode.m_values.size();
            m_ptrNode = &currNode;
            
            if(currNode.m_values.empty() == false)
            {
                m_value = &currNode.m_values[0];
            }
            else
                m_value = nullptr; //FIXME: I don't think this line of code will ever run?
            
            //m_ptrVecValues = currNode.m_values;
        }
        
        Iterator(TreeMultimap<KeyType, ValueType>::Iterator& it, std::vector<ValueType>*& p)
        {
            //constructor to assign an external pointer passed in to a pointer of vector ValueType
            if(this != &it)
            {
                m_ptrNode = it.m_ptrNode;
                m_value = it.m_value;
                m_size = it.m_size;
            }
            if(m_ptrNode != nullptr)
                p = &(m_ptrNode->m_values);
            else
                std::cerr << "Error" << std::endl;
        }

        ValueType& get_value() const
        {
            if(is_valid())
            {
                //m_size--;
                return *m_value;
            }
            else //FIXME: not sure if I need this.
            {
                std::cerr << "Error in getting val." << std::endl;
                return *m_value;
            }
        }

        bool is_valid() const
        {
            //check points to a node, that the node has a value, and that pointer doesn't point past vector
            if(m_ptrNode == nullptr || m_value == nullptr || m_size < 1)
                return false;
            else
                return true;
        }

        void advance()
        {
            // Replace this line with correct code.
            if(is_valid())
            {
                m_value++;
                m_size--; //decrement remaining size of the vector at the node
            }
        }

      private:
        
        
        Node<KeyType, ValueType>* m_ptrNode;
        ValueType* m_value; //can be nullptr. If so, means that no values at node being processed
        //std::vector<ValueType>* m_ptrVecValues;
        int m_size; //size of the vector. When size is 0, the vector contains no more items (invalid)
    };

    TreeMultimap()
    {
        m_head = nullptr;
    }

    ~TreeMultimap()
    {
        //recursion to delete all nodes in tree
        deleteNode(m_head);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        Node<KeyType, ValueType>* p = m_head;
        findNode(p, key);
        
        if(p == nullptr) //will only be true when tree is empty
        {
            //create a new node for head
            m_head = new Node(key, value);
        }
        else if(p->m_key == key)
        {
            //found matching key, so add to vector
            p->m_values.push_back(value);
        }
        //no matching key found so create new node and make appropriate child
        else if(key > p->m_key)
        {
            //create new node and initialize as right child
            p->m_rightChild = new Node(key, value);
        }
        else if (key < p->m_key)
        {
            //create new node and initialize as left child
            p->m_leftChild = new Node(key, value);
        }
        else
        {
            //error
            std::cerr << "Error finding/inserting node for key" << std::endl;
        }
            
    }

    Iterator find(const KeyType& key) const
    {
        Node<KeyType, ValueType>* p = m_head;
        while(true)
        {
            if(p == nullptr)
                return Iterator();
            if(p->m_key == key)
                return Iterator(*p);
            else if(key > p->m_key)
                p = p->m_rightChild;
            else
                p = p->m_leftChild;
        }
    }

  private:
    //find appropriate node
    //pass pointer by reference. After function check if pointer is nullptr or if matches key
    //will point to the node that either matches key or is leaf
    void findNode(Node<KeyType, ValueType>*& head, const KeyType& key)
    {
        //Node<KeyType, ValueType>* copy = head;
        //check if head valid
        if(head == nullptr)
            return;
        //check if found Node where key matches
        if(head->m_key == key)
            return; //head will be a nonnullptr
        //check if leaf node
        if(head->m_leftChild == nullptr && head->m_rightChild == nullptr)
            return; //head will be nonnullptr
        //if key is > node, findLeaf on right subtree
        else if(head->m_rightChild != nullptr && key > head->m_key)
        {
            head = head->m_rightChild;
            findNode(head, key);
            return;
        }
        //else findLeaf on left subtree
        else if(head->m_leftChild != nullptr && key < head->m_key)
        {
            head = head->m_leftChild;
            findNode(head, key);
            return;
        }
        else
        {
            //case: node with one child where key will become new second child
            return;
        }
    }
    
    //delete tree node function
    void deleteNode(Node<KeyType, ValueType>* currNode)
    {
        //check if currNode is nullptr
        if(currNode == nullptr) return;
        //call delete node on left child and right child
        deleteNode(currNode->m_leftChild);
        deleteNode(currNode->m_rightChild);
        //then delete this node
        delete currNode;
    }
    
    Node<KeyType, ValueType>* m_head; //head pointer for the tree
    
};

#endif // TREEMULTIMAP_INCLUDED
