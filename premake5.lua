workspace "Chess"
    language "C++"
    cppdialect "C++17"
    
    architecture "x86_64"
    configurations { "Debug", "Release" }

    warnings "Extra"

    filter { "configurations:Debug" }
        defines { "_DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        optimize "On"

    filter { "configurations:Release", "system:Windows" }
        linkoptions { "-static", "-static-libgcc", "-static-libstdc++" }
        links { "pthread" }

    filter { }

    targetdir ("bin/%{prj.name}/%{cfg.longname}")
    objdir ("obj/%{prj.name}/%{cfg.longname}")

project "Chess"
    kind "ConsoleApp"
    files "Chess/**"

    includedirs {
        "Dependencies/Raylib/%{cfg.system}/include",
        "Dependencies/fmt/%{cfg.system}/include",
    }

    libdirs { 
        "Dependencies/Raylib/%{cfg.system}/lib",
        "Dependencies/fmt/%{cfg.system}/lib"
    }

	prebuildcommands {
		"\"C:\\Dev\\c++\\ResourcesManager\\bin\\ResourceManager\\Release\\ResourceManager.exe\" Chess\\resources.res -p rl -o Chess\\src\\Globals.hpp"
	}

    filter { "system:Windows" }
        links { "raylib", "fmt", "Winmm", "gdi32", "opengl32" }
    
    filter { "system:Linux" }
        links { "raylib", "fmt" } -- , "m", "dl", "rt", "X11"

    filter {}

