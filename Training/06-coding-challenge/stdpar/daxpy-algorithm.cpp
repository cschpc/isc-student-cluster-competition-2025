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
    const size_t nelem = argc > 1 ? (size_t)std::stoll(argv[1]) : 10000000;
    const size_t nit = argc > 2 ? (size_t)std::stoll(argv[2]) : 100;
    const size_t nbytes = sizeof(double) * nelem;
    double a = 3.4;

    printf("Vector size %zu (%.2f MiB)\n", nelem, (double)nbytes / (1024*1024));
    fflush(stdout);

    std::vector<double> x(nelem), y(nelem);

    // Initialize
    for (size_t i = 0; i < nelem; i++) {
        x[i] = sin((double)i) * 2.3;
        y[i] = cos((double)i) * 1.1;
    };

    // Measure performance
    using clock = std::chrono::high_resolution_clock;
    auto t0 = clock::now();
//    auto kernel = [=, x = x.data(), y = y.data()](size_t i) {
    auto kernel = [=, &x, &y](size_t i) {
        y[i] += a * x[i];
    };
    auto start = std::views::iota(0);
    for (size_t n = 0; n < nit; n++) {
      std::for_each_n(start.begin(), nelem, kernel);
    }

    // Calculate checksum 
    double fcheck = 0.0;
    for (size_t i = 0; i < nelem; i++) {
        fcheck += y[i];
    }
    

    auto t1 = clock::now();
    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() / 1e9;
    double gflops = 2.0 * nelem * nit / time / 1e9;
    double gbytess = 3.0 * nbytes * nit / time / 1e9;

    printf("Time: %.4f s\n", time);
    printf("Performance: %.3f GFLOPS, %.3f GB/s\n", gflops, gbytess);

    // print to avoid optimizinf unused results away
    printf("fcheck %f\n", fcheck);

    return 0;
}

