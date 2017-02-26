// bintree.cc: 二分木に挿入やソートした表示をするルーチン群
#include <iostream.h>
class Node {
public:
    static Node *root; // 木全体の根
    int val; // 保存する値
    Node *left; // 小さい子要素へのポインタ
    Node *right; // 大きい子要素へのポインタ
    Node(int x); // コンストラクタ。val = xを設定
    static void insert(int x); // 木にxを挿入
    static void printtree(Node *nptr); // nptrで指定された部分木を表示
};

Node *Node::root = 0;

Node::Node(int x)
{
    val = x;
    left = right = 0;
}

void Node::insert(int x)
{
    if (Node::root == 0) {
        Node::root = new Node(x);
        return;
    }
    
    Node *tmp = root;
    while (1)
    {
        if (x < tmp->val)
        {
            if (tmp->left != 0) {
                tmp = tmp->left;
            } else {
                tmp->left = new Node(x);
                break;
            }
        } else {
            if (tmp->right != 0) {
                tmp = tmp->right;
            } else {
                tmp->right = new Node(x);
                break;
            }
        }
    }
}

void Node::printtree(Node *np)
{
    if (np == 0) return;
    Node::printtree(np->left);
    cout << np->val << endl;
    Node::printtree(np->right);
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
        Node::insert(atoi(argv[i]));
    Node::printtree(Node::root);
}

