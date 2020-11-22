#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ximgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include<numeric>

using namespace cv;
using namespace std;


#define PI 3.141592
#define qSize 20


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
	channels[0] = 0;
}

	  MatND getHistogram(const Mat& image) {
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

	//Mat result = img.clone();
	Mat result = h.equalize(img);

	return result;
}

//color가 0이면 blue
Mat changeColor(Mat img, int color)
{
	Mat hsv = img.clone();
	cvtColor(hsv, hsv, COLOR_BGR2HSV);
	//Scalar lowYellow = Scalar(25, 27, 92);
	//Scalar upYellow = Scalar(42, 60, 166);

	Scalar lowWhite = Scalar(0.21, 0.02, 0.5);
	lowWhite = lowWhite * 255;

	Scalar upWhite = Scalar(0.53, 0.08, 1);
	upWhite = upWhite * 255;


	Scalar lowBlue = Scalar(90, 60, 60);
	Scalar upBlue = Scalar(120, 255, 255);


	//Mat yellow = hsv.clone();
	Mat white = hsv.clone();
	Mat blue = hsv.clone();

	//inRange(hsv, lowYellow, upYellow, yellow);
	inRange(hsv, lowWhite, upWhite, white);
	inRange(hsv, lowBlue, upBlue, blue);


	//imshow("yellow", yellow);
	//cvtColor(yellow, yellow, COLOR_HSV2BGR);

	imshow("white", white);

	imshow("blue", blue);
	if (color == 0)
		return blue;
	if (color == 1)
		return white;
}



// 0: gray, 1: 전체 평활화, 2: 하단부 평활화
Mat makeROI(Mat& img, int type, int makeGray = 1) {
	Mat imgROI = img.clone();

	if (makeGray)
		cvtColor(imgROI, imgROI, COLOR_BGR2GRAY);



	Mat underContrast = imgROI.clone();



	if (type == 0)
	{
		GaussianBlur(imgROI, imgROI, Size(9, 9), 1.0);
		return imgROI;
	}
	//Mat gray = imgROI.clone();




	imgROI = IncreaseContrast(imgROI);
	GaussianBlur(imgROI, imgROI, Size(9, 9), 1.0);

	if (type == 1)
	{
		return imgROI;
	}


	Rect bounds(0, 0, img.cols, img.rows);
	Rect r(0, img.rows / 2, img.cols, img.rows / 2); // partly outside 
	Mat roi = underContrast(r & bounds);
	roi = IncreaseContrast(roi);
	GaussianBlur(roi, roi, Size(9, 9), 1.0);


	// 하단부만 평활화한 영상 만들기
	for (int j = img.rows / 2; j < img.rows; j++)
	{
		//j열의 주소(nc개만큼) 가져오기
		uchar* data = underContrast.ptr<uchar>(j);
		uchar* dataRoi = roi.ptr<uchar>(j - img.rows / 2);

		for (int i = 0; i < img.cols; i++)
		{
			data[i] = dataRoi[i];
		}

	}
	//imshow("tmp", underContrast);
	if (type == 2)
	{
		return underContrast;
	}



}

Mat hideNonROI(Mat& imgROI, int shape) {
	//return imgROI;

	switch (shape) {
	case 0: {
		rectangle(imgROI, Point(0, 0), Point(imgROI.cols, imgROI.rows / 2), Scalar(0, 0, 0), FILLED);
		//imgROI = IncreaseContrast(imgROI);
		//GaussianBlur(imgROI, imgROI, Size(9, 9), 1.0);
		return imgROI;
	}
	case 1: {
		rectangle(imgROI, Point(0, 0), Point(imgROI.cols, imgROI.rows / 2), Scalar(0, 0, 0), FILLED);

		Point rook_points[1][3];
		rook_points[0][0] = Point(imgROI.cols / 2, imgROI.rows / 2);
		rook_points[0][1] = Point(0, imgROI.rows / 2);
		rook_points[0][2] = Point(0, imgROI.rows);

		const Point* ppt[1] = { rook_points[0] };
		int npt[] = { 3 };
		fillPoly(imgROI, ppt, npt, 1, Scalar(0, 0, 0));

		Point rook_points2[1][3];
		rook_points2[0][0] = Point(imgROI.cols / 2, imgROI.rows / 2);
		rook_points2[0][1] = Point(imgROI.cols, imgROI.rows / 2);
		rook_points2[0][2] = Point(imgROI.cols, imgROI.rows);

		const Point* ppt2[1] = { rook_points2[0] };
		int npt2[] = { 3 };
		fillPoly(imgROI, ppt2, npt2, 1, Scalar(0, 0, 0));

		return imgROI;

	}

	case 2: {
		rectangle(imgROI, Point(0, 0), Point(imgROI.cols, imgROI.rows / 2), Scalar(0, 0, 0), FILLED);

		Point rook_points[1][3];
		rook_points[0][0] = Point(imgROI.cols / 4, imgROI.rows / 2);
		rook_points[0][1] = Point(0, imgROI.rows / 2);
		rook_points[0][2] = Point(0, imgROI.rows);

		const Point* ppt[1] = { rook_points[0] };
		int npt[] = { 3 };
		fillPoly(imgROI, ppt, npt, 1, Scalar(0, 0, 0));

		Point rook_points2[1][3];
		rook_points2[0][0] = Point(3 * imgROI.cols / 4, imgROI.rows / 2);
		rook_points2[0][1] = Point(imgROI.cols, imgROI.rows / 2);
		rook_points2[0][2] = Point(imgROI.cols, imgROI.rows);

		const Point* ppt2[1] = { rook_points2[0] };
		int npt2[] = { 3 };
		fillPoly(imgROI, ppt2, npt2, 1, Scalar(0, 0, 0));

		return imgROI;


	}


		  return imgROI;
	}

}



vector<Point> findEdgeLines(Mat original, Mat img, int lowThres = 100, int highThres = 300, const char* windowName = "fel img");

vector<Point> find2Line(vector<Point> lines);
void addNewPoint(vector<Point> lines);
vector<Point> findMeanLine();
Mat drawLines(Mat img, vector<Point> lines);

Mat service(vector<Point> lines);


// Global Variables
queue<Point> lftq1;
queue<Point> lftq2;
queue<Point> rghtq1;
queue<Point> rghtq2;

const int alpha_slider_max = 255;
int alpha_slider;
double alpha;
double beta;
Mat image1, image2, dst;
void on_trackbar(int, void*);





int main(int argc, char** argv)
{



	//VideoCapture cap(argv[1]);
	VideoCapture cap("clip2.mp4");
	if (!cap.isOpened()) {
		printf("Can't open the video");
		return -1;
	}

	Mat img;

	while (1) {
		cap >> img;

		resize(img, img, cv::Size(img.cols / 4, img.rows / 4), 0, 0);
		//resize(img, img, cv::Size(img.cols / 2, img.rows / 2), 0, 0);


		if (img.empty()) {
			printf("empty image");
			return 0;
		}
		imshow("camera img", img);

		Mat blue = img.clone();
		blue = changeColor(img, 0);

		Mat white = img.clone();
		//white = changeColor(img, 1);

		Mat bluecopy = img.clone();
		Mat back = img.clone();
		back = Scalar(0, 255, 255);
		back.copyTo(bluecopy, blue);

		imshow("bluecopy", bluecopy);
		//Mat blueROI = makeROI(blue, 0, 0);


		//Mat imgROI = makeROI(img, 0);
		Mat imgROI = makeROI(bluecopy, 0);


		imgROI = hideNonROI(imgROI, 0);
		imshow("ROI img", imgROI);

		//Mat whiteROI = makeROI(white, 0, 0);
		//whiteROI = hideNonROI(whiteROI, 0);
		//imshow("whiteROI", whiteROI);


		//


		//Mat imgROI1 = makeROI(img, 1);
		//imgROI1 = hideNonROI(imgROI1, 0);
		//imshow("ROI1 img", imgROI1);


		//Mat imgROI2 = makeROI(img, 2);
		//imgROI2 = hideNonROI(imgROI2, 0);
		//imshow("ROI2 img", imgROI2);

		//


		//// 절반 평활화 (나중에 삭제)
		//imgROI = imgROI2.clone();




		vector<Point> lines;
		//lines = findEdgeLines(img, imgROI, 100, 300, "100~300");

		lines = findEdgeLines(img, imgROI, 70, 210, "70~210");

		//lines = findEdgeLines(img, blueROI, 100, 200, "blueROI100~200");

		//lines = findEdgeLines(img, whiteROI, 100, 300, "whiteROI100~300");

		//lines = findEdgeLines(img, imgROI1, 100, 300, "imgROI1");
		//lines = findEdgeLines(img, imgROI2, 100, 300, "imgROI2");

		//findEdgeLines(img, imgROI, 200, 300, "200~300");

		////findEdgeLines(img, imgROI, 70, 210, "70~210");


		//findEdgeLines(img, imgROI, 10, 300, "10~300");

		//findEdgeLines(img, imgROI, 100, 200, "100~200");

		//findEdgeLines(img, imgROI, 100, 400, "100~400");

		//findEdgeLines(imgROI, 100, 300, "rec");
		//findEdgeLines(imgROI1, 100, 300, "roi1");
		//findEdgeLines(imgROI2, 100, 300, "roi2");


		vector<Point> finalLines;
		finalLines = find2Line(lines);

		////if (finalLines.empty() && (!p1.empty()))
		//if (finalLines.empty() && (!(p1.empty())))
		//{
		//	finalLines = findMeanLine();
		//}
		//else if (!finalLines.empty())
		//{
		//	addNewPoint(finalLines);
		//}


		addNewPoint(finalLines);
		finalLines = findMeanLine();

		Mat lineImg;
		lineImg = drawLines(img, finalLines);

		//if (!finalLines.empty())
		//{
		//	Mat lineImg;
		//	lineImg = drawLines(img, finalLines);
		//}




		if (waitKey(25) == 27)
			break;
	}
	return 0;
}


vector<Point> findEdgeLines(Mat original, Mat img, int lowThres, int highThres, const char* windowName)
//vector<Vec2f> findEdgeLines(Mat original, Mat img, int lowThres, int highThres, const char* windowName)
{

	int kernel_size = 3;

	Mat contours = img.clone();
	Mat dst1;
	Canny(img, contours, lowThres, highThres, kernel_size);

	dst1 = Scalar::all(0);





	img.copyTo(dst1, contours);
	imshow(windowName, dst1);

	/*
	// 선 감지 위한 허프 변환
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(contours, lines, 1, CV_PI / 180, 45);

	// 선 그리기
	cv::Mat result(contours.rows, contours.cols, CV_8U, cv::Scalar(255));
	//std::cout << "Lines detected: " << lines.size() << std::endl;

	// 선 벡터를 반복해 선 그리기



	std::vector<cv::Vec2f>::const_iterator it = lines.begin();
	while (it != lines.end()) {
		float rho = (*it)[0]; // 첫 번째 요소는 rho 거리
		float theta = (*it)[1]; // 두 번째 요소는 델타 각도
		if (theta < PI / 4. || theta > 3. * PI / 4.) { // 수직 행
			cv::Point pt1(rho / cos(theta), 0); // 첫 행에서 해당 선의 교차점
			cv::Point pt2((rho - result.rows * sin(theta)) / cos(theta), result.rows);
			// 마지막 행에서 해당 선의 교차점
			cv::line(original, pt1, pt2, cv::Scalar(255), 1); // 하얀 선으로 그리기
		}
		//else { // 수평 행
		//	cv::Point pt1(0, rho / sin(theta)); // 첫 번째 열에서 해당 선의 교차점
		//	cv::Point pt2(result.cols, (rho - result.cols * cos(theta)) / sin(theta));
		//	// 마지막 열에서 해당 선의 교차점
		//	cv::line(original, pt1, pt2, cv::Scalar(255), 1); // 하얀 선으로 그리기
		//}
		//std::cout << "line: (" << rho << "," << theta << ")\n";
		++it;
	}


	string b = "houph";
	string houphWindow = windowName + b;

	cv::namedWindow(houphWindow);
	cv::imshow(houphWindow, original);

	*/


	vector<Vec4i> lines;
	cv::HoughLinesP(contours, lines, 1, CV_PI / 180, 30, 30, 5);



	string b = "houph";
	string houphWindow = windowName + b;

	cv::namedWindow(houphWindow);
	cv::imshow(houphWindow, original);



	vector<Point> lp;

	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		//line(original, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, 8);

		Point p1 = Point(l[0], l[1]);
		Point p2 = Point(l[2], l[3]);


		double dy = l[3] - l[1];
		double dx = l[2] - l[0];
		double angle = atan(dy / dx) * (180.0 / CV_PI);
		if (dx < 0.0)
		{
			angle += 180.0;
		}
		else if (dy < 0.0)
		{
			angle += 360.0;
		}

		if ((angle > 30 && angle < 150) || (angle > 210 && angle < 330))
		{ // 수직 행
			line(original, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, 8);
			//cout << "angle: " << angle << endl;

			lp.push_back(p1);
			lp.push_back(p2);
		}


		cv::imshow(houphWindow, original);

		//waitKey(0.1);

		//if (theta < PI / 4. || theta > 3. * PI / 4.) { // 수직 행

		//line(img_lane, Point(l[0], l[1]), Point(l[2], l[3]), Scalar::all(255), 1, 8);
	}



	//return lines;


	return lp;
}

vector<Point> find2Line(vector<Point> lines)
{
	vector<Point> result;

	Point lft1; // 왼쪽 1
	Point lft2; // 왼쪽 2
	Point rght1; // 오른쪽 1
	Point rght2; // 오른쪽 2

	lft1 = Point(0, 0);
	lft2 = Point(0, 0);
	rght1 = Point(0, 0);
	rght2 = Point(0, 0);

	if (!lines.empty())
	{
		if (lines.size() >= 4)
		{
			lft1 = lines[0];
			lft2 = lines[1];
			rght1 = lines[2];
			rght2 = lines[3];
		}




	}

	// 결과 벡터 리턴
	result.push_back(lft1);
	result.push_back(lft2);
	result.push_back(rght1);
	result.push_back(rght2);

	//printf("lft1 : %3d, %3d \n", lft1.x, lft1.y);
	//printf("lft2 : %3d, %3d \n\n", lft2.x, lft2.y);
	//printf("rght1 : %3d, %3d \n", rght1.x, rght1.y);
	//printf("rght2 : %3d, %3d \n\n\n", rght2.x, rght2.y);

	return result;
}




// 들어온 좌표 큐에 넣기
void addNewPoint(vector<Point> lines)
{
	Point base = Point(0, 0);

	for (int i = 0; i < 4; i ++)
	{
		// find2Line 결과로 찾은 라인이 있는 경우 그 라인을 넣는다.
		if (lines[i] != base)
		{
			//printf("lines[i] : %3d, %3d \n", lines[i].x, lines[i].y);



			if (i == 0)
			{
				lftq1.push(lines[i]);
				if (lftq1.size() > qSize)
				{
					lftq1.pop();
				}
			}
			else if (i == 1)
			{
				lftq2.push(lines[i]);
				if (lftq2.size() > qSize)
				{
					lftq2.pop();
				}
			}
			else if (i == 2)
			{
				rghtq1.push(lines[i]);
				if (rghtq1.size() > qSize)
				{
					rghtq1.pop();
				}
			}
			else if (i == 3)
			{
				rghtq2.push(lines[i]);
				if (rghtq2.size() > qSize)
				{
					rghtq2.pop();
				}
			}
		}
	}

	//// find2Line 결과로 찾은 라인이 있는 경우
	//if (!lines.empty())
	//{
	//	for (int i = 0; i < 3; i += 2)
	//	{
	//		p1.push(lines[i]);
	//		p2.push(lines[i + 1]);

	//		if (p1.size() > 20)
	//		{
	//			p1.pop();
	//			p2.pop();
	//		}

	//	}
	//}
}


vector<Point> findMeanLine()
{
	queue<Point> tmp_lftq1(lftq1);
	queue<Point> tmp_lftq2(lftq2);
	queue<Point> tmp_rghtq1(rghtq1);
	queue<Point> tmp_rghtq2(rghtq2);

	vector<int> lft1x;
	vector<int> lft1y;
	vector<int> lft2x;
	vector<int> lft2y;

	vector<int> rght1x;
	vector<int> rght1y;
	vector<int> rght2x;
	vector<int> rght2y;

	vector<Point> result;

	int i = 0;

	// 큐가 비어있으면 그냥 종료
	if (tmp_lftq1.empty() || tmp_rghtq1.empty())
	{
		Point base = Point(0, 0);

		for (int i = 0; i < 4; i += 1)
		{
			result.push_back(base);
		}

		return result;
	}


	// 큐에서 좌표 분리해내기
	while (!tmp_lftq1.empty())
	{
		Point pt;

		pt = tmp_lftq1.front();

		lft1x.push_back(pt.x);
		lft1y.push_back(pt.y);

		pt = tmp_lftq2.front();
		lft2x.push_back(pt.x);
		lft2y.push_back(pt.y);

		tmp_lftq1.pop();
		tmp_lftq2.pop();


	}

	while (!tmp_rghtq1.empty())
	{
		Point pt;
		pt = tmp_rghtq1.front();

		rght1x.push_back(pt.x);
		rght1y.push_back(pt.y);

		pt = tmp_rghtq2.front();
		rght2x.push_back(pt.x);
		rght2y.push_back(pt.y);

		tmp_rghtq1.pop();
		tmp_rghtq2.pop();


	}

	// 합
	int lft1xS = accumulate(lft1x.begin(), lft1x.end(), 0);
	int lft1yS = accumulate(lft1y.begin(), lft1y.end(), 0);
	int lft2xS = accumulate(lft2x.begin(), lft2x.end(), 0);
	int lft2yS = accumulate(lft2y.begin(), lft2y.end(), 0);

	int rght1xS = accumulate(rght1x.begin(), rght1x.end(), 0);
	int rght1yS = accumulate(rght1y.begin(), rght1y.end(), 0);
	int rght2xS = accumulate(rght2x.begin(), rght2x.end(), 0);
	int rght2yS = accumulate(rght2y.begin(), rght2y.end(), 0);

	// 평균값으로 포인트 만들기
	Point lft1 = (Point(lft1xS / lft1x.size(), lft1yS / lft1y.size()));
	Point lft2 = (Point(lft2xS / lft2x.size(), lft2yS / lft2y.size()));

	Point rght1 = (Point(rght1xS / rght1x.size(), rght1yS / rght1y.size()));
	Point rght2 = (Point(rght2xS / rght2x.size(), rght2yS / rght2y.size()));

	// 결과 벡터 리턴
	result.push_back(lft1);
	result.push_back(lft2);
	result.push_back(rght1);
	result.push_back(rght2);

	return result;
}


Mat drawLines(Mat img, vector<Point> lines)
{
	line(img, lines[0], lines[1], Scalar(0, 0, 255), 2, 8);
	line(img, lines[2], lines[3], Scalar(0, 0, 255), 2, 8);

	imshow("line img", img);
	return img;
}


Mat service(vector<Point> lines)
{
	Mat img;

	result.push_back(lft1);
	result.push_back(lft2);
	result.push_back(rght1);
	result.push_back(rght2);
	//int lft1xS = accumulate(lft1x.begin(), lft1x.end(), 0);
	//int lft1yS = accumulate(lft1y.begin(), lft1y.end(), 0);
	//int lft2xS = accumulate(lft2x.begin(), lft2x.end(), 0);
	//int lft2yS = accumulate(lft2y.begin(), lft2y.end(), 0);

	//int rght1xS = accumulate(rght1x.begin(), rght1x.end(), 0);
	//int rght1yS = accumulate(rght1y.begin(), rght1y.end(), 0);
	//int rght2xS = accumulate(rght2x.begin(), rght2x.end(), 0);
	//int rght2yS = accumulate(rght2y.begin(), rght2y.end(), 0);


	return img;
}
