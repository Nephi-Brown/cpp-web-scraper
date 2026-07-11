#pragma once

#include "ScrapedPage.h"

#include <string>

/*
Reads from and writes to the scraper output file.
*/
class ResultFile {
public:
    explicit ResultFile(std::string filename);

    bool containsSite(const std::string& url) const;

    bool save(
        const ScrapedPage& page,
        bool replaceExisting
    ) const;

    const std::string& filename() const;

private:
    std::string filename_;

    std::string readAll() const;
    void writeAll(const std::string& contents) const;

    static std::string beginMarker(const std::string& url);
    static std::string endMarker(const std::string& url);
};