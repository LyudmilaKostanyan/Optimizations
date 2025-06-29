#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

void print_header(const std::string& title)
{
	std::cout << "\n" << std::setw(60) << std::setfill('=') << "\n";
	std::cout << std::setfill(' ') << std::setw((60 + title.length()) / 2) << title << "\n";
	std::cout << std::setw(60) << std::setfill('=') << "\n";
}

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
	std::cout << "Long string time: " << long_time.count() << " seconds\n\n";

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
	std::cout << "Virtual call time: " << diff.count() << "\n";
}

void test_direct_call(int iterations) {
	Derived d;
	int sum = 0;

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < iterations; ++i)
		sum += d.foo();
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end - start;
	std::cout << "Direct call time: " << diff.count() << "\n";
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
}

void pointer_elimination(int** matrix, int rows, int cols) {
	long long sum = 0;
	for (int i = 0; i < rows; ++i) {
		int* row_ptr = matrix[i];
		for (int j = 0; j < cols; ++j)
			sum += row_ptr[j];
	}
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

void test_fragmentation(size_t iterations, size_t max_block_size) {
	std::vector<char*> blocks;
	blocks.reserve(iterations);

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < iterations; ++i) {
		size_t sz = 1 + std::rand() % max_block_size;
		blocks.push_back(new char[sz]);
		if (i % 10 == 0 && !blocks.empty()) {
			size_t idx = std::rand() % blocks.size();
			delete[] blocks[idx];
			blocks.erase(blocks.begin() + idx);
		}
	}

	for (auto ptr : blocks) {
		delete[] ptr;
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "Fragmentation test done in "
		<< std::chrono::duration<double>(end - start).count()
		<< " seconds\n";
}

void test_contiguous_allocation(size_t iterations, size_t block_size) {
	std::vector<char> memory_pool(iterations * block_size);

	auto start = std::chrono::high_resolution_clock::now();

	long long sum = 0;
	for (size_t i = 0; i < iterations; ++i) {
		char* block = memory_pool.data() + (i * block_size);
		for (size_t j = 0; j < block_size; ++j) {
			block[j] = static_cast<char>(j);
			sum += block[j];
		}
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "Contiguous allocation test done in "
		<< std::chrono::duration<double>(end - start).count()
		<< " seconds\n";
}

int main()
{
	print_header("Performance Optimization Tests");
	test_optimization();

	print_header("Inline vs No Inline Tests");
	test_inline_vs_no_inline();

	print_header("Loop Unrolling Tests");
	test_loop_unrolling();

	print_header("Short-String Optimization Tests");
	test_sso_performance();

	print_header("Virtual vs Direct Call Tests");
	test_virtual_call(10000000);
	test_direct_call(10000000);

	print_header("Algorithmic Optimization Tests");
	test_algorithmic_optimization();

	print_header("Pointer Dereference vs Elimination Tests");
	test_pointer_elimination();

	print_header("Memory Fragmentation and Cache Efficiency Tests");
	test_fragmentation(1000000, 1024);
	test_contiguous_allocation(1000000, 1024);
	std::cout << std::setw(60) << std::setfill('=') << "\n";
	return 0;
}
