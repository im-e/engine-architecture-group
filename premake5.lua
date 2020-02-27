workspace "Engine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Test"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

group "Engine"

project "Engine"
	location "engine"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")
	
	pchheader "engine_pch.h"
	pchsource "engine/precompiled/engine_pch.cpp"
	
	files
	{
		"%{prj.name}/enginecode/**.h",
		"%{prj.name}/enginecode/**.cpp",
		"engine/precompiled/engine_pch.h",
		"engine/precompiled/engine_pch.cpp",
		"vendor/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"%{prj.name}/enginecode/",
		"%{prj.name}/enginecode/include/independent",
		"%{prj.name}/precompiled/",
		"vendor/spdlog/include",
		"vendor/glfw/include",
		"vendor/glad/include",
		"vendor/glm/",
		"vendor/stb_image",
		"vendor/freetype2/include",
		"vendor/json/single_include/nlohmann",
		"vendor/assimp/include",
		"vendor/reactphysics3d/src",
		"vendor/ImGui",
		"vendor/FMOD/core/inc",
		"vendor/FMOD/fsbank/inc",
		"vendor/FMOD/studio/inc"
	}
	
	links 
	{
		"GLFW",
		"Glad",
		"Freetype",
		"assimp",
		"reactphysics3d",
		"IMGui"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"NG_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "NG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Test"
		defines "NG_DEBUG;NG_UNIT_TEST"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "NG_RELEASE"
		runtime "Release"
		optimize "On"

project "LevelEditor"
	location "levelEditor"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp",
		"vendor/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"%{prj.name}/include",
		"engine/enginecode/",
		"engine/enginecode/include/independent",
		"engine/precompiled/",
		"vendor/spdlog/include",
		"vendor/stb_image",
		"vendor/glm/",
		"vendor/json/single_include/nlohmann",
		"vendor/assimp/include",
		"vendor/reactphysics3d/src",
		"vendor/ImGui"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"NG_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "NG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "NG_RELEASE"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp",
		"vendor/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"%{prj.name}/include",
		"engine/enginecode/",
		"engine/enginecode/include/independent",
		"engine/precompiled/",
		"vendor/spdlog/include",
		"vendor/stb_image",
		"vendor/glm/",
		"vendor/json/single_include/nlohmann",
		"vendor/assimp/include",
		"vendor/reactphysics3d/src",
		"vendor/ImGui",
		"vendor/FMOD/core/inc",
		"vendor/FMOD/fsbank/inc",
		"vendor/FMOD/studio/inc"
	}
	
	libdirs
	{
		"vendor/FMOD/core/lib/x64",
		"vendor/FMOD/fsbank/lib/x64",
		"vendor/FMOD/studio/lib/x64"
	}
	

	links
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"NG_PLATFORM_WINDOWS"
		}
		
		debugenvs
		{
			"PATH=PATH;../vendor/FMOD/core/lib/x64;../vendor/FMOD/studio/lib/x64"
		}
		

	filter "configurations:Debug"
		defines "NG_DEBUG"
		runtime "Debug"
		symbols "On"
		
		links
		{
			"Engine",
			"fmodL_vc.lib",
			"fmodstudioL_vc.lib"
		}

	filter "configurations:Release"
		defines "NG_RELEASE"
		runtime "Release"
		optimize "On"
		
		links
		{
			"Engine",
			"fmodL_vc.lib",
			"fmodstudioL_vc.lib"
		}

 project "EngineTests"
		location "engineTests"
        kind "ConsoleApp"
        language "C++"
		staticruntime "off"
		systemversion "latest"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("build/" .. outputdir .. "/%{prj.name}")

        files 
		{ 
			"%{prj.name}/include/*.h",
			"%{prj.name}/src/*.cpp",
			"vendor/stb_image/stb_image.cpp"
		}

        includedirs
		{ 
			"%{prj.name}/include/",
			"vendor/googletest/googletest/include",
			"engine/enginecode/",
			"engine/enginecode/include/independent",
			"engine/enginecode/include/platform",
			"engine/precompiled/",
			"vendor/spdlog/include",
			"vendor/glm/",
			"vendor/stb_image",
			"vendor/freetype2/include",
			"vendor/glfw/include",
			"vendor/glad/include",
			"vendor/json/single_include/nlohmann",
			"vendor/assimp/include"
		}

        links 
		{ 
			"googletest",
			"Engine",
			"GLFW",
			"Glad",
			"Freetype",
			"assimp",
			"reactdphysics3d"
		}
		
		filter "system:windows"
			cppdialect "C++17"
			systemversion "latest"

		filter "configurations:Debug"
			runtime "Debug"
			symbols "On"

		filter "configurations:Test"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			runtime "Release"
			optimize "On"

project "Spike"
	location "spike"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp",
		"vendor/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"%{prj.name}/include",
		"vendor/spdlog/include",
		"vendor/stb_image",
		"vendor/freetype2/include",
		"vendor/glm/",
		"vendor/assimp/include",
		"vendor/glad/include",
		"vendor/reactphysics3d/src"
	}
	
	links 
	{
		"Freetype",
		"assimp",
		"Glad"
	}
	

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines
		{
			"NG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

group "Vendor"



	include "vendor/glfw"
	include "vendor/googletest"
	include "vendor/glad"
	include "vendor/freetype2"
	include "vendor/zlib"
	include "vendor/IrrXML"
	include "vendor/assimp"
	include "vendor/reactphysics3d"
	include "vendor/ImGui"
