#include <mm/mm.h>

#include <dlfcn.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>

class dynamic_library {
public:
  dynamic_library(const std::string& path)
  {
    lib_ = dlopen(path.c_str(), RTLD_NOW);
    if(!lib_) {
      throw std::runtime_error(dlerror());
    }
  }

  ~dynamic_library()
  {
    if(lib_) {
      dlclose(lib_);
    }
  }

  template <typename Func>
  Func *symbol(const std::string& symbol)
  {
    dlerror();
    void *sym = dlsym(lib_, symbol.c_str());
    char *err = dlerror();
    if(err) {
      throw std::runtime_error(err);
    }

    return reinterpret_cast<Func *>(sym);
  }

private:
  void *lib_;
};

template <typename Func>
void run_benchmark(Func&& harness, std::string const& path)
{
  std::cerr << "Loading matrix benchmark: " << path << '\n';
  auto start = std::chrono::system_clock::now();

  auto mat = mm::coordinate_matrix::read_from_file(path);
  auto csr = mm::csr_matrix(mm::one_based_index, mat);

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> time = end - start;
  std::cerr << "Done loading in " << time.count() << "s\n";

  auto x = std::vector<double>(csr.cols());
  auto rd = std::random_device{};
  auto dist = std::uniform_real_distribution<>(-1.0, 1.0);

  std::generate(x.begin(), x.end(), [&rd,&dist] {
    return dist(rd);
  });

  auto y = std::vector<double>(csr.rows(), 0);

  auto raw = mm::csr(csr);

  start = std::chrono::system_clock::now();
  for(auto i = 0; i < 1024; ++i) {
    harness(y.data(), raw.a, x.data(), raw.rowstr, raw.colidx, raw.rows);
  }
  end = std::chrono::system_clock::now();
  time = end - start;
  
  std::cerr << "SPMV iterations done\n";
  std::cout << time.count() << '\n';
}

using harness_t = void (double *, const double *, const double *, const int *, const int *, const int *);

void usage(char **argv)
{
  std::cerr << "Usage: " << argv[0] << " library" << " [benchmarks...]\n";
  std::cerr << "Arguments:\n";
  std::cerr << "  library: Path to a shared library with a compatible spmv_harness_ implementation\n";
  std::cerr << "  benchmarks: One or more paths to matrix market files to benchmark\n";
}

int main(int argc, char **argv)
{
  if(argc < 3) {
    usage(argv);
    return 1;
  }

  auto dyn = dynamic_library(argv[1]);
  auto harness = dyn.symbol<harness_t>("spmv_harness_");

  for(int i = 2; i < argc; ++i) {
    auto path = std::string(argv[i]);
    run_benchmark(harness, path);
  }
}
