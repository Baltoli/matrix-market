#pragma once

#include <map>
#include <memory>
#include <string_view>
#include <vector>

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

  static coordinate_matrix read_from_string(std::string_view data);

private:
  coordinate_matrix() = default;
  
  void process_line(std::string_view line, symmetry sym);

  size_t rows_ = 0;
  size_t cols_ = 0;
  std::map<std::pair<size_t, size_t>, double> entries_ = {};
};

struct one_based_index_t {};
extern one_based_index_t one_based_index;

class csr_matrix : public matrix {
public:
  explicit csr_matrix(coordinate_matrix const& coo);
  csr_matrix(one_based_index_t tag, coordinate_matrix const& coo);

  double operator()(size_t x, size_t y) const override;
  size_t rows() const override;
  size_t cols() const override;

  std::vector<double> const& values() const;
  std::vector<int> const& rowptr() const;
  std::vector<int> const& colidx() const;

  size_t nnz() const;

private:
  csr_matrix(size_t offset, coordinate_matrix const& coo);

  std::vector<double> values_;
  std::vector<int> rowptr_;
  std::vector<int> colidx_;

  size_t offset_;
  size_t rows_;
  size_t cols_;
  size_t nnz_ = 0;
};

struct csr {
  csr(csr_matrix const&);
  ~csr();

  double const * const a;
  int const * const rowstr;
  int const * const colidx;
  int const * const rows;
};

}
