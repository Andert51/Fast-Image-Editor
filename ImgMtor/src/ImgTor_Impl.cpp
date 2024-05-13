#include "ImgTor.hpp"

// -----------------------------------------Image-Proccesor----------------------------------------- //

cv::Mat ImgProc::loadImage(const std::string &filename){ // Funcion para cargar la imagen deseada

    cv::Mat image = cv::imread(filename);
    if(image.empty()){
        std::cerr << "Error: Uploading Faillure" << filename << std::endl;
        exit(1);
    }

    return image;
}

void ImgProc::saveImage(const std::string &filename, const cv::Mat &image){ // Guardar la imagen deseada
    cv::imwrite(filename, image);
    std::cout << "Imagen guardada como: " << filename << std::endl;
}

// --------------------------------------------Getters----------------------------------------------//

std::string ImgProc::getname(const std::string currfile){
    return currfile;
}

// --------------------------------------------Filters----------------------------------------------//

void ImgProc::applyBlur(cv::Mat &image)
{
    blur(image, image, cv::Size(10, 10)); // Aplicar un filtro de desenfoque
    std::cout << "Filter Applied" << std::endl;
}

void ImgProc::applyEdgeEnhancement(cv::Mat &image)
{
    GaussianBlur(image, image, cv::Size(0, 0), 3);
    addWeighted(image, 1.5, image, -0.5, 0, image); // Realce de bordes
    std::cout << "Filter Applied" << std::endl;
}

void ImgProc::applyContoursDetection(cv::Mat &image)
{
    cv::Mat grayImage;
    cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    Canny(grayImage, image, 100, 200); // Detección de contornos
    std::cout << "Filter Applied" << std::endl;
}

void ImgProc::applyBlackAndWhiteFilter(cv::Mat &image)
{
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY); // Aplicar filtro blanco y negro
    std::cout << "Filter Applied" << std::endl;
}

void ImgProc::applyNegativeFilter(cv::Mat &image)
{
    cv::bitwise_not(image, image); // Aplicar filtro negativo
    std::cout << "Filter Applied" << std::endl;
}

// --------------------------------------------Remove-Background----------------------------------------------//

void ImgProc::removeBackground(cv::Mat &image){
    
    // Convertir la imagen original a escala de grises
    cv::Mat grayImage;
    cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    // Aplicar un umbral adaptativo para segmentar el fondo
    cv::Mat mask;
    cv::adaptiveThreshold(grayImage, mask, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);

    // Encontrar los contornos en la máscara
    std::vector<std::vector<cv::Point>> contours;
    findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Crear una máscara negra del mismo tamaño que la imagen original
    cv::Mat blackMask(image.size(), CV_8UC1, cv::Scalar(0));

    // Dibujar los contornos en la máscara negra
    cv::drawContours(blackMask, contours, -1, cv::Scalar(255), cv::FILLED);

    // Crear una imagen de fondo transparente
    cv::Mat transparentImage(image.size(), CV_8UC4);
    cv::cvtColor(image, transparentImage, cv::COLOR_BGR2BGRA);

    // Copiar la imagen original a la imagen transparente usando la máscara negra
    for (int y = 0; y < image.rows; ++y)
    {
        for (int x = 0; x < image.cols; ++x)
        {
            if (blackMask.at<uchar>(y, x) == 255)
            {
                // Si el píxel de la máscara negra es blanco, copiar el píxel de la imagen original
                transparentImage.at<cv::Vec4b>(y, x)[3] = 255; // Establecer el canal alfa a 255 (opaco)
            }
            else
            {
                // Si el píxel de la máscara negra es negro, establecer el píxel de la imagen transparente como transparente
                transparentImage.at<cv::Vec4b>(y, x)[3] = 0; // Establecer el canal alfa a 0 (transparente)
            }
        }
    }



    // Guardar la imagen transparente como PNG
    transparentImage.copyTo(image);

    std::cout << "Background erased." << std::endl;
}

// --------------------------------------------Hash-Table----------------------------------------------//

void ImageRegistry::addImage(const std::string &filename, const cv::Mat &image, const std::string &metadata)
{
    imageTable[filename] = std::make_pair(image, metadata);
}

bool ImageRegistry::findImage(const std::string &filename) const
{
    return imageTable.find(filename) != imageTable.end();
}

void ImageRegistry::removeImage(const std::string &filename)
{
    imageTable.erase(filename);
}

void ImageRegistry::displayImageInfo(const std::string &filename) const
{
    auto it = imageTable.find(filename);
    if (it != imageTable.end())
    {
        std::cout << "File Name: " << it->first << std::endl;
        std::cout << "Metadata: " << it->second.second << std::endl;
    }
    else
    {
        std::cout << "Image '" << filename << "' not found." << std::endl;
    }
}

void ImageRegistry::listAllImages() const
{
    std::cout << "Registered Images:" << std::endl;
    for (const auto &entry : imageTable)
    {
        std::cout << "File Name: " << entry.first << std::endl;
        std::cout << "Metadata: " << entry.second.second << std::endl;
        std::cout << std::endl;
    }
}

int ImageRegistry::getImageCount() const
{
    return imageTable.size();
}

cv::Mat ImageRegistry::getImage(const std::string &filename) const
{
    auto it = imageTable.find(filename);
    if (it != imageTable.end())
    {
        return it->second.first; // Devuelve la imagen correspondiente al nombre de archivo proporcionado
    }
    else
    {
        std::cerr << "Image '" << filename << "' not found." << std::endl;
        return cv::Mat(); // Devuelve una matriz vacía si la imagen no se encuentra
    }
}