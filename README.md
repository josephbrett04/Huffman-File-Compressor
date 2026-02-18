# Huffman File Compressor

A high-performance file compression tool utilizing Huffman Code.

## Technical Implementation

Basic C++ application meant for use on Linux devices.
This application implements the standard Huffman coding algorithm to achieve lossless compression through the following:

1.  The program reads the input file byte-by-byte to calculate the frequency of every character using a `std::map`.
2.  A Min-Heap (via `std::priority_queue`) is used to build the Huffman Tree, ensuring that more frequent characters end up near the root (shorter binary codes) and less frequent characters are deeper (longer codes).
3.  The tree is traversed recursively to generate unique binary string paths (0 for left, 1 for right) for every character.
4.  Instead of writing string characters "0" and "1" to the file (which would make a bigger size), the program uses bitwise operators (`<<`, `|`) to pack 8 bits into a single byte buffer before writing to the binary stream.
5.  The frequency table is serialized and written to the head of the compressed file, allowing the decompressor to reconstruct the exact same Huffman Tree without needing the original data.

## Build & Run Guide (Step-by-Step)

Follow these steps to compile and test the application on your local machine.

# Compile the Source Code
Open your terminal in the project directory and run the following command to link the files and create the executable.
```bash

g++ main.cpp Huffman.cpp -o huffman -std=c++17
```

# Make Test File
Create a simple text file to test the compression. You can use any text file, or generate one using the command line:
```bash

echo "This is a test string I Just made." > input.txt
```

# Start Compression
Run the program with the "-c" flag. This reads your text file and creates a compressed binary file.
```bash

./huffman -c input.txt compressed.bin
```

# Start Decompression
Run the program with the "-d" flag to reverse the process. This takes the binary file and reconstructs the original text.
```bash

./huffman -d compressed.bin restored.txt
```

# Verify the Results
Check that the original file (input.txt) and the restored file (restored.txt) are exactly the same. Nothing should be printed out
```bash

.diff input.txt restored.txt
```
