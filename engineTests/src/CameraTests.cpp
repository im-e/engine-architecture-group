#include "CameraTests.h"

TEST(Camera, Create2DController)
{
	m_camera2D.reset(Engine::CameraController::create2DController(m_translationSpeed));

	EXPECT_NE(nullptr, m_camera2D.get());
}

TEST(Camera, Create3DController)
{
	m_camera3D.reset(Engine::CameraController::create3DController(m_translationSpeed, m_rotationSpeed));

	EXPECT_NE(nullptr, m_camera3D.get());
}

TEST(Camera, Create2D)
{
	m_camera2D->init(0.0f, 0.0f, 0.0f, 0.0f);

	EXPECT_NE(m_camera2D->getCamera().get(), nullptr);
}

TEST(Camera, Create3D)
{
	m_camera3D->init(0.0f, 0.0f, 0.0f, 0.0f);

	EXPECT_NE(m_camera3D->getCamera().get(), nullptr);
}

TEST(Camera, Move2D)
{
	m_camera2D->getCamera()->setPosition(glm::vec3(0.0f));
	glm::vec3 firstPos = m_camera2D->getCamera()->getPosition();

	m_camera2D->getCamera()->setPosition(glm::vec3(1.0f));
	glm::vec3 secondPos = m_camera2D->getCamera()->getPosition();

	for (int i = 0; i < 3; i++)
	{
		EXPECT_NE(firstPos[i], secondPos[i]);
	}
}

TEST(Camera, Move3D)
{
	m_camera3D->getCamera()->setPosition(glm::vec3(0.0f));
	glm::vec3 firstPos = m_camera3D->getCamera()->getPosition();

	m_camera3D->getCamera()->setPosition(glm::vec3(1.0f));
	glm::vec3 secondPos = m_camera3D->getCamera()->getPosition();

	for (int i = 0; i < 3; i++)
	{
		EXPECT_NE(firstPos[i], secondPos[i]);
	}
}

TEST(Camera, Rotate2D)
{
	auto testCam = static_cast<Engine::Camera2D*>(m_camera2D->getCamera().get());
	
	float initialRotation = testCam->getRotation();
	testCam->setRotation(1.0f);
	float postRotation = testCam->getRotation();

	EXPECT_NE(initialRotation, postRotation);
}

TEST(Camera, Rotate3D)
{
	auto testCam = static_cast<Engine::Camera3D*>(m_camera3D->getCamera().get());

	float initialYaw = testCam->getYaw();
	float initialPitch = testCam->getPitch();

	testCam->setYaw(1.0f);
	testCam->setPitch(1.0f);

	float postYaw = testCam->getYaw();
	float postPitch = testCam->getPitch();

	EXPECT_NE(initialYaw, postYaw);
	EXPECT_NE(initialPitch, postPitch);
}