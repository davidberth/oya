﻿cmake_minimum_required (VERSION 3.30)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (oya VERSION 0.1.0)

set(APPNAME ${CMAKE_PROJECT_NAME})
set(APPVERSION ${CMAKE_PROJECT_VERSION})


add_subdirectory(configure)
add_subdirectory(src)
