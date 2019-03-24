#include <fstream>
#include <iostream>
#include <algorithm>
#include "file_reverse.h"


int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Incorrect arguments. Expected: \"<program> <input> <output>\"";
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
        std::cerr << "Unable to reverse \"" << src << "\" into \"" << dst << "\": " << e.what();
        return -1;
    }
}
