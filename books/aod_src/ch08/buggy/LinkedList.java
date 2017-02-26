// LinkedList.java, 整列した整数の連結リストの実装

public class LinkedList
{
    public Node head = null;

    public LinkedList() {
        head = null;
    }

    // ノードnをリストに追加する
    public void insert(Node n) {
        if (head == null) {
            head = n;
            return;
        }
        if (n.value < head.value) {
            n.next = head;
            head = n;
            return;
        }
        if (head.next == null) {
            head.next = n;
            return;
        }
        for (Node d = head; d.next != null; d = d.next) {
            if (n.value < d.next.value) {
                n.next = d.next;
                d.next = n;
                return;
            }
        }
    }

    public void printList() {
        if (head == null) return;
        for (Node d = head; d != null; d = d.next)
            System.out.println(d.value);
    }
}
