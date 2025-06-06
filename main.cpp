#include <iostream>
#include <vector>
#include <chrono>

void test_optimization()
{
	const size_t size = 100000000;
	std::vector<int> data(size, 1);

	auto start = std::chrono::high_resolution_clock::now();

	long long sum = 0;
	for (size_t i = 0; i < size; ++i) {
		sum += data[i];
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;

	std::cout << "Sum: " << sum << "\n";
	std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline __attribute__((always_inline))
#endif

FORCE_INLINE int add_inline(int a, int b) {
    return a + b;
}

FORCE_INLINE int multiply_inline(int a, int b) {
    return a * b;
}


int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

void test_inline_vs_no_inline() {
    volatile int result = 0;
    const int N = 100000000;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        result += add(i, i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_no_inline = end - start;

    result = 0;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        result += add_inline(i, i);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_inline = end - start;

    std::cout << "No inline time: " << diff_no_inline.count() << " s\n";
    std::cout << "Inline time: " << diff_inline.count() << " s\n";
    std::cout << "Result: " << result << "\n";
}

int main()
{
	test_optimization();
	test_inline_vs_no_inline();
	return 0;
}

