// 使い方: [java] TestLL 整数の並び

// 単純な例題プログラム; コマンドラインから整数列を読み出し、
// 昇順を維持して連結リストに保持する。
// 最終的なリストを画面に表示する。

public class TestLL
{
    public static void main(String[] args) {
        int numElements = args.length;
        LinkedList list = new LinkedList();
        for (int i = 1; i <= numElements; i++) {
            int num;
            // Cの"atoi()"を parseInt()を用いて実行する。
            num = Integer.parseInt(args[i-1]);
            Node nn = new Node(num);
            list.insert(nn);
        }
        System.out.println("final sorted list:");
        list.printList();
    }
}
