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
        image_aligner(Mat & greySrc);
        virtual ~image_aligner();
        void drawCircles(Mat& rgbSrc);
        vector<Vec3f> getCircles();
    protected:

    private:
        vector<Vec3f> circles;
        int cannyThresh;
        int centerThresh;
        int minRadius;
        int maxRadius;


        void findCircles(Mat &greySrc);
};

#endif // IMAGE_ALIGNER_H
