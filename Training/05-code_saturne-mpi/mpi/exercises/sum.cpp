#include <vector>
#include <cstdio>

int main(int argc, char** argv)
{
    constexpr int num_elements = 1000;
    std::vector<int> data(num_elements);

    for (int i=0; i < num_elements; i++)
    {
        data[i] = i;
    }

    int sum = 0;
    for (int i=0; i < num_elements; i++) {
        sum += data[i];
    }

    int sum_ref = num_elements * (num_elements - 1) / 2;
    printf("Sum is %d (ref %d)\n", sum, sum_ref);

}
