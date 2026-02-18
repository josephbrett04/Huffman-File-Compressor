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
