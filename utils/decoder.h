#pragma once
#include <string>

std::string decode(const std::string& a, const std::string& b, char op, char out, bool swapped=true);
std::string solveTask(const std::string& file, bool swapped=true);

