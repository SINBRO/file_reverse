#include <fstream>
#include <iostream>
#include <algorithm>


/*
unsigned long long FILE_SIZE = (unsigned long long) 100 << 20;

double time_passed(clock_t &time) {
    clock_t prev_time = time;
    time = clock();
    return (double) (time - prev_time) / CLOCKS_PER_SEC;
}

bool compare_files(std::string const &file1, std::string const &file2) {
    //clock_t start = clock();
    std::ifstream reader1(file1);
    std::ifstream reader2(file2);


    while (!reader1.eof() && !reader2.eof()) {
        if (reader1.get() != reader2.get()) {
            return false;
        }
    }

    //cout << "(compared in " << time_passed(start) << " s) ";


    return reader1.eof() && reader2.eof();
}
*/

/*void compare_files_and_print_res(std::string const &file1, std::string const &file2) {
    cout << "\n----------------------------------------\n";

    cout << "Files \"" << file1 << "\" and \"" << file2 << "\" are "
         << (compare_files(file1, file2) ? "EQUAL" : "DIFFERENT") << "!\n";

    cout << "----------------------------------------\n";
}*/

/*void clear_file(std::string const &file) {
    std::ofstream out(file);
    out.close();
}*/


/**
 *
 *
 *
 *
 *
**/

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
    if (in.fail() || out.fail()) {
        throw std::ios_base::failure("Stream reverse failed");
    }
}

/**
 *
 *
 *
 *
 *
**/

long long reverse_file(std::string const &file, std::string const &reversed) {
    std::ifstream in(file, std::ifstream::binary);
    std::ofstream out(reversed, std::ifstream::binary);

    in.seekg(0, std::ifstream::end);
    long long file_size = in.tellg();
    in.seekg(0, std::ifstream::beg);

    if (in.fail()) {
        throw std::runtime_error("Unable to open file \"" + file + "\" for reading.");
    }
    if (out.fail()) {
        throw std::runtime_error("Unable to open file \"" + reversed + "\" for writing.");
    }

    reverse_stream(in, out, file_size);

    return file_size;
}

/*void reverse_file(std::string const &file, std::string const &reversed) {
    // get file size

    std::ifstream in(file);
    std::ofstream out(reversed);

    char buffer[BUFFER_SIZE];
    size_t read_n;
    unsigned long long cur_buff_pos = FILE_SIZE;

    while (!in.eof()) {
        in.read(buffer, BUFFER_SIZE);
        read_n = static_cast<size_t>(in.gcount());
        std::reverse(buffer, buffer + read_n);
        out.seekp(cur_buff_pos -= read_n);
        out.write(buffer, read_n);
    }

    in.close();
    out.close();
}*/

/*
bool runTest() {
    //clock_t start = clock();

    std::string file = "big_file.in";
    std::string reversed = "reversed.out";
    std::string reReversed = "reReversed.in";

    //cout << "\nFile size is " << ((double) FILE_SIZE) / (1 << 20) << " mb\n";


    {
        std::ofstream created_in(file);
        for (unsigned long long i = 0; i < FILE_SIZE; ++i) {
            created_in << static_cast<char>(rand() % 26 + 'a');
        }
        created_in.close();
    }
    *//*cout << "\nFirst created in " << time_passed(start) << " s\n";
    cout.flush();*//*

    reverse_file(file, reversed);
    *//*cout << "\nFirst reversed in " << time_passed(start) << " s\n";
    cout.flush();*//*

    reverse_file(reversed, reReversed);
    *//*cout << "\n\"Reversed\" reversed in " << time_passed(start) << " s\n";
    cout.flush();*//*

    bool res = compare_files(file, reReversed);
    *//*compare_files_and_print_res(file, reReversed);
    compare_files_and_print_res(file, reversed);*//*
    std::cout.flush();

    *//*clear_file(file);
    clear_file(reversed);
    clear_file(reReversed);*//*
    return res;
}*/


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


/*

int main() {
    const int TEST_RUNS = 1000;
    int MAX_FILE_SIZE = 1;
    const int SIZE_STEP = 10000;
    int res = 0;
    clock_t start = clock();
    for (int i = 0; i < TEST_RUNS; ++i) {
        FILE_SIZE = static_cast<unsigned long long int>(rand() % (MAX_FILE_SIZE));
        MAX_FILE_SIZE += SIZE_STEP;
        std::cout << "\nTest " << i << " for FILE_SIZE = " << FILE_SIZE;
        if (runTest()) {
            ++res;
            std::cout << ": correct";
        } else {
            std::cout << ": WRONG";
        }
    }
    std::cout << "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n" <<
              "Test run finished in " << time_passed(start) << ".\n" << res << " of " << TEST_RUNS << " correct.";
}
*/
