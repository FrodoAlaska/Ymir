#include "ymir_project.h"

#include <cstdio>
#include <string>

/// -------------------------------------------------------------------------------------------------
/// ArgumentType 
enum ArgumentType {
  ARGUMENT_PROJECT_NEW = 0,

  ARGUMENT_STANDARD_VERSION, 
  ARGUMENT_PROJECT_TYPE, 
  ARGUMENT_DEFINITIONS, 
  ARGUMENT_COMPILER, 
  
  ARGUMENT_SRC_DIR,
  ARGUMENT_INCLUDE_DIR,
  ARGUMENT_DEPS_DIR,
  
  ARGUMENT_HAS_GIT,
  
  ARGUMENT_HELP, 

  ARGUMENTS_MAX = ARGUMENT_HELP + 1,
};
/// ArgumentType 
/// -------------------------------------------------------------------------------------------------

/// -------------------------------------------------------------------------------------------------
/// Argument
struct Argument {
  ArgumentType type; 

  std::string option;
  std::string alt_option;

  std::string desc;
};
/// Argument
/// -------------------------------------------------------------------------------------------------

/// -------------------------------------------------------------------------------------------------
/// Private functions

static ymir::ProjectType get_project_type_from_str(const std::string& str) {
  if(str == "CONSOLE_APP") {
    return ymir::PROJECT_CONSOLE_APP;
  }
  else if(str == "GUI_APP") {
    return ymir::PROJECT_GUI_APP;
  }
  else if(str == "LIBRARY") {
    return ymir::PROJECT_LIBRARY;
  }

  return ymir::PROJECT_INVALID;
}

/// Private functions
/// -------------------------------------------------------------------------------------------------

/// -------------------------------------------------------------------------------------------------
/// Argument functions

bool args_is_equal(Argument& argument, char* c_arg) {
  return argument.option == c_arg || argument.alt_option == c_arg;
}

bool args_show_help(const Argument* args) {
  printf("\n### ### Ymir: A C++ project generator ### ###\n\n");

  printf("[Usage]: ymir [options] --new <project_name> \n");
  for(int i = 0; i < ARGUMENTS_MAX; i++) {
    std::string prefix = "\t[" + args[i].option + ", " + args[i].alt_option + "]"; 
    printf("%s = %s\n", prefix.c_str(), args[i].desc.c_str());
  }

  printf("\n### ### Ymir: A C++ project generator ### ###\n\n");

  return true;
}

bool args_parse(ymir::ProjectDesc* out_desc, int argc, char** argv) {
  if(argc < 2) {
    printf("[YMIR-ERROR]: Insufficient amount of arguments given\n");
    return false;
  }

  Argument args[ARGUMENTS_MAX] = {
    {ARGUMENT_PROJECT_NEW, "--new", "-n", "The name of the new project"},
    
    {ARGUMENT_STANDARD_VERSION, "--standard",    "-std", "Define the standard of the language (by default, this is 20)"},
    {ARGUMENT_PROJECT_TYPE,     "--type",        "-t",   "Can be either CONSOLE_APP, GUI_APP, or LIBRARY"},
    {ARGUMENT_DEFINITIONS,      "--definitions", "-d",   "List all of the project's definitions inside quotes"},
    {ARGUMENT_COMPILER,         "--flags", "-f",         "List all of the project's compiler flags inside quotes"},
    
    {ARGUMENT_SRC_DIR,        "--src-dir",     "-sd", "Define the name of the source directory (by default, this is \'src\')"},
    {ARGUMENT_INCLUDE_DIR,    "--include-dir", "-id", "Define the name of the include directory (by default, this is \'src\')"},
    {ARGUMENT_DEPS_DIR,       "--deps-dir",    "-dd", "Define the name of the dependencies directory (by default, this is \'libs\')"},
    
    {ARGUMENT_HAS_GIT, "--has-git",  "-g", "Create both a .gitignore and a .gitattributes file if set (by default, this is unset)"},
    
    {ARGUMENT_HELP,   "--help", "-h", "Show this help screen"},
  };

  // @NOTE: Yeah, awful. Terrible. Slow, even. But it works.
  for(int i = 1; i < argc; i++) {
    if(args_is_equal(args[ARGUMENT_PROJECT_NEW], argv[i])) {
      out_desc->name = argv[++i];
    }
    else if(args_is_equal(args[ARGUMENT_STANDARD_VERSION], argv[i])) {
      out_desc->cpp_version = std::stoi(argv[++i]);
    }
    else if(args_is_equal(args[ARGUMENT_PROJECT_TYPE], argv[i])) {
      out_desc->type = get_project_type_from_str(argv[++i]);
    }
    else if(args_is_equal(args[ARGUMENT_DEFINITIONS], argv[i])) {
      out_desc->definitions = argv[++i];
    }
    else if(args_is_equal(args[ARGUMENT_COMPILER], argv[i])) {
      out_desc->compiler_flags = argv[++i];
    }
    else if(args_is_equal(args[ARGUMENT_SRC_DIR], argv[i])) {
      out_desc->src_dir = argv[++i];
    }
    else if(args_is_equal(args[ARGUMENT_INCLUDE_DIR], argv[i])) {
      out_desc->include_dir = argv[++i];
    }
    else if(args_is_equal(args[ARGUMENT_DEPS_DIR], argv[i])) {
      out_desc->deps_dir = argv[++i];
    }
    else if(args_is_equal(args[ARGUMENT_HAS_GIT], argv[i])) {
      out_desc->has_git = true;
    }
    else if(args_is_equal(args[ARGUMENT_HELP], argv[i])) {
      args_show_help(args);
      return false;
    }
  }

  // Just a sanity check
  if(out_desc->type == ymir::PROJECT_INVALID) {
    return false;
  }

  return true;
}

/// Argument functions
/// -------------------------------------------------------------------------------------------------

/// -------------------------------------------------------------------------------------------------
/// Main function
int main(int argc, char** argv) {
  ymir::ProjectDesc desc;
  if(!args_parse(&desc, argc, argv)) {
    return -1;
  }

  // Create the project if all goes well
  if(!ymir::project_create(desc)) {
    return -1;
  }

  return 0;
}
/// Main function
/// -------------------------------------------------------------------------------------------------
