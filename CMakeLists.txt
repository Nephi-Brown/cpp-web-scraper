cmake_minimum_required(VERSION 3.20)

project(CppWebScraper VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(cpr CONFIG REQUIRED)
find_package(LibXml2 REQUIRED)

add_executable(
    scraper
    src/main.cpp
    src/ScrapedPage.cpp
    src/WebScraper.cpp
    src/ResultFile.cpp
    src/Utilities.cpp
)

target_include_directories(
    scraper
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
    scraper
    PRIVATE
    cpr::cpr
    LibXml2::LibXml2
)

if(MSVC)
    target_compile_options(
        scraper
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        scraper
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()