#pragma once
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

//#include "book.hpp"
//#include "storage.hpp"
#include <cstring>
#include <ostream>
#include <stack>
#include <string>
#include <vector>

class BookSys;
class Book;
class Account {
public:
  char name[31] = "";
  char id[31] = "";
  char password[31] = "";
  int level;
  Account(const std::string &name_, const std::string &id_, const std::string &password_, int &level_);
  Account(const std::string &id_, int &level);
  Account() = default;
  ~Account() = default;
  bool operator>(const Account &rhs) const;
  bool operator<(const Account &rhs) const;
  bool operator>=(const Account &rhs) const;
  bool operator<=(const Account &rhs) const;
  bool operator==(const Account &rhs) const;
  Account operator=(const Account &rhs);
  friend std::ostream &operator<<(std::ostream &op, const Account &rhs);
};

class Log_Stack {
public:
  std::vector<Account> acc_stack;
  std::vector<Book> book_stack;
  Log_Stack();
  ~Log_Stack() = default;
  void push(const Account &acc);
  void select(const Book &book);
  bool check_acc(const Account &acc) const;
  void pop();
  Account top() const;
  int size() const;
  Book &slct_book();
  void modify(const Book &val_old, const Book &val_new);
};

class Account_Operation {
private:
  Block<Account> database;
  Log_Stack log_stack;

public:
  Account_Operation();

  ~Account_Operation() = default;

  void su(const std::string &id, const std::string &password = ""); // prev = 0
  void logout(); // prev = 1
  void registerin(const std::string &id, const std::string &password, const std::string &name); // prev = 0
  void passwd(const std::string &id, const std::string &new_passwd, const std::string &current_passwd = ""); // prev = 1

  void useradd(const std::string &id, const std::string &password,
               int &level,
               const std::string &name); // prev = 3

  void erase(const std::string id); // prev = 7

  Account tellacc() const;

  Book &slct_book();

  void modify(const Book &val_old, const Book &val_new);

  void print();
};

#endif