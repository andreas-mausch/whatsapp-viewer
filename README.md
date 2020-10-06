# Prerequisites

- Conan
- Premake 5
- CMake

## Conan profile

### General

```
conan profile update settings.compiler.cppstd=17 default
```

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
make config=release
```

## Windows

```
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
premake5 vs2019
msbuild /p:Platform=Win32 /p:Configuration=Release
```
