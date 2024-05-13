#include "ImgTor.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    ImgProc processor;
    ImageRegistry registry;

    int choice = 0;
    while (choice != 6)
    {
        cout << "Select an option:" << endl;
        cout << "1. Load image" << endl;
        cout << "2. Apply filter" << endl;
        cout << "3. Save image" << endl;
        cout << "4. Manage image registry" << endl;
        cout << "5. Remove background" << endl;
        cout << "6. Exit program" << endl;
        cout << "Enter your choice (1-6): ";
        cin >> choice;

        switch (choice){
        case 1:
        {
            string filename;
            cout << "Enter the name of the image file: ";
            cin >> filename;

            Mat originalImage = processor.loadImage(filename);

            string imageInfo;
            cout << "Enter information associated with the image: ";
            cin.ignore(); // Clear the input buffer
            getline(cin, imageInfo);

            registry.addImage(filename, originalImage, imageInfo);
            cout << "Image loaded successfully." << endl;
            break;
        }
        case 2:
        {
            if (registry.getImageCount() == 0)
            {
                cout << "No images loaded. Please load an image first." << endl;
                break;
            }

            string filename;
            cout << "Enter the name of the image to apply the filter: ";
            cin >> filename;

            if (!registry.findImage(filename))
            {
                cout << "Image not found in the registry." << endl;
                break;
            }

            Mat originalImage = registry.getImage(filename);
            Mat processedImage = originalImage.clone(); // Use a copy for processing

            int filterChoice;
            cout << "Choose a filter to apply:" << endl;
            cout << "1. Blur" << endl;
            cout << "2. Edge Enhancement" << endl;
            cout << "3. Contours Detection" << endl;
            cout << "4. Black and White" << endl;
            cout << "5. Negative" << endl;
            cout << "Enter your choice (1-5): ";
            cin >> filterChoice;

            switch (filterChoice)
            {
            case 1:
                processor.applyBlur(processedImage);
                break;
            case 2:
                processor.applyEdgeEnhancement(processedImage);
                break;
            case 3:
                processor.applyContoursDetection(processedImage);
                break;
            case 4:
                processor.applyBlackAndWhiteFilter(processedImage);
                break;
            case 5:
                processor.applyNegativeFilter(processedImage);
                break;
            default:
                cerr << "Error: Invalid filter choice." << endl;
                break;
            }
            cout << "Filter applied successfully." << endl;

            // Save the processed image to the registry
            registry.addImage(filename, processedImage, "Processed Image");
            break;
        }
        case 3:
        {
            if (registry.getImageCount() == 0)
            {
                cout << "No images loaded. Please load an image first." << endl;
                break;
            }

            string filename;
            cout << "Enter the name of the image to save: ";
            cin >> filename;

            if (!registry.findImage(filename))
            {
                cout << "Image not found in the registry." << endl;
                break;
            }

            Mat filteredImage = registry.getImage(filename);

            string outputFilename;
            cout << "Enter the name of the output file: ";
            cin >> outputFilename;
            processor.saveImage(outputFilename, filteredImage);
            cout << "Image saved successfully as " << outputFilename << endl;
            break;
        }
        case 4:
        {
            int manageChoice = 0;
            while (manageChoice != 4)
            {
                cout << "Manage image registry:" << endl;
                cout << "1. Display image info" << endl;
                cout << "2. Remove image" << endl;
                cout << "3. List all images" << endl;
                cout << "4. Back to main menu" << endl;
                cout << "Enter your choice (1-4): ";
                cin >> manageChoice;

                switch (manageChoice)
                {
                case 1:
                {
                    if (registry.getImageCount() == 0)
                    {
                        cout << "No images loaded. Please load an image first." << endl;
                        break;
                    }

                    string filename;
                    cout << "Enter the name of the image: ";
                    cin >> filename;
                    registry.displayImageInfo(filename);
                    break;
                }
                case 2:
                {
                    if (registry.getImageCount() == 0)
                    {
                        cout << "No images loaded. Please load an image first." << endl;
                        break;
                    }

                    string filename;
                    cout << "Enter the name of the image to remove: ";
                    cin >> filename;

                    if (!registry.findImage(filename))
                    {
                        cout << "Image not found in the registry." << endl;
                        break;
                    }

                    registry.removeImage(filename);
                    cout << "Image removed successfully." << endl;
                    break;
                }
                case 3:
                {
                    registry.listAllImages();
                    break;
                }
                case 4:
                    cout << "Returning to the main menu." << endl;
                    break;
                default:
                    cerr << "Error: Invalid option." << endl;
                    break;
                }
            }
            break;
        }
        case 5:
        {
            if (registry.getImageCount() == 0)
            {
                cout << "No images loaded. Please load an image first." << endl;
                break;
            }

            string filename;
            cout << "Enter the name of the image to remove the background: ";
            cin >> filename;

            if (!registry.findImage(filename))
            {
                cout << "Image not found in the registry." << endl;
                break;
            }

            Mat originalImage = registry.getImage(filename);
            Mat processedImage = originalImage.clone(); // Use a copy for processing

            processor.removeBackground(processedImage);
            cout << "Background removed successfully." << endl;

            // Update the registry with the processed image
            registry.addImage(filename, processedImage, "Processed Image");

            break;
        }

        case 6:
            cout << "Exiting program." << endl;
            break;
            
        default:
            cerr << "Error: Invalid option." << endl;
            break;
        }
    }

    return 0;
}
