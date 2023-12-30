//
// Created by seif alrahman on 12/30/2023.
//
#include "aux_functions.h"
#include <stack>
#include <string>
#include <algorithm>
#include <cctype>
string removeSpaces(const std::string& str) {
    std::string result;
    std::remove_copy_if(str.begin(), str.end(), std::back_inserter(result), ::isspace);
    return result;
}

bool areLinesIdentical(const std::string& line1, const std::string& line2) {
    return removeSpaces(line1) == removeSpaces(line2);
}