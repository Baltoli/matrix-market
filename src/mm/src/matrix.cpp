#include <mm/mm.h>

namespace mm {

std::unique_ptr<coordinate_matrix> 
coordinate_matrix::read_from_string(std::string_view data)
{
  auto mat = std::unique_ptr<coordinate_matrix>{new coordinate_matrix{}};
  return mat;
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
