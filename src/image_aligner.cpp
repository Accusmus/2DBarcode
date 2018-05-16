#include "image_aligner.h"

image_aligner::image_aligner()
{
    cannyThresh = 200;
    centerThresh = 80;
    minRadius = 10;
    maxRadius = 60;

    angled = false;

    angleToRotate = -500;
}

image_aligner::~image_aligner()
{
    //dtor
}

//------------------------------------------------
// find cirles in image

void image_aligner::findCircles(Mat& greySrc){
    Mat grey;
    grey.create(greySrc.size(), CV_8UC1);

    threshold(greySrc, grey, 0, 255, THRESH_BINARY);

    HoughCircles(grey, circles, HOUGH_GRADIENT, 1, grey.rows/8, cannyThresh, centerThresh, minRadius, maxRadius);
}

//------------------------------------------------
// draw the circles that have been identified onto an rgb image
// used for identifying the orintation of the 2D barcode

void image_aligner::drawCircles(Mat& rgbSrc){
    if(circles.size() <= 0) cout << "There are no circles detected make sure to getCenters first" << endl;

    for(size_t i = 0; i < circles.size(); i++){
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(rgbSrc, center, 3, Scalar(0,255,0), -1, 8, 0);
        circle(rgbSrc, center, radius, Scalar(0,0,255), 3, 8, 0);
    }
}

//------------------------------------------------------
//find the grid for the 2D barcode this is used for fine alignment and finding distance between rows and cols


void image_aligner::findGrid(Mat &greySrc){
    Mat grey;
    grey.create(greySrc.size(), CV_8UC1);
    vector<int> gridAngles = vector<int>();
    vector<int> gridAngCount = vector<int>();

    threshold(greySrc, grey, 0, 255, CV_THRESH_BINARY);

    HoughLines(grey, grid, 1.6, CV_PI/180,770, 0, 0);

    int xCount = 0;
    int yCount = 0;

    for(vector<Vec2f>::iterator ln = grid.begin(); ln != grid.end(); ln++){


        float rho = (*ln)[0], theta = (*ln)[1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

        double angle = atan2(pt2.y - pt1.y, pt2.x - pt1.x)* 180/CV_PI;

        //find the prominant angle to find out how much to rotate
        //gridAngle is a vector of the different angles of the grid
        //gridAngCount is a vector of the count of each angle
        //both share the same index
        if(gridAngles.size() == 0){
            gridAngles.push_back(angle);
            gridAngCount.push_back(1);
        }else{
            int ang = -1;
            //check if it is existing
            for(int i = 0; i < gridAngles.size(); i++){
                if((int)gridAngles[i] == (int)angle){
                    ang = i;
                }
            }
            if(ang != -1){
                gridAngCount[ang] ++;
            }else{
                gridAngles.push_back(angle);
                gridAngCount.push_back(1);
            }
        }
    }

    //count the number of angles that are the same
    //and find the rotation amount
    int maxCount = -10000;
    int angForRot = 0;
    for(int i = 0; i < gridAngCount.size(); i++){
        if(gridAngCount[i] >= maxCount){
            maxCount = gridAngCount[i];
            angForRot = gridAngles[i];
        }
    }

    cout << "Rotation angle is: " << angForRot  << " Degrees" << endl;
    if(abs(angForRot) == 90 || abs(angForRot) == 0){
        angForRot = 0; angled = false;
    }
    else{
        angForRot = angForRot-90;
        angled = true;
    }
    angleToRotate = angForRot;
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

// straighten image within 90 degrees
void image_aligner::applyRotationTransform(Mat &src, Mat &dest){
    // check if angle has been calculated
    if(angleToRotate == -500){
        cout << "Error: you have not called find grid function to set angle" << endl;
        dest = src;
        return;
    }
    //first rotation is to get it so that it is rotated with 90 degrees
    Point2f center(src.cols/2, src.rows/2);
    Mat r = getRotationMatrix2D(center, angleToRotate, 1.0);
    warpAffine(src, dest, r, src.size());

}

//rotate image by either 90, 180 or 270 degrees in order to get it right side up
void image_aligner::rightSideUp(Mat &src, Mat &dest){
    int rotation = 0; //rotation by 90 degrees increments

    //we dont want to do this step if there arent 3 triangles
    if(circles.size() > 3){
        cout << "Error: too many circles detected" << endl;
        dest = src;
        return;
    }
    if(circles.size() < 3){
        cout << "Error: too few circles detected" << endl;
        dest = src;
        return;
    }

    //There are 4 possible orientations
    // x = 0 being left and x = y being right etc
    // the correct orientation has points at:
    // |(0,0) |       |
    // |(0,1) | (1,1) |
    Point2i triangleP[3];

    //we first find where the points are
    for(int i = 0; i < circles.size(); i++){
        cout << "x: " << circles[i][0] << " y: " << circles[i][1] << endl;
        if(circles[i][0] > 500){
            triangleP[i].x = 1;
        }
        if(circles[i][0] < 500){
            triangleP[i].x = 0;
        }
        if(circles[i][1] > 500){
            triangleP[i].y = 1;
        }
        if(circles[i][1] < 500){
            triangleP[i].y = 0;
        }
    }

    //here we find which point is missing (is not part of the triangle)
    int bitx = triangleP[0].x ^ triangleP[1].x ^ triangleP[2].x;
    int bity = triangleP[0].y ^ triangleP[1].y ^ triangleP[2].y;

    //we can deduce by knowing this which orientation it is in
    //and therefore rotate it
    if((bitx == 0) && (bity == 0)){
        rotation = 270;
    }else if((bitx == 0) && (bity == 1)){
        rotation = 180;
    }else if((bitx == 1) && (bity == 1)){
        rotation = 90;
    }else{
        rotation = 0;
    }

    cout << "Rotate image by: " << rotation << " to be right side up." << endl;

    //we then rotate the image around its center point
    Point2f center(src.cols/2, src.rows/2);
    Mat r = getRotationMatrix2D(center, rotation, 1.0);
    warpAffine(src, dest, r, src.size());
}

bool image_aligner::isAngled(){
    return angled;
}
