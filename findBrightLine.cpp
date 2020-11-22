class meanPix
{
public:
	int val = 0;
	float leftRho = 0;
	float leftTheta = 0;
	float rightRho = 0;
	float rightTheta = 0;
}

//std::vector<cv::Vec2f>::const_iterator it = lines.begin();
//while (it != lines.end() && it != ) {
//	float rho = (*it)[0]; // 첫 번째 요소는 rho 거리
//	float theta = (*it)[1]; // 두 번째 요소는 델타 각도
//
//	
//
//	if (theta < PI / 4. || theta > 3. * PI / 4.) { // 수직 행
//		cv::Point pt1(rho / cos(theta), 0); // 첫 행에서 해당 선의 교차점
//		cv::Point pt2((rho - result.rows * sin(theta)) / cos(theta), result.rows);
//		// 마지막 행에서 해당 선의 교차점
//		cv::line(original, pt1, pt2, cv::Scalar(255), 1); // 하얀 선으로 그리기
//	}
//	++it;
//}

vector<Point> find2Line(vector<Point> lines)
{
	int step = img.rows / 20;
	vector<meanPix> meansVec;



	for (int i = 0; i < lines.size() - 1; i++)
	{
		float rho = lines[i][0]; // 첫 번째 요소는 rho 거리
		float theta = lines[i][1]; // 두 번째 요소는 델타 각도

		float rhoNext = lines[i + 1][0]; // 첫 번째 요소는 rho 거리
		float thetaNext = lines[i + 1][1]; // 두 번째 요소는 델타 각도

		// 라인 사이 간격이 baseDistance 이상이면 차선이 아니다.
		int baseDistance = 30;




		cv::Point pt1_check((rho - height * sin(theta)) / cos(theta), height); // 첫 행에서 해당 선의 교차점
		cv::Point ptN_check((rhoNext - height * sin(thetaNext)) / cos(thetaNext), height); // 첫 행에서 해당 선의 교차점

		// 절대값 거리가 크면 (두 라인 사이 거리가 너무 크면) pass
		if (abs(pt1_check - ptN_check.x) > baseDistance)
		{
			continue;
		}



		vector<int> pixVal;

		// 5번씩 거리 구하기
		for (int height = step * 11; height < step * 16; height += step)
		{
			cv::Point pt1((rho - height * sin(theta)) / cos(theta), height); // 첫 행에서 해당 선의 교차점
			cv::Point ptN((rhoNext - height * sin(thetaNext)) / cos(thetaNext), height); // 첫 행에서 해당 선의 교차점

			Point ptMean = Point((int)((pt1.x + ptN.x) / 2), height);


			int bright = img.at(ptMean.x, ptMean.y);
			pixVal.push_back(bright);
		}

		// 밝기 값 평균 구하기
		int sumBright = accumulate(pixVal.begin(), pixVal.end(), 0);
		int meanBright = sumBright / pixVal.size();


		meanPix mean;
		mean.val = meanBright;
		mean.leftRho = rho;
		mean.leftTheta = theta;
		mean.rightRho = rhoNext;
		mean.rightTheta = thetaNext;

		meansVec.push_back(mean);
	}

	meanPix max;
	meanPix max2;
	meanPix defaultMeanPix;


	for (int i = 0; i < meansVex.size(); i++)
	{
		if (max.val < meansVec[i].val)
		{
			max = meansVec[i];
		}

		if (max2.val < meansVec[i].val && meansVec[i].val < max.val)
		{
			max2 = meansVec[i];
		}
	}

	vector<Point> result;


	float leftRho = 0;
	float leftTheta = 0;
	float rightRho = 0;
	float rightTheta = 0;


	Point basePoint = Point(0, 0);

	if (max.value != 0)
	{

		float rho = max.leftRho; // 첫 번째 요소는 rho 거리
		float theta = max.leftTheta; // 두 번째 요소는 델타 각도

		float rhoNext = max.rightRho; // 첫 번째 요소는 rho 거리
		float thetaNext = max.rightTheta; // 두 번째 요소는 델타 각도


		for (int height = step * 12; height < step * 20; height += (step * 5))
		{
			cv::Point pt1((rho - height * sin(theta)) / cos(theta), height); // 첫 행에서 해당 선의 교차점
			cv::Point ptN((rhoNext - height * sin(thetaNext)) / cos(thetaNext), height); // 첫 행에서 해당 선의 교차점

			Point ptMean = Point((int)((pt1.x + ptN.x) / 2), height);

			result.push_back(ptMean);
		}
	}
	else
	{
		result.push_back(basePoint);
		result.push_back(basePoint);
	}





	if (max2.value != 0)
	{

		float rho = max2.leftRho; // 첫 번째 요소는 rho 거리
		float theta = max2.leftTheta; // 두 번째 요소는 델타 각도

		float rhoNext = max2.rightRho; // 첫 번째 요소는 rho 거리
		float thetaNext = max2.rightTheta; // 두 번째 요소는 델타 각도


		for (int height = step * 12; height < step * 20; height += (step * 5))
		{
			cv::Point pt1((rho - height * sin(theta)) / cos(theta), height); // 첫 행에서 해당 선의 교차점
			cv::Point ptN((rhoNext - height * sin(thetaNext)) / cos(thetaNext), height); // 첫 행에서 해당 선의 교차점

			Point ptMean = Point((int)((pt1.x + ptN.x) / 2), height);

			result.push_back(ptMean);
		}
	}
	else
	{
		result.push_back(basePoint);
		result.push_back(basePoint);
	}


	return result;
