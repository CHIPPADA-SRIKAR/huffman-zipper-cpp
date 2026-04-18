# 🗜️ Zipper - CLI File Compressor

![Language](https://img.shields.io/badge/Language-C++-00599C?style=for-the-badge&logo=c%2B%2B)
![Type](https://img.shields.io/badge/Type-System_Tool-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

A high-performance command-line tool built in C++ that compresses text files using **Huffman Coding**. It demonstrates low-level systems programming concepts including **Bit Manipulation**, **Memory Management**, and **File I/O Streams**.

---

## 🚀 Features

* **Lossless Compression:** Reduces file size without losing a single byte of data.
* **Efficient Storage:** Uses bitwise operations to pack variable-length codes into compact binary data.
* **Custom File Format:** Generates `.bin` files with a serialized header table for self-contained decompression.
* **CLI Interface:** Simple command-line arguments for easy integration with scripts.

---

## 🛠️ Technical Implementation

This project implements the **Huffman Coding Algorithm**, a greedy algorithm for lossless data compression.

### Key Concepts Used:
* **Priority Queue (Min-Heap):** Used to build the Huffman Tree efficiently in `O(C log C)`.
* **Bitwise Operators (`<<`, `|`, `&`):** Used to pack string-based binary codes (e.g., "101") into actual bits in memory to optimize storage.
* **Serialization:** The frequency map is serialized into the file header, allowing the decompressor to reconstruct the Huffman Tree dynamically.
* **Stream Handling:** Uses `ifstream` and `ofstream` in `ios::binary` mode to handle raw byte streams and prevent OS-level text formatting issues.

### Complexity Analysis
| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **Frequency Analysis** | O(N) | O(1) (Fixed Alphabet) |
| **Tree Construction** | O(C log C) | O(C) |
| **Encoding/Decoding** | O(N) | O(N) |

*(Where N is file size, C is unique characters)*

---

## 💻 Installation & Usage

### Prerequisites
* G++ Compiler (MinGW for Windows, GCC for Linux/Mac)

### 1. Build the Project
Open your terminal in the project directory and compile the modular files:
```bash
g++ main.cpp huffman.cpp -o zipper
```

### 2. Compress a File
```bash
./zipper -c input.txt compressed.bin
```

### 3. Decompress a File
```bash
./zipper -d compressed.bin output.txt
```

## 📊 Performance Metrics

*Actual test results on sample data:*
### As the size of the original file increases, space saved increases
| File Type | Original Size | Compressed Size | Space Saved |
| :--- | :--- | :--- | :--- |
| **English Text** | 2.89 KB | 2.42 KB | **~16%** |
| **Source Code** | 6 KB | ~3.59 KB | **~33%** |
| **Repeated Char** | 206 bytes | 78 bytes | **~62%** |

---


## 👨‍💻 Author
**CHIPPADA SRIKAR**
