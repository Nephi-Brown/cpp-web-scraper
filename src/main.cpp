#include "ResultFile.h"
#include "Utilities.h"
#include "WebScraper.h"

#include <libxml/parser.h>

#include <exception>
#include <iostream>
#include <string>

namespace {

constexpr const char* OUTPUT_FILE = "scraped_sites.txt";

/*
Processes one website entered by the user.
*/
void processWebsite(
    const WebScraper& scraper,
    const ResultFile& resultFile
) {
    std::cout << "Enter a website URL: ";

    std::string url;
    std::getline(std::cin, url);

    url = normalizeUrl(url);

    if (!isValidUrl(url)) {
        std::cout
            << "That does not look like a valid "
            << "HTTP or HTTPS URL.\n";

        return;
    }

    bool replaceExisting = false;

    if (resultFile.containsSite(url)) {
        replaceExisting = askYesNo(
            "This site is already saved. "
            "Replace only its old section?"
        );

        if (!replaceExisting) {
            std::cout
                << "The existing information was preserved.\n";

            return;
        }
    }

    std::cout
        << "Downloading and parsing "
        << url
        << "...\n";

    const ScrapedPage page = scraper.scrape(url);

    resultFile.save(page, replaceExisting);

    std::cout
        << "Saved successfully.\n"
        << "Found "
        << page.headings.size()
        << " headings, "
        << page.paragraphs.size()
        << " paragraphs, and "
        << page.links.size()
        << " links.\n"
        << "Open "
        << resultFile.filename()
        << " to view the results.\n";
}

} // namespace

/*
Runs the console application.
*/
int main() {
    xmlInitParser();

    WebScraper scraper;
    ResultFile resultFile(OUTPUT_FILE);

    std::cout
        << "C++ Web Scraper\n"
        << "This program saves titles, headings, "
        << "paragraphs, and links.\n"
        << "Results file: "
        << resultFile.filename()
        << "\n\n";

    bool keepRunning = true;

    while (keepRunning) {
        try {
            processWebsite(scraper, resultFile);
        } catch (const std::exception& error) {
            std::cerr
                << "Scrape failed: "
                << error.what()
                << '\n';
        }

        std::cout << '\n';
        keepRunning = askYesNo("Scrape another website?");
        std::cout << '\n';
    }

    xmlCleanupParser();

    std::cout << "Goodbye!\n";

    return 0;
}