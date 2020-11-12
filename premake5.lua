include("conanbuildinfo.premake.lua")

workspace("WhatsAppViewer")
    conan_basic_setup()
    configurations { "Debug", "Release" }

    project "WhatsAppViewer"
        kind "WindowedApp"
        language "C++"
        cppdialect "C++17"
        targetdir "bin/%{cfg.buildcfg}"

        linkoptions { conan_exelinkflags }

        files { "**.h", "**.cpp", "generated/Resources.cpp" }

        prebuildcommands { "{MKDIR} generated" }

        defines { "LIBASYNC_STATIC" }

        filter "system:Windows"
        prebuildcommands { "\"./bin/wxrc.exe\" /c resources/*.xrc /o generated/Resources.cpp" }

        filter "not system:Windows"
        prebuildcommands { "bin/wxrc-3.1 -c resources/*.xrc -o generated/Resources.cpp" }

        filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

        filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

        filter "system:Windows"
        architecture "x86"
        toolset "v141_xp"
        staticruntime "On"
        defines { "wxUSE_DPI_AWARE_MANIFEST=1" }
        files { "resources/windows.rc" }
