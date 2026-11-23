#include "SubtitleSynchronizer.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>

SubtitleSynchronizer::SubtitleSynchronizer(const std::string& file)
    : inputFile(file)
{
}


// --------------------------------------------------
// Convert timestamp:
//
// 00:00:20,461  ->  20461 milliseconds
// --------------------------------------------------

int SubtitleSynchronizer::timestampToMilliseconds(
    const std::string& timestamp) const
{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    int milliseconds = 0;

    char separator1;
    char separator2;
    char separator3;

    std::istringstream stream(timestamp);

    stream >> hours >> separator1
           >> minutes >> separator2
           >> seconds >> separator3
           >> milliseconds;

    return ((hours * 3600 + minutes * 60 + seconds) * 1000)
           + milliseconds;
}


// --------------------------------------------------
// Convert milliseconds back to SRT timestamp:
//
// 20461 -> 00:00:20,461
// --------------------------------------------------

std::string SubtitleSynchronizer::millisecondsToTimestamp(
    int milliseconds) const
{
    // SRT timestamps cannot be negative.
    if (milliseconds < 0)
        milliseconds = 0;

    int hours = milliseconds / 3600000;
    milliseconds %= 3600000;

    int minutes = milliseconds / 60000;
    milliseconds %= 60000;

    int seconds = milliseconds / 1000;
    milliseconds %= 1000;

    std::ostringstream output;

    output << std::setfill('0')
           << std::setw(2) << hours << ':'
           << std::setw(2) << minutes << ':'
           << std::setw(2) << seconds << ','
           << std::setw(3) << milliseconds;

    return output.str();
}


// --------------------------------------------------
// Create output filename
//
// input.srt
// input_vlc_p2000ms.srt
// input_vlc_m2000ms.srt
// --------------------------------------------------

std::string SubtitleSynchronizer::makeOutputName(
    int vlcDelayMs) const
{
    std::string base = inputFile;

    // Remove extension
    size_t dot = base.find_last_of('.');

    if (dot != std::string::npos)
        base = base.substr(0, dot);

    std::string sign;

    if (vlcDelayMs >= 0)
        sign = "p";
    else
        sign = "m";

    int absoluteDelay = vlcDelayMs < 0
        ? -vlcDelayMs
        : vlcDelayMs;

    return base + "_vlc_" + sign
           + std::to_string(absoluteDelay) + "ms.srt";
}


// --------------------------------------------------
// Process one subtitle synchronization
// --------------------------------------------------

bool SubtitleSynchronizer::process(int vlcDelayMs)
{
    std::string outputFile = makeOutputName(vlcDelayMs);

    return synchronizeFile(outputFile, vlcDelayMs);
}


// --------------------------------------------------
// Synchronize the SRT file
// --------------------------------------------------

bool SubtitleSynchronizer::synchronizeFile(
    const std::string& outputFile,
    int vlcDelayMs)
{
    std::ifstream input(inputFile);

    if (!input.is_open())
    {
        std::cerr << "Error: Cannot open input file: "
                  << inputFile << '\n';

        return false;
    }

    std::ofstream output(outputFile);

    if (!output.is_open())
    {
        std::cerr << "Error: Cannot create output file: "
                  << outputFile << '\n';

        return false;
    }

    // SRT timestamp line
    std::regex timestampRegex(
        R"((\d{2,}:\d{2}:\d{2},\d{3})\s*-->\s*(\d{2,}:\d{2}:\d{2},\d{3}))"
    );

    std::string line;

    // VLC correction and file correction have opposite signs.
    const int fileShiftMs = -vlcDelayMs;

    while (std::getline(input, line))
    {
        std::smatch match;

        if (std::regex_search(line, match, timestampRegex))
        {
            std::string startTime = match[1].str();
            std::string endTime   = match[2].str();

            int startMs = timestampToMilliseconds(startTime);
            int endMs   = timestampToMilliseconds(endTime);

            startMs += fileShiftMs;
            endMs   += fileShiftMs;

            // Prevent timestamps below zero.
            if (startMs < 0)
                startMs = 0;

            if (endMs < 0)
                endMs = 0;

            std::string newStart = millisecondsToTimestamp(startMs);
            std::string newEnd   = millisecondsToTimestamp(endMs);

            // Replace only the timestamp portion of the original line,
            // preserving any additional formatting or trailing text.
            line.replace(
                match.position(0),
                match.length(0),
                newStart + " --> " + newEnd
            );
        }

        output << line << '\n';
    }

    input.close();
    output.close();

    std::cout << "[OK] Created: " << outputFile << '\n';

    return true;
}


// --------------------------------------------------
// Generate all 8 synchronization variations
// --------------------------------------------------

// OLD
// void SubtitleSynchronizer::processList()
// {
//     const std::vector<int> delays =
//     {
//         -2000,
//         -1500,
//         -1000,
//         -500,
//          500,
//          1000,
//          1500,
//          2000
//     };

//     std::cout << "\nGenerating 8 subtitle files...\n\n";

//     int successCount = 0;

//     for (int delay : delays)
//     {
//         if (process(delay))
//             ++successCount;
//     }

//     std::cout << "\nFinished: "
//               << successCount << " / "
//               << delays.size() << " files created.\n";
// }



// more
void SubtitleSynchronizer::processList()
{
    const std::vector<int> delays =
    {
        -4000, -3900, -3800, -3700, -3600, -3500, -3400, -3300,
        -3200, -3100, -3000, -2900, -2800, -2700, -2600, -2500,
        -2400, -2300, -2200, -2100, -2000, -1900, -1800, -1700,
        -1600, -1500, -1400, -1300, -1200, -1100, -1000, -900,
        -800, -700, -600, -500, -400, -300, -200, -100,

         100,  200,  300,  400,  500,  600,  700,  800,
         900, 1000, 1100, 1200, 1300, 1400, 1500, 1600,
        1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400,
        2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200,
        3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000
    };
    std::cout << "\nGenerating 8 subtitle files...\n\n";

    int successCount = 0;

    for (int delay : delays)
    {
        if (process(delay))
            ++successCount;
    }

    std::cout << "\nFinished: "
              << successCount << " / "
              << delays.size() << " files created.\n";
}






