#include "book.hpp"
#include "storage.hpp"
#include "diary.hpp"
#include <cstring>
#include <iomanip>
#include <string>


Book::Book(const std::string &ISBN_, const std::string &name_, const std::string &author_, const std::string &keyword_, const int &quantity_, const double &price_)
    : quantity(quantity_), price(price_) 
{
  strcpy(ISBN, ISBN_.c_str());
  strcpy(name, name_.c_str());
  strcpy(author, author_.c_str());
  strcpy(keyword, keyword_.c_str());
}

Book::Book(const std::string &ISBN_) 
{ 
  strcpy(ISBN, ISBN_.c_str()); 
}
bool Book::operator>(const Book &obj) const 
{
  return strcmp(ISBN, obj.ISBN) > 0;
}
bool Book::operator<(const Book &obj) const 
{
  return strcmp(ISBN, obj.ISBN) < 0;
}
bool Book::operator>=(const Book &obj) const 
{
  return strcmp(ISBN, obj.ISBN) >= 0;
}
bool Book::operator<=(const Book &obj) const 
{
  return strcmp(ISBN, obj.ISBN) <= 0;
}
bool Book::operator==(const Book &obj) const 
{
  return strcmp(ISBN, obj.ISBN) == 0;
}
Book Book::operator=(const Book &obj) 
{
  if (this == &obj) 
  {
    return *this;
  }
  strcpy(ISBN, obj.ISBN);
  strcpy(name, obj.name);
  strcpy(author, obj.author);
  strcpy(keyword, obj.keyword);
  quantity = obj.quantity;
  price = obj.price;
  return *this;
}

std::ostream &operator<<(std::ostream &op, const Book &obj) 
{
  if (strlen(obj.ISBN) == 0) 
  {
    return op;
  }

  op << obj.ISBN << '\t' << obj.name << '\t' << obj.author << '\t'
     << obj.keyword << '\t' << std::fixed << std::setprecision(2) << obj.price
     << '\t' << obj.quantity << '\n';
  return op;
}

Book_Operation::Book_Operation(): database_ISBNindex("books_ISBN.dat"), database_authorindex("database_authorindex.dat"),
                                  database_nameindex("database_nameindex.dat"), database_keywordindex("database_keywordindex.dat") {}

void Book_Operation::show(const int &type, const std::string &str) 
{
  if (type == 0) 
  {
    database_ISBNindex.print();
  } 
  else if (type == 1) 
  {
    auto *const ans = database_ISBNindex.get(Book(str));
    if (ans->empty())
    {
      std::cout << '\n';
    } 
    else 
    {
      for (auto i = ans->cbegin(); i != ans->cend(); ++i) 
      {
        std::cout << *i;
      }
    }
    delete ans;
  } 
  else if (type == 2) 
  {
    auto *const indexes = database_nameindex().get(Index(str));
    if (indexes->empty()) 
    {
      std::cout << '\n';
    } 
    else 
    {
      std::vector<Book> ans;
      ans.resize(indexes->size());
      for (auto i = 0; i != indexes->size(); ++i) 
      {
        const std::string ISBN((*indexes)[i].ISBN);
        ans[i] = database_ISBNindex.find(Book(ISBN));
      }
      std::sort(ans.begin(), ans.end());
      for (auto i = ans.cbegin(); i != ans.cend(); ++i) 
      {
        std::cout << *i;
      }
    }
    delete indexes;
  } 
  else if (type == 3) 
  { 
    auto *const indexes = database_authorindex().get(Index(str));
    if (indexes->empty()) 
    {
      std::cout << '\n';
    } 
    else 
    {
      std::vector<Book> ans;
      ans.resize(indexes->size());
      for (auto i = 0; i != indexes->size(); ++i) 
      {
        const std::string ISBN((*indexes)[i].ISBN);
        ans[i] = database_ISBNindex.find(Book(ISBN));
      }
      std::sort(ans.begin(), ans.end());
      for (auto i = ans.cbegin(); i != ans.cend(); ++i) {
        std::cout << *i;
      }
    }
    delete indexes;
  } 
  else if (type == 4) 
  {
    auto *const indexes = database_keywordindex.get(Index(str));
    if (indexes->empty()) 
    {
      std::cout << '\n';
    } 
    else 
    {
      std::vector<Book> ans;
      ans.resize(indexes->size());
      for (auto i = 0; i != indexes->size(); ++i) 
      {
        const std::string ISBN((*indexes)[i].ISBN);
        ans[i] = database_ISBNindex.find(Book(ISBN));
      }
      std::sort(ans.begin(), ans.end());
      for (auto i = ans.cbegin(); i != ans.cend(); ++i) 
      {
        std::cout << *i;
      }
    }
    delete indexes;
  }
}

void Book_Operation::buy(const std::string &ISBN, const int &quantity) 
{
  if (accounts.telllvl() < 1) 
  {
    throw 0;
    return;
  }
  auto buy_book = database_ISBNindex.find(Book(ISBN));
  if (buy_book == Book() || buy_book.quantity < quantity || quantity <= 0) 
  {
    throw 0;
    return;
  }
  buy_book.quantity -= quantity;
  std::cout << std::fixed << std::setprecision(2) << quantity * buy_book.price << '\n';
  database_ISBNindex.update(buy_book);
  accounts.modify(buy_book, buy_book);
}
void Book_Operation::select(const std::string &ISBN) 
{
  Book &slct_book = select_book();
  slct_book = database_ISBNindex.find(Book(ISBN));
  if (strlen(slct_book.ISBN) == 0) 
  {
    strcpy(slct_book.ISBN, ISBN.c_str());
    database_ISBNindex.insert(slct_book);
  }
}
void Book_Operation::modify(const std::string &ISBN, const std::string &name, const std::string &author, const std::string &keyword, double price) {
  auto &slct_book = select_book();
  if (strlen(slct_book.ISBN) == 0) 
  {
    throw 0;
    return;
  }
  const int res = strcmp(slct_book.ISBN, ISBN.c_str());
  if (res == 0) 
  {
    throw 0;
    return;
  }
  if (ISBN != "" && database_ISBNindex.count(Book(ISBN))) {
    throw 0;
    return;
  }
  const auto book_tmp = slct_book;
  bool is_ISBN_changed = false;
  if (ISBN != "") 
  {
    const Index id_name_old(book_tmp.name, book_tmp.ISBN),
        id_author_old(book_tmp.author, book_tmp.ISBN);
    strcpy(slct_book.ISBN, ISBN.c_str());
    const Index id_name_new(book_tmp.name, slct_book.ISBN),
        id_author_new(book_tmp.author, slct_book.ISBN);
    database_nameindex().update(id_name_old, id_name_new);
    database_authorindex().update(id_author_old, id_author_new);
    is_ISBN_changed = true;
  }

  if (name != "") 
  {
    strcpy(slct_book.name, name.c_str());
    Index id_name(book_tmp.name, slct_book.ISBN);
    database_nameindex().erase(id_name);
    strcpy(id_name.index, slct_book.name);
    database_nameindex().insert(id_name);
  }

  if (author != "") 
  {
    strcpy(slct_book.author, author.c_str());
    Index id_author(book_tmp.author, slct_book.ISBN);
    database_authorindex().erase(id_author);
    strcpy(id_author.index, slct_book.author);
    database_authorindex().insert(id_author);
  }
  if (keyword != "") 
  {
    strcpy(slct_book.keyword, keyword.c_str());
    char keywords_old[61], keywords_new[61];
    const char *delim = "|";
    strcpy(keywords_old, book_tmp.keyword);
    strcpy(keywords_new, slct_book.keyword);
    auto tok = strtok(keywords_old, delim);
    while (tok != nullptr) 
    {
      database_keywordindex.erase(Index(tok, slct_book.ISBN));
      tok = strtok(nullptr, delim);
    }
    tok = strtok(keywords_new, delim);
    while (tok != nullptr) 
    {
      database_keywordindex.insert(Index(tok, slct_book.ISBN));
      tok = strtok(nullptr, delim);
    }
  }
  if (price >= 0.0) 
  {
    slct_book.price = price;
  }

  if (is_ISBN_changed) 
  {
    database_ISBNindex.erase(book_tmp);
    database_ISBNindex.insert(slct_book);
  } 
  else 
  {
    database_ISBNindex.update(slct_book);
  }
  accounts.modify(book_tmp, slct_book);
}

void Book_Operation::import(const int &quantity, const double &total_cost) 
{
  auto &slct_book = select_book();
  if (strlen(slct_book.ISBN) == 0) 
  {
    throw 0;
    return;
  }
  if (quantity <= 0 || total_cost <= 0) 
  {
    throw 0;
    return;
  }
  slct_book.quantity += quantity;
  database_ISBNindex.update(slct_book);
  accounts.modify(slct_book, slct_book);
}
void Book_Operation::select_clear() 
{ 
  select_book() = Book(); 
}
Book &Book_Operation::select_book() 
{ 
  return accounts.slct_book(); 
}
void Book_Operation::print_id_name() 
{ 
  database_nameindex().print(); 
}
void Book_Operation::print_id_author() 
{ 
  database_authorindex().print(); 
}
void Book_Operation::print_id_keyword() 
{ 
  database_keywordindex.print(); 
}
Index::Index(const std::string &index_, const std::string &ISBN_) 
{
  strcpy(index, index_.c_str());
  strcpy(ISBN, ISBN_.c_str());
}
bool Index::operator>(const Index &obj) const 
{
  return strcmp(index, obj.index) > 0;
}
bool Index::operator<(const Index &obj) const 
{
  return strcmp(index, obj.index) < 0;
}
bool Index::operator>=(const Index &obj) const 
{
  return strcmp(index, obj.index) >= 0;
}
bool Index::operator<=(const Index &obj) const 
{
  return strcmp(index, obj.index) <= 0;
}
bool Index::operator==(const Index &obj) const 
{
  return strcmp(index, obj.index) == 0;
}
Index Index::operator=(const Index &obj) 
{
  if (this == &obj) 
  {
    return *this;
  }
  strcpy(index, obj.index);
  strcpy(ISBN, obj.ISBN);
  return *this;
}
std::ostream &operator<<(std::ostream &os, const Index &obj) 
{
  os << obj.index << '\t' << obj.ISBN << '\n';
  return os;
}
bool strictly_equal(const Index &lhs, const Index &obj) 
{
  const int res1 = strcmp(lhs.index, obj.index),
            res2 = strcmp(lhs.ISBN, obj.ISBN);
  return res1 == 0 && res2 == 0;
}