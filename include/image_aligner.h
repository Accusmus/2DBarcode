#ifndef IMAGE_ALIGNER_H
#define IMAGE_ALIGNER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


class image_aligner
{
    public:
        image_aligner();
        virtual ~image_aligner();

        void findCircles(Mat &greySrc);
        vector<Vec3f> getCircles();
        void drawCircles(Mat& rgbSrc);

        vector<Vec2f> getGrid();
        void drawGrid(Mat& rgbSrc);
        void findGrid(Mat &greySrc);

        void applyRotationTransform(Mat &src, Mat &dest);
        void rightSideUp(Mat &src, Mat &dest);

        bool isAngled();

    protected:

    private:
        // warp rotation matrices
        Mat warpDst;
        Mat warpRotateDst;
        Mat rotMat;
        Mat warpMat;

        vector<Vec3f> circles;
        vector<Vec2f> grid;

        bool angled;
        //parameters used for circle identification
        int cannyThresh;
        int centerThresh;
        int minRadius;
        int maxRadius;

        int angleToRotate;
};

#endif // IMAGE_ALIGNER_H
