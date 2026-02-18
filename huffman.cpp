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

    while (inFile.get(ch)) {
        std::string code = huffmanCode[ch];
        for (char bit : code) {
            if (bit == '1') {
                buffer |= (1 << (7 - bitCount));
            }
            bitCount++;
            if (bitCount == 8) {
                outFile.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }
    
    if (bitCount > 0) {
        outFile.put(buffer);
    }

    inFile.close();
    outFile.close();
}

void Huffman::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    readHeader(inFile);
    
    root = buildTree(freqMap);
    
    int totalChars = root->freq;
    Node* curr = root;
    
    std::ofstream outFile(outputFile, std::ios::binary);
    char byte;

    while (inFile.get(byte)) {
        std::bitset<8> bits(byte);
        for (int i = 7; i >= 0; --i) {
            if (totalChars == 0) break;
            
            if (bits[i] == 0) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }

            if (!curr->left && !curr->right) {
                outFile.put(curr->ch);
                curr = root;
                totalChars--;
            }
        }
    }
    
    inFile.close();
    outFile.close();
}
