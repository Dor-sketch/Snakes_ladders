#include "parse_music.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>

MusicParser::MusicParser(const std::string &filename) : filename(filename) {
    parse();
}

MusicParser::MusicParser(const cv::Mat &screenshot) {
    cv::imwrite("screenshot.png", screenshot);
    filename = "screenshot.png";
    parse();
}

std::string MusicParser::pixelsToNoteHeight(int height) {
    std::map<int, std::string> heightMap = {
            {10, "whole"},        {20, "half"},
            {30, "quarter"},      {40, "eighth"},
            {50, "sixteenth"},    {60, "thirty-second"},
            {70, "sixty-fourth"}, {80, "hundred-twenty-eighth"}};

    auto it = heightMap.lower_bound(height);
    if (it == heightMap.end()) {
        return "unknown";
    }
    return it->second;
}

void MusicParser::parse() {
    auto image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return;
    }
    cv::threshold(image, image, 200, 255, cv::THRESH_BINARY);
    cv::Mat verticalStructure =
            cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, image.rows / 30));
    cv::erode(image, image, verticalStructure, cv::Point(-1, -1));
    cv::dilate(image, image, verticalStructure, cv::Point(-1, -1));

    std::vector<std::vector<cv::Point>> barLineContours;
    cv::findContours(image, barLineContours, cv::RETR_EXTERNAL,
                                     cv::CHAIN_APPROX_SIMPLE);

    std::vector<int> barLinePositions;
    for (const auto &contour : barLineContours) {
        cv::Rect boundingBox = cv::boundingRect(contour);
        barLinePositions.push_back(boundingBox.x);
    }

    std::sort(barLinePositions.begin(), barLinePositions.end());

    image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    cv::threshold(image, image, 127, 255, cv::THRESH_BINARY);

    for (size_t i = 0; i < barLinePositions.size() - 1; ++i) {
        int startX = barLinePositions[i];
        int endX = barLinePositions[i + 1];

        if (startX != 0) {
            startX += 1;
        }

        if (endX != image.cols - 1) {
            endX -= 1;
        }

        std::cout << "Measuring notes in measure " << i + 1 << "..." << std::endl;
        try {
            cv::Rect measureROI(startX, 0, endX - startX, image.rows);
            cv::Mat measure = image(measureROI);

            std::vector<std::vector<cv::Point>> noteContours;
            cv::findContours(measure, noteContours, cv::RETR_EXTERNAL,
                                             cv::CHAIN_APPROX_SIMPLE);

            for (const auto &contour : noteContours) {
                cv::Rect boundingBox = cv::boundingRect(contour);
                int noteHeight = boundingBox.height;
                int noteWidth = boundingBox.width;

                if (noteWidth > 5 && noteHeight > 5) {
                    std::string note = pixelsToNoteHeight(noteHeight);
                    notes.push_back(note);
                    std::cout << "Detected note: " << note << " (Height: " << noteHeight
                                        << ", Width: " << noteWidth << ", Position: " << startX
                                        << "-" << endX << ")" << std::endl;
                }
            }
        } catch (cv::Exception &e) {
            std::cerr << "Error while processing measure " << i + 1 << ": "
                                << e.what() << std::endl;
        }
    }

    std::cout << "Finished processing music sheet!" << std::endl;
    cv::Mat outputImage = cv::Mat::zeros(image.size(), CV_8UC3);
    cv::RNG rng(12345);
    try {
        cv::cvtColor(image, outputImage, cv::COLOR_GRAY2BGR);
    } catch (cv::Exception &e) {
        std::cerr << "Error while converting image to BGR: " << e.what()
                            << std::endl;
    }
    for (const auto &contour : barLineContours) {
        try {
            cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256),
                                                                        rng.uniform(0, 256));
            cv::drawContours(outputImage,
                                             std::vector<std::vector<cv::Point>>{contour}, 0, color,
                                             2);
        } catch (cv::Exception &e) {
            std::cerr << "Error while drawing bar line contour: " << e.what()
                                << std::endl;
        }
    }
    cv::imshow("Detected Bar Lines", outputImage);
    cv::waitKey(0);
    auto noteContours = std::vector<std::vector<cv::Point>>{};
        for (const auto &contour : noteContours) {
                try {
                cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256),
                                                                                rng.uniform(0, 256));
                cv::drawContours(outputImage,
                                                 std::vector<std::vector<cv::Point>>{contour}, 0, color,
                                                 2);
                } catch (cv::Exception &e) {
                std::cerr << "Error while drawing note contour: " << e.what()
                                        << std::endl;
                }
        }

    cv::Mat outputImageNotes = cv::Mat::zeros(image.size(), CV_8UC3);
        try {
                cv::cvtColor(image, outputImageNotes, cv::COLOR_GRAY2BGR);
        } catch (cv::Exception &e) {
                std::cerr << "Error while converting image to BGR: " << e.what()
                                    << std::endl;
        }
        for (const auto &contour : noteContours) {
                try {
                        cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256),
                                                                                    rng.uniform(0, 256));
                        cv::drawContours(outputImageNotes,
                                                        std::vector<std::vector<cv::Point>>{contour}, 0, color,
                                                        2);
                } catch (cv::Exception &e) {
                        std::cerr << "Error while drawing note contour: " << e.what()
                                        << std::endl;
                }
        }
        cv::imshow("Detected Notes", outputImageNotes);
        cv::waitKey(0);
}

void MusicParser::print() {
    std::cout << "Notes in the music sheet:" << std::endl;
    for (const auto &note : notes) {
        std::cout << note << std::endl;
    }
}