#pragma once

#include <string>
#include <vector>

/*
Stores all information collected from one webpage.
*/
class ScrapedPage {
public:
    std::string url;
    std::string title;
    std::string scrapedAt;

    long statusCode = 0;

    std::vector<std::string> headings;
    std::vector<std::string> paragraphs;
    std::vector<std::string> links;

    std::string beginMarker() const;
    std::string endMarker() const;
    std::string toText() const;
};