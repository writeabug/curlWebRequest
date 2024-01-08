# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)


project("web")

add_library(${CMAKE_PROJECT_NAME} SHARED
        unity.cpp
        jsoncpp.cpp
        web.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_LIST_DIR} PATH)
add_library(libcurl SHARED IMPORTED)
set_target_properties(libcurl PROPERTIES IMPORTED_LOCATION ${PARENT_DIR}/jniLibs/${ANDROID_ABI}/libcurl.so)
find_library(log-lib log)
target_link_libraries(${CMAKE_PROJECT_NAME}
        libcurl
        ${log-lib}
        android
        log)