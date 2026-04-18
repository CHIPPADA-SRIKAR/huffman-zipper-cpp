#include "huffman.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <cstdint>
using namespace std;


namespace Huffman{
// The Tree Node 
struct Node {
    char ch;
    int32_t freq;
    Node *left, *right;

    Node(char c, int32_t f, Node* l = nullptr, Node* r = nullptr) 
        : ch(c), freq(f), left(l), right(r) {}
};
 //comparator fn for priority queue
struct Compare {
    bool operator()(Node* l, Node* r) {
        if (l->freq != r->freq) {
            return l->freq > r->freq; // Primary sort by frequency
        }

        return l->ch > r->ch;//secondary sort by ascii value of character
    }
};

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

Node* buildHuffmanTree(const unordered_map<char, int32_t>& freqMap) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }

    // If only 1 unique character, create a dummy parent
    if (pq.size() == 1) {
        Node* child = pq.top(); pq.pop();
        pq.push(new Node('\0', child->freq, child, nullptr));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        int32_t sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right));
    }
    return pq.top();
}

void generateCodes(Node* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;
    if (!root->left && !root->right) {
        //  If file has only 1 unique char, code might be empty. Assigning "0".
        huffmanCodes[root->ch] = code.empty() ? "0" : code;
    }
    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

void writeHeader(ofstream& out, const unordered_map<char, int32_t>& freqMap) {
    int32_t mapSize = freqMap.size();
    out.write((char*)&mapSize, sizeof(mapSize)); 
    for (auto pair : freqMap) {
        out.write((char*)&pair.first, sizeof(pair.first)); 
        out.write((char*)&pair.second, sizeof(pair.second)); 
    }
}

unordered_map<char, int32_t> readHeader(ifstream& in) {
    unordered_map<char, int32_t> freqMap;
    int32_t mapSize;
    in.read((char*)&mapSize, sizeof(mapSize));
    for (int i = 0; i < mapSize; i++) {
        char ch;
        int32_t freq;
        in.read((char*)&ch, sizeof(ch));
        in.read((char*)&freq, sizeof(freq));
        freqMap[ch] = freq;
    }
    return freqMap;
}

void compressFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in) { cerr << "Error: Input file not found!" << endl; return; }
    
    
    string text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    if (text.empty()) { cerr << "Error: File is empty!" << endl; return; }

    
    long originalSize = text.size();

    unordered_map<char, int32_t> freqMap;
    for (char c : text) freqMap[c]++;

    Node* root = buildHuffmanTree(freqMap);
    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    ofstream out(outputFile, ios::binary);
    writeHeader(out, freqMap);

    int32_t totalChars = text.length();
    out.write((char*)&totalChars, sizeof(totalChars));

    char buffer = 0;
    int bitCount = 0;

    for (char c : text) {
        string code = huffmanCodes[c];
        for (char bit : code) {
            if (bit == '1') {
                buffer = buffer | (1 << (7 - bitCount));
            }
            bitCount++;
            if (bitCount == 8) {
                out.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) out.put(buffer);

    
    long compressedSize = out.tellp();
    out.close();

    
    freeTree(root);

    cout << "Success! " << inputFile << " -> " << outputFile << endl;
    cout << "Original: " << originalSize << " bytes | Compressed: " << compressedSize << " bytes" << endl;
    double savings = (1.0 - (double)compressedSize / originalSize) * 100;
    cout << "Storage saved: " << savings << "%" << endl;
}

void decompressFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in) { cerr << "Error: Compressed file not found!" << endl; return; }

    unordered_map<char, int32_t> freqMap = readHeader(in);
    Node* root = buildHuffmanTree(freqMap);

    int32_t totalChars;
    in.read((char*)&totalChars, sizeof(totalChars));

    ofstream out(outputFile, ios::binary);
    Node* curr = root;
    char byte;
    int count = 0;

    while (in.get(byte) && count < totalChars) {
        for (int i = 0; i < 8; i++) {
            bool isSet = byte & (1 << (7 - i));
            if (isSet) curr = curr->right;
            else curr = curr->left;

            if (!curr->left && !curr->right) {
                out.put(curr->ch);
                curr = root;
                count++;
                if (count == totalChars) break;
            }
        }
    }

    in.close();
    out.close();
    
    
    freeTree(root);
    
    cout << "Decompression Complete! Restored to " << outputFile << endl;
}
}
