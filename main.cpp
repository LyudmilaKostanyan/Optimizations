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

inline int add_inline(int a, int b) __attribute__((always_inline));
inline int add_inline(int a, int b) {
    return a + b;
}

inline int multiply_inline(int a, int b) __attribute__((always_inline));
inline int multiply_inline(int a, int b) {
    return a * b;
}

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

void test_inline()
{
	volatile int result = 0;
	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 100000000; ++i) {
		result += add(i, i);
		result += multiply(i, i);
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end - start;

	std::cout << "Result: " << result << "\n";
	std::cout << "Elapsed time: " << diff.count() << " seconds\n";

	result = 0;
	start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 100000000; ++i) {
		result += add_inline(i, i);
		result += multiply_inline(i, i);
	}

	end = std::chrono::high_resolution_clock::now();
	diff = end - start;

	std::cout << "Result with inline: " << result << "\n";
	std::cout << "Elapsed time with inline: " << diff.count() << " seconds\n";
}

int main()
{
	test_optimization();
	test_inline();
	return 0;
}

