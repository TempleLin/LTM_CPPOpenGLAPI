#include <gtest/gtest.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <chrono>

#define COUNT_FUNCTION_DURATION

#ifdef COUNT_FUNCTION_DURATION
#include <thread>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using namespace std::chrono_literals;
void passVec3ToVec3() {
	for (int i = 0; i < 1000; i++) {
		glm::vec3 toPass { 0,0,0 };
		glm::vec3 toReceive = toPass;
	}
	std::this_thread::sleep_for(150ms);
}
void passFloatsToVec3() {
	for (int i = 0; i < 1000; i++) {
		float test{ 0 }, test1{ 0 }, test2{ 0 };
		glm::vec3 toReceive;
		toReceive[0] = test;
		toReceive[1] = test1;
		toReceive[2] = test2;
	}
	std::this_thread::sleep_for(150ms);
}
void testGLMPassDuration() {
	auto t1 = high_resolution_clock::now();
	passFloatsToVec3();
	auto t2 = high_resolution_clock::now();

	auto t3 = high_resolution_clock::now();
	passVec3ToVec3();
	auto t4 = high_resolution_clock::now();


	/* Getting number of milliseconds as an integer. */
	duration<double, std::milli> firstFuncDuration = t2 - t1;
	duration<double, std::milli> secondFuncDuration = t4 - t3;

	std::cout << "First function duration: " << firstFuncDuration.count() << "\n";
	std::cout << "Second function duration: " << secondFuncDuration.count() << "\n";
}
#endif

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

int test(int argc, char** argv) {
	printf("Running main() from %s\n", __FILE__);
	testGLMPassDuration();
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

GTEST_API_ int main(int argc, char** argv) {
	test(argc, argv);
	std::cout << "Press any key to end." << std::endl;
	std::cin.get();
}