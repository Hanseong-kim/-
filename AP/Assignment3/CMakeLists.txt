cmake_minimum_required(VERSION 3.10)
project(GUI_Assignment)

set(CMAKE_CXX_STANDARD 17)

# SDL2 및 확장 라이브러리 찾기
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# include 디렉토리 설정
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# 실행 파일 추가
add_executable(main05 main05.cc gui.cc)

# 라이브러리 링크
target_link_libraries(main05 ${SDL2_LIBRARIES} SDL2_ttf SDL2_image)
