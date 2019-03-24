//
// Just contents file_reverse.h, file_reverse.cpp and main.cpp in one file
//
#include <fstream>
#include <iostream>
#include <algorithm>


/// You can also see this project with tests on my GitHub: https://github.com/SINBRO/file_reverse

long long size(std::ifstream &file);

void reverse_stream(std::istream &in, std::ostream &out, long long n);

long long reverse_file(std::string const &file, std::string const &reversed);


int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Incorrect arguments. Expected: \"program.exe input.dat output.dat\"";
        return -1;
    }

    auto src = argv[1];
    auto dst = argv[2];

    try {
        clock_t working_time = clock();
        double file_size = (double) reverse_file(src, dst) / (1 << 20);
        working_time = clock() - working_time;

        std::cout << "Successfully reversed " << file_size << " mb \"" << src << "\" into \"" << dst << "\" in "
                  << (double) working_time / CLOCKS_PER_SEC << " s ("
                  << file_size / ((double) working_time / CLOCKS_PER_SEC) << " mb/s).";

    } catch (std::runtime_error &e) {
        std::cerr << "Unable to reverse \"" << src << "\" in \"" << dst << ": " << e.what();
        return -1;
    }
}


long long reverse_file(std::string const &file, std::string const &reversed) {
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

long long size(std::ifstream &file) {
    file.seekg(0, std::ifstream::end);
    long long res = file.tellg();
    file.seekg(0, std::ifstream::beg);
    return res;
}
