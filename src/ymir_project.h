#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace ymir { // Start of ymir

/// -------------------------------------------------------------------------------------------------
/// ProjectType
enum ProjectType {
  PROJECT_CONSOLE_APP = 0, 
  PROJECT_GUI_APP, 
  PROJECT_LIBRARY, 
  PROJECT_INVALID, 
};
/// ProjectType
/// -------------------------------------------------------------------------------------------------

/// -------------------------------------------------------------------------------------------------
/// ProjectDesc
struct ProjectDesc {
  std::string name; 
  std::filesystem::path full_path;

  ProjectType type; 

  int cpp_version = 20; 
  
  std::string definitions; 
  std::string compiler_flags;

  std::filesystem::path src_dir     = "src";
  std::filesystem::path include_dir = "src"; 
  std::filesystem::path deps_dir    = "libs";

  bool has_git = false;
  
};
/// ProjectDesc
/// -------------------------------------------------------------------------------------------------

/// -------------------------------------------------------------------------------------------------
/// Project functions

bool project_create(ProjectDesc& desc);

/// Project functions
/// -------------------------------------------------------------------------------------------------

} // End of ymir
