cmake_minimum_required(VERSION 3.19)
project(omp-dotenv LANGUAGES CXX DESCRIPTION ".env support for open.mp (Pawn natives)")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# open.mp servers are 32-bit, so the component must be built 32-bit too
if(UNIX AND NOT APPLE)
    add_compile_options(-m32)
    add_link_options(-m32)
elseif(MSVC)
    # configure with cmake -A Win32 for a 32-bit build
endif()

# open.mp sdk (also pulls glm, robin-hood, span and string-view)
add_subdirectory(lib/omp-sdk)

add_library(${PROJECT_NAME} MODULE
    src/dotenv.cpp
    src/component.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    lib                    # pawn-natives
    lib/pawn/source        # amx headers
    lib/pawn/source/linux  # sclinux on linux
)

target_link_libraries(${PROJECT_NAME} PRIVATE OMP-SDK)

# tell amx.h to use msvc's stdint instead of re-typedefing the int types
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_STDINT_H)
endif()

# output omp-dotenv.so / omp-dotenv.dll without the lib prefix for components
set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "omp-dotenv"
    CXX_VISIBILITY_PRESET hidden
)