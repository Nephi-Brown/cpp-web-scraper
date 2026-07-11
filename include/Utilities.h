#pragma once

#include <string>

/*
Checks whether text begins with a particular prefix.
*/
bool startsWith(
    const std::string& text,
    const std::string& prefix
);

/*
Removes whitespace from the beginning and end of a string.
*/
std::string trim(const std::string& value);

/*
Replaces repeated spaces, tabs, and line breaks with one space.
*/
std::string cleanText(const std::string& value);

/*
Adds https:// when the user does not enter a protocol.
*/
std::string normalizeUrl(std::string url);

/*
Performs basic HTTP or HTTPS URL validation.
*/
bool isValidUrl(const std::string& url);

/*
Returns the current local date and time.
*/
std::string currentTimestamp();

/*
Converts common relative links into absolute links.
*/
std::string makeAbsoluteUrl(
    const std::string& pageUrl,
    const std::string& link
);

/*
Asks the user a yes-or-no question.
*/
bool askYesNo(const std::string& question);