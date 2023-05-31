#include <iostream>
using namespace std;
/// Struct for the tree intervals
struct Interval
{
    int lo, hi;
    /// default constructor
    Interval(){}
    Interval(int lo, int hi)
    {
        this->lo = lo;
        this->hi = hi;
    }
};
/// class for interval tree nodes
class Node
{
public:
    ///Interval representing each node
    Interval *interval;
    ///Max endpoint in each node
    int max;
    ///pointers to the left and right children
    Node *left, *right;
    Node()
    {
        interval = nullptr;
        max = 0;
        left = nullptr;
        right = nullptr;
    }
    Node(Interval i)
    {
        interval = new Interval(i);
        //Assigns max to the hi variable in the interval
        this->max = i.hi;
        left = nullptr;
        right = nullptr;
    }
};
/// IntervalTree class has a root attribute of Node,
/// and have the functions delete(), insert(), and search() in the tree.
class IntervalTree
{
    Node *root;
public:
    IntervalTree()
    {
        root = nullptr;
    }
    /// search for all intervals that overlap with the given interval
    void searchAll(Node *node, Interval x)
    {
        //array to store the overlap intervals
        Interval *arr = new Interval[100];
        int index = 0;
        Interval *overlapInterval = search(node, x);
        while (overlapInterval != nullptr)
        {
            arr[index] = *overlapInterval;
            //delete the overlapped interval
            node = deleteNode(node, overlapInterval->lo);
            //then search again for another overlapped interval
            overlapInterval = search(node, x);
            index++;
        }
        if(index == 0)
        {
            cout << "The overlapped interval: Not found" << endl;
            return;
        }
        cout<< "The deleted intervals: ";
        for(int i = 0 ;i < index ; i++)
        {
            cout << "[" << arr[i].lo << "," << arr[i].hi <<"]";
            if(i < index-1)
            {
                cout << ", ";
            }
        }
        cout << endl;
        cout << "The tree after deletion: " << endl;
        //displays the tree after the deletion process
        displayTree(node);
        cout << endl;
        //insert the deleted nodes again after deleting the overlapped nodes
        cout << "The tree after inserting the deleted nodes: " << endl;
        for(int i = 0; i < index; i++)
        {
            node = insert(node, arr[i]);
        }
        root = node;
    }
    /// function to insert nodes in the interval tree
    Node* insert(Node *node, Interval i)
    {
        // Base case
        //the node is a leaf node
        if(node == nullptr)
        {
            //tree is empty
            if(root == nullptr)
            {
                // create new node, and assign the root to this new node
                root = new Node(i);
                return root;
            }
            //create a new node
            return new Node(i);
        }
        //If the lo of the interval(needs to be inserted) is less than the low
        //of current node interval then, we go left and re-check
        if(i.lo < node->interval->lo)
        {
            node->left = insert(node->left, i);
        }
        //If the lo of the current interval(needs to be inserted) is greater than the lo
        //of current node interval then, we go right and re-check
        else
        {
            node->right = insert(node->right, i);
        }
        // updates max in each node after insertion
        node->max = max(node->interval->hi, i.hi);
        if(node->left != nullptr)
        {
            //maximize between the max of the current node and the max of the left node.
            node->max = max(node->max, node->left->max);
        }
        if(node->right != nullptr)
        {
            //maximize between the max of the current node and the max of the right node.
            node->max = max(node->max, node->right->max);
        }
        return node;
    }
    //Function to print the tree inorder traversal
    void displayTree(Node *node)
    {
        if(node == nullptr)//tree is empty
        {
            return;
        }
        displayTree(node->left);
        cout << "[" << node->interval->lo << ", " << node->interval->hi << "]," << " max = " << node->max << endl;
        displayTree(node->right);
    }
    // boolean function to check if there is an overlap
    bool checkIntersection(Interval interval1, Interval interval2)
    {
        if(interval1.lo <= interval2.hi && interval2.lo <= interval1.hi)
        {
            return true;
        }
        return false;
    }
    // search function that returns an interval that overlaps with the given interval.
    Interval* search(Node* node, Interval i)
    {
        if (node == nullptr)//tree is empty
        {
            return nullptr;
        }

        Interval* temp = nullptr;
        // check if the interval has an intersection in the left subtree
        if(node->left != nullptr && i.lo <= node->left->max)
        {
            temp = search(node->left, i);
            if(temp != nullptr)  ///returns the overlapped interval
            {
                return temp;
            }
            if(checkIntersection(*node->interval, i))
            {
                return node->interval;  //returns the interval of the node that overlaps with given interval
            }
            return nullptr;  //if there is no overlap
        }

            // check if the interval has an intersection in the right subtree
        else
        {
            temp = search(node->right, i);
            if(temp != nullptr)
            {
                return temp;
            }
            if(checkIntersection(*node->interval, i))
            {
                return node->interval;
            }
            return nullptr;
        }
    }

    /// function to find the maximum node
    Node* getPredecessor(Node* node)
    {
        // Base case (returns the maximum node)
        if (node->right == nullptr)
        {
            return node;
        }

        Node* temp = getPredecessor(node->right);
        //updates the max of each node
        node->max = node->interval->hi;
        if(node->left != nullptr)
        {
            node->max = max(node->max, node->left->max);
        }
        if(node->right != nullptr)
        {
            node->max = max(node->max, node->right->max);
        }
        return temp;
    }


    ///function to delete nodes
    Node* deleteNode(Node* node, int key) //key is the lo of the interval
    {
        if (node == nullptr)//tree is empty
            return nullptr;

        // search for the key in the left subtree
        if (key < node->interval->lo)
            node->left = deleteNode(node->left, key);

            // search for the key in the right subtree
        else if (key > node->interval->lo)
            node->right =deleteNode(node->right, key);

        else
        {
            ///If the node has no children
            if(node->left == nullptr && node->right == nullptr)
            {
                delete node;
                return nullptr;
            }

                ///If the node has one child on the left
            else if(node->left != nullptr && node->right == nullptr)
            {
                //temp to points to the left subtree
                Node* temp = node->left;
                delete node;
                return temp;

            }

                ///If the node has one child on the right
            else if(node->left == nullptr && node->right != nullptr)
            {
                //temp to points to the right subtree
                Node* temp = node->right;
                delete node;
                return temp;
            }

                ///If the node has two children
            else
            {
                //predecessor pointer that maximum node to be copied
                Node* predecessor = getPredecessor(node->left);

                // assign the interval of the predecessor to the interval of the node that'll be deleted
                node->interval = predecessor->interval;
                node->left=	deleteNode(node->left, predecessor->interval->lo);
            }
        }

        // update the max of the nodes after deletion
        node->max = node->interval->hi;
        if(node->left != nullptr)
        {
            node->max = max(node->max, node->left->max);
        }

        if(node->right != nullptr)
        {
            node->max = max(node->max, node->right->max);
        }
        return node;
    }


    //interface to call the insert function to pass the interval only
    void insert(Interval i)
    {
        insert(root, i);
    }

    //interface to call the searchAll function to pass the interval only
    void searchAll(Interval x)
    {
        searchAll(root, x);
    }

    Node *getRoot()
    {
        return root;
    }

    //interface to call the displayTree function
    void displayTree()
    {
        displayTree(root);
    }

    //interface to call the insert function to pass the interval only
    void search(Interval i)
    {
        Interval* x = search(root, i);

        //if there is no overlap
        if(x == nullptr)
        {
            cout << "The overlapped interval: Not found"<< endl;
            return;
        }

        cout << "The overlapped interval: ";
        cout << "[" << x->lo << ", " << x->hi <<"]"<< endl;
    }

    //interface to the deleteNode class to pass the key only
    void deleteNode(int key)
    {
        root = deleteNode(root, key);
    }

    // function to delete all the tree (to be called in the destructor).
    void deleteAll(Node* node)
    {
        if (node)
        {
            deleteAll(node->left);
            deleteAll(node->right);
            delete node;
        }
    }

    //destructor to delete the tree after the program terminates
    ~IntervalTree()
    {
        deleteAll(root);
    }
};
int main()
{
    cout << "------TestCase 1------" << endl;
    IntervalTree T1;
    T1.insert(Interval(15,20));
    T1.insert(Interval(10,30));
    T1.insert(Interval(17,19));
    T1.insert(Interval(5,20));
    T1.insert(Interval(12,15));
    T1.insert(Interval(30,40));

    cout << "The interval tree is: " << endl;
    T1.displayTree();
    cout << endl;
    T1.searchAll(Interval(16,18));
    cout << "-------" << endl;
    T1.searchAll(Interval(6,7));
    T1.displayTree();//after inserting the deleted nodes
    cout << endl;

    cout << "--------------------------------------------------------" << endl;
    cout << endl;
    cout << "------TestCase 2------" << endl;

    IntervalTree T2;
    T2.insert(Interval(1,2));
    T2.insert(Interval(3,4));
    T2.insert(Interval(5,6));
    T2.insert(Interval(7,8));

    cout << "The interval tree is: " << endl;
    T2.displayTree();
    cout << endl;
    T2.searchAll(Interval(2,3));
    T2.displayTree();//after inserting the deleted nodes
    cout << endl;

    cout << "--------------------------------------------------------" << endl;

    cout << endl;

    cout << "------TestCase 3------" << endl;
    IntervalTree T3;
    T3.insert(Interval(7,8));
    T3.insert(Interval(6,7));
    T3.insert(Interval(5,6));
    T3.insert(Interval(4,5));
    T3.insert(Interval(3,4));
    T3.insert(Interval(2,3));
    T3.insert(Interval(1,2));
    cout << "The interval tree is: " << endl;
    T3.displayTree();
    cout << endl;
    T3.searchAll(Interval(1,10));
    T3.displayTree();//after inserting the deleted nodes
    cout << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << endl;
    cout << "------TestCase 4------" << endl;
    IntervalTree T4;
    T4.insert(Interval(1, 2));
    T4.insert(Interval(2, 3));
    T4.insert(Interval(3, 4));
    T4.insert(Interval(4, 5));
    T4.insert(Interval(5, 6));
    T4.insert(Interval(6, 7));
    T4.insert(Interval(7, 8));

    cout << "The interval tree is: " << endl;
    T4.displayTree();
    cout << endl;
    T4.searchAll(Interval(1, 10));
    T4.displayTree();//after inserting the deleted nodes
    cout << endl;
    T4.searchAll(Interval(0, 0));
    T4.displayTree();//after inserting the deleted nodes
    cout << endl;
    return 0;
}