set_property(DIRECTORY PROPERTY LLDB_PLUGIN_KIND ScriptInterpreter)

add_subdirectory(None)
if (LLDB_ENABLE_PYTHON)
  add_subdirectory(Python)
endif()

if (LLDB_ENABLE_LUA)
  add_subdirectory(Lua)
endif()
