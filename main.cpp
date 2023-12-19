#include <iostream>
using namespace std;

struct Interval{
    int start;
    int end;
    Interval()
    {
        start = -1;
        end = -1;
    }
    Interval(int start, int end)
    {
        this->start = start;
        this->end = end;
    }
};

ostream& operator <<(ostream & out, Interval interval){
    if (interval.start == INT_MAX && interval.end == INT_MAX)
    {
        out << "Interval doesn't overlap with any intervals" << '\n';
    }
    else
    {
        out << interval.start << " " << interval.end << '\n';
    }
    return out;
}

struct Node{
    Interval interval;
    int maxHighInSubtree;
    Node* left;
    Node* right;
    Node(){
        maxHighInSubtree = -1;
        left = nullptr;
        right = nullptr;
    }
};

class IntervalTree{
private:
    Node* root;
    int insert(Node *curr, Interval interval)
    {
        int insertedMex = 0;
        // the current interval is less than to the interval that we want to InsertInterval so, go to the curr-> right
        if( (interval.start > curr->interval.start) || (interval.start == curr->interval.start && interval.end > curr->interval.end))
        {
            if(curr->right == nullptr)
            {
                Node* node = new Node();
                node->interval = interval;
                node->maxHighInSubtree = interval.end;
                curr->right = node;
                insertedMex = node->maxHighInSubtree;
            }
            else insertedMex = insert(curr->right, interval);
        }
        // the current interval is greater than to the interval that we want to InsertInterval so, go to the curr-> left
        else if( (interval.start < curr->interval.start) || (interval.start == curr->interval.start && interval.end < curr->interval.end))
        {
            if(curr->left == nullptr)
            {
                Node* node = new Node();
                node->interval = interval;
                node->maxHighInSubtree = interval.end;
                curr->left = node;
                insertedMex = node->maxHighInSubtree;
            }
            else insertedMex = insert(curr->left, interval);
        }

        if(insertedMex > curr->maxHighInSubtree)
        {
            curr->maxHighInSubtree = insertedMex;
        }
        else
        {
            insertedMex = curr->maxHighInSubtree;
        }
        return insertedMex;
    }

public:
    IntervalTree()
    {
        root = nullptr;
    }

    void InsertInterval(Interval interval)
    {
        if(root == nullptr)
        {
            root = new Node();
            root->interval = interval;
            root->maxHighInSubtree = interval.end;
        }
        else insert(root, interval);
    }

    Interval SearchInterval(Interval query)
    {
        Node* curr = root;
        Interval minInterval(INT_MAX, INT_MAX);
        while (curr != nullptr)
        {
            // store the current interval if it overlaps the query interval and it is less than the low in the current minInterval
            if(query.start <= curr->interval.end && curr->interval.start <= query.end && minInterval.start > curr->interval.start){
                minInterval = curr->interval;
            }

            if(curr->left != nullptr && query.start <= curr->left->maxHighInSubtree)
            {
                curr = curr->left;
            }
            else curr = curr->right;
        }
        return minInterval;
    }

    void printDfs(Node* curr)
    {
        if(curr == nullptr)
            return;
        cout << curr->interval.start << " " << curr->interval.end << '\n';
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