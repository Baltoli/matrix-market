#include "catch.h"

#include <mm/mm.h>

#include <iostream>

using namespace mm;

TEST_CASE("can use coordinate matrices")
{
  SECTION("with general symmetry") {
    auto data = R"(
%%MatrixMarket matrix coordinate real general
% Can have some comment lines after the header
% and another
%
% one final one
% rows cols nnz
10 8 5
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
    REQUIRE(mat(3, 7) == 0.0);
    REQUIRE(mat(6, 1) == 0.0);
    REQUIRE(mat(2, 3) == 0.0);
    REQUIRE(mat(0, 7) == 0.0);

    REQUIRE(mat.cols() == 8);
    REQUIRE(mat.rows() == 10);
  }

  SECTION("with symmetric symmetry") {
    auto data = R"(
%%MatrixMarket matrix coordinate real symmetric
% Can have some comment lines after the header
% and another
%
% one final one
% rows cols nnz
10 10 5
0 0 1.02
2 1 2.34234
3 2 3.234
)";
    auto mat = coordinate_matrix::read_from_string(data);
    REQUIRE(mat(0, 0) == 1.02);
    REQUIRE(mat(2, 1) == 2.34234);
    REQUIRE(mat(2, 1) == mat(1, 2));
    REQUIRE(mat(3, 2) == 3.234);
    REQUIRE(mat(3, 2) == mat(2, 3));
  }

  SECTION("with skew-symmetric symmetry") {
    auto data = R"(
%%MatrixMarket matrix coordinate real skew-symmetric
% Can have some comment lines after the header
% and another
%
% one final one
% rows cols nnz
10 10 5
0 4 1.02
2 1 2.34234
3 2 3.234
)";
    auto mat = coordinate_matrix::read_from_string(data);
    REQUIRE(mat(0, 4) == 1.02);
    REQUIRE(mat(0, 4) == -mat(4, 0));
    REQUIRE(mat(2, 1) == 2.34234);
    REQUIRE(mat(2, 1) == -mat(1, 2));
    REQUIRE(mat(3, 2) == 3.234);
    REQUIRE(mat(3, 2) == -mat(2, 3));
  }
}

TEST_CASE("can construct CSR matrices")
{
  SECTION("using C indexing for triples") {
    auto data = R"(
%%MatrixMarket matrix coordinate real general
8 12 6
0 0 1.0
0 7 2.0
1 11 3.0
5 3 4.0
5 4 5.0
5 8 6.0
)";

    auto coo = coordinate_matrix::read_from_string(data);
    auto csr = csr_matrix(coo);

    REQUIRE(csr.nnz() == 6);

    REQUIRE(csr(0, 0) == 1.0);
    REQUIRE(csr(0, 7) == 2.0);
    REQUIRE(csr(1, 11) == 3.0);
    REQUIRE(csr(5, 3) == 4.0);
    REQUIRE(csr(5, 4) == 5.0);
    REQUIRE(csr(5, 8) == 6.0);

    REQUIRE(csr.values() == std::vector<double>{{1.0, 2.0, 3.0, 4.0, 5.0, 6.0}});
    REQUIRE(csr.colidx() == std::vector<int>{{0, 7, 11, 3, 4, 8}});
    REQUIRE(csr.rowptr() == std::vector<int>{{0, 2, 3, 3, 3, 3, 6, 6, 6}});
  }

  SECTION("using fortran indexing for triples") {
    auto data = R"(
%%MatrixMarket matrix coordinate real general
12 8 6
1 0 0.1
1 1 0.2
1 6 0.3
1 7 0.4
8 3 0.5
11 7 0.6
)";

    auto coo = coordinate_matrix::read_from_string(data);
    auto csr = csr_matrix(one_based_index, coo);

    REQUIRE(csr.nnz() == 6);
    REQUIRE(csr(2, 1) == 0.1);
    REQUIRE(csr(2, 2) == 0.2);
    REQUIRE(csr(2, 7) == 0.3);
    REQUIRE(csr(2, 8) == 0.4);
    REQUIRE(csr(9, 4) == 0.5);
    REQUIRE(csr(12, 8) == 0.6);

    REQUIRE(csr.values() == std::vector<double>{{0.1, 0.2, 0.3, 0.4, 0.5, 0.6}});
    REQUIRE(csr.colidx() == std::vector<int>{{1, 2, 7, 8, 4, 8}});
    REQUIRE(csr.rowptr() == std::vector<int>{{1, 1, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 7}});
  }
}

TEST_CASE("can load matrices from file")
{
  auto path = RESOURCE_DIR "/test/test.mtx";

  auto coo = coordinate_matrix::read_from_file(path);

  REQUIRE(coo.rows() == 9);
  REQUIRE(coo.cols() == 10);

  REQUIRE(coo(0, 0) == 1.00);
  REQUIRE(coo(1, 2) == 2.00);
  REQUIRE(coo(5, 2) == 3.00);
  REQUIRE(coo(5, 6) == 4.00);
  REQUIRE(coo(7, 1) == 5.00);
  REQUIRE(coo(8, 8) == 6.00);
}
