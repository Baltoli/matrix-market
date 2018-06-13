project(libmm VERSION 0.1 LANGUAGES CXX)

add_library(mm
  src/header.cpp
  src/matrix.cpp
  src/utils.cpp)

target_include_directories(mm PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

target_compile_features(mm
  PUBLIC cxx_std_17
)

install(TARGETS mm EXPORT MatrixMarketConfig
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT MatrixMarketConfig DESTINATION share/MatrixMarket/cmake)

export(TARGETS mm FILE MatrixMarketConfig.cmake)

add_executable(test-mm
  test/main.cpp
  test/header.cpp)

target_link_libraries(test-mm
  mm)

add_test(test-mm test-mm)
