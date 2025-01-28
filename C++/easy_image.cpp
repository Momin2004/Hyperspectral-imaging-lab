#define WIN32_LEAN_AND_MEAN 
#include "halconc/HDevThread.h"
#include "halconcpp/HalconCpp.h"
#include <windows.h>

using namespace HalconCpp;

int main()
{
    // Local iconic variables
    HObject ho_ImageEasy, ho_BandImage;
    HTuple hv_ImageDir, hv_Width, hv_Height, hv_NumBands, hv_Band, hv_WindowID;

    // Set the directory where the images are stored
    hv_ImageDir = "C:/Users/Momin/Downloads/Afbeeldingen FX17/testimage_easy.hobj";

    try
    {
        // Read the easy image
        std::cout << "Opening Image " << hv_ImageDir << std::endl;
        ReadImage(&ho_ImageEasy, hv_ImageDir);

        // Open a HALCON window for displaying the image
        SetWindowAttr("background_color", "black");
        OpenWindow(0, 0, 512, 512, 0, "", "", &hv_WindowID);
        HDevWindowStack::Push(hv_WindowID);

        // Get the size and number of bands for the image
        GetImageSize(ho_ImageEasy, &hv_Width, &hv_Height);
        CountChannels(ho_ImageEasy, &hv_NumBands);

        // Get the size and number of bands for the reflectance image
        GetImageSize(ho_ImageEasy, &hv_Width, &hv_Height);
        CountChannels(ho_ImageEasy, &hv_NumBands);

        // Loop through each band and display it
        {
            HTuple end_val = hv_NumBands;
            HTuple step_val = 1;
            for (hv_Band = 1; hv_Band.Continue(end_val, step_val); hv_Band += step_val)
            {
                AccessChannel(ho_ImageEasy, &ho_BandImage, hv_Band);
                if (HDevWindowStack::IsOpen())
                    DispObj(ho_BandImage, HDevWindowStack::GetActive());
                
                // Wait for 1 second before showing the next band
                WaitSeconds(1);
            }
        }
        // Wait for a key press before closing the window
        std::cout << "Press Enter to close the window..." << std::endl;
        std::cin.get();

        // Close the HALCON window
        CloseWindow(hv_WindowID);
    }
    catch (HException &exception)
    {
        // Handle errors
        std::cerr << "HALCON Error #" << exception.ErrorCode() << ": "
                  << exception.ErrorMessage().Text() << std::endl;
        return 1;
    }

    return 0;
}
