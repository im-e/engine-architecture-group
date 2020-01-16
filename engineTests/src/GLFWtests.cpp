#include "GLFWtests.h"

TEST(KeyCodes, Press)
{
	EXPECT_EQ(GLFW_PRESS, NG_PRESS);
}

TEST(KeyCodes, Release)
{
	EXPECT_EQ(GLFW_RELEASE, NG_RELEASE);
}

TEST(KeyCodes, Repeat)
{
	EXPECT_EQ(GLFW_REPEAT, NG_REPEAT);
}

TEST(KeyCodes, Key)
{
	EXPECT_EQ(GLFW_KEY_A, NG_KEY_A);
}

TEST(KeyCodes, UnknownKey)
{
	int key = -1;
	EXPECT_EQ(key, NG_KEY_UNKNOWN);
}

TEST(KeyCodes, GLFW_UnknownKey)
{
	EXPECT_EQ(GLFW_KEY_UNKNOWN, NG_KEY_UNKNOWN);
}

TEST(GLFW, Init)
{
	bool result = glfwInit();

	EXPECT_EQ(true, result);
}

TEST(GLFW, OpenWindow)
{
	std::unique_ptr<Engine::Window> m_appWindow = std::unique_ptr<Engine::Window>(Engine::Window::create());
	
	EXPECT_NE(nullptr, m_appWindow.get());
}

TEST(GLFW, Terminate)
{
	bool init = glfwInit();

	if (init == true)
	{
		glfwTerminate();
		init = false;
	}	

	EXPECT_NE(true, init);

}

TEST(Windows, SystemInit)
{
	windowsSys = std::shared_ptr<Engine::WindowsSys>(new Engine::GLFWWindowsSys());

	EXPECT_NE(nullptr, windowsSys.get());
}

TEST(InputPoller, InputPollerInitialization)
{	
	poller = new Engine::GLFWInputPoller();

	EXPECT_NE(nullptr, poller->getInstance());
}

