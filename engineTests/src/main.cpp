#include <gtest/gtest.h>
#include <iostream>
#include "core/application.h"

#include "platform/OpenGL/OpenGL_Graphics.h"
#include "windows/WinPlatformWindow.h"

class TestSandbox : public Engine::Application
{
public:
	TestSandbox()
	{
	}

	~TestSandbox() override
	{
	};
};

int main(int argc, char **argv) 
{
	auto app = new TestSandbox();

	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	std::cout << std::endl << "Press enter to continue" << std::endl;
	std::getchar();

	delete app;

	return 0;
}