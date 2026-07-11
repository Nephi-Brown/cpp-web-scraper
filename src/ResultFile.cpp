#include "ResultFile.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

/*
Stores the name of the output file.
*/
ResultFile::ResultFile(std::string filename)
    : filename_(std::move(filename)) {
}

/*
Checks whether a saved section already exists for a URL.
*/
bool ResultFile::containsSite(const std::string& url) const {
    const std::string contents = readAll();

    return contents.find(beginMarker(url)) !=
           std::string::npos;
}

/*
Saves a new website or replaces one existing website section.
*/
bool ResultFile::save(
    const ScrapedPage& page,
    bool replaceExisting
) const {
    std::string contents = readAll();

    const std::string begin = beginMarker(page.url);
    const std::string end = endMarker(page.url);

    const std::size_t beginPosition =
        contents.find(begin);

    if (beginPosition != std::string::npos) {
        if (!replaceExisting) {
            return false;
        }

        const std::size_t endPosition =
            contents.find(end, beginPosition);

        if (endPosition == std::string::npos) {
            throw std::runtime_error(
                "The results file contains an incomplete site section."
            );
        }

        const std::size_t sectionEnd =
            endPosition + end.size();

        contents.replace(
            beginPosition,
            sectionEnd - beginPosition,
            page.toText()
        );
    } else {
        if (!contents.empty() &&
            contents.back() != '\n') {
            contents.push_back('\n');
        }

        if (!contents.empty()) {
            contents.push_back('\n');
        }

        contents += page.toText();
    }

    writeAll(contents);

    return true;
}

/*
Returns the name of the output file.
*/
const std::string& ResultFile::filename() const {
    return filename_;
}

/*
Reads the entire contents of the output file.

An empty string is returned if the file does not exist yet.
*/
std::string ResultFile::readAll() const {
    std::ifstream input(filename_);

    if (!input) {
        return "";
    }

    std::ostringstream contents;
    contents << input.rdbuf();

    return contents.str();
}

/*
Writes all updated contents to the output file.
*/
void ResultFile::writeAll(
    const std::string& contents
) const {
    std::ofstream output(
        filename_,
        std::ios::trunc
    );

    if (!output) {
        throw std::runtime_error(
            "Could not open " +
            filename_ +
            " for writing."
        );
    }

    output << contents;

    if (!output) {
        throw std::runtime_error(
            "An error occurred while writing to " +
            filename_ +
            "."
        );
    }
}

/*
Creates the beginning marker for a saved URL.
*/
std::string ResultFile::beginMarker(
    const std::string& url
) {
    return "===== BEGIN SITE: " + url + " =====";
}

/*
Creates the ending marker for a saved URL.
*/
std::string ResultFile::endMarker(
    const std::string& url
) {
    return "===== END SITE: " + url + " =====";
}