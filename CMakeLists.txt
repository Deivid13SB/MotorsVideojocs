cmake_minimum_required(VERSION 3.22)
project(YourEngine LANGUAGES CXX) 
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 


# vcpkg toolchain
# set(CMAKE_TOOLCHAIN_FILE 
"${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "") 


add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS) 


find_package(SDL2 CONFIG REQUIRED) 
find_package(assimp CONFIG REQUIRED) 
find_package(unofficial-glad CONFIG REQUIRED) 
find_package(imgui CONFIG REQUIRED) 
find_package(DevIL CONFIG REQUIRED) # IL, ILU, ILUT 
find_package(glm CONFIG REQUIRED) # o MathGeoLib 


file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp src/*.h) add_executable(${PROJECT_NAME} ${SRC}) 



target_link_libraries(${PROJECT_NAME} PRIVATE 
SDL2::SDL2 SDL2::SDL2main 
unofficial::glad::glad 
imgui::imgui 
assimp::assimp 
IL::IL IL::ILU IL::ILUT 
glm::glm 
) 


if(MSVC) 
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-) 
else() 
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic) 
endif() 



add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
COMMAND ${CMAKE_COMMAND} -E copy_directory 
${CMAKE_SOURCE_DIR}/assets 
$<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets)