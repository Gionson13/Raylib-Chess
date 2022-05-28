workspace "Chess"
    language "C++"
    cppdialect "C++17"
    
    architecture "x86_64"
    configurations { "Debug", "Release" }

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
	"Dependencies/fmt/%{cfg.system}/include" 
    }
    libdirs { "Dependencies/Raylib/%{cfg.system}/lib" }

    filter { "system:Windows" }
        links { "raylib", "Winmm", "gdi32", "opengl32" }
    
    filter { "system:Linux" }
        links { "raylib", "fmt" } -- , "m", "dl", "rt", "X11"

    filter {}

