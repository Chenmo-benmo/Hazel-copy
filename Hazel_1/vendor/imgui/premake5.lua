project "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "on"
 
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	defines { "IMGUI_API=__declspec(dllexport)" }
	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_tables.cpp",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp"
	}
 
	filter "system:windows"
		systemversion "latest"
	
 
	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
 
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
 
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
 
    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"