cmake_minimum_required(VERSION 3.19)
project(omp-dotenv LANGUAGES CXX DESCRIPTION ".env support for open.mp (Pawn natives)")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# open.mp servers are 32-bit (i386) on Linux / x86 on Windows. The component
# must match. Force a 32-bit build (needs a 32-bit toolchain / multilib).
if(UNIX AND NOT APPLE)
    add_compile_options(-m32)
    add_link_options(-m32)
elseif(MSVC)
    # Configure with: cmake -A Win32 ...
endif()

# open.mp SDK (also pulls glm / robin-hood / span / string-view).
add_subdirectory(lib/omp-sdk)

add_library(${PROJECT_NAME} MODULE
    src/dotenv.cpp
    src/component.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    lib                    # <pawn-natives/...>
    lib/pawn/source        # <amx/amx.h>
    lib/pawn/source/linux  # <sclinux.h> (Linux build)
)

target_link_libraries(${PROJECT_NAME} PRIVATE OMP-SDK)

# Output "omp-dotenv.so" / "omp-dotenv.dll" (no "lib" prefix) for components/.
set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "omp-dotenv"
    CXX_VISIBILITY_PRESET hidden
)