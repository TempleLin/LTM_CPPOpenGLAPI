#include <gtest/gtest.h>
#include <glad/glad.h>
#include <iostream>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

int test(int argc, char** argv) {
	printf("Running main() from %s\n", __FILE__);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

GTEST_API_ int main(int argc, char** argv) {
	test(argc, argv);
	std::cout << "Press any key to end." << std::endl;
	std::cin.get();
}