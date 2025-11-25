#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <exception>
#include <string>
#include <cstdint>

namespace srilakshmikanthanp::clipbirdesk::common::types::exceptions {
/**
 * @brief Exception for Invalid Packet
 */
class MalformedPacket : public std::exception {
 private:

  std::string message;
  std::uint8_t code;

 public:

  // Constructor
  MalformedPacket(std::uint8_t code, const std::string& message) noexcept;

  // get message
  const char* what() const noexcept override;

  // get Code
  std::uint8_t getCode() const noexcept;

  // Virtual destructor for inheritance
  virtual ~MalformedPacket() noexcept = default;
};

/**
 * @brief NotThisPacket exception
 */
class NotThisPacket : public std::exception {
 private:

  std::string message;

 public:

  // Constructor
  NotThisPacket(const std::string& message) noexcept;

  // get message
  const char* what() const noexcept override;
};

/**
 * @brief Exception for Not supported type
 */
class NotSupported : public std::exception {
 private:

  std::string message;

 public:

  // Constructor
  NotSupported(const std::string& message) noexcept;

  // get message
  const char* what() const noexcept override;
};

class ErrorCodeException : public std::exception {
 private:

  std::string message;
  int code;

 public:

  // Constructor
  ErrorCodeException(int code, const std::string& message) noexcept;

  // get message
  const char* what() const noexcept override;

  // get Code
  int getCode() const noexcept;

  // Virtual destructor for inheritance
  virtual ~ErrorCodeException() noexcept = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::common::types::exceptions
