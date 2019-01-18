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
  auto pair = starts_with(str, "%%MatrixMarket");

  auto match = pair.first;
  auto rest = pair.second;

  if(!match) { 
    throw std::invalid_argument("Invalid header line");
  }

  return left_trim(rest);
}

std::string header::read_object(std::string str)
{
  auto pair1 = starts_with(str, "matrix");
  auto m1 = pair1.first;
  auto r1 = pair1.second;
  if(m1) { object_type_ = object::matrix; return r1; }

  auto pair2 = starts_with(str, "vector");
  auto m2 = pair2.first;
  auto r2 = pair2.second;
  if(m2) { object_type_ = object::vector; return r2; }

  throw std::invalid_argument("Invalid object type");
}

std::string header::read_format(std::string str)
{
  auto pair1 = starts_with(str, "coordinate");
  auto m1 = pair1.first;
  auto r1 = pair1.second;
  if(m1) { format_type_ = format::coordinate; return r1; }

  auto pair2 = starts_with(str, "array");
  auto m2 = pair2.first;
  auto r2 = pair2.second;
  if(m2) { format_type_ = format::array; return r2; }

  throw std::invalid_argument("Invalid format type");
}

std::string header::read_field(std::string str)
{
  auto pair1 = starts_with(str, "real");
  auto m1 = pair1.first;
  auto r1 = pair1.second;
  if(m1) { field_type_ = field::real; return r1; }

  auto pair2 = starts_with(str, "double");
  auto m2 = pair2.first;
  auto r2 = pair2.second;
  if(m2) { field_type_ = field::double_; return r2; }

  auto pair3 = starts_with(str, "complex");
  auto m3 = pair3.first;
  auto r3 = pair3.second;
  if(m3) { field_type_ = field::complex_; return r3; }

  auto pair4 = starts_with(str, "integer");
  auto m4 = pair4.first;
  auto r4 = pair4.second;
  if(m4) { field_type_ = field::integer; return r4; }

  auto pair5 = starts_with(str, "pattern");
  auto m5 = pair5.first;
  auto r5 = pair5.second;
  if(m5) { field_type_ = field::pattern; return r5; }

  throw std::invalid_argument("Invalid field type");
}

std::string header::read_symmetry(std::string str)
{
  auto pair1 = starts_with(str, "general");
  auto m1 = pair1.first;
  auto r1 = pair1.second;
  if(m1) { symmetry_type_ = symmetry::general; return r1; }

  auto pair2 = starts_with(str, "symmetric");
  auto m2 = pair2.first;
  auto r2 = pair2.second;
  if(m2) { symmetry_type_ = symmetry::symmetric; return r2; }

  auto pair3 = starts_with(str, "skew-symmetric");
  auto m3 = pair3.first;
  auto r3 = pair3.second;
  if(m3) { symmetry_type_ = symmetry::skew_symmetric; return r3; }

  auto pair4 = starts_with(str, "hermitian");
  auto m4 = pair4.first;
  auto r4 = pair4.second;
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
