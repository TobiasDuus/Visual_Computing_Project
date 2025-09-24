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

	sift->detect(img1, keypoints1);
	sift->detect(img2, keypoints2);

	cv::drawKeypoints(img1, keypoints1, features1, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::imshow("sift-features1", features1);

	cv::drawKeypoints(img2, keypoints2, features2, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::imshow("sift-featues2", features2);

	descriptor->compute(img1, keypoints1, descriptor1);
	descriptor->compute(img2, keypoints2, descriptor2);

	std::vector<cv::DMatch> unprunedMatches;
	std::vector<int> matchDistances;
	cv::Ptr<cv::BFMatcher> bf = cv::BFMatcher::create();
	bf->match(descriptor1, descriptor2, unprunedMatches);
	//TODO
	//std::sort(unprunedMatches.begin(), unprunedMatches.end(), comp);
	for (int i = 0; i < unprunedMatches.size() - 1; i++) {
	//	if (unprunedMatches[i].distance*0.8 > unprunedMatches[i + 1].distance) {
		matches.push_back(unprunedMatches[i]);
		matchDistances.push_back(unprunedMatches[i].distance);
	//	}

	}

	cv::Mat imgWithMatches;
	cv::drawMatches(img1, keypoints1, img2, keypoints2, matches, imgWithMatches);
	cv::imshow("sift-matches", imgWithMatches);

	std::cout << "Number of keypoints in image 1: " << keypoints1.size() << std::endl;
	std::cout << "Number of keypoints in image 2: " << keypoints2.size() <<std::endl;
	std::cout << "Number of matches: " << matches.size() << std::endl;
	
	//HOMOGRAPHY
	//HOMOGRAPHY
	std::vector<cv::Point2f> srcPts;
	std::vector<cv::Point2f> dstPts;

	for (cv::DMatch match : matches) {
		srcPts.push_back(keypoints1.at(match.imgIdx).pt);
		dstPts.push_back(keypoints2.at(match.trainIdx).pt);
	}

	cv::Mat mask;
	cv::Mat homography = cv::findHomography(srcPts, dstPts, mask, cv::RANSAC, 3);
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

	orb->detect(img1, keypoints1);
	orb->detect(img2, keypoints2);

	cv::drawKeypoints(img1, keypoints1, features1, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::imshow("orb-features1", features1);

	cv::drawKeypoints(img2, keypoints2, features2, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cv::imshow("orb-featues2", features2);

	orb->compute(img1, keypoints1, descriptor1);
	orb->compute(img2, keypoints2, descriptor2);

	std::vector<cv::DMatch> unprunedMatches;
	std::vector<int> matchDistances;
	cv::Ptr<cv::BFMatcher> bf = cv::BFMatcher::create();
	bf->match(descriptor1, descriptor2, unprunedMatches);	
	//TODO
	//std::sort(unprunedMatches.begin(), unprunedMatches.end(), comp);
	for (int i = 0; i < unprunedMatches.size() - 1; i++) {
		//	if (unprunedMatches[i].distance*0.8 > unprunedMatches[i + 1].distance) {
		matches.push_back(unprunedMatches[i]);
		matchDistances.push_back(unprunedMatches[i].distance);
		//	}

	}

	cv::Mat imgWithMatches;
	cv::drawMatches(img1, keypoints1, img2, keypoints2, matches, imgWithMatches);
	cv::imshow("orb-matches", imgWithMatches);

	std::cout << "Number of keypoints in image 1: " << keypoints1.size() << std::endl;
	std::cout << "Number of keypoints in image 2: " << keypoints2.size() << std::endl;
	std::cout << "Number of matches: " << matches.size() << std::endl;


	//HOMOGRAPHY
	std::vector<cv::Point2f> srcPts;
	std::vector<cv::Point2f> dstPts;

	for (cv::DMatch match : matches) {
		srcPts.push_back(keypoints1.at(match.imgIdx).pt);
		dstPts.push_back(keypoints2.at(match.trainIdx).pt);
	}

	cv::Mat mask;
	cv::Mat homography = cv::findHomography(srcPts, dstPts, mask, cv::RANSAC, 3);
	
}


int main() {
	cv::Mat img1 = cv::imread("C:/Users/tobia/Desktop/Visual-Computing/project_2/images/volleyball-left.jpg", cv::IMREAD_GRAYSCALE);
	cv::Mat resizedImg1;
	cv::resize(img1, resizedImg1, cv::Size(800 * img1.cols / img1.rows, 800));

	cv::Mat img2 = cv::imread("C:/Users/tobia/Desktop/Visual-Computing/project_2/images/volleyball-right.jpg", cv::IMREAD_GRAYSCALE);
	cv::Mat resizedImg2;
	cv::resize(img2, resizedImg2, cv::Size(800 * img2.cols / img2.rows, 800));
	
	siftFeatures(resizedImg1, resizedImg2);
	orbFeatures(resizedImg1, resizedImg2);
	cv::waitKey(0);
	return 0;
}