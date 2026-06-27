cmake_minimum_required(VERSION 3.19)
project(omp-dotenv LANGUAGES CXX DESCRIPTION ".env support for open.mp and sa-mp (pawn natives)")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(UNIX AND NOT APPLE)
    add_compile_options(-m32)
    add_link_options(-m32)
elseif(MSVC)
    # configure with cmake -A Win32 for a 32-bit build
endif()

# open.mp sdk (also pulls glm, robin-hood, span and string-view)
add_subdirectory(lib/omp-sdk)

# sources shared by both the open.mp component and the sa-mp plugin
set(DOTENV_SHARED_SOURCES
    src/dotenv.cpp
    src/natives.cpp
)

# apply the common include paths and flags to a dotenv target
function(dotenv_configure_target target)
    target_include_directories(${target} PRIVATE
        src
        lib                    # pawn-natives
        lib/pawn/source        # amx headers
        lib/pawn/source/linux  # sclinux on linux
        lib/samp-sdk           # plugincommon.h
    )
    target_link_libraries(${target} PRIVATE OMP-SDK)
    # tell amx.h to use msvc's stdint instead of re-typedefing the int types
    if(MSVC)
        target_compile_definitions(${target} PRIVATE HAVE_STDINT_H)
    endif()
    # output without the lib prefix so servers can load it by plain name
    set_target_properties(${target} PROPERTIES
        PREFIX ""
        CXX_VISIBILITY_PRESET hidden
    )
endfunction()

# open.mp component -> omp-dotenv.so / omp-dotenv.dll (goes in components/)
add_library(omp-dotenv MODULE ${DOTENV_SHARED_SOURCES} src/component.cpp)
dotenv_configure_target(omp-dotenv)
set_target_properties(omp-dotenv PROPERTIES OUTPUT_NAME "omp-dotenv")

# sa-mp legacy plugin -> dotenv.so / dotenv.dll (goes in plugins/)
add_library(dotenv MODULE ${DOTENV_SHARED_SOURCES} src/plugin.cpp)
dotenv_configure_target(dotenv)
set_target_properties(dotenv PROPERTIES OUTPUT_NAME "dotenv")