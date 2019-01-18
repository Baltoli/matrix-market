#include "utils.h"

#include <mm/mm.h>

#include <array>
#include <iostream>
#include <stdexcept>

namespace mm {

header::header(std::string header_line)
{
  auto after_header = read_prefix(header_line);
  auto after_object = read_object(after_header);
  auto after_format = read_format(after_object);
  auto after_field = read_field(after_format);
  read_symmetry(after_field);
}

std::string header::read_prefix(std::string str) const
{
  auto [match, rest] = starts_with(str, "%%MatrixMarket");
  if(!match) { 
    throw std::invalid_argument("Invalid header line");
  }

  return left_trim(rest);
}

std::string header::read_object(std::string str)
{
  auto [m1, r1] = starts_with(str, "matrix");
  if(m1) { object_type_ = object::matrix; return r1; }

  auto [m2, r2] = starts_with(str, "vector");
  if(m2) { object_type_ = object::vector; return r2; }

  throw std::invalid_argument("Invalid object type");
}

std::string header::read_format(std::string str)
{
  auto [m1, r1] = starts_with(str, "coordinate");
  if(m1) { format_type_ = format::coordinate; return r1; }

  auto [m2, r2] = starts_with(str, "array");
  if(m2) { format_type_ = format::array; return r2; }

  throw std::invalid_argument("Invalid format type");
}

std::string header::read_field(std::string str)
{
  auto [m1, r1] = starts_with(str, "real");
  if(m1) { field_type_ = field::real; return r1; }

  auto [m2, r2] = starts_with(str, "double");
  if(m2) { field_type_ = field::double_; return r2; }

  auto [m3, r3] = starts_with(str, "complex");
  if(m3) { field_type_ = field::complex_; return r3; }

  auto [m4, r4] = starts_with(str, "integer");
  if(m4) { field_type_ = field::integer; return r4; }

  auto [m5, r5] = starts_with(str, "pattern");
  if(m5) { field_type_ = field::pattern; return r5; }

  throw std::invalid_argument("Invalid field type");
}

std::string header::read_symmetry(std::string str)
{
  auto [m1, r1] = starts_with(str, "general");
  if(m1) { symmetry_type_ = symmetry::general; return r1; }

  auto [m2, r2] = starts_with(str, "symmetric");
  if(m2) { symmetry_type_ = symmetry::symmetric; return r2; }

  auto [m3, r3] = starts_with(str, "skew-symmetric");
  if(m3) { symmetry_type_ = symmetry::skew_symmetric; return r3; }

  auto [m4, r4] = starts_with(str, "hermitian");
  if(m4) { symmetry_type_ = symmetry::hermitian; return r4; }

  throw std::invalid_argument("Invalid symmetry type");
}

object header::object_type() const
{
  return object_type_;
}

format header::format_type() const
{
  return format_type_;
}

field header::field_type() const
{
  return field_type_;
}

symmetry header::symmetry_type() const
{
  return symmetry_type_;
}

}
