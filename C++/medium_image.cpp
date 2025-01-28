#define WIN32_LEAN_AND_MEAN 
#include "halconc/HDevThread.h"
#include "halconcpp/HalconCpp.h"
#include <windows.h>

using namespace HalconCpp;

int main()
{
    // Local iconic variables
    HObject ho_ImageMedium, ho_RefDarkMedium, ho_RefWhiteMedium;
    HObject ho_ImageMinusDarkRawMedium, ho_WhiteMinusDarkMedium, ho_ReflectanceMedium;
    HObject ho_BandImage;
    HTuple hv_ImageDir, hv_Width, hv_Height, hv_NumBands, hv_Band, hv_WindowID;

    // Set the directory where the images are stored
    hv_ImageDir = "C:/Users/Momin/Downloads/Afbeeldingen FX17/";

    try
    {
        // Read the medium image and reference images
        ReadImage(&ho_ImageMedium, hv_ImageDir + "testimage_medium.hobj");
        ReadImage(&ho_RefDarkMedium, hv_ImageDir + "RefDarkMedium.hobj");
        ReadImage(&ho_RefWhiteMedium, hv_ImageDir + "RefWhiteMedium.hobj");

        // Open a HALCON window for displaying the image
        SetWindowAttr("background_color", "black");
        OpenWindow(0, 0, 512, 512, 0, "", "", &hv_WindowID);
        HDevWindowStack::Push(hv_WindowID);

        // Subtract the dark reference from the raw image
        SubImage(ho_ImageMedium, ho_RefDarkMedium, &ho_ImageMinusDarkRawMedium, 1, 0);

        // Subtract the dark reference from the white reference
        SubImage(ho_RefWhiteMedium, ho_RefDarkMedium, &ho_WhiteMinusDarkMedium, 1, 0);

        // Divide the resulting images to calculate the reflectance
        DivImage(ho_ImageMinusDarkRawMedium, ho_WhiteMinusDarkMedium, &ho_ReflectanceMedium, 1, 0);

        // Get the size and number of bands for the reflectance image
        GetImageSize(ho_ReflectanceMedium, &hv_Width, &hv_Height);
        CountChannels(ho_ReflectanceMedium, &hv_NumBands);

        // Loop through each band and display it
        {
            HTuple end_val = hv_NumBands;
            HTuple step_val = 1;
            for (hv_Band = 1; hv_Band.Continue(end_val, step_val); hv_Band += step_val)
            {
                AccessChannel(ho_ReflectanceMedium, &ho_BandImage, hv_Band);
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