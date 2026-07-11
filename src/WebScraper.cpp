#include "WebScraper.h"

#include "Utilities.h"

#include <cpr/cpr.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

constexpr std::size_t MAX_HEADINGS = 50;
constexpr std::size_t MAX_PARAGRAPHS = 50;
constexpr std::size_t MAX_LINKS = 100;

} // namespace

/*
Downloads and parses one webpage.
*/
ScrapedPage WebScraper::scrape(const std::string& url) const {
    const cpr::Response response = cpr::Get(
        cpr::Url{url},
        cpr::Header{
            {"User-Agent", "CSE310-Cpp-Web-Scraper/1.0"}
        },
        cpr::Timeout{15000},
        cpr::Redirect{true}
    );

    if (response.error.code != cpr::ErrorCode::OK) {
        throw std::runtime_error(
            "Network error: " + response.error.message
        );
    }

    if (response.status_code < 200 ||
        response.status_code >= 400) {
        throw std::runtime_error(
            "The server returned HTTP status " +
            std::to_string(response.status_code) +
            "."
        );
    }

    htmlDocPtr document = htmlReadMemory(
        response.text.c_str(),
        static_cast<int>(response.text.size()),
        url.c_str(),
        nullptr,
        HTML_PARSE_RECOVER |
        HTML_PARSE_NOERROR |
        HTML_PARSE_NOWARNING |
        HTML_PARSE_NONET
    );

    if (document == nullptr) {
        throw std::runtime_error(
            "The downloaded content could not be parsed as HTML."
        );
    }

    ScrapedPage page;

    page.url = url;
    page.title = firstText(document, "//title");
    page.scrapedAt = currentTimestamp();
    page.statusCode = response.status_code;

    page.headings = collectText(
        document,
        "//h1 | //h2 | //h3 | //h4 | //h5 | //h6",
        MAX_HEADINGS
    );

    page.paragraphs = collectText(
        document,
        "//p",
        MAX_PARAGRAPHS
    );

    page.links = collectLinks(
        document,
        url,
        MAX_LINKS
    );

    xmlFreeDoc(document);

    return page;
}

/*
Returns text from the first HTML node matching an XPath expression.
*/
std::string WebScraper::firstText(
    xmlDocPtr document,
    const char* xpathExpression
) {
    const std::vector<std::string> values =
        collectText(document, xpathExpression, 1);

    return values.empty() ? "" : values.front();
}

/*
Collects unique text from HTML nodes matching an XPath expression.
*/
std::vector<std::string> WebScraper::collectText(
    xmlDocPtr document,
    const char* xpathExpression,
    std::size_t maximum
) {
    std::vector<std::string> results;
    std::unordered_set<std::string> seen;

    xmlXPathContextPtr context =
        xmlXPathNewContext(document);

    if (context == nullptr) {
        return results;
    }

    xmlXPathObjectPtr xpathResult =
        xmlXPathEvalExpression(
            reinterpret_cast<const xmlChar*>(xpathExpression),
            context
        );

    if (xpathResult != nullptr &&
        xpathResult->nodesetval != nullptr) {
        const int nodeCount =
            xpathResult->nodesetval->nodeNr;

        for (
            int index = 0;
            index < nodeCount &&
            results.size() < maximum;
            ++index
        ) {
            xmlNodePtr node =
                xpathResult->nodesetval->nodeTab[index];

            xmlChar* rawText = xmlNodeGetContent(node);

            if (rawText == nullptr) {
                continue;
            }

            const std::string text = cleanText(
                reinterpret_cast<const char*>(rawText)
            );

            xmlFree(rawText);

            if (!text.empty() &&
                seen.insert(text).second) {
                results.push_back(text);
            }
        }
    }

    if (xpathResult != nullptr) {
        xmlXPathFreeObject(xpathResult);
    }

    xmlXPathFreeContext(context);

    return results;
}

/*
Collects unique links from anchor elements.
*/
std::vector<std::string> WebScraper::collectLinks(
    xmlDocPtr document,
    const std::string& pageUrl,
    std::size_t maximum
) {
    std::vector<std::string> links;
    std::unordered_set<std::string> seen;

    xmlXPathContextPtr context =
        xmlXPathNewContext(document);

    if (context == nullptr) {
        return links;
    }

    xmlXPathObjectPtr xpathResult =
        xmlXPathEvalExpression(
            reinterpret_cast<const xmlChar*>("//a[@href]"),
            context
        );

    if (xpathResult != nullptr &&
        xpathResult->nodesetval != nullptr) {
        const int nodeCount =
            xpathResult->nodesetval->nodeNr;

        for (
            int index = 0;
            index < nodeCount &&
            links.size() < maximum;
            ++index
        ) {
            xmlNodePtr node =
                xpathResult->nodesetval->nodeTab[index];

            xmlChar* href = xmlGetProp(
                node,
                reinterpret_cast<const xmlChar*>("href")
            );

            if (href == nullptr) {
                continue;
            }

            const std::string relativeLink = trim(
                reinterpret_cast<const char*>(href)
            );

            const std::string absoluteLink =
                makeAbsoluteUrl(pageUrl, relativeLink);

            xmlFree(href);

            if (!absoluteLink.empty() &&
                seen.insert(absoluteLink).second) {
                links.push_back(absoluteLink);
            }
        }
    }

    if (xpathResult != nullptr) {
        xmlXPathFreeObject(xpathResult);
    }

    xmlXPathFreeContext(context);

    return links;
}