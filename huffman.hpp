#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <string>
#include <unordered_map>

namespace Huffman {

    
    void compressFile(const std::string& inputFile, const std::string& outputFile);

   
    void decompressFile(const std::string& inputFile, const std::string& outputFile);

}

#endif // HUFFMAN_HPP
