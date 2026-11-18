#-------------------------------------------------------------------------------
    #
    #  Filename       : CMakeLists.txt
    #  Author         : Huang Leilei
    #  Status         : phase 000
    #  Created        : 2026-09-17
    #  Description    : input list for cmake
    #
#-------------------------------------------------------------------------------

#*** GENERAL *******************************************************************
    cmake_minimum_required (VERSION 2.8)

    project(xkjpg)

    option(FLAG_X86 "use X86 instead of X64"    OFF)
    option(FLAG_ASM "use x265 assembly func"    OFF)

    if (${FLAG_X86})
    message("[CMakeLists.txt] X86 IS ENABLED!")
    message("[CMakeLists.txt] (if you do not have X86 packages, please install it with \"sudo apt-get install gcc-multilib g++-multilib\".)")
    endif()

    if (${FLAG_ASM})
    message("[CMakeLists.txt] ASM IS ENABLED!")
    message("[CMakeLists.txt] (if you do not have ASM packages, please intall it with \"cd ../script/getNasm ; ./install.sh\".)")
    endif()

    set(CMAKE_C_FLAGS      "${CMAKE_C_FLAGS}              -g -Wall -O3 -static -fno-strict-aliasing -pthread")
    set(CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -O3 -static -fno-strict-aliasing -pthread")

    if (${FLAG_X86})
    set(CMAKE_C_FLAGS      "${CMAKE_C_FLAGS}   -m32"        )
    set(CMAKE_CXX_FLAGS    "${CMAKE_CXX_FLAGS} -m32 -mssse3")
    endif()

    if (${FLAG_ASM})
    enable_language(ASM_NASM)
    endif()


#*** DEFINITION ****************************************************************
    if (${FLAG_ASM})
    add_definitions(-DFLAG_ASM)
    endif()


#*** INCLUDE *******************************************************************
    include_directories(../top                               )
    include_directories(../top/configuration                 )
    include_directories(../kernel                            )
    include_directories(../kernel/fetcher                    )
    include_directories(../kernel/dumper                     )
    include_directories(../unit                              )
    include_directories(../unit/discrete_cosine_transform    )
    include_directories(../unit/huffman_encoding             )
    include_directories(../unit/huffman_decoding             )
    include_directories(../common                            )


#*** SOURCE ********************************************************************
    aux_source_directory(../top                                SOURCE)
    aux_source_directory(../top/configuration                  SOURCE)
    aux_source_directory(../kernel                             SOURCE)
    aux_source_directory(../kernel/fetcher                     SOURCE)
    aux_source_directory(../kernel/dumper                      SOURCE)
    aux_source_directory(../unit                               SOURCE)
    aux_source_directory(../unit/discrete_cosine_transform     SOURCE)
    aux_source_directory(../unit/huffman_encoding              SOURCE)
    aux_source_directory(../unit/huffman_decoding              SOURCE)
    aux_source_directory(../common                             SOURCE)


#*** EXECUTION *****************************************************************
    add_executable(xkjpg ${SOURCE})
