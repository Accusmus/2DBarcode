#include <iostream>

#include "colour_detector.h"
#include "image_aligner.h"

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


const string imgPaths[17] =
{
"res/2Dempty.jpg", "res/abcde.jpg",
"res/abcde_rotated.jpg", "res/abcde_rotated_scaled.jpg",
"res/abcde_scaled.jpg", "res/congratulations.jpg",
"res/congratulations_rotated.jpg", "res/congratulations_rotated_scaled.jpg",
"res/congratulations_scaled.jpg", "res/Darwin.jpg",
"res/Darwin_rotated.jpg", "res/Darwin_rotated_scaled.jpg",
"res/Darwin_scaled.jpg", "res/farfaraway.jpg",
"res/farfaraway_rotated.jpg", "res/farfaraway_rotated_scaled.jpg",
"res/farfaraway_scaled.jpg"
};

const char encodingarray[64]={' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
'0','1','2','3','4','5','6','7','8','9','.'};

string decode2DBarCode(double xMin, double xMax, double yMin, double yMax, Mat &rgbImg);
int main()
{
    Mat rgb;
    Mat grid;
    Mat rgbFiltered;
    Mat transformed;

    namedWindow("Original", 0);
    namedWindow("Filtered", 0);
    namedWindow("Transform", 0);

    resizeWindow("Original", 500, 500);
    resizeWindow("Filtered", 500, 500);
    resizeWindow("Transform", 500, 500);

    colour_detector colourDet = colour_detector();

    image_aligner align = image_aligner();

    rgb = imread(imgPaths[13], 1);
    if(rgb.data == NULL){
        cout << "Error: File could not be read" << endl;
        exit(1);
    }


    //creates RGB filtered image that is clearer for easy reading for decoding message
    //in the process is converted to hsv and then calculates a better RGB equivilant
    colourDet.makeRGB(rgb, rgbFiltered);

    colourDet.getCircles(rgb, grid);

    align.findCircles(grid);
    align.drawCircles(rgb);

    colourDet.getGrid(rgb, grid);

    align.findGrid(grid);
    align.drawGrid(rgb);

    string decodedMsg = decode2DBarCode(align.getxMin(), align.getxMax(), align.getyMin(), align.getyMax(), rgbFiltered);
    cout << decodedMsg << endl;

   //----------------------------------------------------------------------
   //Simplified way of reading file

    imshow("Original", rgb);
    imshow("Filtered", rgbFiltered);

    waitKey(0);
    return 0;
}


string decode2DBarCode(double xMin, double xMax, double yMin, double yMax, Mat &rgbImg){
    int maxColumnsNum = 48;
    int maxRowNum = 48;
    string decodedMsg;

    int colSizeX = round((xMax - xMin)/48);
    int colSizeY = round((yMax - yMin)/48);
    int centerX = colSizeX / 2;
    int centerY = colSizeY / 2;

    bool skip = false;
    for(int y = 1; y < maxRowNum; y++){
        int length;
        if(y <= 6){
            length = 22;
        }else if(y >= 42){
            cout << "y = " << y << endl;
        }else{
            length = 25;
        }
        for(int x = 1; x < length; x++){
            int startX1, startX2, startY1, startY2;

            int currentX1, currentX2, currentY1, currentY2;

            // sent points for where to start reading colour data
            // there is an x and y point for each colour
            // 2 colours make up 1 character
            if(y <= 6){
                //offset x by 6 colours because of circle identifiers
                startX1 = xMin + ((colSizeX * 2) * 3) + centerX;
                startX2 = xMin + ((colSizeX * 2) * 3) + centerX + colSizeX;
            }else if(y >= 42){
                startX1 = xMin + ((colSizeX * 2) * 3) + centerX;
                startX2 = xMin + ((colSizeX * 2) * 3) + centerX + colSizeX;
            }else{
                startX1 = xMin + (colSizeX * 2) + centerX;
                startX2 = xMin + (colSizeX * 2) + centerX + colSizeX;
            }
            startY1 = yMin + centerY;
            startY2 = yMin + centerY;

            // each line has an odd number of colours
            if(y % 2 == 0) { //is an even line
                if((x == 21 && length == 22) || ((x == 24) && (y > 5)) || ((x > 17) && (y >=42))){ //(x == 24 && length == 25) ||
                    skip = true;
                }else{
                    currentX1 = startX1 + ((colSizeX *2)*(x-1)) + 20;
                    currentX2 = startX2 + ((colSizeX *2)*(x-1)) + 20;

                    currentY1 = startY1 + ((colSizeY)*(y-1));
                    currentY2 = startY2 + ((colSizeY)*(y-1));
                    skip = false;
                }
            }else{ // is an odd line
                if(((x > 18) && (y >=42))){
                    skip = true;
                }else if((x != 21 && length == 22) || (x != 24 && length == 25)){
                    currentX1 = startX1 + ((colSizeX *2)*(x-1));
                    currentX2 = startX2 + ((colSizeX *2)*(x-1));

                    currentY1 = startY1 + ((colSizeY)*(y-1));
                    currentY2 = startY2 + ((colSizeY)*(y-1));
                    skip = false;
                }else{
                    currentX1 = startX1 + (40*20);
                    currentX2 = startX1;

                    currentY1 = startY1 + ((colSizeY)*(y-1));
                    currentY2 = startY2 + ((colSizeY)*y);
                    skip = false;
                }
            }

            if(!skip){
                unsigned char temp = 0;
                if(MpixelR(rgbImg, currentX1, currentY1) >= 220){
                    temp = temp | (1<<5);
                }
                if((int)MpixelG(rgbImg, currentX1, currentY1) >= 220){
                    temp = temp | (1<<4);
                }
                if((int)MpixelB(rgbImg, currentX1, currentY1) >= 220){
                    temp = temp | (1<<3);
                }
                if((int)MpixelR(rgbImg, currentX2, currentY2) >= 220){
                    temp = temp | (1<<2);
                }
                if((int)MpixelG(rgbImg, currentX2, currentY2) >= 220){
                    temp = temp | (1<<1);
                }
                if((int)MpixelB(rgbImg, currentX2, currentY2) >= 220){
                    temp = temp | (1);
                }
                decodedMsg += encodingarray[(unsigned int)temp];
            }
        }
    }
    return decodedMsg;
}
