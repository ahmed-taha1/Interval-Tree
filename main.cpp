#include <iostream>
using namespace std;

/**
 * The interval struct that consists of a range starting at 'low' and ending at 'high'
 */
struct Interval{
    int low;
    int high;
    Interval()
    {
        low = -1;
        high = -1;
    }
    Interval(int start, int end)
    {
        this->low = start;
        this->high = end;
    }
};

/**
 * Output operator overloading for Interval structure
 * @param out The ostream
 * @param interval The interval to be printed
 * @return The ostream
 */
ostream& operator <<(ostream & out, Interval interval){
    if (interval.low == INT_MAX && interval.high == INT_MAX)
    {
        out << "Interval doesn't overlap with any intervals" << '\n';
    }
    else
    {
        out << interval.low << " " << interval.high << '\n';
    }
    return out;
}

/**
 * The tree node
 */
struct Node{
    Node* left;
    Node* right;
    // the interval in node
    Interval interval;
    // the max high value in the tree rooted at this node
    int max;

    Node()
    {
        max = -1;
        left = nullptr;
        right = nullptr;
    }
};

class IntervalTree{
private:
    // root of the interval tree
    Node* root;

    /**
     * recursive method to insert query into the tree
     * @param curr the current node that will change when we go on depth or back track
     * @param query the query that we want to insert
     * @return currMax high in the current back track calls to check if it greater than the current node it will update it's max
     * if not it will change to be the current node max till the root
     */
    int insert(Node *curr, Interval query)
    {
        int currMax = 0;
        // the current interval is less than to the query interval that we want to InsertInterval so, go to the curr-> right
        if((query.low > curr->interval.low) || (query.low == curr->interval.low && query.high > curr->interval.high))
        {
            // base case, if the right is null just insert in the current right
            if(curr->right == nullptr)
            {
                Node* node = new Node();
                node->interval = query;
                node->max = query.high;
                curr->right = node;
                // set the currMax to the query high
                currMax = query.high;
            }
            // go depth in the right if it is not null
            else currMax = insert(curr->right, query); // the currMax will take the max high value from the depth till here
        }
        // the current interval is greater than to the query interval that we want to InsertInterval so, go to the curr-> left
        else if((query.low < curr->interval.low) || (query.low == curr->interval.low && query.high < curr->interval.high))
        {
            // base case, if the left is null just insert in the current right
            if(curr->left == nullptr)
            {
                Node* node = new Node();
                node->interval = query;
                node->max = query.high;
                curr->left = node;
                // set the currMax to the query high
                currMax = query.high;
            }
            // if the left is not null go depth in the left
            else currMax = insert(curr->left, query);   // the currMax will take the max high value from the depth till here
        }
        // if the current max greater than the current node max update the current node max
        if(currMax > curr->max)
        {
            curr->max = currMax;
        }
        // if the current node has a max greater than the current max change the current max value for the back track to update parents
        else
        {
            currMax = curr->max;
        }
        return currMax;
    }

public:
    /**
     * Tree constructor
     */
    IntervalTree()
    {
        root = nullptr;
    }

    /**
     * The insertion function to insert a given query
     * @param query Interval to be inserted
     */
    void InsertInterval(Interval query)
    {
        // insert first query
        if(root == nullptr)
        {
            root = new Node();
            root->interval = query;
            root->max = query.high;
        }
        // insert other intervals
        else
        {
            insert(root, query);
        }
    }

    /**
     * The search function to search and return the interval with the minimum low that overlaps with the query interval
     * @param query The query interval
     * @return The interval with a minimum low value that overlaps with the query interval
     */
    Interval SearchInterval(Interval query)
    {
        Node* curr = root;

        // the interval to be updated and returned
        Interval minInterval(INT_MAX, INT_MAX);

        // apply iterative DFS along a path that matches the search overlapping conditions
        while (curr != nullptr)
        {
            // store the current interval if it overlaps the query interval and is less than the low in the current minInterval
            if(query.low <= curr->interval.high && curr->interval.low <= query.high && curr->interval.low < minInterval.low)
            {
                minInterval = curr->interval;
            }

            // choose the subtree to search through
            if(curr->left != nullptr && query.low <= curr->left->max)
            {
                curr = curr->left;
            }
            else
            {
                curr = curr->right;
            }
        }
        return minInterval;
    }

    void printDfs(Node* curr)
    {
        if(curr == nullptr)
            return;
        cout << curr->interval.low << " " << curr->interval.high << '\n';
        printDfs(curr->left);
        printDfs(curr->right);
    }
    void dfs()
    {
        printDfs(root);
    }
};

int main()
{
    IntervalTree intervalTree;
    intervalTree.InsertInterval(Interval(1, 2));
    intervalTree.InsertInterval(Interval(2, 3));
    intervalTree.InsertInterval(Interval(0, 21));
    intervalTree.InsertInterval(Interval(3, 21));
    intervalTree.InsertInterval(Interval(40, 100));
    intervalTree.dfs();
    cout << intervalTree.SearchInterval(Interval(101, 200));
}