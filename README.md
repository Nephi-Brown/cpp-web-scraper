# C++ Web Scraper

# Overview

As a software engineer, I am continually expanding my knowledge of programming languages and software design by building projects that apply real-world concepts. This project allowed me to learn how C++ can be used outside of traditional console applications by interacting with web resources, parsing HTML, and managing persistent data through file operations.

This software is a console-based web scraper written in C++. The program prompts the user to enter a website URL, downloads the webpage, extracts useful information such as the page title, headings, paragraphs, and hyperlinks, and saves the collected data to a text file. If a website has already been scraped, the user can choose to replace only that website's saved information while preserving all previously scraped websites.

The purpose of writing this software was to gain hands-on experience with modern C++ programming while learning how to work with external libraries, perform HTTP requests, parse HTML documents, organize a multi-file C++ project, and read from and write to files. Throughout this project, I strengthened my understanding of object-oriented programming, the C++ Standard Library, error handling, and project organization.

[[Software Demo Video](https://www.loom.com/share/8deb242b2ebc4065a3d184370bdf3f89)] 

# Development Environment

**Tools Used**
- Visual Studio Code
- CMake
- vcpkg
- Git
- Apple Clang Compiler (macOS)

**Programming Language and Libraries**
- C++17
- CPR (HTTP requests)
- libxml2 (HTML parsing and XPath)
- C++ Standard Library (STL), including:
  - `std::vector`
  - `std::unordered_set`
  - `std::string`
  - `std::ifstream`
  - `std::ofstream`
  - Exception handling

# Useful Websites

- [Microsoft C++ Language Reference](https://learn.microsoft.com/en-us/cpp/cpp/cpp-language-reference?view=msvc-170)
- [brightdata](https://brightdata.com/blog/how-tos/web-scraping-in-c-plus-plus)
- [zenrows](https://www.zenrows.com/blog/c-plus-plus-web-scraping#request-target-page)
- [Learn C++](https://www.learncpp.com/)

# Future Work

- Improve URL validation and support additional URL formats.
- Add support for exporting scraped data to CSV or JSON.
- Allow users to choose which HTML elements to scrape instead of collecting a fixed set of data.
- Add multithreading to scrape multiple websites concurrently.
- Improve error handling for websites that block automated requests or require JavaScript.
