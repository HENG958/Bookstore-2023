#include "check.hpp"

bool str_check::num_letter(const std::string &str) 
{
  auto check = [](char ch) 
  {
    return (ch == '_') || (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
  };

  for (int i = 0; i != str.size(); ++i) 
  {
    if (!check(str[i])) 
    {
      return false;
    }
  }
  return true;
}

bool str_check::non_invisible(const std::string &str) 
{
  int len = str.length();
  for (int i = 0; i < len; ++i) 
  {
    if (str[i] <= 32 || str[i] > 126) 
    {
      return false;
    }
  }
  return true;
}
bool str_check::non_invisible_quotes(const std::string &str) 
{

  int len = str.length();
  for (int i = 0; i < len; ++i) 
  {
    if (str[i] <= 32 || str[i] > 127) 
    {
      return false;
    }
  }
  return true;
}

bool str_check::check_keyword(const std::string &str) 
{
  int len = str.length();
  for (int i = 0; i < len; ++i) 
  {
    if (str[i] <= 32 || str[i] > 127) 
    {
      return false;
    }
    if (str[i] == '|' && i + 1 < str.size() && str[i + 1] == '|') 
    {
      return false;
    }
  }
  if (str[0] == '|' || str.back() == '|') 
  {
    return false;
  }

  return true;
}

bool str_check::check_single_keyword(const std::string &str) 
{
  if (!non_invisible_quotes(str)) 
  {
    return false;
  }
  if (str.find("|") != std::string::npos) 
  {
    return false;
  }
  return true;
}

bool str_check::check_keyword_non_repetition(const std::string &str) 
{
  if (!check_keyword(str)) 
  {
    return false;
  }
  if (str.size() > 60) 
  {
    return false;
  }
  std::unordered_set<std::string> used_keys;
  char src_str[61];
  const char *const delim = "|";
  strcpy(src_str, str.c_str());
  char *ptr = strtok(src_str, delim);
  while (ptr != nullptr) 
  {
    const std::string tok(ptr);
    if (tok.size() == 0 || !non_invisible_quotes(tok)) 
    {
      return false;
    }

    if (used_keys.find(tok) != used_keys.end()) 
    {
      return false;
    }
    used_keys.emplace(tok);
    ptr = strtok(nullptr, delim);
  }
  return true;
}
bool str_check::check_int(const std::string &str) 
{
  for (int i = 0; i != str.size(); ++i) 
  {
    if (!(str[i] >= '0' && str[i] <= '9')) 
    {
      return false;
    }
  }
  if (std::stoi(str) > 2147483647) 
  {
    return false;
  }
  return true;
}

bool str_check::check_float(const std::string &str) 
{
  if (str.size() > 13) 
  {
    return false;
  }
  for (int i = 0; i != str.size(); ++i) 
  {
    if (!((str[i] >= '0' && str[i] <= '9') || str[i] == '.')) {
      return false;
    }
  }
  auto pos = str.find(".");
  if (pos == std::string::npos) 
  {
    return true;
  } 
  else 
  {
    if (pos == 0 || pos == str.size() - 1) 
    {
      return false;
    }
    auto pos_ = str.rfind(".");
    if (pos != pos_) 
    {
      return false;
    }
    if (str.size() - pos > 3) 
    {
      return false;
    }
    return true;
  }
}