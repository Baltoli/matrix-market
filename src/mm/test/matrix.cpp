#include "catch.h"

#include <mm/mm.h>

using namespace mm;

TEST_CASE("can use coordinate matrices")
{
  auto data = R"(
%%MatrixMarket matrix coordinate real general
% Can have some comment lines after the header
% and another
%
% one final one
% rows cols nnz
10 10 5
0 0 1
0 1 2
0 2 3
0 3 4
0 4 5
)";

  coordinate_matrix::read_from_string(data);
}
