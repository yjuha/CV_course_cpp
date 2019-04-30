#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QLabel>
#include <QImage>
#include <QGridLayout>

#include <iostream>
#include <opencv2/opencv.hpp>

#include "qtutils.h"
#include "utils.h"

int main( int argc, char** argv ) {

    //! Help 
    if ( argc != 2 )
    {
        std::cout << "Usage: ./ex3 <Image_Path>\n" << std::endl;
        return -1;
    }

    //! Create cv::Mat object and load image
    cv::Mat img_;
    cv::Mat img;
    img_ = cv::imread(argv[1], 1);

    if ( !img_.data ) {
        std::cout << "No image data\n" << std::endl;
        return -1;
    }

    //! Resize input image to half
    float fx = 0.5;
    float fy = fx;
    int interpolation = cv::INTER_LINEAR;
    cv::resize(img_, img, cv::Size(), fx, fy, interpolation); 

    //! Convert image to grayscale
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    //! Convert from CV_8U to CV_32F, and scale to the range [0,1]
    cv::Mat graysp; // saltnpepper noise corrupted
    cv::Mat graygn; // gaussian noise corrupted
    gray.convertTo(graysp, CV_32F);
    graysp /= 255.; 
    graygn = graysp.clone();

    //! Apply "salt'n pepa" noise
    // options: (1) "salt-&-pepper", (2) "gaussian", (3) ...
    imnoise(graysp, "salt-&-pepper", 0.05);

    //! Apply "gaussian" noise
    imnoise(graygn, "gaussian", 0.05);

    cv::Mat graysp_mf (graysp.size(), CV_32F, cv::Scalar(0));
    cv::Mat graysp_gf (graysp.size(), CV_32F, cv::Scalar(0));

    cv::Mat graygn_mf (graygn.size(), CV_32F, cv::Scalar(0));
    cv::Mat graygn_gf (graygn.size(), CV_32F, cv::Scalar(0));
    
    //! Apply 3x3 median filter to sp corrupted image
    int ksize_median = 3;
    cv::medianBlur(graysp, graysp_mf, ksize_median); 
 
    //! Apply 13x13 gaussian filter to sp corrupted image
    int ksize = 13;
    double sigma = 2.5;
    int ktype = CV_32F;
    cv::Mat k = cv::getGaussianKernel(ksize, sigma, ktype);
    //cv::filter2D(graysp, graysp_gf, graysp.depth(), k);
    conv2D(graysp, graysp_gf, k);

    //! Convert from CV_32F to CV_8U
    cv::Mat graysp_8U;
    graysp *= 255.;
    graysp.convertTo(graysp_8U, CV_8U);

    cv::Mat graygn_8U;
    graygn *= 255.;
    graygn.convertTo(graygn_8U, CV_8U);

    //! Qt stuff 
    QApplication app(argc, argv);

    //! Convert cv::Mat to QImage
    QImage qimg_gn = qtutils::cvmat2qimg(graygn_8U);
    QImage qimg_sp = qtutils::cvmat2qimg(graysp_8U);
    QImage qimg = qtutils::cvmat2qimg(gray);

    //! QGridLayout for showing results
    QGridLayout *gridLayout = new QGridLayout;

    QLabel *tlabel1 = new QLabel;
    QLabel *tlabel2 = new QLabel;
    QLabel *tlabel3 = new QLabel;

    tlabel1->setText("original");
    tlabel2->setText("salt-and-pepper");
    tlabel3->setText("gaussian noise");

    QLabel *qlabel1 = new QLabel;
    QLabel *qlabel2 = new QLabel;
    QLabel *qlabel3 = new QLabel;

    qlabel1->setPixmap(QPixmap::fromImage(qimg));
    qlabel2->setPixmap(QPixmap::fromImage(qimg_sp));
    qlabel3->setPixmap(QPixmap::fromImage(qimg_gn));

    gridLayout->addWidget(tlabel1, 0, 0, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(tlabel2, 0, 1, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(tlabel3, 0, 2, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(qlabel1, 1, 0, 1, 1);
    gridLayout->addWidget(qlabel2, 1, 1, 1, 1);
    gridLayout->addWidget(qlabel3, 1, 2, 1, 1);

    QWidget *w = new QWidget();

    w->setLayout(gridLayout);
    w->setWindowTitle("Grid Layouts");
    w->show();
    
    //! Result images of median filtering
    cv::Mat graysp_mf_U8;
    graysp_mf *= 255.;
    graysp_mf.convertTo(graysp_mf_U8, CV_8U);

    QGridLayout *gridLayout2 = new QGridLayout;

    QLabel *tlabel4 = new QLabel;
    QLabel *tlabel5 = new QLabel;
    QLabel *tlabel6 = new QLabel;

    QLabel *qlabel4 = new QLabel;
    QLabel *qlabel5 = new QLabel;
    QLabel *qlabel6 = new QLabel;

    tlabel4->setText("original");
    tlabel5->setText("salt-and-pepper");
    tlabel6->setText("median filtered");

    QImage qgraysp_mf_U8 = qtutils::cvmat2qimg(graysp_mf_U8);

    qlabel4->setPixmap(QPixmap::fromImage(qimg));
    qlabel5->setPixmap(QPixmap::fromImage(qimg_sp));
    qlabel6->setPixmap(QPixmap::fromImage(qgraysp_mf_U8));

    gridLayout2->addWidget(tlabel4, 0, 0, 1, 1, Qt::AlignCenter);
    gridLayout2->addWidget(tlabel5, 0, 1, 1, 1, Qt::AlignCenter);
    gridLayout2->addWidget(tlabel6, 0, 2, 1, 1, Qt::AlignCenter);
    gridLayout2->addWidget(qlabel4, 1, 0, 1, 1);
    gridLayout2->addWidget(qlabel5, 1, 1, 1, 1);
    gridLayout2->addWidget(qlabel6, 1, 2, 1, 1);

    QWidget *w2 = new QWidget();

    w2->setLayout(gridLayout2);
    w2->setWindowTitle("Grid Layouts");
    w2->show();
 
    return app.exec();

}
