#include "Utilities.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

/*
Checks whether text begins with a particular prefix.
*/
bool startsWith(
    const std::string& text,
    const std::string& prefix
) {
    return text.rfind(prefix, 0) == 0;
}

/*
Removes whitespace from the beginning and end of a string.
*/
std::string trim(const std::string& value) {
    const auto first = std::find_if_not(
        value.begin(),
        value.end(),
        [](unsigned char character) {
            return std::isspace(character) != 0;
        }
    );

    if (first == value.end()) {
        return "";
    }

    const auto last = std::find_if_not(
        value.rbegin(),
        value.rend(),
        [](unsigned char character) {
            return std::isspace(character) != 0;
        }
    ).base();

    return std::string(first, last);
}

/*
Replaces repeated whitespace characters with one space.
*/
std::string cleanText(const std::string& value) {
    std::ostringstream output;
    bool previousWasSpace = false;

    for (unsigned char character : value) {
        if (std::isspace(character) != 0) {
            if (!previousWasSpace) {
                output << ' ';
                previousWasSpace = true;
            }
        } else {
            output << static_cast<char>(character);
            previousWasSpace = false;
        }
    }

    return trim(output.str());
}

/*
Adds https:// when the user does not provide a protocol.
*/
std::string normalizeUrl(std::string url) {
    url = trim(url);

    if (!startsWith(url, "http://") &&
        !startsWith(url, "https://")) {
        url = "https://" + url;
    }

    return url;
}

/*
Performs basic URL validation.
*/
bool isValidUrl(const std::string& url) {
    const bool hasProtocol =
        startsWith(url, "http://") ||
        startsWith(url, "https://");

    const std::size_t protocolEnd = url.find("://");

    if (!hasProtocol || protocolEnd == std::string::npos) {
        return false;
    }

    const std::string remainder = url.substr(protocolEnd + 3);

    return !remainder.empty() &&
           remainder.find('.') != std::string::npos &&
           remainder.find(' ') == std::string::npos;
}

/*
Returns the current local date and time.
*/
std::string currentTimestamp() {
    const auto now = std::chrono::system_clock::now();

    const std::time_t timeValue =
        std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

#if defined(_WIN32)
    localtime_s(&localTime, &timeValue);
#else
    localtime_r(&timeValue, &localTime);
#endif

    std::ostringstream output;
    output << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

    return output.str();
}

/*
Converts common relative links into absolute URLs.
*/
std::string makeAbsoluteUrl(
    const std::string& pageUrl,
    const std::string& link
) {
    if (link.empty() ||
        link[0] == '#' ||
        startsWith(link, "javascript:") ||
        startsWith(link, "mailto:") ||
        startsWith(link, "tel:")) {
        return "";
    }

    if (startsWith(link, "http://") ||
        startsWith(link, "https://")) {
        return link;
    }

    const std::size_t schemeEnd = pageUrl.find("://");

    if (schemeEnd == std::string::npos) {
        return link;
    }

    if (startsWith(link, "//")) {
        return pageUrl.substr(0, schemeEnd) + ":" + link;
    }

    const std::size_t hostEnd =
        pageUrl.find('/', schemeEnd + 3);

    const std::string origin =
        hostEnd == std::string::npos
            ? pageUrl
            : pageUrl.substr(0, hostEnd);

    if (link[0] == '/') {
        return origin + link;
    }

    const std::size_t lastSlash = pageUrl.rfind('/');

    const std::string base =
        lastSlash == std::string::npos ||
        lastSlash < schemeEnd + 3
            ? pageUrl + "/"
            : pageUrl.substr(0, lastSlash + 1);

    return base + link;
}

/*
Repeatedly asks for y or n until valid input is entered.
*/
bool askYesNo(const std::string& question) {
    while (true) {
        std::cout << question << " (y/n): ";

        std::string answer;
        std::getline(std::cin, answer);

        if (!answer.empty()) {
            const char choice = static_cast<char>(
                std::tolower(
                    static_cast<unsigned char>(answer.front())
                )
            );

            if (choice == 'y') {
                return true;
            }

            if (choice == 'n') {
                return false;
            }
        }

        std::cout << "Please enter y or n.\n";
    }
}