#include <opencv2/opencv.hpp> 
#include <iostream>

bool comp(cv::DMatch a, cv::DMatch b) {
	return a.distance > b.distance;
}

void siftFeatures(cv::Mat img1, cv::Mat img2) {
	cv::Ptr<cv::SIFT> sift = cv::SiftFeatureDetector::create();
	cv::Ptr<cv::SIFT> descriptor = cv::SiftDescriptorExtractor::create();
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;
	cv::Mat descriptor1;
	cv::Mat descriptor2;
	cv::Mat features1;
	cv::Mat features2;
	std::vector<cv::DMatch> matches;

	clock_t start = clock();

	sift->detect(img1, keypoints1);
	sift->detect(img2, keypoints2);

	descriptor->compute(img1, keypoints1, descriptor1);
	descriptor->compute(img2, keypoints2, descriptor2);

	std::vector<cv::DMatch> unprunedMatches;
	std::vector<int> matchDistances;
	cv::Ptr<cv::BFMatcher> bf = cv::BFMatcher::create();
	bf->match(descriptor1, descriptor2, unprunedMatches);

	clock_t duration = clock() - start;
	std::cout << "SIFT Matching took " << duration << std::endl;

	//TODO
	//std::sort(unprunedMatches.begin(), unprunedMatches.end(), comp);
	std::cout << "SIFT match distances: " << std::endl;
	for (int i = 0; i < unprunedMatches.size() - 1; i++) {
	//	if (unprunedMatches[i].distance*0.8 > unprunedMatches[i + 1].distance) {
		std::cout << unprunedMatches[i].distance << std::endl;
		matches.push_back(unprunedMatches[i]);
		matchDistances.push_back(unprunedMatches[i].distance);
	//	}

	}

	cv::drawKeypoints(img1, keypoints1, features1, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::imshow("sift-features1", features1);

	cv::drawKeypoints(img2, keypoints2, features2, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::imshow("sift-featues2", features2);

	cv::Mat imgWithMatches;
	cv::drawMatches(img1, keypoints1, img2, keypoints2, matches, imgWithMatches);
	cv::imshow("sift-matches", imgWithMatches);

	std::cout << "SIFT: Number of keypoints in image 1: " << keypoints1.size() << std::endl;
	std::cout << "SIFT: Number of keypoints in image 2: " << keypoints2.size() <<std::endl;
	std::cout << "SIFT: Number of matches: " << matches.size() << std::endl;
	
	//HOMOGRAPHY
	std::vector<cv::Point2f> srcPts;
	std::vector<cv::Point2f> dstPts;

	for (cv::DMatch match : matches) {
		srcPts.push_back(keypoints1.at(match.queryIdx).pt);
		dstPts.push_back(keypoints2.at(match.trainIdx).pt);
	}

	clock_t start2 = clock();
	cv::Mat mask;
	cv::Mat homography = cv::findHomography(srcPts, dstPts, mask, cv::RANSAC, 20);
	clock_t duration2 = clock() - start2;
	std::cout << "SIFT Homography took " << duration2 << std::endl;
	std::cout << "Sift Inliers: " << cv::sum(mask)[0] << std::endl;

	cv::Mat warpedImg;
	cv::warpPerspective(
		img2, 
		warpedImg, 
		homography, 
		cv::Size(img1.cols + img2.cols, img1.rows),
		cv::WARP_INVERSE_MAP,
		cv::BORDER_CONSTANT,
		cv::Scalar());
	cv::imshow("sift-warped-img", warpedImg);
	
	cv::Mat stitchedImg = warpedImg;
	for (int i = 0; i < img1.rows; i++) {
		for (int j = 0; j < img1.cols; j++) {
			if (stitchedImg.at<uchar>(i, j) == 0) {
				stitchedImg.at<uchar>(i, j) = img1.at<uchar>(i, j);
			}
		}
	}
	cv::imshow("sift-stitched-img", stitchedImg);

	cv::Mat stitchedImgBlending = warpedImg;
	for (int i = 0; i < img1.rows; i++) {
		for (int j = 0; j < img1.cols; j++) {
			if (stitchedImgBlending.at<uchar>(i, j) == 0) {
				stitchedImgBlending.at<uchar>(i, j) = img1.at<uchar>(i, j);
			}
			if (stitchedImgBlending.at<uchar>(i, j) != 0) {
				stitchedImgBlending.at<uchar>(i, j) = img1.at<uchar>(i, j) / 2 + stitchedImgBlending.at<uchar>(i, j) / 2;
			}
		}
	}
	cv::imshow("sift-stitched-blended-img", stitchedImg);
}

void orbFeatures(cv::Mat img1, cv::Mat img2) {
	cv::Ptr<cv::ORB> orb = cv::ORB::create();
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;
	cv::Mat descriptor1;
	cv::Mat descriptor2;
	cv::Mat features1;
	cv::Mat features2;
	std::vector<cv::DMatch> matches;

	clock_t start = clock();

	orb->detect(img1, keypoints1);
	orb->detect(img2, keypoints2);

	orb->compute(img1, keypoints1, descriptor1);
	orb->compute(img2, keypoints2, descriptor2);

	std::vector<cv::DMatch> unprunedMatches;
	std::vector<int> matchDistances;
	cv::Ptr<cv::BFMatcher> bf = cv::BFMatcher::create();
	bf->match(descriptor1, descriptor2, unprunedMatches);	
	//TODO
	//std::sort(unprunedMatches.begin(), unprunedMatches.end(), comp);
	std::cout << "ORB match distances: " << std::endl;
	for (int i = 0; i < unprunedMatches.size() - 1; i++) {
		//	if (unprunedMatches[i].distance*0.8 > unprunedMatches[i + 1].distance) {
		std::cout << unprunedMatches[i].distance << std::endl;
		matches.push_back(unprunedMatches[i]);
		matchDistances.push_back(unprunedMatches[i].distance);
		//	}

	}

	clock_t duration = clock() - start;
	std::cout << "ORB Matching took " << duration << std::endl;

	cv::drawKeypoints(img1, keypoints1, features1, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::imshow("orb-features1", features1);

	cv::drawKeypoints(img2, keypoints2, features2, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::imshow("orb-featues2", features2);

	cv::Mat imgWithMatches;
	cv::drawMatches(img1, keypoints1, img2, keypoints2, matches, imgWithMatches);
	cv::imshow("orb-matches", imgWithMatches);

	std::cout << "ORB: Number of keypoints in image 1: " << keypoints1.size() << std::endl;
	std::cout << "ORB: Number of keypoints in image 2: " << keypoints2.size() << std::endl;
	std::cout << "ORB: Number of matches: " << matches.size() << std::endl;


	//HOMOGRAPHY
	std::vector<cv::Point2f> srcPts;
	std::vector<cv::Point2f> dstPts;

	for (cv::DMatch match : matches) {
		srcPts.push_back(keypoints1.at(match.queryIdx).pt);
		dstPts.push_back(keypoints2.at(match.trainIdx).pt);
	}

	clock_t start2 = clock();
	cv::Mat mask;
	cv::Mat homography = cv::findHomography(srcPts, dstPts, mask, cv::RANSAC, 20);
	clock_t duration2 = clock() - start2;
	std::cout << "ORB Homography took " << duration2 << std::endl;
	std::cout << "Orb Inliers: " << cv::sum(mask)[0] << std::endl;

	cv::Mat warpedImg;
	cv::warpPerspective(img2, 
		warpedImg, 
		homography, 
		cv::Size(img1.cols + img2.cols, img1.rows), 
		cv::WARP_INVERSE_MAP,
		cv::BORDER_CONSTANT,
		cv::Scalar());
	cv::imshow("orb-warped-img", warpedImg);

	cv::Mat stitchedImg = warpedImg;
	for(int i = 0; i < img1.rows; i++) {
		for(int j = 0; j < img1.cols; j++) {
			if(stitchedImg.at<uchar>(i,j) == 0) {
				stitchedImg.at<uchar>(i,j) = img1.at<uchar>(i,j);
			}
		}
	}
	cv::imshow("orb-stitched-img", stitchedImg);

	cv::Mat stitchedImgBlending = warpedImg;
	for (int i = 0; i < img1.rows; i++) {
		for (int j = 0; j < img1.cols; j++) {
			if (stitchedImgBlending.at<uchar>(i, j) == 0) {
				stitchedImgBlending.at<uchar>(i, j) = img1.at<uchar>(i, j);
			}
			if (stitchedImgBlending.at<uchar>(i, j) != 0) {
				stitchedImgBlending.at<uchar>(i, j) = img1.at<uchar>(i, j)/2 + stitchedImgBlending.at<uchar>(i, j)/2;
			}
		}
	}
	cv::imshow("orb-stitched-blended-img", stitchedImg);

}


int main() {
	cv::Mat img1 = cv::imread("C:/Users/tobia/Desktop/Visual_Computing_Project/images/indoor-left.jpg", cv::IMREAD_GRAYSCALE);
	cv::Mat img2 = cv::imread("C:/Users/tobia/Desktop/Visual_Computing_Project/images/indoor-right.jpg", cv::IMREAD_GRAYSCALE);

	cv::Mat resizedImg1;
	cv::resize(img1, resizedImg1, cv::Size(500 * img1.cols / img1.rows, 500));

	cv::Mat resizedImg2;
	cv::resize(img2, resizedImg2, cv::Size(500 * img2.cols / img2.rows, 500));
	
	siftFeatures(resizedImg1, resizedImg2);
	orbFeatures(resizedImg1, resizedImg2);
	cv::waitKey(0);
	return 0;
}