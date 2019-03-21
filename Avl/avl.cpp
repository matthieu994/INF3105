#include <vector>
#include <iostream>
#include <algorithm>

template <class T>
struct AVLTreeNode
{
    AVLTreeNode *left;
    AVLTreeNode *right;

    const T value;
    int count; // how many nodes are there in this subtree
    int height;

    AVLTreeNode(T value);
    void updateValues();
    int balanceFactor();

    AVLTreeNode *left_rotate();
    AVLTreeNode *right_rotate();
};

template <class T>
AVLTreeNode<T>::AVLTreeNode(T value) : value(value)
{
    count = 1;
    height = 1;

    value = value;
    left = nullptr;
    right = nullptr;
}

template <class T>
void AVLTreeNode<T>::updateValues()
{
    count = (left != nullptr ? left->count : 0) + (right != nullptr ? right->count : 0) + 1;

    height = std::max(left != nullptr ? left->height : 0,
                      right != nullptr ? right->height : 0) +
             1;
}

template <class T>
int AVLTreeNode<T>::balanceFactor()
{
    return (left != nullptr ? left->height : 0) - (right != nullptr ? right->height : 0);
}

template <class T>
AVLTreeNode<T> *AVLTreeNode<T>::left_rotate()
{
    AVLTreeNode *R = right;
    right = right->left;
    R->left = this;

    this->updateValues(); // the order is important
    R->updateValues();

    return R;
}

template <class T>
AVLTreeNode<T> *AVLTreeNode<T>::right_rotate()
{
    AVLTreeNode *L = left;
    left = left->right;
    L->right = this;

    this->updateValues(); // the order is important
    L->updateValues();

    return L;
}

template <class T>
class AVLTree
{
    int _size;
    AVLTreeNode<T> *root;

    void balance(std::vector<AVLTreeNode<T> **> path);
    void display(AVLTreeNode<T> *cur, int depth = 0, int state = 0);

  public:
    AVLTree();
    ~AVLTree();

    void insert(T value);
    void erase(T value);

    void clear();
    bool empty() const;
    int size() const;

    int find(T value) const;
    const T &operator[](std::size_t idx) const;

    void display();
};

template <class T>
AVLTree<T>::AVLTree()
{
    root = nullptr;
    _size = 0;
}

template <class T>
AVLTree<T>::~AVLTree()
{
    clear();
}

template <class T>
void AVLTree<T>::clear()
{
    std::vector<AVLTreeNode<T> *> stack;

    if (root != nullptr)
        stack.push_back(root);

    while (!stack.empty())
    {
        AVLTreeNode<T> *node = stack.back();
        stack.pop_back();

        if (node->left != nullptr)
            stack.push_back(node->left);

        if (node->right != nullptr)
            stack.push_back(node->right);

        _size--;
        delete node;
    }

    root = nullptr;
}

template <class T>
void AVLTree<T>::insert(T value)
{
    AVLTreeNode<T> **indirect = &root;   // to generalize insertion
    std::vector<AVLTreeNode<T> **> path; // to update height values

    while (*indirect != nullptr)
    {
        path.push_back(indirect);

        if ((*indirect)->value > value)
            indirect = &((*indirect)->left);
        else
            indirect = &((*indirect)->right);
    }

    *indirect = new AVLTreeNode<T>(value);
    path.push_back(indirect);

    balance(path);
    _size++;
}

template <class T>
void AVLTree<T>::balance(std::vector<AVLTreeNode<T> **> path)
{ // starting from root
    std::reverse(path.begin(), path.end());

    for (auto indirect : path)
    {
        (*indirect)->updateValues();

        if ((*indirect)->balanceFactor() >= 2 && (*indirect)->left->balanceFactor() >= 1) // left - left
            *indirect = (*indirect)->right_rotate();

        else if ((*indirect)->balanceFactor() >= 2)
        { // left - right
            (*indirect)->left = (*indirect)->left->left_rotate();
            *indirect = (*indirect)->right_rotate();
        }

        else if ((*indirect)->balanceFactor() <= -2 && (*indirect)->right->balanceFactor() <= -1) // right - right
            *indirect = (*indirect)->left_rotate();

        else if ((*indirect)->balanceFactor() <= -2)
        { // right - left
            (*indirect)->right = ((*indirect)->right)->right_rotate();
            *indirect = (*indirect)->left_rotate();
        }
    }
}

template <class T>
bool AVLTree<T>::empty() const
{
    return _size == 0;
}

template <class T>
int AVLTree<T>::size() const
{
    return _size;
}

template <class T>
int AVLTree<T>::find(T value) const
{
    AVLTreeNode<T> *direct = root; // to generalize insertion
    int idx = 0;

    while (direct != nullptr && direct->value != value)
    {
        if (direct->value > value)
            direct = direct->left;
        else
        {
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }

    if (direct == nullptr)
        return -1;
    else
        return idx + (direct->left ? direct->left->count : 0);
}

template <class T>
const T &AVLTree<T>::operator[](std::size_t idx) const
{
    AVLTreeNode<T> *cur = root;
    int left = cur->left != nullptr ? cur->left->count : 0;

    while (left != idx)
    {
        if (left < idx)
        {
            idx -= left + 1;

            cur = cur->right;
            left = cur->left != nullptr ? cur->left->count : 0;
        }
        else
        {
            cur = cur->left;
            left = cur->left != nullptr ? cur->left->count : 0;
        }
    }

    return cur->value;
}

template <class T>
void AVLTree<T>::display()
{
    printf("\n");
    if (root != nullptr)
        display(root);
    else
        printf("Empty");
    printf("\n");
}

template <class T>
void AVLTree<T>::display(AVLTreeNode<T> *cur, int depth, int state)
{ // state: 1 -> left, 2 -> right , 0 -> root
    if (cur->right)
        display(cur->right, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("          ");

    if (state == 1) // left
        std::cout << "┌───────";
    else if (state == 2) // right
        std::cout << "└───────";

    std::cout << "[" << cur->value << "]-(" << cur->count << ", " << cur->height << ")" << std::endl;

    if (cur->left)
        display(cur->left, depth + 1, 2);
}

int main(int argc, const char *argv[])
{
    AVLTree<long long> tree;

    int size = 10;
    for (int i = 0; i < size; i++)
    {
        long long int x = rand() % 100;
        tree.insert(x);
    }

    for (int i = 0; i < tree.size(); i++)
        printf("%lld, ", tree[i]);

    tree.display();

    tree.clear();
    printf("%d\n", tree.size());
    return 0;
}