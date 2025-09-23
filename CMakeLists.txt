cmake_minimum_required(VERSION 3.16) 
project(MyProject) 

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file") 

#set(OpenCV_DIR "C:/vcpkg/installed/x64-windows/share/opencv4")
#set(Protobuf_DIR "C:/vcpkg/installed/x64-windows/share/protobuf")
#set(quirc_DIR "C:/vcpkg/installed/x64-windows/share/quirc")
#set(TIFF_INCLUDE_DIR "C:/vcpkg/installed/x64-windows/share")
#set(TIFF_LIBRARY "C:/vcpkg/installed/x64-windows/share")

#set(OpenCV_DIR "C:/vcpkg/packages/opencv4_x64-windows/share/opencv4")
#set(OpenCV_DIR "C:/vcpkg/buildtrees/opencv4/x64-windows-dbg")
#set(OpenCV_DIR "C:/vcpkg/buildtrees/opencv4/x64-windows-rel")
#set(OpenCV_DIR "C:/opencv/cmake")


#set(Protobuf_DIR "C:/vcpkg/buildtrees/protobuf/x64-windows-dbg/cmake/protobuf")
#set(quirc_DIR, "C:/vcpkg/installed/x64-windows/share/quirc")
#set(CMAKE_PREFIX_PATH, "C:/vcpkg/installed/x64-windows/share")
#set(quirc_DIR, "C:/vcpkg/packages/quirc_x64-windows/share/quirc")
#set(quirc_DIR, "C:/vcpkg/buildtrees/quirc/x64-windows-rel/CMakeFiles/Export/dda14f499e67c865f1fe2ae0908248bd")
#set(quirc_DIR, "C:/vcpkg/buildtrees/quirc/x64-windows-dbg/CMakeFiles/Export/dda14f499e67c865f1fe2ae0908248bd")
#find_package(quirc CONFIG REQUIRED)
#target_link_libraries(main PRIVATE quirc::quirc)

find_package(OpenCV REQUIRED) 
add_executable(MyProject main.cpp) 
target_link_libraries(MyProject PRIVATE ${OpenCV_LIBS})