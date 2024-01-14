#pragma once
#ifndef STRING_CHECK_HPP
#define STRING_CHECK_HPP

#include <cstring>
#include <string>
#include <unordered_set>

namespace str_check 
{

bool num_letter(const std::string &str);
bool non_invisible(const std::string &str);
bool non_invisible_quotes(const std::string &str);
bool check_keyword(const std::string &str);
bool check_single_keyword(const std::string &str);
bool check_keyword_non_repetition(const std::string &str);
bool check_int(const std::string &str);
bool check_float(const std::string &str);
}

#endif