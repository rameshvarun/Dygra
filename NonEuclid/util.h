#ifndef UTIL_H
#define UTIL_H

#include<string>

std::string file_to_string(const char* filename);

// trim from start
std::string &ltrim(std::string &s);

// trim from end
std::string &rtrim(std::string &s);

// trim from both ends
std::string &trim(std::string &s);

#endif