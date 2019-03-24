#include "../file_reverse.h"
#include "gtest.h"

bool compare_files(std::string const &file1, std::string const &file2);

bool files_are_reversed_copies(std::string const &file1, std::string const &file2);

template<typename ByteGenerator>
void generate_file(std::string const &file_name, long long size, ByteGenerator generator);

void clear_file(std::string const &file);


const auto FILE_NAME_1 = "file1.test";
const auto FILE_NAME_2 = "file2.test";
const auto FILE_NAME_3 = "file3.test";
const auto NO_SUCH_FILE = "no_such_file_1648767562374652640.test";


TEST(correctness, SAMPLE) {
    generate_file(FILE_NAME_1, 10, [](long long x) { return (char) (x + '0'); });
    reverse_file(FILE_NAME_1, FILE_NAME_2);
    EXPECT_TRUE(files_are_reversed_copies(FILE_NAME_1, FILE_NAME_2));
}

TEST(correctness, EMPTY) {
    generate_file(FILE_NAME_1, 0, [](long long) { return 0; });
    reverse_file(FILE_NAME_1, FILE_NAME_2);
    EXPECT_TRUE(files_are_reversed_copies(FILE_NAME_1, FILE_NAME_2));
}

TEST(correctness, 1BYTE) {
    generate_file(FILE_NAME_1, 1, [](long long) { return (char) (rand() % 256); });
    reverse_file(FILE_NAME_1, FILE_NAME_2);
    EXPECT_TRUE(files_are_reversed_copies(FILE_NAME_1, FILE_NAME_2));
}

TEST(correctness, 1MB) {
    generate_file(FILE_NAME_1, 1 << 20, [](long long) { return (char) (rand() % 256); });
    reverse_file(FILE_NAME_1, FILE_NAME_2);
    EXPECT_TRUE(files_are_reversed_copies(FILE_NAME_1, FILE_NAME_2));
}

TEST(correctness, 100MB) {
    generate_file(FILE_NAME_1, (long long) 100 << 20, [](long long) { return (char) (rand() % 256); });
    reverse_file(FILE_NAME_1, FILE_NAME_2);
    EXPECT_TRUE(files_are_reversed_copies(FILE_NAME_1, FILE_NAME_2));
}

/*TEST(correctness, 1GB_FILE) {  // ~1 min
    generate_file(FILE_NAME_1, (long long) 1 << 30, [](long long x) { return (char) (rand() % 256); });
    reverse_file(FILE_NAME_1, FILE_NAME_2);
    EXPECT_TRUE(files_are_reversed_copies(FILE_NAME_1, FILE_NAME_2));
}*/

TEST(correctness, STRANGE_SIZED_FILE) {
    generate_file(FILE_NAME_1, (1 << 20) + 227, [](long long) { return (char) (rand() % 256); });
    reverse_file(FILE_NAME_1, FILE_NAME_2);
    EXPECT_TRUE(files_are_reversed_copies(FILE_NAME_1, FILE_NAME_2));
}

TEST(correctness, WRONG_FILE_NAME) {
    EXPECT_ANY_THROW(reverse_file(NO_SUCH_FILE, FILE_NAME_2));
}

TEST(correctness, SAME_FILE_NAMES) {
    generate_file(FILE_NAME_1, 1 << 20, [](long long) { return (char) (rand() % 256); });
    EXPECT_ANY_THROW(reverse_file(FILE_NAME_1, FILE_NAME_1));
}

TEST(correctness, DOUBLE_REVERSE) {
    generate_file(FILE_NAME_1, 1 << 20, [](long long) { return (char) (rand() % 256); });
    reverse_file(FILE_NAME_1, FILE_NAME_2);
    reverse_file(FILE_NAME_2, FILE_NAME_3);
    EXPECT_TRUE(compare_files(FILE_NAME_1, FILE_NAME_3));
}

TEST(correctness, TRIPLE_REVERSE) {
    generate_file(FILE_NAME_1, 1 << 20, [](long long) { return (char) (rand() % 256); });
    reverse_file(FILE_NAME_1, FILE_NAME_2);
    reverse_file(FILE_NAME_2, FILE_NAME_3);
    reverse_file(FILE_NAME_3, FILE_NAME_1);
    EXPECT_TRUE(compare_files(FILE_NAME_1, FILE_NAME_2));
}

TEST(correctness, RANDOM_SMALL) {
    const int TEST_NUM = 200;
    int passed_random_tests = 0;
    for (int i = 0; i < TEST_NUM; ++i) {
        generate_file(FILE_NAME_1, 10*i, [](long long) { return (char) (rand() % 256); });
        reverse_file(FILE_NAME_1, FILE_NAME_2);
        passed_random_tests += files_are_reversed_copies(FILE_NAME_1, FILE_NAME_2);
    }
    EXPECT_EQ(passed_random_tests, TEST_NUM);
}

TEST(correctness, RANDOM) {
    const int TEST_NUM = 200;
    int passed_random_tests = 0;
    for (int i = 0; i < TEST_NUM; ++i) {
        generate_file(FILE_NAME_1, rand() % (10000 * i + 1), [](long long) { return (char) (rand() % 256); });
        reverse_file(FILE_NAME_1, FILE_NAME_2);
        passed_random_tests += files_are_reversed_copies(FILE_NAME_1, FILE_NAME_2);
    }
    EXPECT_EQ(passed_random_tests, TEST_NUM);
}


bool compare_files(std::string const &file1, std::string const &file2) {
    if (file1 == file2) {
        return true;
    }

    std::ifstream reader1(file1);
    std::ifstream reader2(file2);

    if (reader1.fail() || reader2.fail()) {
        return false;
    }

    while (!reader1.eof() && !reader2.eof()) {
        if (reader1.get() != reader2.get()) {
            return false;
        }
    }

    return reader1.eof() && reader2.eof();
}


bool files_are_reversed_copies(std::string const &file1, std::string const &file2) {
    std::ifstream reader1(file1);
    std::ifstream reader2(file2);
    long long file1_size = size(reader1);
    long long file2_size = size(reader2);

    if (file1_size != file2_size) {
        return false;
    }

    const long long BUFFER_SIZE = 8 << 10;
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    long long unhandled = file1_size;
    long long read_n;

    while (unhandled > 0) {
        unhandled = std::max((long long) 0, unhandled - BUFFER_SIZE);

        reader2.seekg(unhandled);
        reader1.read(buffer1, BUFFER_SIZE);
        reader2.read(buffer2, BUFFER_SIZE);
        read_n = reader1.gcount();

        std::reverse(buffer2, buffer2 + read_n);
        for (long long i = 0; i < read_n; ++i) {
            if (buffer1[i] != buffer2[i]) {
                return false;
            }
        }
    }
    return true;
}

void clear_file(std::string const &file) {
    std::ofstream out(file);
    out.close();
}

template<typename ByteGenerator>
void generate_file(std::string const &file_name, long long size, ByteGenerator generator) {
    std::ofstream file(file_name, std::ofstream::binary);
    for (long long i = 0; i < size; ++i) {
        file << generator(i);
    }
}
