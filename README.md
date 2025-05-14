# Ymir 

Ymir is a lightweight and flexible C++ project generator for both Windows and Linux. 

Ymir is __not__ a C++ build system. 
It is __not__ a replacement for CMake or any other build system. In fact, Ymir uses CMake under the hood. 

Ymir is only a useful tool to generate all the necessary files and directories (specified by you) in order to start developing a C++ project faster. 

Ymir will: 
    - *Create* a main directory for the project.
    - *Generate* a top-level `CMakeLists.txt` file with all the required options set. The file is commented and quite robust although somewhat simple. 
    - *Create* the main directories where all your source files will live as well as a directory for all your third-party libraries. 
    - *Generate* a default `main.cpp` file for convenience 
    - *Generate* a `.gitignore` and a `.gitattributes` if Git is enabled.

Besides that, Ymir can set a few configurations for the initial state of your project. Configurations such as the C++ standard, any initial project definitions, compiler flags, and so on.


## Build Instructions 

Like most C++ projects, Ymir uses CMake as its build system. The following commands will compile Ymir and produce a `ymir` executable. 

```bash
mkdir build && cd build 
cmake .. 
make
```

Ymir does not have any dependencies at all. It only uses `C++17` features and that's it. The compilation process should be quite simple and fast. 

Afterwards, you can execute the `ymir` command from the build directory or install it system-wide like so: 

```bash
sudo make install
```

That command will only work for Linux, however. For Windows, you can place Ymir anywhere but you _must_ add it to your environment variables in order to use it from anywhere.

## Usage

Finally, now you can use Ymir in all its glory. You should pass the `--help` flag to Ymir to get a better idea of what can be done. 

```
ymir --help
```

Will produce: 

```
### ### Ymir: A C++ project generator ### ###

[Usage]: ymir [options] --new <project_name>
        [--new, -n]          = The name of the new project
        [--standard, -std]   = Define the standard of the language (by default, this is 20)
        [--type, -t]         = Can be either CONSOLE_APP, GUI_APP, or LIBRARY
        [--definitions, -d]  = List all of the project's definitions inside quotes
        [--flags, -f]        = List all of the project's compiler flags inside quotes
        [--src-dir, -sd]     = Define the name of the source directory (by default, this is 'src')
        [--include-dir, -id] = Define the name of the include directory (by default, this is 'src')
        [--deps-dir, -dd]    = Define the name of the dependencies directory (by default, this is 'libs')
        [--has-git, -g]      = Create both a .gitignore and a .gitattributes file if set (by default, this is unset)
        [--help, -h]         = Show this help screen

### ### Ymir: A C++ project generator ### ###
```

The most important flag is the `--new` flag since it will start the process of creating a C++ project with the given name. 

```
ymir --new "NewCPPProject"
```

With only this command, you'll have a new project directory with the name `NewCPPProject` with the following contents:

```
- NewCPPProject/
    - libs/ 
    - src/
        main.cpp
    - CMakeLists.txt
```

Although it is somewhat minimal, it is just enough to get started. However, if you wish to change or add a few configurations, you can use the various flags Ymir offers. 

For example, the `--standard` flag or its alias `-std` will set the C++ language standard. 

```
ymir -std 20 --new "NewCPPProject"
```

You can also change the _type_ of the C++ project. A library is different from a console application, and those are different still from a GUI application. Using the `--type` (or `-t`) flag will give you the flexibility of choosing a specific project type. 

```
ymir -std 20 -t CONSOLE_APP --new "NewConsoleApp"
```

The `--type` flag accepts either `CONSOLE_APP`, `GUI_APP`, or `LIBRARY`. The difference between these types under the hood is the way the `CMakeLists.txt` and the default `main.cpp` files will be configured. For example, for the `CONSOLE_APP` project type, the CMake file will have the usual: 

```cmake
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
```

However, for a `LIBRARY` project, the following will be used: 

```cmake
# 'SHARED' is used by default but it can be changed
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS})
```
The `GUI_APP` project does not differ _too_ much from a regular `CONSOLE_APP`. However, on both Windows and MacOS, if you wish to disable the console completely (which is quite likely in a GUI application), the `add_executable` CMake command needs to include either the `WIN32` or the `MACOSX_BUNDLE` flags respectfully. Besides that, GUI applications on Windows have a unique entry point.

```c++
#include <windows.h>

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev_inst, LPSTR cmd_line, int cmd_show) {
  return 0;
}
```

Moving on, we can also add some configurations to the C++ project by using both the `--definitions` (or `-d`) and the `--flags` (or `-f`) flags. 

```
ymir -std 20 -t GUI_APP -d "GLFW_INCLUDE_NONE" -f "-lm -w" --new "NewGameProject"
```

Just make sure to include multiple flags or definitions in the same string on the command line.

By default, Ymir keeps both the translation units (`.cpp` or `.c`) and the header files in the same directory--`src`. But that can easily be changed. 

```
ymir -std 20 -t LIBRARY --src-dir "src" --include-dir "include/library_name" -n "NewLibrary"
```

Ymir also generates a directory for third-party libraries. And, like the source and include directories, it can also be changed to your liking. 

```
ymir -std 20 -t LIBRARY --deps-dir "thirdparty" -n "NewLibrary"
```

Ymir can also generate two Git files if the `--has-git` (or the alias `-g`) flag is set. Mainly, Ymir will generate a `.gitignore` and a `.gitattributes` file in the main directory.

```
ymir -std 20 -t LIBRARY --src-dir "src" --include-dir "include/library_name" --deps-dir "thirdparty" -n "NewLibrary" -g
```

With the previous command, the newly generated project directory should look something like this: 

```
- NewLibrary/ 
    - include/ 
    - src/
        main.cpp
    - thirdparty/ 
    - CMakeLists.txt
    - .gitignore
    - .gitattributes
```

If the `cmake ..` command is run on the main directory, a new binary will be produced with no issues (hopefully).
