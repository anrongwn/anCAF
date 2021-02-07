#pragma once

#include "gtest/gtest.h"
#include <string>
#include <vector>

// Returns n! (the factorial of n).  For negative n, n! is defined to be 1.
int Factorial(int n);

// Returns true if and only if n is a prime number.
bool IsPrime(int n);

std::string str2str(const std::string &sub1, const std::string &sub2);

class anData {
  public:
	anData();
	~anData();

	anData(const anData &) = delete;
	anData &operator=(const anData &) = delete;

	std::size_t get_size() { return data_.size(); }
	std::size_t push(const std::string &e);
	std::size_t pop(const std::string &e);

  private:
	std::vector<std::string> data_;
};

class anDataTest : public ::testing::Test {
  protected:
	anDataTest();
	~anDataTest();

	virtual void SetUp() override;
	virtual void TearDown() override;

	// private:
	anData *p_data_;
};