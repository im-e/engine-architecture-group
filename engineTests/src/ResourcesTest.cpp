#include "ResourcesTest.h"

int OpenGL2()
{
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(1, 1, "test", NULL, NULL);
	glfwMakeContextCurrent(win);
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return result;
}

int CloseGL2()
{
	glfwTerminate();
	return 0;
}

TEST(Textures, LoadTexture)
{
	testTexture.reset(Engine::Texture::createFromFile("assets/textures/letterCube.png"));
	EXPECT_NE(testTexture.get(), nullptr);
}
TEST(Textures, GetTextureData)
{
	int width = testTexture->getWidth();
	int height = testTexture->getHeight();
	int channels = testTexture->getChannels();
	int slot = testTexture->getSlot();

	EXPECT_NE(0, width);
	EXPECT_NE(0, height);
	EXPECT_NE(0, channels);
	EXPECT_NE(0, slot);
}

TEST(Shader, LoadShader)
{
	testShader.reset(Engine::Shader::create("assets/shaders/flatColour.glsl"));
	EXPECT_NE(testShader.get(), nullptr);
}
TEST(Shader, GetID)
{
	testShader.reset(Engine::Shader::create("assets/shaders/flatColour.glsl"));
	int id = testShader->getID();
	EXPECT_EQ(testShader->getID(), id);
}
TEST(Shader, UploadData)
{
	OpenGL2(); // must stay here for internal OpenGL function calls

	testShader.reset(Engine::Shader::create("assets/shaders/flatColour.glsl"));
	glm::mat4 testMat = glm::mat4(1.0f);

	testShader->bind();
	testShader->uploadData("u_MVP", &testMat[0][0]);

	GLfloat data[16];
	GLuint loc = glGetUniformLocation(testShader->getID(), "u_MVP");

	glGetUniformfv(testShader->getID(), loc, data);

	int k = 0;
	for (int i = 0; i < testMat.length(); i++)
	{
		for (int j = 0; j < testMat.length(); j++)
		{
			EXPECT_EQ(testMat[i][j], data[k]);
			k++;
		}
	}

	CloseGL2();
}
TEST(Shader, ExtractBufferLayout)
{
	testShader.reset(Engine::Shader::create("assets/shaders/flatColour.glsl"));
	Engine::BufferLayout layout = testShader->getBufferLayout();
	Engine::BufferLayout testLayout = 
	{
		Engine::ShaderDataType::Float3,
		Engine::ShaderDataType::Float3
	};

	auto it2 = testLayout.begin();
	for (std::vector<Engine::BufferElement>::iterator it = layout.begin(); it != layout.end(); ++it, ++it2)
	{
		EXPECT_EQ(it->getDataType(), it2->getDataType());
	}
}

TEST(ResourceManagement, GetManagerInstance)
{
	EXPECT_NE(ResourceManagerInstance, nullptr);
}
TEST(ResourceManagement, AddAsset)
{
	ResourceManagerInstance->addTexture("testTex", "assets/textures/letterCube.png");
	EXPECT_NE(ResourceManagerInstance->getTexture().getCollection().size(), 0);
}
TEST(ResourceManagement, ContainsAsset)
{
	bool contains = ResourceManagerInstance->getTexture().contains("testTex");
	EXPECT_EQ(true, contains);
}
TEST(ResourceManagement, GetAsset)
{
	EXPECT_NE(ResourceManagerInstance->getTexture().getAsset("testTex"), nullptr);
}