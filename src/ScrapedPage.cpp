#include "ScrapedPage.h"

#include <sstream>

/*
Writes a numbered section such as headings, paragraphs, or links.
*/
namespace {

void writeSection(
    std::ostringstream& output,
    const std::string& name,
    const std::vector<std::string>& items
) {
    output << name << " (" << items.size() << ")\n";

    for (std::size_t index = 0; index < items.size(); ++index) {
        output << index + 1 << ". " << items[index] << '\n';
    }

    output << '\n';
}

} // namespace

/*
Creates the marker placed before this website's saved information.
*/
std::string ScrapedPage::beginMarker() const {
    return "===== BEGIN SITE: " + url + " =====";
}

/*
Creates the marker placed after this website's saved information.
*/
std::string ScrapedPage::endMarker() const {
    return "===== END SITE: " + url + " =====";
}

/*
Converts all scraped webpage information into formatted text.
*/
std::string ScrapedPage::toText() const {
    std::ostringstream output;

    output << beginMarker() << '\n'
           << "URL: " << url << '\n'
           << "Scraped at: " << scrapedAt << '\n'
           << "HTTP status: " << statusCode << '\n'
           << "Title: "
           << (title.empty() ? "(none found)" : title)
           << "\n\n";

    writeSection(output, "HEADINGS", headings);
    writeSection(output, "PARAGRAPHS", paragraphs);
    writeSection(output, "LINKS", links);

    output << endMarker() << '\n';

    return output.str();
}