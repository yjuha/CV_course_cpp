#include <iostream>
#include <opencv2/opencv.hpp>

#include "utils.h"

int main(int argc, char** argv ) {

    //! Help 
    if ( argc != 2 )
    {
        std::cout << "Usage: ./ex3 <Image_Path>\n" << std::endl;
        return -1;
    }

    //! Create cv::Mat object and load image
    cv::Mat image;
    image = cv::imread(argv[1], 1);

    if ( !image.data )
    {
        std::cout << "No image data\n" << std::endl;
        return -1;
    }

    //! Convert image to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    //! Convert from CV_8U to CV_32F, and scale to the range [0,1]
    cv::Mat grayf;
    gray.convertTo(grayf, CV_32F);
    grayf /= 255.;

    //! Apply "salt'n pepa" noise
    // options: (1) "salt-&-pepper", (2) "gaussian", (3) ...
    //imnoise(grayf, "salt-&-pepper", 0.05);

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", grayf);

    cv::waitKey(0);

    int ksize = 13;
    double sigma = 2.5;
    int ktype = CV_32F;
    cv::Mat k = cv::getGaussianKernel(ksize, sigma, ktype);
    cv::Mat dst (grayf.size(), CV_32F, cv::Scalar(0));	

    cv::filter2D(grayf, dst, grayf.depth(), k);

    cv::Mat A = (cv::Mat_<float>(4,4) << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    cv::Mat B = (cv::Mat_<float>(4,4) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    cv::Mat C = (cv::Mat_<float>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

    conv2D(grayf, dst, k);

    cv::namedWindow( "Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", dst);

    cv::waitKey(0);

    return 0;
}
