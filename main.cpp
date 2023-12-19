#include <iostream>
#include <queue>
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

struct Node{
    Interval interval;
    int mex;
    Node* left;
    Node* right;
    Node(){
        mex = -1;
        left = nullptr;
        right = nullptr;
    }
};

class IntervalTree{
    private:
        Node* root;
        int insertInterval(Node *curr, Interval interval)
        {
            int insertedMex = 0;
            // the current interval is less than to the interval that we want to insert so, go to the curr-> right
            if( (interval.start > curr->interval.start) || (interval.start == curr->interval.start && interval.end > curr->interval.end))
            {
                if(curr->right == nullptr)
                {
                    Node* node = new Node();
                    node->interval = interval;
                    node->mex = interval.end;
                    curr->right = node;
                    insertedMex = node->mex;
                }
                else insertedMex = insertInterval(curr->right, interval);
            }
            // the current interval is greater than to the interval that we want to insert so, go to the curr-> left
            else if( (interval.start < curr->interval.start) || (interval.start == curr->interval.start && interval.end < curr->interval.end))
            {
                if(curr->left == nullptr)
                {
                    Node* node = new Node();
                    node->interval = interval;
                    node->mex = interval.end;
                    curr->left = node;
                    insertedMex = node->mex;
                }
                else insertedMex = insertInterval(curr->left, interval);
            }

            if(insertedMex > curr->mex)
            {
                curr->mex = insertedMex;
            }
            return insertedMex;
        }

    public:
        IntervalTree()
        {
            root = nullptr;
        }

        void insert(Interval interval)
        {
            if(root == nullptr)
            {
                root = new Node();
                root->interval = interval;
                root->mex = interval.end;
            }
            else insertInterval(root, interval);
        }

        void bfs(){
            queue<Node*> q;
            q.push(root);
            while (!q.empty())
            {
                Node* curr = q.front();
                q.pop();
                cout << curr->interval.start << " " << curr->interval.end << '\n';

                if(curr->right != nullptr)
                    q.push(curr->right);

                if(curr->left != nullptr)
                    q.push(curr->left);
            }
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

int main() {
    IntervalTree intervalTree;
    intervalTree.insert(Interval(1,2));
    intervalTree.insert(Interval(2,3));
    intervalTree.insert(Interval(0,21));
    intervalTree.insert(Interval(3,21));
    intervalTree.dfs();
}