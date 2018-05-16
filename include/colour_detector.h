#ifndef COLOUR_DETECTOR_H
#define COLOUR_DETECTOR_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//Macros for getting pixels from images
#define MpixelGrey(image, x, y) ((uchar *) (((image).data) + (y)*((image).step)))[(x)]
#define MpixelB(image, x, y) ((uchar *) (((image).data) + (y)*((image).step)))[(x)*((image).channels())]
#define MpixelG(image, x, y) ((uchar *) (((image).data) + (y)*((image).step)))[(x)*((image).channels())+1]
#define MpixelR(image, x, y) ((uchar *) (((image).data) + (y)*((image).step)))[(x)*((image).channels())+2]


//ranges for finding different colours
const Scalar rangeRed[2] =        {Scalar(0, 50, 50), Scalar(20, 255,255)};
const Scalar rangeGreen[2] =      {Scalar(55, 50, 50), Scalar(75, 255,255)};
const Scalar rangeBlue[2] =       {Scalar(110, 50, 50), Scalar(130, 255,255)};
const Scalar rangeCyan[2] =       {Scalar(85, 50, 50), Scalar(110, 255,255)};
const Scalar rangeMagenta[2] =    {Scalar(140, 50, 50), Scalar(160, 255,255)};
const Scalar rangeYellow[2] =     {Scalar(30, 50, 50), Scalar(50, 255,255)};
const Scalar rangeGrid[2] =      {Scalar(0, 0, 0), Scalar(255, 255,170)};
const Scalar rangeWhite[2] =      {Scalar(0, 0, 240), Scalar(255, 50,255)};
const Scalar rangeBlack[2] =      {Scalar(0, 0, 0), Scalar(255, 255, 20)};

class colour_detector
{
    public:
        colour_detector();
        virtual ~colour_detector();
        void isBlue(Mat &hsvinput, Mat &bgroutput);
        void makeRGB(Mat &inRGB, Mat &outRGB);

        void getGrid(Mat &rgbinput, Mat &bgroutput);
        void getCircles(Mat &bgrInput, Mat&greyOutput);

    protected:

    private:
        Mat hsv;
        Mat mask;
        Mat res;
};

#endif // COLOUR_DETECTOR_H
