#pragma once
#include "account.hpp"
#ifndef BOOK_HPP
#define BOOK_HPP

class Book_Operation;

#include "storage.hpp"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>

class Account_Operation;
class LogStack;

extern Account_Operation accounts;

class Book 
{
public:
  char ISBN[21] = "";
  char name[61] = "";
  char author[61] = "";
  char keyword[61] = "";
  int quantity = 0;
  double price = 0.0;

  Book(const std::string &ISBN_, const std::string &name_, const std::string &author_, const std::string &keyword_, const int &quantity_ = 0, const double &price_ = 0.0);
  Book(const std::string &ISBN_);
  Book() = default;
  ~Book() = default;
  bool operator>(const Book &rhs) const;
  bool operator<(const Book &rhs) const;
  bool operator>=(const Book &rhs) const;
  bool operator<=(const Book &rhs) const;
  bool operator==(const Book &rhs) const;
  Book operator=(const Book &rhs);
  friend std::ostream &operator<<(std::ostream &op, const Book &rhs);
};

class Book_Operation {
private:
  Block<Book> database_ISBNindex;
  SubBlock database_nameindex;
  SubBlock database_authorindex;
  SubBlock database_keywordindex;
public:
  Book_Operation();
  ~Book_Operation() = default;

  void show(const int &type, const std::string &str = "");
  void buy(const std::string &ISBN, const int &quantity);
  void select(const std::string &ISBN);
  void modify(const std::string &ISBN, const std::string &name, const std::string &author, const std::string &keyword, double price);
  void import(const int &quantity, const double &total_cost);
  void select_clear();
  Book &select_book();
  void print_id_name();
  void print_id_author();
  void print_id_keyword();
};

#endif