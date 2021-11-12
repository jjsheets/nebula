// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_EXCEPTIONS_H
#define NEBULA_EXCEPTIONS_H

#include <stdexcept>
#include <string>

struct sqlite3;

namespace nebula {

class glfwException : public std::runtime_error {
  std::string _description;
  int _code;

public:
  explicit glfwException();
  explicit glfwException(const std::string &logMsg);
  explicit glfwException(const glfwException &other) noexcept
      : std::runtime_error("")
  {
    _description = other._description;
    _code        = other._code;
  }
  virtual const char *what() const noexcept
  {
    return _description.c_str();
  }
  glfwException &operator=(const glfwException &other) noexcept
  {
    _description = other._description;
    _code        = other._code;
    return *this;
  }
  bool operator==(int other) noexcept
  {
    return _code == other;
  }
};

class sqliteException : public std::runtime_error {
  std::string _description;
  int _code;

public:
  explicit sqliteException(sqlite3 *db);
  explicit sqliteException(int err);
  sqliteException(const sqliteException &other) : std::runtime_error("")
  {
    _description = other._description;
    _code        = other._code;
  }
  virtual const char *what() const noexcept
  {
    return _description.c_str();
  }
  sqliteException &operator=(const sqliteException &other) noexcept
  {
    _description = other._description;
    _code        = other._code;
    return *this;
  }
  bool operator==(int other) noexcept
  {
    return _code == other;
  }
};

class nebulaException : public std::runtime_error {
  std::string _description;

public:
  explicit nebulaException(const std::string &description);
  nebulaException(const nebulaException &other) : std::runtime_error("")
  {
    _description = other._description;
  }
  virtual const char *what() const noexcept
  {
    return _description.c_str();
  }
  nebulaException &operator=(const nebulaException &other) noexcept
  {
    _description = other._description;
    return *this;
  }
};

} // namespace nebula
#endif // NEBULA_EXCEPTIONS_H
