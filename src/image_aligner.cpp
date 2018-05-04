#include "image_aligner.h"

image_aligner::image_aligner()
{
    cannyThresh = 200;
    centerThresh = 80;
    minRadius = 20;
    maxRadius = 60;
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


void image_aligner::findGrid(Mat &greySrc){
    Mat grey;
    grey.create(greySrc.size(), CV_8UC1);

    threshold(greySrc, grey, 0, 255, CV_THRESH_BINARY);

    HoughLines(grey, grid, 0.55, CV_PI/180,600, 0, 0);

    int xCount = 0;
    int yCount = 0;
    double xMin = 1000.0, xMax = -1000.0, yMin = 1000.0, yMax = -1000.0;

    for(vector<Vec2f>::iterator ln = grid.begin(); ln != grid.end(); ln++){


        float rho = (*ln)[0], theta = (*ln)[1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

        double angle = abs(atan2(pt2.y - pt1.y, pt2.x - pt1.x)* 180/CV_PI);
        if(angle == 0){
            //line(rgb, pt1, pt2, Scalar(0,0, 255), 2);
            yCount++;
            if(pt1.y >= yMax){
                yMax = pt1.y;
            }else if(pt1.y <= yMin){
                yMin = pt1.y;
            }
        }else if(angle == 90){
            //line(rgb, pt1, pt2, Scalar(0,0, 255), 2);
            xCount++;
            if(pt1.x >= xMax){
                xMax = pt1.x;
            }else if(pt1.x <= xMin){
                xMin = pt1.x;
            }
        }else{
            grid.erase(ln);
        }
    }
}

void image_aligner::drawGrid(Mat& rgbSrc){
    for(vector<Vec2f>::iterator ln = grid.begin(); ln != grid.end(); ln++){
        float rho = (*ln)[0], theta = (*ln)[1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

        line(rgbSrc, pt1, pt2, Scalar(0,0, 255), 2);
    }
}

vector<Vec2f> image_aligner::getGrid(){
    return grid;
}
