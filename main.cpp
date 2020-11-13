#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ximgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Histogram1D {
private: int histSize[1]; // 히스토그램 빈도수 
	   float hranges[2]; // 히스토그램 최소/최대 화소값 
	   const float* ranges[1]; 
	   int channels[1]; // 1채널만 사용

public: Histogram1D() { // 1차원 히스토그램을 위한 인자 준비 
	histSize[0] = 256; 
	hranges[0] = 0.0; 
	hranges[1] = 255.0; 
	ranges[0] = hranges; 
	channels[0] = 0; }

	  MatND getHistogram(const Mat &image){ 
		  MatND hist; 
		  calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
					
		  return hist; 
	  }

	  MatND equalize(const cv::Mat& image) {
		  cv::Mat result; cv::equalizeHist(image, result);
		  return result;
	  }
};

Mat IncreaseContrast(Mat img) {
	Histogram1D h;

	Mat result= h.equalize(img);

	return result;
}

Mat makeROI(Mat& img,int shape) {
	Mat imgROI=img.clone();

	switch (shape) {
	case 0:
		rectangle(imgROI, Point(0, 0), Point(img.cols, img.rows/2), Scalar(0, 0, 0),FILLED);
		cvtColor(imgROI, imgROI,COLOR_BGR2GRAY);

		imgROI = IncreaseContrast(imgROI);
		GaussianBlur(imgROI, imgROI, Size(9, 9), 1.0);
		return imgROI;
	case 1: {
		rectangle(imgROI, Point(0, 0), Point(img.cols, img.rows / 2), Scalar(0, 0, 0), FILLED);

		Point rook_points[1][3];
		rook_points[0][0] = Point(img.cols / 2, img.rows / 2);
		rook_points[0][1] = Point(0, img.rows/2);
		rook_points[0][2] = Point(0, img.rows);

		const Point* ppt[1] = { rook_points[0] };
		int npt[] = { 3 };
		fillPoly(imgROI, ppt, npt, 1, Scalar(0, 0, 0));

		Point rook_points2[1][3];
		rook_points2[0][0] = Point(img.cols / 2, img.rows / 2);
		rook_points2[0][1] = Point(img.cols, img.rows / 2);
		rook_points2[0][2] = Point(img.cols, img.rows);

		const Point* ppt2[1] = { rook_points2[0] };
		int npt2[] = { 3 };
		fillPoly(imgROI, ppt2, npt2, 1, Scalar(0, 0, 0));

		cvtColor(imgROI, imgROI, COLOR_BGR2GRAY);

		imgROI = IncreaseContrast(imgROI);
		GaussianBlur(imgROI, imgROI, Size(9, 9), 1.0);
		
		return imgROI;
	}
	case 2: {
		rectangle(imgROI, Point(0, 0), Point(img.cols, img.rows / 2), Scalar(0, 0, 0), FILLED);

		Point rook_points[1][3];
		rook_points[0][0] = Point(img.cols / 4, img.rows / 2);
		rook_points[0][1] = Point(0, img.rows/2);
		rook_points[0][2] = Point(0, img.rows);

		const Point* ppt[1] = { rook_points[0] };
		int npt[] = { 3 };
		fillPoly(imgROI, ppt, npt, 1, Scalar(0, 0, 0));

		Point rook_points2[1][3];
		rook_points2[0][0] = Point(3*img.cols / 4, img.rows / 2);
		rook_points2[0][1] = Point(img.cols, img.rows / 2);
		rook_points2[0][2] = Point(img.cols, img.rows);

		const Point* ppt2[1] = { rook_points2[0] };
		int npt2[] = { 3 };
		fillPoly(imgROI, ppt2, npt2, 1, Scalar(0, 0, 0));

		cvtColor(imgROI, imgROI, COLOR_BGR2GRAY);

		imgROI = IncreaseContrast(imgROI);
		GaussianBlur(imgROI, imgROI, Size(9, 9), 1.0);
		
		return imgROI;
		}	
	}

}

int main(int argc, char** argv)
{
	VideoCapture cap(argv[1]);

	if (!cap.isOpened()) {
		printf("Can't open the video");
		return -1;
	}

	Mat img;

	while (1) {
		cap >> img;

		resize(img, img, cv::Size(img.cols / 4, img.rows / 4), 0, 0);

		if (img.empty()) {
			printf("empty image");
			return 0;
		}
		imshow("camera img", img);
		Mat imgROI = makeROI(img, 0);
		imshow("ROI img", imgROI);
		Mat imgROI1 = makeROI(img, 1);
		imshow("ROI1 img", imgROI1);
		Mat imgROI2 = makeROI(img, 2);
		imshow("ROI2 img", imgROI2);

		if (waitKey(25) == 27)
			break;
	}
	return 0;
}
