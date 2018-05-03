#include "colour_detector.h"

colour_detector::colour_detector()
{
    //ctor
}

colour_detector::~colour_detector()
{
    //dtor
}

void colour_detector::isBlue(Mat &rgbinput, Mat &bgroutput){
    hsv.create(rgbinput.size(), CV_8UC3);
    mask.create(rgbinput.size(), CV_8UC1);
    res.create(rgbinput.size(), CV_8UC3);

    cvtColor(rgbinput, hsv, COLOR_BGR2HSV);

    inRange(hsv, rangeBlue[0], rangeBlue[1], mask);
    bitwise_and(rgbinput, rgbinput, res, mask);

    bgroutput = res;
}

void colour_detector::getGrid(Mat &bgrinput, Mat &bgroutput){
    hsv.create(bgrinput.size(), CV_8UC3);
    mask.create(bgrinput.size(), CV_8UC1);
    res.create(bgrinput.size(), CV_8UC3);

    cvtColor(bgrinput, hsv, COLOR_BGR2HSV);

    inRange(hsv, rangeBlack[0], rangeBlack[1], mask);

    bgroutput = mask;
}

void colour_detector::makeRGB(Mat &inRGB, Mat &outRGB){
    hsv.create(inRGB.size(), CV_8UC3);
    mask.create(inRGB.size(), CV_8UC1);
    res.create(inRGB.size(), CV_8UC3);

    cvtColor(inRGB, hsv, COLOR_BGR2HSV);

    inRange(hsv, rangeRed[0], rangeRed[1], mask);

    for(int y = 0; y < mask.rows; y++){
        for(int x = 0; x < mask.cols; x++){
            if(MpixelGrey(mask, x, y) == 255){
                MpixelR(res, x, y) = 255;
                MpixelG(res, x, y) = 0;
                MpixelB(res, x, y) = 0;
            }
        }
    }

    inRange(hsv, rangeGreen[0], rangeGreen[1], mask);

    for(int y = 0; y < mask.rows; y++){
        for(int x = 0; x < mask.cols; x++){
            if(MpixelGrey(mask, x, y) == 255){
                MpixelR(res, x, y) = 0;
                MpixelG(res, x, y) = 255;
                MpixelB(res, x, y) = 0;
            }
        }
    }

    inRange(hsv, rangeBlue[0], rangeBlue[1], mask);

    for(int y = 0; y < mask.rows; y++){
        for(int x = 0; x < mask.cols; x++){
            if(MpixelGrey(mask, x, y) == 255){
                MpixelR(res, x, y) = 0;
                MpixelG(res, x, y) = 0;
                MpixelB(res, x, y) = 255;
            }
        }
    }

    inRange(hsv, rangeCyan[0], rangeCyan[1], mask);

    for(int y = 0; y < mask.rows; y++){
        for(int x = 0; x < mask.cols; x++){
            if(MpixelGrey(mask, x, y) == 255){
                MpixelR(res, x, y) = 0;
                MpixelG(res, x, y) = 255;
                MpixelB(res, x, y) = 255;
            }
        }
    }

    inRange(hsv, rangeMagenta[0], rangeMagenta[1], mask);

    for(int y = 0; y < mask.rows; y++){
        for(int x = 0; x < mask.cols; x++){
            if(MpixelGrey(mask, x, y) == 255){
                MpixelR(res, x, y) = 255;
                MpixelG(res, x, y) = 0;
                MpixelB(res, x, y) = 255;
            }
        }
    }

    inRange(hsv, rangeYellow[0], rangeYellow[1], mask);

    for(int y = 0; y < mask.rows; y++){
        for(int x = 0; x < mask.cols; x++){
            if(MpixelGrey(mask, x, y) == 255){
                MpixelR(res, x, y) = 255;
                MpixelG(res, x, y) = 255;
                MpixelB(res, x, y) = 0;
            }
        }
    }

    inRange(hsv, rangeWhite[0], rangeWhite[1], mask);

    for(int y = 0; y < mask.rows; y++){
        for(int x = 0; x < mask.cols; x++){
            if(MpixelGrey(mask, x, y) == 255){
                MpixelR(res, x, y) = 255;
                MpixelG(res, x, y) = 255;
                MpixelB(res, x, y) = 255;
            }
        }
    }

    cv::medianBlur(res, res, 9);

    outRGB = res;
}
