Sample project to test the compatibility of wxWidgets, conan and premake
in order to build cross-platform GUI applications for Linux, macOS and Windows.

(Check the `xrc` branch for [XML Based Resource System](https://docs.wxwidgets.org/3.1/overview_xrc.html))

# Prerequisites

- Conan
- Premake 5

## Conan profile

### Linux profile

```
conan profile update settings.compiler.libcxx=libstdc++11 default
```

### Windows profile

```
conan profile update settings.arch=x86 default
conan profile update settings.compiler.toolset=v141_xp default
conan profile update settings.compiler.runtime=MT default
```

## Conan remote

```
conan remote add bincrafters "https://api.bintray.com/conan/bincrafters/public-conan"
```

# Build

```
conan install . --build=missing
```

## Linux & Mac

```
premake5 gmake2
make
```

## Windows

```
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
premake5 vs2019
msbuild /p:Platform=Win32
```

# My personal conclusion

I've written a blog post about this experiment:

[https://andreas-mausch.de/blog/2020/09/29/wxwidgets/](https://andreas-mausch.de/blog/2020/09/29/wxwidgets/)
