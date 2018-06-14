#include "utils.h"

#include <mm/mm.h>

namespace mm {

std::unique_ptr<coordinate_matrix> 
coordinate_matrix::read_from_string(std::string_view data)
{
  auto mat = std::unique_ptr<coordinate_matrix>{new coordinate_matrix{}};

  auto all_lines = lines(data);
  auto line = all_lines.begin();
  auto data_header = header(*line++);

  // TODO: validate header

  while(line->at(0) == '%') {
    ++line;
  }

  auto size_line = *line;
  auto col = columns(size_line).begin();
  ++col; ++col;
  auto nnz = std::atoi(col->data());

  for(auto i = 0; line != all_lines.end() && i < nnz; ++line, ++nnz) {
    mat->process_line(left_trim(*line));
  }

  return mat;
}

void coordinate_matrix::process_line(std::string_view line)
{
  auto col = columns(line).begin();
  auto row = std::atoi((col++)->data());
}

double coordinate_matrix::operator()(size_t x, size_t y) const
{
  return 0.0;
}

size_t coordinate_matrix::rows() const
{
}

size_t coordinate_matrix::cols() const
{
}

}
