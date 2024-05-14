// parse_music.h
#ifndef PARSE_MUSIC_H
#define PARSE_MUSIC_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

class MusicParser {
public:
    explicit MusicParser(const std::string& filename);
    explicit MusicParser(const cv::Mat& screenshot);
    void print();

private:
    std::string filename;
    std::vector<std::string> notes;

    std::string pixelsToNoteHeight(int height);
    void parse();
};

#endif // PARSE_MUSIC_H
