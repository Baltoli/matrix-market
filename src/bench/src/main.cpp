#include <mm/mm.h>

#include <chrono>
#include <iostream>
#include <string>

void run_benchmark(std::string const& path)
{
  std::cerr << "Loading matrix benchmark: " << path << '\n';
  auto start = std::chrono::system_clock::now();

  auto mat = mm::coordinate_matrix::read_from_file(path);
  auto csr = mm::csr_matrix(mm::one_based_index, mat);

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> time = end - start;
  std::cerr << "Done in " << time.count() << "s\n";
}

int main(int argc, char **argv)
{
  for(int i = 1; i < argc; ++i) {
    auto path = std::string(argv[i]);
    run_benchmark(path);
  }
}
