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


    protected:

    private:
        vector<Vec3f> circles;
        vector<Vec2f> grid;

        int cannyThresh;
        int centerThresh;
        int minRadius;
        int maxRadius;
};

#endif // IMAGE_ALIGNER_H
