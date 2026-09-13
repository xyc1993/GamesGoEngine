cmake_minimum_required(VERSION 3.20)

project(GamesGoEngine LANGUAGES CXX)

set(GAMES_GO_ENGINE_SOURCES
    Source/Engine/Core/Public/GamesGoEngineExport.h

    Source/Engine/Diagnostics/Public/Log.h
    Source/Engine/Diagnostics/Private/Log.cpp
)

add_library(GamesGoEngine SHARED
    ${GAMES_GO_ENGINE_SOURCES}
)

target_include_directories(GamesGoEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine/Core/Public
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine/Diagnostics/Public

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine/Diagnostics/Private
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source
    FILES ${GAMES_GO_ENGINE_SOURCES}
)