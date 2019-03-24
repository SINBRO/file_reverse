#include "file_reverse.h"


long long reverse_file(std::string const &file, std::string const &reversed) {
    if (file == reversed) {
        throw std::runtime_error("Input and output files should be different");
    }

    std::ifstream in(file, std::ifstream::binary);
    std::ofstream out(reversed, std::ifstream::binary);

    long long file_size = size(in);

    if (in.fail()) {
        throw std::runtime_error("Unable to open file \"" + file + "\" for reading.");
    }
    if (out.fail()) {
        throw std::runtime_error("Unable to open file \"" + reversed + "\" for writing.");
    }

    reverse_stream(in, out, file_size);
    return file_size;
}

void reverse_stream(std::istream &in, std::ostream &out, long long n) {
    const size_t BUFFER_SIZE = 16 << 10;

    char buffer[BUFFER_SIZE];
    size_t read_n = 0;
    long long current_buffer_pos = n;

    while (!in.eof() && current_buffer_pos > 0) {
        in.read(buffer, BUFFER_SIZE);
        read_n = static_cast<size_t>(in.gcount());

        std::reverse(buffer, buffer + read_n);

        out.seekp(current_buffer_pos -= read_n);
        out.write(buffer, read_n);
    }

    if (current_buffer_pos != 0) {
        throw std::runtime_error("Wrong size of stream was given");
    }
    if (out.fail()) {
        throw std::ios_base::failure("Stream reverse failed");
    }
}

long long size(std::ifstream &file) { // returns stream pointer to the beginning of the file
    file.seekg(0, std::ifstream::end);
    long long res = file.tellg();
    file.seekg(0, std::ifstream::beg);
    return res;
}
