cmake_minimum_required(VERSION 3.27)
project(OpenGL_Engine)

set(CMAKE_CXX_STANDARD 23)


find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)


include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(include)
include_directories(src)



#Platform-specific settings
#if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
#
#
#    # macOS specific settings
#    add_executable(OpenGL_Engine MACOSX_BUNDLE main.cpp src/glad.c
#            src/MeshLoader.hpp
#            src/Vectors.hpp
#
#    )
#
#    # Set properties for the macOS bundle
#    set_target_properties(OpenGL_Engine PROPERTIES
#            MACOSX_BUNDLE_GUI_IDENTIFIER your.domain.app
#            MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
#            MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
#
#
#            # Optionally set an icon: MACOSX_BUNDLE_ICON_FILE YourIcon.icns
#    )
#
#
#
#
#
#
#
#else()
    # Non-macOS settings
    add_executable(OpenGL_Engine main.cpp src/glad.c
            src/MeshLoader.hpp
            src/Vectors.hpp
    )
#endif()




target_link_libraries(OpenGL_Engine ${GLM_LIBRARIES})

target_link_libraries(OpenGL_Engine glfw)

target_link_libraries(OpenGL_Engine OpenGL::GL)



