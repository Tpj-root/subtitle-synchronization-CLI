#ifndef SUBTITLESYNCHRONIZER_H
#define SUBTITLESYNCHRONIZER_H

#include <string>
#include <vector>

class SubtitleSynchronizer
{
private:
    std::string inputFile;

public:
    explicit SubtitleSynchronizer(const std::string& file);

    bool process(int vlcDelayMs);

    void processList();

private:
    std::string makeOutputName(int vlcDelayMs) const;

    bool synchronizeFile(
        const std::string& outputFile,
        int vlcDelayMs
    );

    int timestampToMilliseconds(const std::string& timestamp) const;

    std::string millisecondsToTimestamp(int milliseconds) const;
};

#endif