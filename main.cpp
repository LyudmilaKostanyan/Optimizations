#include <iostream>
#include <vector>
#include <chrono>

void test_optimization()
{
	const size_t size = 100000000;
	std::vector<int> data(size, 1);

	auto start = std::chrono::high_resolution_clock::now();

	long long sum = 0;
	for (size_t i = 0; i < size; ++i)
		sum += data[i];

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

#if defined(_MSC_VER)
  #define NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
  #define NOINLINE __attribute__((noinline))
#else
  #define NOINLINE
#endif

NOINLINE int add(int a, int b) {
	return a + b;
}

NOINLINE int multiply(int a, int b) {
	return a * b;
}

void test_inline_vs_no_inline() {
	volatile int result = 0;
	const int N = 100000000;

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; ++i) {
		result += add(i, i);
		multiply(i, i);
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff_no_inline = end - start;

	result = 0;
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; ++i) {
		result += add_inline(i, i);
		multiply_inline(i, i);
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff_inline = end - start;

	std::cout << "No inline time: " << diff_no_inline.count() << " s\n";
	std::cout << "Inline time: " << diff_inline.count() << " s\n";
	std::cout << "Result: " << result << "\n";
}

void test_loop_unrolling()
{
	const size_t size = 100000000;
	std::vector<int> src(size, 1);
	std::vector<int> dst(size, 0);

	auto start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < size; ++i)
		dst[i] = src[i];

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_normal = end - start;
	std::cout << "Normal copy time: " << elapsed_normal.count() << " seconds\n";

	std::fill(dst.begin(), dst.end(), 0);

	start = std::chrono::high_resolution_clock::now();
	size_t i = 0;
	for (; i + 3 < size; i += 4) {
		dst[i] = src[i];
		dst[i + 1] = src[i + 1];
		dst[i + 2] = src[i + 2];
		dst[i + 3] = src[i + 3];
	}
	for (; i < size; ++i)
		dst[i] = src[i];

	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_unrolled = end - start;
	std::cout << "Unrolled copy time: " << elapsed_unrolled.count() << " seconds\n";

	long long sum = 0;
	for (size_t j = 0; j < size; ++j)
		sum += dst[j];

	std::cout << "Sum after unrolled copy: " << sum << "\n";
}

void test_sso_performance() {
	const int iterations = 1000000;

	std::string short_str = "Hello";

	std::string long_str = "This is a very long string example!";

	auto start = std::chrono::high_resolution_clock::now();
	std::vector<std::string> short_strings;
	short_strings.reserve(iterations);
	for (int i = 0; i < iterations; ++i)
		short_strings.push_back(short_str);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> short_time = end - start;

	start = std::chrono::high_resolution_clock::now();
	std::vector<std::string> long_strings;
	long_strings.reserve(iterations);
	for (int i = 0; i < iterations; ++i)
		long_strings.push_back(long_str);

	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> long_time = end - start;

	std::cout << "Short string time: " << short_time.count() << " seconds\n";
	std::cout << "Long string time: " << long_time.count() << " seconds\n";

	std::cout << "Short string data pointer: " << static_cast<const void*>(short_strings[0].data()) << "\n";
	std::cout << "Long string data pointer: " << static_cast<const void*>(long_strings[0].data()) << "\n";
}

class Base {
public:
	virtual int foo() const {
		return 1;
	}
	virtual ~Base() = default;
};

class Derived final : public Base {
public:
	int foo() const override {
		return 2;
	}
};

void test_virtual_call(int iterations) {
	Derived d;
	Base* base_ptr = &d;
	int sum = 0;

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < iterations; ++i)
		sum += base_ptr->foo();
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end - start;
	std::cout << "Virtual call time: " << diff.count() << " s, sum=" << sum << "\n";
}

void test_direct_call(int iterations) {
	Derived d;
	int sum = 0;

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < iterations; ++i)
		sum += d.foo();
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end - start;
	std::cout << "Direct call time: " << diff.count() << " s, sum=" << sum << "\n";
}


void compute_original(const std::vector<float>& a, const std::vector<float>& b, std::vector<float>& out) {
	size_t n = a.size();
	for (size_t i = 0; i < n; ++i)
		out[i] = a[i] * 2.0f + b[i] * 3.0f - 10.0f;
}

void compute_simplified(const std::vector<float>& a, const std::vector<float>& b, std::vector<float>& out) {
	size_t n = a.size();
	for (size_t i = 0; i < n; ++i)
		out[i] = (a[i] * 2.0f) + (b[i] * 3.0f) - 10.0f;
}

void test_algorithmic_optimization() {
	const size_t size = 10000000;
	std::vector<float> a(size, 1.5f);
	std::vector<float> b(size, 2.5f);
	std::vector<float> out(size, 0.0f);

	auto start = std::chrono::high_resolution_clock::now();
	compute_original(a, b, out);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_orig = end - start;

	start = std::chrono::high_resolution_clock::now();
	compute_simplified(a, b, out);
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_simpl = end - start;

	std::cout << "Original time: " << time_orig.count() << " seconds\n";
	std::cout << "Simplified time: " << time_simpl.count() << " seconds\n";
}

void pointer_dereference(int** matrix, int rows, int cols) {
	long long sum = 0;
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			sum += *(*(matrix + i) + j);
	std::cout << "Sum (pointer dereference): " << sum << "\n";
}

void pointer_elimination(int** matrix, int rows, int cols) {
	long long sum = 0;
	for (int i = 0; i < rows; ++i) {
		int* row_ptr = matrix[i];
		for (int j = 0; j < cols; ++j)
			sum += row_ptr[j];
	}
	std::cout << "Sum (pointer elimination): " << sum << "\n";
}

void test_pointer_elimination() {
	const int rows = 1000;
	const int cols = 1000;

	int** matrix = new int*[rows];
	for (int i = 0; i < rows; ++i) {
		matrix[i] = new int[cols];
		for (int j = 0; j < cols; ++j)
			matrix[i][j] = i + j;
	}

	auto start = std::chrono::high_resolution_clock::now();
	pointer_dereference(matrix, rows, cols);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Pointer dereference time: " << std::chrono::duration<double>(end - start).count() << " s\n";

	start = std::chrono::high_resolution_clock::now();
	pointer_elimination(matrix, rows, cols);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Pointer elimination time: " << std::chrono::duration<double>(end - start).count() << " s\n";

	for (int i = 0; i < rows; ++i)
		delete[] matrix[i];
	delete[] matrix;
}

int main()
{
	std::cout << "==========================\n";
	std::cout << "Performance Optimization Tests\n";
	test_optimization();
	std::cout << "==========================\n";
	std::cout << "Inline vs No Inline Tests\n";
	test_inline_vs_no_inline();
	std::cout << "==========================\n";
	std::cout << "Loop Unrolling Tests\n";
	test_loop_unrolling();
	std::cout << "==========================\n";
	std::cout << "SSO Performance Tests\n";
	test_sso_performance();
	std::cout << "==========================\n";
	std::cout << "Virtual Call Performance Tests\n";
	test_virtual_call(10000000);
	test_direct_call(10000000);
	std::cout << "==========================\n";
	std::cout << "Algorithmic Optimization Tests\n";
	test_algorithmic_optimization();
	std::cout << "==========================\n";
	std::cout << "Pointer Dereference vs Elimination Tests\n";
	test_pointer_elimination();
	return 0;
}

