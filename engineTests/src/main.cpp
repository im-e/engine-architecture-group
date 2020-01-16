#include <gtest/gtest.h>
#include <iostream>

#include "platform/OpenGL/OpenGL_Graphics.h"
#include "windows/WinPlatformWindow.h"

int OpenGL()
{
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(1, 1, "test", NULL, NULL);
	glfwMakeContextCurrent(win);
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return result;
}

int CloseGL()
{
	glfwTerminate();
	return 0;
}


int main(int argc, char **argv) 
{
	OpenGL();
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	std::cout << std::endl << "Press enter to continue" << std::endl;
	std::getchar();

	CloseGL();
	return 0;
}