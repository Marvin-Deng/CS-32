#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED
#include <vector>
#include <iostream>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
private:
    struct Node {
        Node(const KeyType& key, const ValueType& value) {
            m_left = nullptr;
            m_right = nullptr;
            m_key = key;
            m_vals.push_back(value);
        }
        Node* m_left;
        Node* m_right;
        KeyType m_key;
        std::vector<ValueType> m_vals;
    };

public:
    class Iterator
    {
    public:
        Iterator()
        {
            m_iter = nullptr;
            m_idx = 0;
        }

        Iterator(Node* node)
        {
            m_iter = node;
            m_idx = 0;
        }

        ValueType& get_value() const
        {
            return m_iter->m_vals.at(m_idx);
        }

        bool is_valid() const
        {
            if (m_iter == nullptr || m_idx == m_iter->m_vals.size()) {
                return false;
            }
            return true;
        }

        void advance()
        {
            if (m_idx == m_iter->m_vals.size()) {
                m_idx = 0;
            }
            else {
                m_idx++;
            }
        }

    private:
        Node* m_iter;
        int m_idx;
        friend class Node;
    };

    Iterator find(const KeyType& key) const
    {
        Node* ptr = m_root;
        while (ptr != nullptr) {
            if (key == ptr->m_key) { // Key found
                return Iterator(ptr); // Return Iterator with valid node pointer
            }
            else if (key < ptr->m_key) { // Search left
                ptr = ptr->m_left;
            }
            else if (key > ptr->m_key) { // Search right
                ptr = ptr->m_right;
            }
        }
        Iterator invalid;
        return invalid; // Return invalid Iterator
    }

    TreeMultimap()
    {
        m_root = nullptr;
    }

    ~TreeMultimap()
    {
        cleanUp(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if (m_root == nullptr) {
            m_root = new Node(key, value);
            return;
        }
        Node* curr = m_root;
        bool done = false;
        while (!done) {
            if (curr->m_key == key) { // Duplicate value found
                curr->m_vals.push_back(value);
                done = true;
            }
            if (key < curr->m_key) { // Search left subtree
                if (curr->m_left != nullptr) {
                    curr = curr->m_left; // Continue searching
                }
                else {
                    curr->m_left = new Node(key, value); // Insert posiiton found
                    done = true;
                }
            }
            else if (key > curr->m_key) { // Search right subtree
                if (curr->m_right != nullptr) { // Continue searching
                    curr = curr->m_right;
                }
                else {
                    curr->m_right = new Node(key, value); // Insert posiiton found
                    done = true;
                }
            }
        }
    }
    private:
        void cleanUp(Node* curr) {
            if (curr == nullptr) { // Tree is empty
                return;
            }
            cleanUp(curr->m_left); // Delete left subtree
            cleanUp(curr->m_right); // Delete right subtree
            delete curr; // Delete node
        }
        Node* m_root;

};


#endif // TREEMULTIMAP_INCLUDED
