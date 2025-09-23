#include <opencv2/opencv.hpp> 
#include <iostream>

int main() {
	cv::Mat img = cv::imread("C:/Users/tobia/Desktop/Visual-Computing/project_2/images/volleyball-left.jpg", cv::IMREAD_GRAYSCALE);
	cv::Mat resizedImg;
	cv::resize(img, resizedImg, cv::Size(800 * img.cols / img.rows, 800));

	// Compute gaussians
	cv::Mat gaussians[6][5];
	gaussians[0][0] = resizedImg;
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 6; i++) {
			if (i < 5) {
				cv::GaussianBlur(gaussians[i][j], gaussians[i + 1][j], cv::Size(7, 7), 0);
			}
		}
		if (j < 4) {
			cv::resize(gaussians[5][j], gaussians[0][j+1], cv::Size(gaussians[5][j].cols / 2, gaussians[5][j].rows / 2));
		}
	}

	// Compute the difference of the gaussians
	cv::Mat differenceGaussians[5][5];
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			cv::Mat img1;
			gaussians[i][j].convertTo(img1, CV_32FC3);
			cv::Mat img2;
			gaussians[i+1][j].convertTo(img2, CV_32FC3);
			cv::Mat difference;
			cv::subtract(img1, img2, difference);
			cv::Mat normalized = difference / 2 + 128;
			normalized.convertTo(differenceGaussians[i][j], CV_8UC3);
		}
	} 

	cv::Mat kk = cv::Mat::eye(cv::Size(50, 50), CV_32FC3);
	cv::imshow("Demo0", kk);
	cv::imshow("Demo1", gaussians[1][1]);
	cv::imshow("Demo2", gaussians[2][1]);
	cv::imshow("Demo3", differenceGaussians[1][1]);

	cv::waitKey(0);
	return 0;
}