project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	warnings "off"
 
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
 
	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}
 
	includedirs{
		"include"-- 为了glad.c直接#include <glad/glad.h>，而不用#include <include/glad/glad.h
	}
	filter "system:windows"
		systemversion "latest"
 
	
	filter { "system:windows", "configurations:Debug-AS" }	
		runtime "Debug"
		symbols "on"
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }
 
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		buildoptions "/MTd"
		symbols "On"
 
	filter "configurations:Release"
		defines "HZ_RELEASE"
		buildoptions "/MT"
		symbols "On"
 
		filter "configurations:Dist"
		defines "HZ_DIST"
		buildoptions "/MT"
		symbols "On"