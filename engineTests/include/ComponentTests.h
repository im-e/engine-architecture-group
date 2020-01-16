#pragma once

#include <gtest/gtest.h>

#include "resources/ResourceManager.h"

#include "components/Component.h"
#include "components/GameObject.h"
#include "components/MaterialComponent.h"
#include "components/OscillateComponent.h"
#include "components/PositionComponent.h"
#include "components/TextureComponent.h"
#include "components/VelocityComponent.h"


std::shared_ptr<Engine::GameObject> m_go;

std::shared_ptr<Engine::MaterialComponent> m_materialComponent;
std::shared_ptr<Engine::TextureComponent> m_textureComponent;