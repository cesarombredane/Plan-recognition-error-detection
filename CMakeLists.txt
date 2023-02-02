cmake_minimum_required(VERSION 3.24)
project(Plan_recognition_error_detection)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/headers)

add_executable(Plan_recognition_error_detection
        src/context/extendedPlanLibrary.cpp
        src/context/planLibrary.cpp
        src/context/probabilityDistribution.cpp
        src/context/rule.cpp
        src/headers/CognitiveDistressManagement.h
        src/headers/extendedPlanLibrary.h
        src/headers/planLibrary.h
        src/headers/probabilityDistribution.h
        src/headers/rule.h
        src/headers/solver.h
        src/solver/CognitiveDistressManagement.cpp
        src/solver/solver.cpp
        src/main.cpp)
