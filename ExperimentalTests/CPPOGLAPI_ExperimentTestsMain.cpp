#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <bitset>

#include <CPPOGLAPI_ExperimentTestsConfig.h>


#define COUNT_FUNCTION_DURATION

#ifdef COUNT_FUNCTION_DURATION
#include <thread>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using namespace std::chrono_literals;

void setColorWithFloats(float R, float G, float B, float A) {
	for (int i = 0; i < 10000; i++) {
		int shaderProgram{ 0 };
		static glm::vec4 color;
		if (R > 1.0f || G > 1.0f || B > 1.0f || A > 1.0f) {
			color[0] = R / 255.f;
			color[1] = G / 255.f;
			color[2] = B / 255.f;
			color[3] = A / 255.f;
		} else {
			color[0] = R;
			color[1] = G;
			color[2] = B;
			color[3] = A;
		}
		int test = R, test1 = G, test2 = B, test3 = A;
	}
}
void setColorWithGLM(glm::vec4 rgbaValue) {
	for (int i = 0; i < 10000; i++) {
		int shaderProgram{ 0 };
		static glm::vec4 color;
		if (rgbaValue[0] > 1.0f || rgbaValue[1] > 1.0f || rgbaValue[2] > 1.0f || rgbaValue[3] > 1.0f) {
			color = rgbaValue / 255.f;
		} else {
			color = rgbaValue;
		}
		int test = rgbaValue[0], test1 = rgbaValue[1], test2 = rgbaValue[2], test3 = rgbaValue[3];
	}
}
void passVec3ToVec3() {
	for (int i = 0; i < 10000; i++) {
		glm::vec3 toPass{ 0,0,0 };
		glm::vec3 toReceive = toPass;
	}
	std::this_thread::sleep_for(150ms);
}
void passFloatsToVec3() {
	for (int i = 0; i < 10000; i++) {
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

	auto t5 = high_resolution_clock::now();
	setColorWithGLM(glm::vec4(1, 0, 0, 1));
	auto t6 = high_resolution_clock::now();

	auto t7 = high_resolution_clock::now();
	setColorWithFloats(1, 0, 0, 1);
	auto t8 = high_resolution_clock::now();

	/* Getting number of milliseconds as an integer. */
	duration<double, std::milli> firstFuncDuration = t2 - t1;
	duration<double, std::milli> secondFuncDuration = t4 - t3;
	duration<double, std::milli> thirdFuncDuration = t6 - t5;
	duration<double, std::milli> fourthFuncDuration = t8 - t7;

	std::cout << "First function duration: " << firstFuncDuration.count() << "\n";
	std::cout << "Second function duration: " << secondFuncDuration.count() << "\n";
	std::cout << "Third function duration: " << thirdFuncDuration.count() << "\n";
	std::cout << "Fourth function duration: " << fourthFuncDuration.count() << "\n";

	glm::vec4 testDivided(255.f, 255.f, 255.f, 255.f);
	std::cout << glm::to_string(testDivided / 255.f) << "\n";
}
#endif

void testNestedTernaryOperator() {
	float opacity = .5f;
	opacity = (opacity > 1) ? 1 : ((opacity < 0) ? 0 : opacity);
	std::cout << opacity << "\n";
}

class ClassC;

class ClassA {
	friend class ClassC;
protected:
	int a;
public:
	ClassA() {
		a = 12;
	}
};

class ClassB : public ClassA {

};

class ClassC {
public:
	void printA(ClassA A) {
		std::cout << "a of ClassA: " << A.a << "\n";
	}
};

void testPrintFriendClassMember() {
	ClassB b;
	ClassC c;
	c.printA(b);
}

class A {
protected:
	int test;
public:
	A() {
		test = 12;
	}
	int getTest() {
		return test;
	}
};

class B : public A {
protected:
	int test2;
public:
	B() {
		test2 = 13;
	}
	int getTest2() {
		return test2;
	}
};

void testDownCast() {
	A* testing = new B();
	std::cout << testing->getTest() << "\n";
	/*B testing2 = static_cast<B>(*testing);*/ //Cannot downcast.
}

void testSFMLWindow() {
	sf::RenderWindow sfmlWin(sf::VideoMode(600, 360), "Hello World SFML Window");
	sf::Font font;
	//You need to pass the font file location
	//if (!font.loadFromFile(/*
 //                      Put the filename that identify the font file you want to load*/"myfont.ttf")) {
	//	return -1;
	//}
	sf::Text message("Hello, World !", font);

	while (sfmlWin.isOpen()) {

		sf::Event e;
		while (sfmlWin.pollEvent(e)) {

			switch (e.type) {
			case sf::Event::EventType::Closed:
				sfmlWin.close();
				break;
			}
		}

		sfmlWin.clear();
		sfmlWin.draw(message);
		sfmlWin.display();
	}
}

void testDotProduct() {
	glm::vec3 test(0.1f, .3f, .5f), test2(.8f, .7f, .4f);
	std::cout << glm::dot(test, test2) << std::endl;
	std::cout << glm::dot(-test, -test2) << std::endl;
}

void testBitwiseOperation() {
	int a, b, c, d;
	a = 5;
	b = 1;
	d = 3;
	c = 0;

	c = (a << 6 | b << 3 | d);

	//std::bitset<9> e(c);

	std::cout << c << "\n";
	if (c == 0b10100101100) {
		std::cout << "TRUE" << std::endl;
	}
	if (c == 0b101001011) {
		std::cout << "TRUE2" << std::endl;
	}
}

int main(){
	//testGLMPassDuration();
	//testNestedTernaryOperator();
	//testPrintFriendClassMember();
	//testDownCast();
	//testSFMLWindow();
	//testDotProduct();
	testBitwiseOperation();
}