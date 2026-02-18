#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <map>
#include <vector>

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    
    Node(char character, int frequency, Node* l = nullptr, Node* r = nullptr)
        : ch(character), freq(frequency), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

class Huffman {
private:
    std::map<char, std::string> huffmanCode;
    std::map<char, int> freqMap;
    Node* root;

    void generateCodes(Node* root, std::string str);
    void deleteTree(Node* root);
    void writeHeader(std::ofstream& outFile);
    void readHeader(std::ifstream& inFile);
    Node* buildTree(const std::map<char, int>& freq);

public:
    Huffman();
    ~Huffman();
    void compress(const std::string& inputFile, const std::string& outputFile);
    void decompress(const std::string& inputFile, const std::string& outputFile);
};

#endif
