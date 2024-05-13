#ifndef IMGTOR_HPP
#define IMGTOR_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>


class ImgProc
{
private:
    std::string currfile;

public:
    std::string getname(const std::string currfile);
    cv::Mat loadImage(const std::string &filename);
    void applyBlur(cv::Mat &image);
    void applyEdgeEnhancement(cv::Mat &image);
    void applyContoursDetection(cv::Mat &image);
    void applyBlackAndWhiteFilter(cv::Mat &image);
    void applyNegativeFilter(cv::Mat &image);
    void removeBackground(cv::Mat &image);
    void saveImage(const std::string &filename, const cv::Mat &image);
};

class ImageRegistry
{
private:
    std::unordered_map<std::string, std::pair<cv::Mat, std::string>> imageTable;

public:
    void addImage(const std::string &filename, const cv::Mat &image, const std::string &metadata);
    bool findImage(const std::string &filename) const;
    void removeImage(const std::string &filename);
    void displayImageInfo(const std::string &filename) const;
    int getImageCount() const;
    void listAllImages() const;
    cv::Mat getImage(const std::string &filename) const;
};

#endif // IMGTOR_HPP
#pragma once