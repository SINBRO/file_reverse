#ifndef UNTITLED_FILE_REVERSE_H
#define UNTITLED_FILE_REVERSE_H


#include <fstream>
#include <iostream>
#include <algorithm>


long long size(std::ifstream &file);

void reverse_stream(std::istream &in, std::ostream &out, long long n);

long long reverse_file(std::string const &file, std::string const &reversed);


#endif //UNTITLED_FILE_REVERSE_H
