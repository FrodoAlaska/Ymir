#include "ymir_project.h"

#include <string>
#include <filesystem>
#include <cstdio>
#include <fstream>

namespace ymir { // Start of ymir

/// -------------------------------------------------------------------------------------------------
/// Defines

// Defining the CMake GUI application type
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
  #define EXE_TYPE "WIN32"
#elif defined(__linux__) || defined(__gnu_linux__)
  #define EXE_TYPE ""
#else
  #define EXE_TYPE "MAXOSX_BUNDLE"
#endif

// Defining the default C++ file's entry point
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
  #define MAIN_CONSOLE_ENTRY               \
  "#include <iostream>\n\n"                \
  "int main(int argc, char** argv) {\n"    \
  "  std::cout << argv[0] << std::endl;\n" \
  "}\n"                                                                                     

  #define MAIN_GUI_ENTRY                                                                      \
  "#include <windows.h>\n\n"                                                                  \
  "int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev_inst, LPSTR cmd_line, int cmd_show) {\n" \
  "  return 0;\n"                                                                             \
  "}\n"                                                                                     
#else
  #define MAIN_CONSOLE_ENTRY               \
  "#include <iostream>\n\n"                \
  "int main(int argc, char** argv) {\n"    \
  "  std::cout << argv[0] << std::endl;\n" \
  "}\n"                                                                                     

  #define MAIN_GUI_ENTRY     \
  "int main() {\n"           \
  "  return 0;\n"            \
  "}\n"                                                                                     
#endif

/// Defines
/// -------------------------------------------------------------------------------------------------

/// -------------------------------------------------------------------------------------------------
/// Private functions

bool create_directories(const ProjectDesc& desc) {
  // Create the main directory
  if(!std::filesystem::create_directory(desc.full_path)) {
    printf("[YMIR-ERROR]: Failed to create the main project directory at \'%s\'\n", desc.full_path.c_str());
    return false;
  }
  printf("[YMIR-TRACE]: Creating the main directory...\n");

  // Create the source directory
  if(!std::filesystem::create_directory(desc.full_path)) {
    printf("[YMIR-ERROR]: Failed to create soruce directory at \'%s\'\n", (desc.full_path / desc.src_dir).string().c_str());
    return false;
  }
  printf("[YMIR-TRACE]: Creating the source directory...\n");

  // Create the include directory (only if it has a different name)
  if(desc.src_dir != desc.include_dir) {
    if(!std::filesystem::create_directory(desc.full_path)) {
      printf("[YMIR-ERROR]: Failed to create include directory at \'%s\'\n", (desc.full_path / desc.include_dir).string().c_str());
      return false;
    }
    printf("[YMIR-TRACE]: Creating the include directory...\n");
  }
  
  // Create the dependencies directory
  if(!std::filesystem::create_directory(desc.full_path)) {
    printf("[YMIR-ERROR]: Failed to create dependencies directory at \'%s\'\n", (desc.full_path / desc.deps_dir).string().c_str());
    return false;
  }
  printf("[YMIR-TRACE]: Creating the dependencies directory...\n");

  // Create any additional directories
  std::string dir = "";
  for(const char ch : desc.additional_dirs.string()) {
    if(ch != ';' || ch != ' ') {
      dir += ch;
      continue;
    }

    std::filesystem::path path = desc.full_path / std::filesystem::path(dir);
    if(!std::filesystem::create_directory(path)) {
      printf("[YMIR-ERROR]: Failed to create directory at \'%s\'\n", path.string().c_str());
      return false;
    }
    printf("[YMIR-TRACE]: Creating the \'%s\' directory...\n", dir.c_str());

    dir = "";
  }

  return true;
}

std::string get_cmake_target_syntax(const ProjectDesc& desc) {
  switch(desc.type) {
    case PROJECT_CONSOLE_APP:
      return "add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})";
    case PROJECT_GUI_APP:
      return std::string("add_executable(${PROJECT_NAME}") + EXE_TYPE + "${PROJECT_SOURCES})";
    case PROJECT_LIBRARY:
      return "add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_INCLUDE_DIR}/define_headers_here.h)";
    case PROJECT_INVALID:
    default:
      return "YMIR_ERROR_SYNTAX";
  }
}

void create_cmake_entries(const ProjectDesc& desc) {
  // Create a top-level CMake file
  std::ofstream cmake_file(desc.full_path / std::filesystem::path("CMakeLists.txt"), std::ios::out | std::ios::trunc);
  if(!cmake_file.is_open()) {
    printf("[YMIR-ERROR]: Failed to create CMake file at \'%s\'\n", (desc.full_path).c_str());
    return;
  }

  printf("[YMIR-TRACE]: Create a default CMakeLists.txt file for you...\n");

  // CMake info
  cmake_file << "cmake_minimum_required(VERSION 3.27)\n";
  cmake_file << "project(" << desc.name << ")\n\n";

  // Project variables
  cmake_file << "### Project variables ###\n";
  cmake_file << "############################################################\n";
  cmake_file << "# @NOTE: Add any project-specific variables here\n";
  cmake_file << "set(PROJECT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/" << desc.src_dir << '\n';
  cmake_file << "set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/" << desc.include_dir << '\n';
  cmake_file << "set(PROJECT_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/" << desc.deps_dir << "\n\n";
  cmake_file << "set(PROJECT_BUILD_FLAGS " << desc.compiler_flags << ")\n";
  cmake_file << "set(PROJECT_BUILD_DEFINITIONS " << desc.definitions << ")\n";
  cmake_file << "############################################################\n\n";
  
  // CMake variables
  cmake_file << "### CMake variables ###\n";
  cmake_file << "############################################################\n";
  cmake_file << "# @NOTE: Include any CMake-specific variable configurations here\n";
  cmake_file << "# (example) set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n";
  cmake_file << "############################################################\n\n";
  
  // Project options
  cmake_file << "### Project options ###\n";
  cmake_file << "############################################################\n";
  cmake_file << "# @NOTE: Include any CMake-specific variable configurations here\n";
  cmake_file << "# (example) option(PROJECT_BUILD_SHARED \"Build the project as a shared library\" ON)\n";
  cmake_file << "############################################################\n\n";
  
  // Project sources
  cmake_file << "### Project sources ###\n";
  cmake_file << "############################################################\n";
  cmake_file << "# @NOTE: All of the source files (.cpp and/or .c) will be listed here\n";
  cmake_file << "set(PROJECT_SOURCES\n  ${PROJECT_SRC_DIR}/main.cpp\n)\n";
  cmake_file << "############################################################\n\n";
  
  // Sub-projects
  cmake_file << "### Sub-projects ###\n";
  cmake_file << "############################################################\n";
  cmake_file << "# @NOTE: Any sub-projects can be added in here\n";
  cmake_file << "# (example) add_subdirectory(${PROJECT_LIBS_DIR}/glfw)\n";
  cmake_file << "############################################################\n\n";
 
  // The target differs depending on the platform and the project type
  std::string target_syntax = get_cmake_target_syntax(desc);

  // Targets
  cmake_file << "### Targets ###\n";
  cmake_file << "############################################################\n";
  cmake_file << "# @NOTE: Define the final targets here\n";
  cmake_file << target_syntax << '\n';
  cmake_file << "############################################################\n\n";
  
  // Linking
  cmake_file << "### Linking ###\n";
  cmake_file << "############################################################\n";
  cmake_file << "# @NOTE: Define all of the linking options here\n";
  cmake_file << "target_include_directories(${PROJECT_NAME} PUBLIC BEFORE\n  ${PROJECT_INCLUDE_DIR}\n  ${PROJECT_LIBS_DIR}\n)\n";
  cmake_file << "# (example) target_link_libraries(${PROJECT_NAME} PUBLIC library::library)\n";
  cmake_file << "############################################################\n\n";
  
  // Compiling options
  cmake_file << "### Compiling options ###\n";
  cmake_file << "############################################################\n";
  cmake_file << "# @NOTE: Define all of the compiling options here\n";
  cmake_file << "target_compile_options(${PROJECT_NAME} PUBLIC ${PROJECT_BUILD_FLAGS})\n";
  cmake_file << "target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_" << std::to_string(desc.cpp_version) << ")\n";
  cmake_file << "target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_BUILD_DEFINITIONS})\n";
  cmake_file << "############################################################\n\n";

  cmake_file.close();
}

void create_default_cpp(const ProjectDesc& desc) {
  // Create the file first
  std::ofstream cpp_file(desc.full_path / desc.src_dir / std::filesystem::path("main.cpp"), std::ios::out | std::ios::trunc);
  if(!cpp_file.is_open()) {
    printf("[YMIR-ERROR]: Failed to create C++ file at \'%s\'\n", (desc.full_path / desc.src_dir).string().c_str());
    return;
  }
  
  printf("[YMIR-TRACE]: Create a default main.cpp file for you...\n");

  std::string main_entry_syntax;
  switch(desc.type) {
    case PROJECT_CONSOLE_APP:
      main_entry_syntax = MAIN_CONSOLE_ENTRY;
      break;
    case PROJECT_GUI_APP:
      main_entry_syntax = MAIN_GUI_ENTRY;
      break;
    case PROJECT_LIBRARY:
      main_entry_syntax = MAIN_CONSOLE_ENTRY;
      break;
    case PROJECT_INVALID:
      printf("[YMIR-ERROR]: Invalid project type given\n");
      cpp_file.close();
      return;
    default:
      main_entry_syntax = "YMIR_ERROR_SYNTAX";
      break;
  }
  
  cpp_file << main_entry_syntax;
  cpp_file.close();
}

static void create_git_entries(const ProjectDesc& desc) {
  // Create the .gitignore file first
  std::ofstream file(desc.full_path / std::filesystem::path(".gitignore"), std::ios::out | std::ios::trunc);
  if(!file.is_open()) {
    printf("[YMIR-ERROR]: Failed to create .gitignore file at \'%s\'\n", desc.full_path.c_str());
    return;
  }
  
  printf("[YMIR-TRACE]: Create a default .gitignore file for you...\n");

  file << "# Ignore build directories\n";
  file << "build/\n";
  file << "build-debug/\n";
  file << "build-release/\n";
  file << "bin/\n";
  file << "obj/\n";
  file << "out/\n";
  file << "dist/\n";
  file << "Debug/\n";
  file << "Release/\n";
  file << "res/\n";

  file << "\n# Ignore compiled object files and binaries\n";
  file << "*.o\n";
  file << "*.obj\n";
  file << "*.exe\n";
  file << "*.dll\n";
  file << "*.so\n";
  file << "*.dylib\n";
  file << "*.a\n";
  file << "*.lib\n";

  file << "\n# Ignore temporary files\n";
  file << "*.log\n";
  file << "*.tmp\n";
  file << "*.cache\n";
  file << "*.bak\n";

  file << "\n# Ignore CMake and Makefile artifacts\n";
  file << "CMakeCache.txt\n";
  file << "CMakeFiles/\n";
  file << "Makefile\n";
  file << "cmake_install.cmake\n";

  file << "\n# Ignore Visual Studio-specific files\n";
  file << "*.vcxproj*\n";
  file << "*.sln\n";
  file << "*.ncb\n";
  file << "*.suo\n";
  file << "*.user\n";
  file << "*.pdb\n";
  file << "*.ipch\n";
  file << "*.tlog\n";
  file << "*.lastbuildstate\n";

  file << "\n# Ignore package manager files (optional)\n";
  file << "node_modules/\n";
  file << ".vscode/\n";
  file << ".idea/\n";

  file.close();
  
  printf("[YMIR-TRACE]: Create a default .gitattributes file for you...\n");

  // Now creating the .gitattributes file
  file.open(desc.full_path / std::filesystem::path(".gitattributes"), std::ios::out | std::ios::trunc);
  if(!file.is_open()) {
    printf("[YMIR-ERROR]: Failed to create .gitattributes file at \'%s\'\n", desc.full_path.c_str());
    return;
  }

  file << "# Auto detect text files and perform LF normalization\n";
  file << "* text=auto\n\n";
  file << "# Just look at the cpp and hpp files\n";
  file << "* linguist-vendored\n";
  file << "*.cpp linguist-vendored=false\n";
  file << "*.hpp linguist-vendored=false\n";

  file.close();
}

/// Private functions
/// -------------------------------------------------------------------------------------------------

/// -------------------------------------------------------------------------------------------------
/// Project functions

bool project_create(ProjectDesc& desc) {
  desc.full_path = (std::filesystem::current_path() / desc.name).string();
  
  if(!create_directories(desc)) {
    return false;
  }

  // Creating indivisual files
  create_cmake_entries(desc);
  create_default_cpp(desc);
  
  // Creating GIT entries only if requested
  if(desc.has_git) {
    create_git_entries(desc);
  }

  printf("[YMIR-TRACE]: Successfully created a new project with the name \'%s\'!\n", desc.name.c_str());
  return true;
}

/// Project functions
/// -------------------------------------------------------------------------------------------------

} // End of ymir
