//
// Created by andrey on 11.10.18.
//

#include <iostream>
#include "file_reader.h"

file_reader::file_reader(std::string const &file_name) : in(file_name, std::ifstream::binary),
                                                         file(file_name), res(0), cur_symbol(0), s_in_buff(0),
                                                         cur_code_piece(0), useful_bits(0) {
    if (in.fail()) {
        in.close();
        throw std::runtime_error("Unable to open file \"" + file_name + "\"");
    }
    upload();
}


symbol file_reader::get_symbol() {
    res = buffer[cur_symbol++];
    upload();
    return res;
}

bool file_reader::eof() const {
    return cur_symbol >= s_in_buff && in.eof();
}

void file_reader::upload() {
    if (cur_symbol >= s_in_buff) {
        in.read(reinterpret_cast<char *>(buffer), OLD_BUFFER_SIZE * sizeof(symbol) / sizeof(char));
        cur_symbol = 0;
        s_in_buff = static_cast<uint64_t>(in.gcount()) * sizeof(char) / sizeof(symbol);

        if (s_in_buff == 0) {
            in.read(reinterpret_cast<char *>(buffer), OLD_BUFFER_SIZE * sizeof(symbol) / sizeof(char));
            s_in_buff = static_cast<uint64_t>(in.gcount()) * sizeof(char) / sizeof(symbol);
        }
    }
}

file_reader::~file_reader() {
    in.close();
}
