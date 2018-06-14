#pragma once

#include <map>
#include <memory>
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

class matrix {
public:
  virtual double operator()(size_t x, size_t y) const = 0;
  virtual size_t rows() const = 0;
  virtual size_t cols() const = 0;
};

class coordinate_matrix : public matrix {
public:
  double operator()(size_t x, size_t y) const override;
  size_t rows() const override;
  size_t cols() const override;

  static std::unique_ptr<coordinate_matrix> read_from_string(std::string_view data);

private:
  coordinate_matrix() = default;

  size_t rows_ = 0;
  size_t cols_ = 0;
  std::map<std::pair<size_t, size_t>, double> entries = {};
};

}
