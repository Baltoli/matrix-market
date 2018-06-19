#include "utils.h"

#include <mm/mm.h>

namespace mm {

coordinate_matrix coordinate_matrix::read_from_string(std::string_view data)
{
  auto mat = coordinate_matrix{};

  auto all_lines = lines(data);
  auto line = all_lines.begin();
  auto data_header = header(*line++);

  // TODO: validate header

  while(line->at(0) == '%') {
    ++line;
  }

  auto size_line = *line++;
  auto col = columns(size_line).begin();

  mat.rows_ = std::atoi((col++)->data());
  mat.cols_ = std::atoi((col++)->data());
  auto nnz = std::atoi(col->data());

  for(auto i = 0; line != all_lines.end() && i < nnz; ++line, ++nnz) {
    mat.process_line(left_trim(*line));
  }

  return mat;
}

void coordinate_matrix::process_line(std::string_view line)
{
  auto it = columns(line).begin();
  auto row = std::atoi((it++)->data());
  auto col = std::atoi((it++)->data());
  auto val = std::stod(it->data());

  entries_.insert_or_assign({row, col}, val);
}

double coordinate_matrix::operator()(size_t row, size_t col) const
{
  auto found = entries_.find({row, col});
  if(found == entries_.end()) {
    return 0.0;
  }

  return entries_.at({row, col});
}

size_t coordinate_matrix::rows() const
{
  return rows_;
}

size_t coordinate_matrix::cols() const
{
  return cols_;
}

csr_matrix::csr_matrix(coordinate_matrix const& coo) :
  csr_matrix(0, coo)
{
}

csr_matrix::csr_matrix(one_based_index_t tag, coordinate_matrix const& coo) :
  csr_matrix(1, coo)
{
}

csr_matrix::csr_matrix(size_t o, coordinate_matrix const& coo) :
 offset_(o),  rows_(coo.rows()), cols_(coo.cols())
{
  rowptr_.push_back(offset_);

  for(auto row = 0; row < rows_; ++row) {
    size_t row_count = 0;

    for(auto col = 0; col < cols_; ++col) {
      auto val = coo(row, col);
      if(val != 0) {
        row_count++;
        nnz_++;

        colidx_.push_back(col + offset_);
        values_.push_back(val);
      }
    }

    rowptr_.push_back(row_count + rowptr_.back());
  }
}

double csr_matrix::operator()(size_t row, size_t col) const
{
  row = row - offset_;

  for(auto i = rowptr().at(row) - offset_; 
      i < rowptr().at(row + 1); 
      ++i) 
  {
    if(colidx().at(i) == col) {
      return values().at(i);
    }
  }

  return 0.0;
}

size_t csr_matrix::rows() const
{
  return rows_;
}

size_t csr_matrix::cols() const
{
  return cols_;
}

size_t csr_matrix::nnz() const
{
  return nnz_;
}

std::vector<double> const& csr_matrix::values() const
{
  return values_;
}

std::vector<int> const& csr_matrix::rowptr() const
{
  return rowptr_;
}

std::vector<int> const& csr_matrix::colidx() const
{
  return colidx_;
}

csr::csr(csr_matrix const& coo)
  : a(coo.values().data()), 
    rowstr(coo.rowptr().data()), colidx(coo.colidx().data()),
    rows(new int(coo.rows()))
{
}

csr::~csr()
{
  delete rows;
}

one_based_index_t one_based_index = {};

}
