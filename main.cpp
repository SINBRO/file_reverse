#include <fstream>
#include <iostream>
#include <algorithm>
//#include <random>

using namespace std;


unsigned long long FILE_SIZE = (unsigned long long) 100 << 20;
const size_t BUFFER_SIZE = 16 << 10;


double time_passed(clock_t& time) {
    clock_t res = time;
    time = clock();
    return (double) (time - res) / CLOCKS_PER_SEC;
}

bool compare_files(std::string const &file1, std::string const &file2) {
    clock_t start = clock();
    ifstream reader1(file1);
    ifstream reader2(file2);


    while (!reader1.eof() && !reader2.eof()) {
        if (reader1.get() != reader2.get()) {
            return false;
        }
    }

    //cout << "(compared in " << time_passed(start) << " s) ";


    return reader1.eof() && reader2.eof();
}

/*void compare_files_and_print_res(std::string const &file1, std::string const &file2) {
    cout << "\n----------------------------------------\n";

    cout << "Files \"" << file1 << "\" and \"" << file2 << "\" are "
         << (compare_files(file1, file2) ? "EQUAL" : "DIFFERENT") << "!\n";

    cout << "----------------------------------------\n";
}*/

void clear_file(string const &file) {
    ofstream out(file);
    out.close();
}

void reverse_file(string const &file, string const &reversed) {
    ifstream in(file);
    ofstream out(reversed);

    //cout << "\nReversing \"" << file << "\"...";

    //unsigned long long progress = FILE_SIZE - FILE_SIZE / 10 + 1;
    char buffer[BUFFER_SIZE];
    size_t read_n;
    unsigned long long cur_buff_pos = FILE_SIZE;

    while (!in.eof()) {
        in.read(buffer, BUFFER_SIZE);
        read_n = static_cast<size_t>(in.gcount());
        reverse(buffer, buffer + read_n);
        out.seekp(cur_buff_pos -= read_n); // max((unsigned long long) 0, FILE_SIZE - read_n)
        out.write(buffer, read_n);
    }

    in.close();
    out.close();
}


bool runTest() {
    clock_t start = clock();

    string file = "big_file.in";
    string reversed = "reversed.out";
    string reReversed = "reReversed.in";

    //cout << "\nFile size is " << ((double) FILE_SIZE) / (1 << 20) << " mb\n";


    {
        ofstream created_in(file);
        for (unsigned long long i = 0; i < FILE_SIZE; ++i) {
            created_in << static_cast<char>(rand() % 26 + 'a');
        }
        created_in.close();
    }
    /*cout << "\nFirst created in " << time_passed(start) << " s\n";
    cout.flush();*/

    reverse_file(file, reversed);
    /*cout << "\nFirst reversed in " << time_passed(start) << " s\n";
    cout.flush();*/

    reverse_file(reversed, reReversed);
    /*cout << "\n\"Reversed\" reversed in " << time_passed(start) << " s\n";
    cout.flush();*/

    bool res = compare_files(file, reReversed) && !compare_files(file, reversed);
    /*compare_files_and_print_res(file, reReversed);
    compare_files_and_print_res(file, reversed);*/
    cout.flush();

    clear_file(file);
    clear_file(reversed);
    clear_file(reReversed);
    return res;
}


int main() {
    const int TEST_RUNS = 100;
    int res = 0;
    clock_t start = clock();
    for (int i = 0; i < TEST_RUNS; ++i) {
        FILE_SIZE = static_cast<unsigned long long int>(rand() % (1 << 20));
        cout << "\nTest " << i << " for FILE_SIZE = " << FILE_SIZE;
        if (runTest()) {
            ++res;
            cout << ": correct";
        } else {
            cout << ": WRONG";
        }
    }
    cout << "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n" <<
            "Test run finished in " << time_passed(start) << ".\n" << res << " of " << TEST_RUNS << " correct.";
}
