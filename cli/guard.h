#include <exception>
#include <iostream>

#include <boost/exception/all.hpp>

#include <nit/error.h>

/// Guards a main function from exceptions.
///
/// @tparam T  Function type returning integer code.
///
/// @param[in] runner  The body of the main function.
///
/// @returns The return of the runner if no exceptions are encountered.
///          1 if an exception is encountered.
template <class T>
int guard(T&& runner) noexcept {
  try {
    return runner();
  } catch (const nit::Error& nit_err) {
    std::cerr << boost::diagnostic_information(nit_err);
    return 1;
  } catch (const boost::exception& boost_err) {
    std::cerr << "Unexpected Boost Exception:\n"
              << boost::diagnostic_information(boost_err);
    return 1;
  } catch (const std::exception& err) {
    std::cerr << "Unexpected Exception:\n" << err.what();
    return 1;
  }
}
