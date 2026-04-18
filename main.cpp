#include <iostream>
#include <string>
#include "huffman.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage:\n";
        // here I am taking zipper to be the executable file name
        cout << "  Compress:   ./zipper -c <input_file> <output_file>\n";
        cout << "  Decompress: ./zipper -d <input_file> <output_file>\n";
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    if (mode == "-c") {
     Huffman::compressFile(inputFile, outputFile);
    } else if (mode == "-d") {
       Huffman::decompressFile(inputFile, outputFile);
    } else {
        cout << "Invalid mode. Use -c or -d" << endl;
    }

    return 0;
}
