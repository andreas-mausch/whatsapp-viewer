package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "whatsapp-viewer",
    srcs = glob(["source/**/*.cpp", "source/**/*.c", "source/**/*.h", "resources/resource.h"]),
    copts = ["/GS", "/D WIN32", "/D UNICODE", "/D _UNICODE", "/D WIN32"],
    linkopts = ["/MACHINE:X86",
                "/SUBSYSTEM:WINDOWS",
                "resources/Resources.res",
                "advapi32.lib",
                "comctl32.lib",
                "comdlg32.lib",
                "gdi32.lib",
                "msimg32.lib",
                "ole32.lib",
                "shell32.lib",
                "user32.lib",
                "version.lib",
                "windowscodecs.lib"]
)
