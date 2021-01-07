#include <iostream>;
struct test {
	int a = 2;
	int b = 10;

	void move() {
		std::cout << a << b << std::endl;
	}
};

extern struct test test_struct;