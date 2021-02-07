#include "an_test.h"

// Returns n !(the factorial of n).For negative n, n !is defined to be 1.
int Factorial(int n) {
	int result = 1;
	for (int i = 1; i <= n; ++i) {
		result *= i;
	}

	return result;
}

// Returns true if and only if n is a prime number.
bool IsPrime(int n) {
	if (n <= 1)
		return false;

	if (n % 2 == 0)
		return n == 2;

	// Try to divide n by every odd number i, starting from 3
	for (int i = 3;; i += 2) {
		// We only have to try i up to the square root of n
		if (i > n / i)
			break;

		// Now, we have i <= n/i < n.
		// If n is divisible by i, n is not prime.
		if (n % i == 0)
			return false;
	}

	// n has no integer factor in the range (1, n), and thus is prime.
	return true;
}

std::string str2str(const std::string &sub1, const std::string &sub2) {
	std::string result = "";

	result += sub1;
	result += "-";
	result += sub2;

	return result;
}

anData::anData() { data_.reserve(10); }

anData::~anData() { std::cout << "anData::~anData() exit." << std::endl; }

std::size_t anData::pop(const std::string &e) {

	for (auto i : data_) {
		if (i == e) {
			return 1;
		}
	}

	return 0;
}

std::size_t anData::push(const std::string &e) {
	if (e.empty())
		return 0;

	data_.push_back(e);
	return 1;
}

anDataTest::anDataTest() : p_data_(nullptr) { std::cout << "anDataTest::anDataTest()" << std::endl; }

anDataTest::~anDataTest() { std::cout << "anDataTest::~anDataTest() exit." << std::endl; }

void anDataTest::SetUp() {
	std::cout << "anDataTest::SetUp() " << std::endl;
	p_data_ = new anData();

	ASSERT_FALSE(p_data_ == nullptr);
}

void anDataTest::TearDown() {
	std::cout << "anDataTest::TearDown() " << std::endl;

	delete p_data_;
	p_data_ = nullptr;
}