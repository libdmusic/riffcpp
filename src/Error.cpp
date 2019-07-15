#include <riffcpp.hpp>

using namespace riffcpp;

Error::Error(const std::string &message, ErrorType type)
  : std::runtime_error(message), m_type(type) {}

ErrorType Error::type() const { return m_type; }