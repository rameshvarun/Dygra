#ifndef UTIL_H
#define UTIL_H

#include "dependencies.h"
#include<string>

std::string file_to_string(const char* filename);

// trim from start
std::string &ltrim(std::string &s);

// trim from end
std::string &rtrim(std::string &s);

// trim from both ends
std::string &trim(std::string &s);

float length( Vector3f a );
Vector3f normalize( Vector3f a );

Vector3f cross( Vector3f left, Vector3f right );

float dot( Vector3f left, Vector3f right );

#endif