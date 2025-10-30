cmake_minimum_required(VERSION 3.22)
project(YourEngine LANGUAGES CXX)

# ====== Configuración básica ======
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPORTANTE:
# NO pongas aquí el toolchain de vcpkg.
# Pásalo por línea de comandos:
#   -DCMAKE_TOOLCHAIN_FILE=C:/Users/TU_USUARIO/vcpkg/scripts/buildsystems/vcpkg.cmake

# Define para helpers de ImGui
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)

# ====== Dependencias (vcpkg manifest) ======
# Requiere vcpkg.json en la raíz con: sdl2, assimp, glad, glm, devil, imgui[ sdl2-binding, opengl3-binding ]
find_package(SDL2 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
# glad puede exportar dos nombres de target según el port usado
find_package(glad CONFIG QUIET)
if(glad_FOUND)
  set(GLAD_TARGET glad::glad)
else()
  find_package(unofficial-glad CONFIG REQUIRED)
  set(GLAD_TARGET unofficial::glad::glad)
endif()
find_package(imgui CONFIG REQUIRED)
find_package(DevIL CONFIG REQUIRED)  # IL::IL / IL::ILU / IL::ILUT
find_package(glm CONFIG REQUIRED)

# ====== Fuentes ======
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
  src/*.cpp
  src/*.c
  src/*.h
  src/*.hpp
)
add_executable(${PROJECT_NAME} ${SRC})

# ====== Warnings por plataforma ======
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ====== Include dirs (si hace falta añadir algo propio) ======
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ====== Enlazado ======
target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL2::SDL2
  SDL2::SDL2main
  ${GLAD_TARGET}
  imgui::imgui
  assimp::assimp
  IL::IL
  IL::ILU
  IL::ILUT
  glm::glm
)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets
)

# ====== (Opcional) Salidas organizadas por config ======
# set_target_properties(${PROJECT_NAME} PROPERTIES
#   RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/bin/Debug
#   RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
# )