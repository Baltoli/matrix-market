#include "utils.h"

#include <mm/mm.h>

#include <algorithm>
#include <fstream>

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

  for(auto i = 0; line != all_lines.end() && i < nnz; ++line, ++i) {
    mat.process_line(left_trim(*line), data_header.symmetry_type(), data_header.field_type());
  }

  return mat;
}

coordinate_matrix coordinate_matrix::read_from_file(std::string const& filename)
{
  auto mat = coordinate_matrix{};
  
  auto in_stream = std::ifstream(filename);
  auto line = std::string{};

  std::getline(in_stream, line);
  auto data_header = header(line);

  do {
    std::getline(in_stream, line);
  } while(line.at(0) == '%');

  auto col = columns(line).begin();
  mat.rows_ = std::atoi((col++)->data());
  mat.cols_ = std::atoi((col++)->data());
  auto nnz = std::atoi(col->data());

  for(auto i = 0; std::getline(in_stream, line) && i < nnz; ++i) {
    mat.process_line(left_trim(line), data_header.symmetry_type(), data_header.field_type());
  }

  return mat;
}

void coordinate_matrix::process_line(std::string_view line, symmetry sym, field f)
{
  auto it = columns(line).begin();
  auto row = std::atoi((it++)->data());
  auto col = std::atoi((it++)->data());

  auto val = [f,it] {
    if(f == field::pattern) {
      return 1.0;
    } else {
      return std::stod(it->data());
    }
  }();

  if(sym == symmetry::general) {
    entries_.insert_or_assign({row, col}, val);
  } else if(sym == symmetry::symmetric) {
    entries_.insert_or_assign({row, col}, val);
    entries_.insert_or_assign({col, row}, val);
  } else if(sym == symmetry::skew_symmetric) {
    entries_.insert_or_assign({row, col}, val);
    entries_.insert_or_assign({col, row}, -val);
  }
}

void coordinate_matrix::normalise()
{
  auto lim = std::min(rows(), cols());

  /* for(auto i = 0; i < lim; ++i) { */
  /*   entries_.insert_or_assign({i, i}, 0); */
  /* } */
  
  auto row_totals = std::vector<double>(rows() + 1, 0.0);
  
  for(auto& entry : entries_) {
    if(entry.first.first == entry.first.second) {
      entry.second = 0.0;
    }
    row_totals.at(entry.first.first) += entry.second;
  }

  for(auto& entry : entries_) {
    entry.second = entry.second / row_totals.at(entry.first.first);
  }

  for(auto i = 0; i <= rows(); ++i) {
    if(row_totals.at(i) == 0) {
      entries_.insert_or_assign({i, (i == 0 ? 1 : 0)}, 1.0);
    }
  }

  /*
  for(auto row = 0; row < rows(); ++row) {
    auto total = 0.0;
    for(auto col = 0; col < cols(); ++col) {
      total += (*this)(row, col);
    }

    if(total == 0.0) {
      auto off_diag = [&] {
        if(row == 0) { return 1; }
        else { return row - 1; } 
      }();
      entries_.insert_or_assign({row, off_diag}, 1.0);
    } else {
      for(auto col = 0; col < cols(); ++col) {
        auto found = entries_.find({row, col});
        if(found != entries_.end()) {
          entries_.at({row, col}) /= total;
        }
      }
    }
  }
  */
}

double coordinate_matrix::operator()(size_t row, size_t col) const
{
  auto found = entries_.find({row, col});
  if(found == entries_.end()) {
    return 0.0;
  }

  return entries_.at({row, col});
}

std::map<std::pair<size_t, size_t>, double> const& coordinate_matrix::entries() const
{
  return entries_;
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

  size_t current_row = 0;
  size_t running_total = rowptr_.front();

  for(auto e : coo.entries()) {
    for( ; current_row < e.first.first; ++current_row) {
      rowptr_.push_back(running_total);
    }

    running_total++;
    colidx_.push_back(e.first.second + offset_);
    values_.push_back(e.second);
  }

  for( ; current_row < rows_; ++current_row) {
    rowptr_.push_back(running_total);
  }

  nnz_ = running_total - offset_;
}

double csr_matrix::operator()(size_t row, size_t col) const
{
  row = row - offset_;

  for(auto i = rowptr().at(row) - static_cast<int>(offset_);
      i < rowptr().at(row + 1); 
      ++i) 
  {
    if(colidx().at(i) == static_cast<int>(col)) {
      return values().at(i);
    }
  }

  return 0.0;
}

void csr_matrix::scale(double d)
{
  for(auto i = 0; i < nnz_; ++i) {
    values_[i] *= d;
  }
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
