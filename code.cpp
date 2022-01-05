// writen by spln
#include <bits/stdc++.h>
using namespace std;
struct HuffmanNode {
    int info; //存
    int index;
    int weight;
    int parent; //存
    int left;
    int right;
    char side; //存
    string BinaryCode;
    friend bool operator>(HuffmanNode f1, HuffmanNode f2) {
        return f1.weight > f2.weight;
    }
};
typedef HuffmanNode Node;
typedef HuffmanNode *Tree;
int ShowHelp() {
    cerr << "输入错误,请按要求进行输入：" << endl;
    cerr << "-z/-x 文件名1 文件名2" << endl;
    return 0;
}
class Compression {
  public:
    int CreatHuffmanTree(int tong[], int LeafNumber, Node HuffmanTree[]) {
        // TODO:创建哈夫曼树
        int k = 0;
        priority_queue<Node, vector<Node>, greater<Node>> pq;
        // Tree HuffmanTree = new Node[2 * LeafNumber - 1];
        // 0--->LeafNumber - 1            是叶节点
        // laefNumber--->2*LeafNumber - 2 是根节点
        for (int i = 0; i <= 256; i++) {
            if (tong[i]) {
                HuffmanTree[k].info = i;
                HuffmanTree[k].index = k;
                HuffmanTree[k].left = HuffmanTree[k].right =
                    HuffmanTree[k].parent = -1;
                HuffmanTree[k].weight = tong[i];
                pq.push(HuffmanTree[k]);
                k++;
            }
        }
        int j = LeafNumber;
        //通过优先队列构建哈夫曼树
        while (pq.size() > 1) {
            Node t1 = pq.top();
            pq.pop();
            Node t2 = pq.top();
            pq.pop();
            HuffmanTree[t1.index].parent = j;
            HuffmanTree[t2.index].parent = j;
            HuffmanTree[j].index = j;
            HuffmanTree[j].parent = -1;
            // cout<<HuffmanTree[j].index;
            HuffmanTree[j].left = t1.index;
            // cout<<HuffmanTree[j].left;
            HuffmanTree[j].right = t2.index;
            // cout<<HuffmanTree[j].right;
            HuffmanTree[j].weight = t1.weight + t2.weight;
            HuffmanTree[j].info = -127;
            pq.push(HuffmanTree[j]);
            j++;
        }
        j--;
        Node HuffmanTreeHead = pq.top();
        HuffmanTreeHead.parent = -127;
        HuffmanTree[j] = HuffmanTreeHead;
        HuffmanTree[j].info = -127;
        pq.pop();
        cerr << "Creat Huffman Tree Completed" << endl;
        return 0;
    }
    int GetCodeNode(Node HuffmanTree[], int LeafNumber) {
        for (int i = 0; i < 2 * LeafNumber - 2; i++) {
            if (HuffmanTree[i].info == -127)
                continue;
            int IndexForSearching = i;
            HuffmanTree[i].BinaryCode = "";
            int j = 0;
            while (HuffmanTree[IndexForSearching].parent != -127) {
                j = HuffmanTree[IndexForSearching].parent;
                if (HuffmanTree[j].left == IndexForSearching)
                    HuffmanTree[i].BinaryCode += '0';
                if (HuffmanTree[j].right == IndexForSearching)
                    HuffmanTree[i].BinaryCode += '1';
                IndexForSearching = j;
            }
            reverse(HuffmanTree[i].BinaryCode.begin(),
                    HuffmanTree[i].BinaryCode.end());
        }
        cerr << "Get Node Code Completed" << endl;
        return 0;
    }
    string Encode(string path1, Node HuffmanTree[], int LeafNumber) {
        ifstream instr(path1, ios::in | ios::binary);
        char ch;
        unsigned int bytebuff = 0;
        int bitmask = 0x80;
        string HuffmanPath = "";
        while (instr.get(ch)) {
            bytebuff = (int)(unsigned char)ch;
            int value = bytebuff;
            for (int i = 0; i < LeafNumber; i++) {
                if (HuffmanTree[i].info == value) {
                    HuffmanPath += HuffmanTree[i].BinaryCode;
                    break;
                }
            }
        }
        cerr << "Encode Completed" << endl;
        return HuffmanPath;
    }
    int ShowTable(Node HuffmanTree[], int LeafNumber) {
        for (int i = 0; i < 2 * LeafNumber - 1; i++) {
            cout << "i:" << i << endl;
            cout << HuffmanTree[i].index << "<-index" << endl;
            cout << HuffmanTree[i].info << "<-info" << endl;
            cout << HuffmanTree[i].side << "<-side" << endl;
            cout << HuffmanTree[i].left << "<-left" << endl;
            cout << HuffmanTree[i].right << "<-right" << endl;
            cout << HuffmanTree[i].parent << "<-parent" << endl;
            cout << HuffmanTree[i].weight << "<-weight" << endl;
            cout << HuffmanTree[i].BinaryCode << "<-code" << endl;
        }
        return 0;
    }
    int Count(string op, string path1, string path2, int tong[]) {
        // TODO:统计文件1字符出现频率
        // path1是源文件,path2是目标文件
        ifstream instr(path1, ios::in | ios::binary);
        unsigned int bytebuff = 0;
        char ch;
        while (instr.get(ch)) {
            bytebuff =(int)(unsigned char)ch;
            tong[bytebuff]++;
        } //统计weight完成！
        int LeafNumber = 0;
        for (int i = 0; i <= 256; i++) {
            if (tong[i] != 0)
                LeafNumber++;
        }
        instr.close();
        cerr << "Count Completed" << endl;
        return LeafNumber;
    }
    //把哈夫曼树存到FinalOutputString
    string SwitchStringToBinary(string HuffmanPath, int &Sign) {
        // TODO:将字符串里的01序列修改为bit
        // 最后一个字节要处理多余的0-->把0放后面
        string BinaryPath = "";
        int bytebuff = 0;
        int shiftcount = 0;
        for (int i = 0; i < HuffmanPath.size(); i++) {
            bytebuff += (HuffmanPath[i] == '1' ? 1 : 0);
            bytebuff <<= 1;
            shiftcount++;
            if (shiftcount == 8) {
                bytebuff >>= 1;
                BinaryPath += (char)bytebuff;
                bytebuff = 0;
                shiftcount = 0;
                if (i == HuffmanPath.size() - 1)
                    break;
                if (i + 8 > HuffmanPath.size()) {
                    i++;
                    while (i <= HuffmanPath.size() - 1) {
                        bytebuff += (HuffmanPath[i] == '1' ? 1 : 0);
                        bytebuff <<= 1;
                        shiftcount++;
                        i++;
                    }
                    bytebuff <<= 7 - shiftcount;
                    BinaryPath += (char)bytebuff;
                }
            }
        }
        Sign = 8 - shiftcount;
        cerr << "Switch String To Binary Completed" << endl;
        return BinaryPath;
    }
    int GetSide(Node HuffmanTree[], int LeafNumber) {
        for (int i = 0; i < 2 * LeafNumber - 2; i++) {
            if (i == HuffmanTree[HuffmanTree[i].parent].left)
                HuffmanTree[i].side = 'l';
            if (i == HuffmanTree[HuffmanTree[i].parent].right)
                HuffmanTree[i].side = 'r';
        }
        return 0;
    }
    int WriteToFile(string path2, Node HuffmanTree[], int LeafNumber,
                    string BinaryPath, int Sign) {
        // path2是要写的目标文件
        //打开二进制文件输出流
        // ShowTable(HuffmanTree, LeafNumber);
        LeafNumber -= 1;
        ofstream outstr(path2, ios::binary);
        outstr.write(reinterpret_cast<char *>(&Sign), sizeof(char));
        outstr.write(reinterpret_cast<char *>(&LeafNumber), sizeof(char));
        LeafNumber += 1;
        for (int i = 0; i < LeafNumber; i++) {
            //获取要写的内容的地址，转换为char*
            HuffmanTree[i].parent -= LeafNumber;
            outstr.write(reinterpret_cast<char *>(&HuffmanTree[i].info),
                         sizeof(char));
            outstr.write(reinterpret_cast<char *>(&HuffmanTree[i].parent),
                         sizeof(char));
            outstr.write(reinterpret_cast<char *>(&HuffmanTree[i].side),
                         sizeof(char));
        }
        for (int i = LeafNumber; i < 2 * LeafNumber - 1; i++) {
            HuffmanTree[i].parent -= LeafNumber;
            outstr.write(reinterpret_cast<char *>(&HuffmanTree[i].parent),
                         sizeof(char));
            outstr.write(reinterpret_cast<char *>(&HuffmanTree[i].side),
                         sizeof(char));
        }
        for (int i = 0; i < BinaryPath.size(); i++) {
            outstr.put(BinaryPath[i]);
        }
        outstr.close();
        cerr << "Write To File Completed" << endl;
        return 0;
    }
};
class Decompression {
  public:
    int ShowTable(Node HuffmanTree[], int LeafNumber) {
        for (int i = 0; i < 2 * LeafNumber - 1; i++) {
            cout << "i:" << i << endl;
            cout << HuffmanTree[i].index << "<-index" << endl;
            cout << HuffmanTree[i].info << "<-info" << endl;
            cout << HuffmanTree[i].side << "<-side" << endl;
            cout << HuffmanTree[i].left << "<-left" << endl;
            cout << HuffmanTree[i].right << "<-right" << endl;
            cout << HuffmanTree[i].parent << "<-parent" << endl;
        }
        return 0;
    }
    Tree ReadFile(string path1, unsigned int &LeafNumber, string &SearchPath,
                  int &Sign) {
        // Sign是要删去末尾的几个0
        ifstream instr(path1, ios::in | ios::binary);
        SearchPath = "";
        char ch;
        instr.get(ch);
        Sign = ch;
        instr.get(ch);
        LeafNumber = (int)(unsigned char)ch;
        LeafNumber += 1;
        Tree HuffmanTree = new Node[2 * LeafNumber - 1];
        unsigned int num;
        for (int i = 0; i < LeafNumber; i++) {
            instr.get(ch);
            num = (int)(unsigned char)ch;
            HuffmanTree[i].info = num;
            instr.get(ch);
            num = (int)(unsigned char)ch;
            HuffmanTree[i].parent = num + LeafNumber;
            instr.get(ch);
            HuffmanTree[i].side = ch;
            //初始化其他信息
            HuffmanTree[i].BinaryCode = "";
            HuffmanTree[i].index = i;
            HuffmanTree[i].left = -1;
            HuffmanTree[i].right = -1;
            HuffmanTree[i].weight = -1;
        }
        for (int i = LeafNumber; i < 2 * LeafNumber - 1; i++) {
            instr.get(ch);
            num = (int)(unsigned char)ch;
            HuffmanTree[i].parent = num + LeafNumber;
            instr.get(ch);
            HuffmanTree[i].side = ch;
            //初始化其他信息
            HuffmanTree[i].info = -10000;
            HuffmanTree[i].BinaryCode = "";
            HuffmanTree[i].index = i;
            HuffmanTree[i].left = -1;
            HuffmanTree[i].right = -1;
            HuffmanTree[i].weight = -1;
        }
        int bitmask = 0x80;
        while (instr.get(ch)) {
            num = (int)(unsigned char)ch;
            while (bitmask != 0) {
                if ((bitmask & num) != 0) {
                    SearchPath += '1';
                }
                if ((bitmask & num) == 0) {
                    SearchPath += '0';
                }
                bitmask >>= 1;
            }
            bitmask = 0x80;
        }
        SearchPath.erase(SearchPath.end() - Sign, SearchPath.end());
        instr.close();
        cerr << "Read File Completed" << endl;
        return HuffmanTree;
    }
    int BuiltHuffmanTree(Node HuffmanTree[], int LeafNumber) {
        for (int i = 0; i < 2 * LeafNumber - 2; i++) {
            if (HuffmanTree[i].side == 'l') {
                HuffmanTree[HuffmanTree[i].parent].left = i;
            }
            if (HuffmanTree[i].side == 'r') {
                HuffmanTree[HuffmanTree[i].parent].right = i;
            }
        }
        cerr << "Built Huffman Tree Completed" << endl;
        return 0;
    }
    int RestoreFile(string SearchPath, Node HuffmanTree[], string path2,
                    int LeafNumber) {
        ofstream outstr(path2, ios::out | ios::binary);
        int head = 2 * LeafNumber - 2, now = head;
        for (int i = 0; i < SearchPath.size(); i++) {
            if (SearchPath[i] == '0') {
                now = HuffmanTree[now].left;
            }
            if (SearchPath[i] == '1') {
                now = HuffmanTree[now].right;
            }
            if (HuffmanTree[now].left == -1 && HuffmanTree[now].right == -1) {
                char res = HuffmanTree[now].info;
                outstr.put(res);
                now = head;
            }
        }
        outstr.close();
        cerr << "Restore File Completed" << endl;
        return 0;
    }
};
//解析命令行：
int main(int argc, char *argv[]) {
    // //定义类
    Compression Compress;
    Decompression Decompress;
    if (argc != 4)
        ShowHelp();
    else if (stricmp(argv[1], "-z") == 0)
        cerr << "Zip " << argv[2] << " to " << argv[3] << " ..." << endl;
    else if (stricmp(argv[1], "-x") == 0)
        cerr << "Extract " << argv[2] << " to " << argv[3] << " ..." << endl;
    else {
        ShowHelp();
        return 0;
    }
    //把路径赋值给字符串
    const string op = argv[1];
    const string path1 = argv[2];
    const string path2 = argv[3];
    ifstream instr(path1, ios::in | ios::binary);
    if (!instr) {
        cerr << "Open File failed" << endl;
        return 0;
    }
    //路径分配
    if (op == "-z") {
        // cerr << "Ziping..." << endl;
        int tong[257] = {0};
        int LeafNumber =
            Compress.Count(op, path1, path2, tong); //统计字符出出现频率
        Tree HuffmanTree = new Node[2 * LeafNumber - 1];          //初始化树
        Compress.CreatHuffmanTree(tong, LeafNumber, HuffmanTree); //建树
        Compress.GetSide(HuffmanTree, LeafNumber);
        Compress.GetCodeNode(HuffmanTree, LeafNumber); //获得叶节点的编码
        // Compress.ShowTable(HuffmanTree, LeafNumber);
        string HuffmanPath =
            Compress.Encode(path1, HuffmanTree, LeafNumber); //文件进行编码
        int Sign;
        string BinaryPath =
            Compress.SwitchStringToBinary(HuffmanPath, Sign); //获得二进制串
        Compress.WriteToFile(path2, HuffmanTree, LeafNumber, BinaryPath,
                             Sign); //把哈夫曼树写入文件
        // Compress.ShowTable(HuffmanTree, LeafNumber);
        cerr << "Compression Completed" << endl;
        return 0;
    } 
    else if (op == "-x") {
        unsigned int LeafNumber;
        int Sign;
        string SearchPath;
        Tree HuffmanTree =
            Decompress.ReadFile(path1, LeafNumber, SearchPath, Sign);
        Decompress.BuiltHuffmanTree(HuffmanTree, LeafNumber);
        Decompress.RestoreFile(SearchPath, HuffmanTree, path2, LeafNumber);
        // Decompress.ShowTable(HuffmanTree, LeafNumber);
        cerr << "Decompress Completed" << endl;
    } else
        return 0;
    return 0;
}
