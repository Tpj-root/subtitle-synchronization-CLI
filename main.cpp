#include "SubtitleSynchronizer.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage:\n\n"
                  << "  sub_synchronization input.srt delay_ms\n"
                  << "  sub_synchronization input.srt --list\n\n"
                  << "Examples:\n"
                  << "  sub_synchronization movie.srt 2000\n"
                  << "  sub_synchronization movie.srt -2000\n"
                  << "  sub_synchronization movie.srt --list\n";

        return 1;
    }

    std::string inputFile = argv[1];
    std::string argument  = argv[2];

    SubtitleSynchronizer synchronizer(inputFile);

    // Batch mode: generate all 8 files.
    if (argument == "--list")
    {
        synchronizer.processList();
        return 0;
    }

    // Single synchronization mode.
    try
    {
        int delayMs = std::stoi(argument);

        synchronizer.process(delayMs);
    }
    catch (const std::exception&)
    {
        std::cerr << "Error: Invalid delay value.\n"
                  << "Use milliseconds, for example: 2000 or -2000\n";

        return 1;
    }

    return 0;
}