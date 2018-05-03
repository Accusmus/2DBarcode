#include "image_aligner.h"

image_aligner::image_aligner(Mat & greySrc)
{
    cannyThresh = 200;
    centerThresh = 80;
    minRadius = 20;
    maxRadius = 60;
    findCircles(greySrc);
}

image_aligner::~image_aligner()
{
    //dtor
}

void image_aligner::findCircles(Mat& greySrc){
    Mat grey;
    grey.create(greySrc.size(), CV_8UC1);

    threshold(greySrc, grey, 0, 255, THRESH_BINARY);

    HoughCircles(grey, circles, HOUGH_GRADIENT, 1, grey.rows/8, cannyThresh, centerThresh, minRadius, maxRadius);
}


void image_aligner::drawCircles(Mat& rgbSrc){
    if(circles.size() <= 0) cout << "There are no circles detected make sure to getCenters first" << endl;

    for(size_t i = 0; i < circles.size(); i++){
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        circle(rgbSrc, center, 3, Scalar(0,255,0), -1, 8, 0);

        circle(rgbSrc, center, radius, Scalar(0,0,255), 3, 8, 0);
    }
}

vector<Vec3f> image_aligner::getCircles(){
    return circles;
}
