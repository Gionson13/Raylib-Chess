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
        linkoptions { "-static", "-static-libgcc", "-static-libstdc++" }
        links { "pthread" }

    filter { }

    targetdir ("bin/%{prj.name}/%{cfg.longname}")
    objdir ("obj/%{prj.name}/%{cfg.longname}")

project "Chess"
    kind "ConsoleApp"
    files "Chess/**"

    includedirs {
        "Dependencies/Raylib/include" 
    }
    libdirs { "Dependencies/Raylib/lib" }

    links { "raylib", "Winmm", "gdi32", "opengl32" }
