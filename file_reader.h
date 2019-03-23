//
// Created by andrey on 11.10.18.
//

#ifndef HUFFMAN_FILE_READER_H
#define HUFFMAN_FILE_READER_H

#include "types_consts.h"
#include <fstream>

struct file_reader {
    file_reader() = delete;

    explicit file_reader(std::string const &file_name);

    ~file_reader();

    void upload();

    symbol get_symbol();


    bool eof() const;

private:

    std::ifstream in;
    std::string file;
    symbol buffer[OLD_BUFFER_SIZE];
    symbol res = 0;
    uint64_t cur_symbol = 0;
    uint64_t s_in_buff = 0;
    uint64_t cur_code_piece = 0;
    int8_t useful_bits = 0;
};


#endif //HUFFMAN_FILE_READER_H
