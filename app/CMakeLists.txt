# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


file(GLOB CLIENT_SRCS "./src/main/cpp/client/*.cpp")
file(GLOB COMMON_SRCS "./src/main/cpp/common/*.cpp")


include_directories(./src/main/cpp/client ./src/main/cpp/common)

add_library(p2pclient-lib SHARED ${CLIENT_SRCS} ${COMMON_SRCS} ./src/main/cpp/clientnodemain.cpp)


find_library(log-lib log)
target_link_libraries(p2pclient-lib ${log-lib})

