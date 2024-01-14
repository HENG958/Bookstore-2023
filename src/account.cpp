#include "account.hpp"
#include <cassert>
#include "storage.hpp"
Account::Account(const std::string &name_, const std::string &id_, const std::string &password_, int &level_) 
{
  strcpy(name, name_.c_str());
  strcpy(id, id_.c_str());
  strcpy(password, password_.c_str());
  level = level_;
}
Account::Account(const std::string &id_, int &level_) 
{
  strcpy(id, id_.c_str());
  level = level_;
}
bool Account::operator>(const Account &obj) const 
{
  return strcmp(id, obj.id) > 0;
}
bool Account::operator<(const Account &obj) const 
{
  return strcmp(id, obj.id) < 0;
}
bool Account::operator>=(const Account &obj) const 
{
  return strcmp(id, obj.id) >= 0;
}
bool Account::operator<=(const Account &obj) const 
{
  return strcmp(id, obj.id) <= 0;
}
bool Account::operator==(const Account &obj) const 
{
  return strcmp(id, obj.id) == 0;
}
Account Account::operator=(const Account &obj) 
{
  if (this == &obj) 
  {
    return *this;
  }
  strcpy(name, obj.name);
  strcpy(id, obj.id);
  strcpy(password, obj.password);
  level = obj.level;
  return *this;
}

std::ostream &operator<<(std::ostream &os, const Account &obj) 
{
  os << "id: " << obj.id << "\nname: " << obj.name << "\npassword: " << obj.password << "\nLevel: " << obj.level << '\n';
  return os;
}
Log_Stack::Log_Stack() 
{
  acc_stack.clear();
  book_stack.clear();
}
void Log_Stack::push(const Account &acc) 
{
  acc_stack.push_back(acc);
  book_stack.push_back(Book());
}
void Log_Stack::select(const Book &book) 
{ 
  book_stack.back() = book; 
}
bool Log_Stack::check_acc(const Account &acc) const 
{
  for (int i = 0; i != acc_stack.size(); ++i) 
  {
    if (acc_stack[i] == acc) 
    {
      return true;
    }
  }
  return false;
}

void Log_Stack::pop() 
{
  acc_stack.pop_back();
  book_stack.pop_back();
}
Account Log_Stack::top() const 
{ 
  return acc_stack.back(); 
}
int Log_Stack::size() const 
{ 
  return acc_stack.size(); 
}
Book &Log_Stack::slct_book() 
{ 
  return book_stack.back(); 
}
void Log_Stack::modify(const Book &val_old, const Book &val_new) 
{
  for (int i = 0; i != book_stack.size(); ++i) 
  {
    if (book_stack[i] == val_old) 
    {
      book_stack[i] = val_new;
    }
  }
}
Account_Operation::Account_Operation() 
{
  bool count;//看数据库里是否能找到
  if (count == 0) 
    database.insert(Account("root", "root", "sjtu", 1));
  log_stack.push(Account(""));
}
void Account_Operation::su(const std::string &id, const std::string &password) 
{
  Account acc;
  //看数据库里是否能找到
  const auto &current_acc = log_stack.top();
  if (strlen(acc.id) == 0) 
  {
    throw 0;
    return;
  }
  if (password == "") 
  {
    if (current_acc.level > acc.level) 
    {
      log_stack.push(acc);
    } 
    else 
    {
      throw 0;
      return;
    }
  } 
  else 
  {
    const int res = strcmp(password.c_str(), acc.password);
    if (res == 0) 
    {
      log_stack.push(acc);
    } 
    else 
    {
      throw 0;
      return;
    }
  }
}

void Account_Operation::logout() 
{

  if (log_stack.size() == 1) 
  {
    throw 0;
    return;
  } 
  else 
  {
    log_stack.pop();
  }
}

void Account_Operation::registerin(const std::string &id, const std::string &password, const std::string &name) 
{
  bool count;
  //看在数据库里是否能找到
  if (count) 
  {
    throw 0;
    return;
  } 
  else 
  {
    const Account new_acc(name, id, password, customer);
    database.insert(new_acc);
  }
} 

void Account_Operation::passwd(const std::string &id, const std::string &new_passwd, const std::string &current_passwd) 
{
  if (log_stack.top().level < 1) 
  {
    throw 0;
    return;
  }
  Account acc_id;
  //看数据库里是否能找到
  if (strlen(acc_id.id) == 0) 
  {
    throw 0;
    return;
  }
  if (current_passwd == "") 
  {
    if (log_stack.top().level == 7) 
    {
      strcpy(acc_id.password, new_passwd.c_str());
      database.update(acc_id);
    } else 
    {
      throw 0;
      return;
    }
  } 
  else 
  {
    const int res = strcmp(current_passwd.c_str(), acc_id.password);
    if (res == 0) 
    {
      strcpy(acc_id.password, new_passwd.c_str());
      database.update(acc_id);
    } 
    else 
    {
      throw 0;
      return;
    }
  }
} 
void Account_Operation::useradd(const std::string &id, const std::string &password, int &level, const std::string &name) 
{
  if (log_stack.top().level < 3 || log_stack.top().level <= level) 
  {
    throw 0;
    return;
  }
  bool count;
  if (count) 
  {
    throw 0;
    return;
  } 
  else 
  {
    const Account new_acc(name, id, password, level);
    database.insert(new_acc);
  }
} // level = 3

void Account_Operation::erase(const std::string id) 
{
  if (log_stack.top().level < 7) 
  {
    throw 0;
    return;
  }
  const auto acc = database.find(Account(id));
  if (strlen(acc.id) == 0) 
  {
    throw 0;
    return;
  }
  if (log_stack.check_acc(acc)) 
  {
    throw 0;
    return;
  }
  database.erase(acc);
} 
Account Account_Operation::tellacc() const 
{ 
  return log_stack.top(); 
}
void Account_Operation::print() 
{ 
  database.print(); 
}
Book &Account_Operation::slct_book() { return log_stack.slct_book(); }

void Account_Operation::modify(const Book &val_old, const Book &val_new) {
  log_stack.modify(val_old, val_new);
}

