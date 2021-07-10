# About

WhatsApp Viewer is a small tool to display chats from the Android msgstore.db database.

# How to use

1. **You need root access to your phone.** If you don't know what it is: [Wikipedia](https://en.wikipedia.org/wiki/Rooting_%28Android%29)
2. Download your WhatsApp database and key files:
  - */data/data/com.whatsapp/files/key*
  - */data/data/com.whatsapp/databases/msgstore.db*
  - */data/data/com.whatsapp/databases/wa.db*
3. Open WhatsApp Viewer
4. File -> Open database...
5. Select your downloaded *msgstore.db*
6. Optional: If you want, you can import contact names from the wa.db file

# Develop

## Prerequisites

- Conan
- Premake 5
- CMake (for building conan dependencies)

### Conan profile

#### General

```
conan profile new default --detect
conan profile update settings.compiler.cppstd=17 default
```

#### Linux profile

```
conan profile update settings.compiler.libcxx=libstdc++11 default
```

#### Windows profile

```
conan profile update settings.arch=x86 default
conan profile update settings.compiler.toolset=v141_xp default
conan profile update settings.compiler.runtime=MT default
```

### Conan remote

```
conan remote add bincrafters https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
conan config set general.revisions_enabled=1
```

## Build

```
conan install . --build=missing
```

### Linux & Mac

```
premake5 gmake2
make config=release
```

### Windows

```
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
premake5 vs2019
msbuild /p:Platform=Win32 /p:Configuration=Release
```

Note: There is also a *Vagrantfile* available for Windows.

## Tipps

For Visual Studio Code I use the `llvm-vs-code-extensions.vscode-clangd` extension for formatting (which itself uses `clang-format`).

# Third-party used

- conan
- premake
- wxWidgets
- sqlite and sqlitecpp
- cxxopts
