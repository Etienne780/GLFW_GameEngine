#pragma once
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <CoreLib\Log.h>
#include <CoreLib\Math.h>
#include "CoreLib\File.h"
#include <CoreLib\FormatUtils.h>
#include <CoreLib\ColorUtils.h>
#include <CoreLib\TimeUtils.h>
#include <CoreLib\ConversionUtils.h>
#include <CoreLib\stb_image.h>

#include "EngineLib\Time.h"
#include "EngineLib\Input.h"
#include "EngineLib\Shader.h"
#include "EngineLib\Texture2D.h"
#include "EngineLib\Material.h"
#include "EngineLib\ResourceManager.h"
#include "EngineLib\GameObject.h"
#include "EngineLib\AllComponents.h"
