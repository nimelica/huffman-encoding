#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <map>
using namespace std;

using Letter = char;
using Freq = int;

struct HuffTree {
    int val{};
    char letter{};
    HuffTree* left, * right;
    HuffTree(int val, char c) : val(val), letter(c) {
        left = right = nullptr;
    }
};
// A helper method to convert default max heap priority queue--> min heap
struct Compare {
    bool operator() (HuffTree* x, HuffTree* y) {
        return true ? x->val > y->val : false;
    }
};
// Keep track of the letter and frequency of each
vector<pair<Freq, Letter>>Frequeny(string& str) {
    map<char, int>hashmap;
    for (int i = 0; i != str.size(); ++i) {
        hashmap[str[i]]++;
    }
    vector <pair<int, char>> freqs;
    for (int i = 0; i != str.size(); ++i) {
        if (hashmap[str[i]]) {
            freqs.push_back({ hashmap[str[i]], str[i] });
            hashmap[str[i]] = 0;
        }
    }
    return freqs;
}
// For the sake of readability
typedef priority_queue<HuffTree*, vector<HuffTree*>, Compare >TREE;

// Print letters and their number of frequency in given string
void PrintFrequency(TREE tree) {
    cout << "\nFrequency of each character: \n";
    while (tree.size()) {
        string letter{};
        if (tree.top()->letter == ' ') {
            letter = "''";
        }else {
            letter = tree.top()->letter;
        }
        cout << "\t" << letter << " -> " << tree.top()->val << "\n";
        tree.pop();
    }
}
// Growing a huffman tree with O(n * log(n)) time complexity using Priority Queue!
HuffTree* GrowTree(vector<pair<int, char>>& freqs) {
    TREE tree;
    for (int i = 0; i != freqs.size(); ++i) {
        tree.push(new HuffTree(freqs[i].first, freqs[i].second));
    }
    PrintFrequency(tree);
    while (tree.size() > 1) {
        HuffTree* left = tree.top(); tree.pop();
        HuffTree* right = tree.top(); tree.pop();

        HuffTree* huff = new HuffTree(left->val + right->val, '#');
        huff->left = left;
        huff->right = right;
        tree.push(huff);
    }
    return tree.top();
}
// Print Huffman tree
void PrintHuffTree(vector<pair<char, string>>& res, string& str) {
    for (auto& r : res) {
        string letter{};
        if (r.first == ' ') {
            letter = "''";
        }else {
            letter = r.first;
        }
        cout << "\t" << letter << " ->  " << r.second << "\n";
        str += r.second;
    }
}
// Get Huffman code results
void GetHuffCodes(HuffTree* root, string str, vector<pair<char, string>>& res) {
    if (!root) return;
    else if (root->letter != '#') {
        res.push_back({ root->letter, str });
        return;
    }
    GetHuffCodes(root->left, str + "0", res);
    GetHuffCodes(root->right, str + "1", res);
}
// Decode string from binary values to characters
string Decode(HuffTree* tree, string str, int& index) {
    string ans = "";
    while (index < str.size()) {
        if (tree->letter != '#') {
            ans += tree->letter;
            return ans;
        }
        else if (str[index] == '0') {
            if (tree->left) {
                tree = tree->left;
                index++;
            }
            else {
                return ans;
            }
        }
        else if (str[index] == '1') {
            if (tree->right) {
                tree = tree->right;
                index++;
            }
            else {
                return ans;
            }
        }
    }
    return "";
}
// Drive main program
int main() {
    string str{}; cout << "Enter a string input: \t";
    // accept spaces as character
    getline(std::cin, str);

    vector<pair<int, char>>freqs = Frequeny(str);
    std::sort(freqs.begin(), freqs.end());
    HuffTree* tree = GrowTree(freqs);

    vector<pair<char, string>>res;
    GetHuffCodes(tree, "", res);
    cout << "\nHuffman codes for " << str << " \n";
    string s{ "" };
    PrintHuffTree(res, s);

    cout << "\n" << str << " --> " << s << "\n";

    string ans{ "" };
    for (int index = 0; index != s.size(); ) {
        ans += Decode(tree, s, index);
    }
    cout << "\nDecoded " << s << ": " << ans << "\n";


    return 0;
}
