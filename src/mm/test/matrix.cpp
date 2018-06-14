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
0 0 1.02
0 1 2.34234
0 2 3.234
0 3 4.1
0 4 5.0
)";

  auto mat = coordinate_matrix::read_from_string(data);
  REQUIRE(mat(0, 0) == 1.02);
  REQUIRE(mat(0, 1) == 2.34234);
  REQUIRE(mat(0, 2) == 3.234);
  REQUIRE(mat(0, 3) == 4.1);
  REQUIRE(mat(0, 4) == 5.0);

  REQUIRE(mat(1, 1) == 0.0);
  REQUIRE(mat(3, 8) == 0.0);
  REQUIRE(mat(6, 1) == 0.0);
  REQUIRE(mat(2, 3) == 0.0);
  REQUIRE(mat(0, 7) == 0.0);
}
