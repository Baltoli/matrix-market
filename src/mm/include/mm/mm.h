#pragma once

#include <string_view>

namespace mm {

enum class object { matrix, vector };
enum class format { coordinate, array };
enum class field { real, double_, complex_, integer, pattern };
enum class symmetry { general, symmetric, skew_symmetric, hermitian };

class header {
public:
  explicit header(std::string_view header_line);

  object object_type() const;
  format format_type() const;
  field field_type() const;
  symmetry symmetry_type() const;

private:
  std::string_view read_prefix(std::string_view) const;
  std::string_view read_object(std::string_view);
  std::string_view read_format(std::string_view);
  std::string_view read_field(std::string_view);
  std::string_view read_symmetry(std::string_view);

  object object_type_;
  format format_type_;
  field field_type_;
  symmetry symmetry_type_;
};

}
