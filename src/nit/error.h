/*
 * Copyright (C) 2018 Olzhas Rakhimov
 */

/// @file
/// Exceptions for Nit.

#ifndef NIT_ERROR_H_
#define NIT_ERROR_H_

#include <exception>
#include <string>

#include <boost/exception/exception.hpp>
#include <boost/exception/info.hpp>

namespace nit {

/// The generic tag to carry an erroneous value.
/// Use this tag only if another more-specific error tag is not available.
using errinfo_value = boost::error_info<struct tag_value, std::string>;

/// Base class for exceptions specific to Nit.
class Error : virtual public std::exception, virtual public boost::exception {
 public:
  /// Constructs a new error with a provided message.
  ///
  /// @param[in] msg  The message to be passed with this error.
  explicit Error(std::string msg) : msg_(std::move(msg)) {}

  /// @returns The formatted error message to be printed.
  const char* what() const noexcept final { return msg_.c_str(); }

 private:
  std::string msg_;  ///< The error message.
};

/// Errors in parsing text, files, strings into data.
struct ParseError : public Error {
  using Error::Error;
};

/// Errors for invalid arguments.
struct InvalidArgument : public Error {
  using Error::Error;
};

/// Invalid domain for arguments or values.
struct DomainError : public InvalidArgument {
  using InvalidArgument::InvalidArgument;
};

/// Signals internal errors or pre-condition failure.
struct LogicError : public Error {
  using Error::Error;
};

}  // namespace nit

#endif  // NIT_ERROR_H_
