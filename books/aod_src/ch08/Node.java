// Node.java, 整列された整数の連結リストのノードクラス

public class Node
{
    int value;
    Node next; // 次の項目への"ポインタ"
    
    // コンストラクタ
    public Node(int v) {
        value = v;
        next = null;
    }
}
