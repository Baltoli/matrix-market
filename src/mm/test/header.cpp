#include "catch.h"

#include <mm/mm.h>

using namespace mm;

TEST_CASE("constructing headers")
{
  SECTION("cannot construct with invalid prefixes") {
    REQUIRE_THROWS_AS(header("gfhwwef"), std::invalid_argument);
    REQUIRE_THROWS_AS(header(""), std::invalid_argument);
    REQUIRE_THROWS_AS(header("%MatrixMarket"), std::invalid_argument);
    REQUIRE_THROWS_AS(header("%%matrix_market"), std::invalid_argument);
    REQUIRE_THROWS_AS(header("%%matrixMarket"), std::invalid_argument);
  }

  SECTION("cannot construct with invalid object type") {
    REQUIRE_THROWS_AS(header("%%MatrixMarket thing array real general"), std::invalid_argument);
  }

  SECTION("cannot construct with invalid object type") {
    REQUIRE_THROWS_AS(header("%%MatrixMarket matrix fjiowe real general"), std::invalid_argument);
  }

  SECTION("cannot construct with invalid field type") {
    REQUIRE_THROWS_AS(header("%%MatrixMarket matrix array wefj general"), std::invalid_argument);
  }

  SECTION("cannot construct with invalid symmetry type") {
    REQUIRE_THROWS_AS(header("%%MatrixMarket matrix array real woo"), std::invalid_argument);
  }

  SECTION("can construct with valid headers") {
    auto h1 = header("%%MatrixMarket matrix coordinate real general");
    REQUIRE(h1.object_type() == object::matrix);
    REQUIRE(h1.format_type() == format::coordinate);
    REQUIRE(h1.field_type() == field::real);
    REQUIRE(h1.symmetry_type() == symmetry::general);

    auto h2 = header("%%MatrixMarket vector array double symmetric");
    REQUIRE(h2.object_type() == object::vector);
    REQUIRE(h2.format_type() == format::array);
    REQUIRE(h2.field_type() == field::double_);
    REQUIRE(h2.symmetry_type() == symmetry::symmetric);
    
    auto h3 = header("%%MatrixMarket matrix array complex skew_symmetric");
    REQUIRE(h3.object_type() == object::matrix);
    REQUIRE(h3.format_type() == format::array);
    REQUIRE(h3.field_type() == field::complex_);
    REQUIRE(h3.symmetry_type() == symmetry::skew_symmetric);

    auto h4 = header("%%MatrixMarket matrix array integer hermitian");
    REQUIRE(h4.object_type() == object::matrix);
    REQUIRE(h4.format_type() == format::array);
    REQUIRE(h4.field_type() == field::integer);
    REQUIRE(h4.symmetry_type() == symmetry::hermitian);

    auto h5 = header("%%MatrixMarket vector coordinate pattern general");
    REQUIRE(h5.object_type() == object::vector);
    REQUIRE(h5.format_type() == format::coordinate);
    REQUIRE(h5.field_type() == field::pattern);
    REQUIRE(h5.symmetry_type() == symmetry::general);
  }
}
