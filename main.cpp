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
     * recursive method to insert query into the tree and updating max value to the inserted
     * @param curr the current node that will change when we go on depth or back track
     * @param query the query that we want to insert
     */
    void insert(Node *curr, Interval query)
    {
        // the current interval is less than to the query interval that we want to InsertInterval so, go to the curr-> right or equal them compare by high value
        if((query.low > curr->interval.low) || (query.low == curr->interval.low && query.high > curr->interval.high))
        {
            // base case, if the right is null just insert in the current right
            if(curr->right == nullptr)
            {
                Node* node = new Node();
                node->interval = query;
                node->max = query.high;
                curr->right = node;
            }
            // go depth in the right if it is not null
            else insert(curr->right, query);
        }
        // the current interval is greater than to the query interval that we want to InsertInterval so, go to the curr-> left or equal them compare by high value
        else if((query.low < curr->interval.low) || (query.low == curr->interval.low && query.high < curr->interval.high))
        {
            // base case, if the left is null just insert in the current right
            if(curr->left == nullptr)
            {
                Node* node = new Node();
                node->interval = query;
                node->max = query.high;
                curr->left = node;
            }
            // if the left is not null go depth in the left
            else insert(curr->left, query);
        }

        // back track max updating

        // if the current node has right child let its max equal to maximum number between its max and its right child max
        if(curr->right != nullptr)
        {
            curr->max = max(curr->max, curr->right->max);
        }
        // if the current node has left child let its max equal to maximum number between its max and its left child max
        if(curr->left != nullptr)
        {
            curr->max = max(curr->max, curr->left->max);
        }
    }

    /**
     * this method removes all nodes from the tree from the leaves to the current node which is passed at the method call
     * @param curr the current node
     */
    void removeNodesFromLeafs(Node*& curr)
    {
        if(curr == nullptr) return;
        removeNodesFromLeafs(curr->right);
        removeNodesFromLeafs(curr->left);
        delete curr;
        curr = nullptr;
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
     * tree destructor to remove the nodes from the tree
     */
    ~IntervalTree()
    {
        removeNodesFromLeafs(root);
    }
    /**
     * The insertion function to insert a given query
     * @param query Interval to be inserted
     */
    void InsertInterval(Interval query)
    {
        // invalid interval
        if(query.high <= query.low || query.high < 0 || query.low < 0)
        {
            return;
        }

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

        // invalid interval
        if(query.high <= query.low || query.high < 0 || query.low < 0)
        {
            return minInterval;
        }

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

};

int main()
{
    IntervalTree iT1;
    iT1.InsertInterval(Interval(20, 25));
    iT1.InsertInterval(Interval(10, 30));
    iT1.InsertInterval(Interval(30, 50));
    iT1.InsertInterval(Interval(5, 10));
    iT1.InsertInterval(Interval(15, 50));
    iT1.InsertInterval(Interval(25, 40));
    iT1.InsertInterval(Interval(35, 40));
    iT1.InsertInterval(Interval(5, 25));
    iT1.InsertInterval(Interval(55, 70));

    cout << "Q01: " << iT1.SearchInterval(Interval(1, 5)); // 5 10
    cout << "Q02: " << iT1.SearchInterval(Interval(0, 4)); // NF
    cout << "Q03: " << iT1.SearchInterval(Interval(15, 100)); // 5 25
    cout << "Q04: " << iT1.SearchInterval(Interval(10, 40)); // 5 10
    cout << "Q05: " << iT1.SearchInterval(Interval(80, 100)); // NF
    cout << "Q06: " << iT1.SearchInterval(Interval(40, 60)); // 15 50
    cout << "Q07: " << iT1.SearchInterval(Interval(60, 80)); // 55 70
    cout << "Q08: " << iT1.SearchInterval(Interval(36, 40)); // 15 50
    cout << "Q09: " << iT1.SearchInterval(Interval(75, 80)); // NF
    cout << "Q10: " << iT1.SearchInterval(Interval(10, 20)); // 5 10
    cout << "Q11: " << iT1.SearchInterval(Interval(2, 3)); // NF
    cout << "Q12: " << iT1.SearchInterval(Interval(3, 5)); // 5 10
    cout << "Q13: " << iT1.SearchInterval(Interval(60, 85)); // 55 70
    cout << "Q14: " << iT1.SearchInterval(Interval(100, 150)); // NF
    cout << "Q15: " << iT1.SearchInterval(Interval(21, 25)); // 5 25

    cout << "------------------------------" << '\n';

    IntervalTree iT2;
    iT2.InsertInterval(Interval(300, 320));
    iT2.InsertInterval(Interval(310, 360));
    iT2.InsertInterval(Interval(390, 420));
    iT2.InsertInterval(Interval(350, 450));
    iT2.InsertInterval(Interval(180, 250));
    iT2.InsertInterval(Interval(130, 200));
    iT2.InsertInterval(Interval(250, 350));
    iT2.InsertInterval(Interval(100, 150));
    iT2.InsertInterval(Interval(170, 300));
    iT2.InsertInterval(Interval(200, 250));
    iT2.InsertInterval(Interval(270, 300));

    cout << "Q16: " << iT2.SearchInterval(Interval(100, 101));  // 100 150
    cout << "Q17: " << iT2.SearchInterval(Interval(12, 400));   // 100 150
    cout << "Q18: " << iT2.SearchInterval(Interval(12, 15));    // NF
    cout << "Q19: " << iT2.SearchInterval(Interval(190, 201));  // 130 200
    cout << "Q20: " << iT2.SearchInterval(Interval(362, 400));  // 350 450
    cout << "Q21: " << iT2.SearchInterval(Interval(161, 270));  // 130 200
    cout << "Q22: " << iT2.SearchInterval(Interval(414, 600));  // 350 450
    cout << "Q23: " << iT2.SearchInterval(Interval(-1, -1));    // NF
    cout << "Q24: " << iT2.SearchInterval(Interval(330, 335));  // 250 350
    cout << "Q25: " << iT2.SearchInterval(Interval(300, 301));  // 170 300
    cout << "Q26: " << iT2.SearchInterval(Interval(107, 110));  // 100 150
    cout << "Q27: " << iT2.SearchInterval(Interval(210, 230));  // 170 300
    cout << "Q28: " << iT2.SearchInterval(Interval(50, 100));   // 100 150
    cout << "Q29: " << iT2.SearchInterval(Interval(50, 99));    // NF
    cout << "Q30: " << iT2.SearchInterval(Interval(401, 450));  // 350 450
    cout << "Q31: " << iT2.SearchInterval(Interval(400, 200));  // NF
    cout << "Q32: " << iT2.SearchInterval(Interval(1000, 1001));  // NF
}