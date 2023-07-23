workspace "Chess"
    language "C++"
    cppdialect "C++17"
    
    configurations { "Debug", "Release" }
	platforms { "Desktop", "Web" }
	defaultplatform "Desktop"

	filter { "platforms:Desktop" }
		architecture "x86_64"
	filter { "platforms:Web" }
		architecture "x86"

    filter { }

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

    targetdir ("bin/%{prj.name}/%{cfg.buildcfg}")
    objdir ("obj/%{prj.name}/%{cfg.buildcfg}")

project "Chess"
    kind "ConsoleApp"
    files "Chess/**"

	filter { "platforms:Desktop" }
        includedirs {
    		"Chess/src",
            "Dependencies/Raylib/%{cfg.system}/include",
            "Dependencies/fmt/%{cfg.system}/include",
        }

        libdirs { 
            "Dependencies/Raylib/%{cfg.system}/lib",
            "Dependencies/fmt/%{cfg.system}/lib"
        }

	filter { "platforms:Web" }
		defines { "PLATFORM_WEB" }

        includedirs {
    		"Chess/src",
            "Dependencies/Raylib/webassembly/include",
            "Dependencies/fmt/%{cfg.system}/include",
        }

        libdirs { 
            "Dependencies/Raylib/webassembly/lib",
            "Dependencies/fmt/%{cfg.system}/lib"
        }
	
	filter {}

	prebuildcommands {
		"~/Dev/c++/ResourceManager/bin/ResourceManager/Debug/ResourceManager Chess/Globals.txt -p rl -o Chess/src/Globals.hpp"
	}

    filter { "system:Windows" }
        links { "raylib", "fmt", "Winmm", "gdi32", "opengl32" }
		linkoptions { "-static-libstdc++" } -- filesystem dll not linking
    
    filter { "system:Linux" }
        links { "raylib", "fmt" } -- , "m", "dl", "rt", "X11"

    filter {}

