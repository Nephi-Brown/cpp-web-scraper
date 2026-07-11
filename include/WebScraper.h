#pragma once

#include "ScrapedPage.h"

#include <libxml/tree.h>

#include <cstddef>
#include <string>
#include <vector>

/*
Downloads webpages and extracts information from HTML.
*/
class WebScraper {
public:
    ScrapedPage scrape(const std::string& url) const;

private:
    static std::string firstText(
        xmlDocPtr document,
        const char* xpathExpression
    );

    static std::vector<std::string> collectText(
        xmlDocPtr document,
        const char* xpathExpression,
        std::size_t maximum
    );

    static std::vector<std::string> collectLinks(
        xmlDocPtr document,
        const std::string& pageUrl,
        std::size_t maximum
    );
};