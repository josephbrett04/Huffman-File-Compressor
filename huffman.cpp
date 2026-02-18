#include "Huffman.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <bitset>

Huffman::Huffman() : root(nullptr) {}

Huffman::~Huffman() {
    deleteTree(root);
}

void Huffman::deleteTree(Node* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void Huffman::generateCodes(Node* root, std::string str) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }
    generateCodes(root->left, str + "0");
    generateCodes(root->right, str + "1");
}

Node* Huffman::buildTree(const std::map<char, int>& freq) {
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

    for (auto const& [key, val] : freq) {
        pq.push(new Node(key, val));
    }

    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        int sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right));
    }

    return pq.top();
}

void Huffman::writeHeader(std::ofstream& outFile) {
    int mapSize = freqMap.size();
    outFile.write(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
    for (auto const& [key, val] : freqMap) {
        outFile.write(&key, sizeof(key));
        outFile.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
}

void Huffman::readHeader(std::ifstream& inFile) {
    int mapSize;
    inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
    for (int i = 0; i < mapSize; ++i) {
        char key;
        int val;
        inFile.read(&key, sizeof(key));
        inFile.read(reinterpret_cast<char*>(&val), sizeof(val));
        freqMap[key] = val;
    }
}

void Huffman::compress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::map<char, int> tempFreq;
    char ch;
    while (inFile.get(ch)) {
        tempFreq[ch]++;
    }
    inFile.clear();
    inFile.seekg(0);

    freqMap = tempFreq;
    root = buildTree(freqMap);
    generateCodes(root, "");

    std::ofstream outFile(outputFile, std::ios::binary);
    writeHeader(outFile);

    char buffer = 0;
    int bitCount = 0;


}
