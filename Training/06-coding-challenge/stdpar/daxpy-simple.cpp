#include <chrono>
#include <cmath>
#include <vector>
#include <algorithm>
#include <execution>
#include <ranges>
#include <cstdio>
#include <string>

int main(int argc, char *argv[])
{
    const size_t n = argc > 1 ? (size_t)std::stoll(argv[1]) : 10000000;
    const size_t nit = argc > 2 ? (size_t)std::stoll(argv[2]) : 1000;
    const size_t nbytes = sizeof(double) * n;
    double a = 3.4;

    printf("Vector size %zu (%.2f MiB)\n", n, (double)nbytes / (1024*1024));
    fflush(stdout);

    std::vector<double> x(n), y(n);

    // Initialize
    for (size_t i = 0; i < n; i++) {
        x[i] = sin((double)i) * 2.3;
        y[i] = cos((double)i) * 1.1;
    };

    // Measure performance
    using clock = std::chrono::high_resolution_clock;
    auto t0 = clock::now();
    for (size_t n = 0; n < nit; n++) {
      for (size_t i = 0; i < n; i++) {
          y[i] += a * x[i];
      }
    }

    // Calculate checksum 
    double fcheck = 0.0;
    for (size_t i = 0; i < n; i++) {
        fcheck += y[i];
    }
    

    auto t1 = clock::now();
    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() / 1e9;
    double gflops = 2.0 * n * nit / time / 1e9;
    double gbytess = 3.0 * nbytes * nit / time / 1e9;

    printf("Time: %.4f s\n", time);
    printf("Performance: %.3f GFLOPS, %.3f GB/s\n", gflops, gbytess);

    // print to avoid optimizinf unused results away
    printf("fcheck %f\n", fcheck);

    return 0;
}

