#include "SLAM_AR.h"



SLAM_AR::SLAM_AR()
{
	mbFinishRequested = false;
	mbFinished = false;
}


SLAM_AR::~SLAM_AR()
{
	//delete PTAM_ptr;
}

bool SLAM_AR::init()
{
	PTAM_ptr = new PTAM_AR();

	if (!PTAM_ptr->init())
		return false;

	FileStorage fs("./save_param/calib_para.yml", CV_STORAGE_READ);
	if (fs.isOpened())
	{
		fs["MX1"] >> mx1;
		fs["MX2"] >> mx2;
		fs["MY1"] >> my1;
		fs["MY2"] >> my2;

		return true;
	}
	else
	{
		return false;
	}
}

void SLAM_AR::update(Mat& pSrc, Mat& pSrc_right)
{
	unique_lock<mutex> lock(mMutex);
	pSrc.copyTo(frame_left);
	pSrc_right.copyTo(frame_right);
}






void SLAM_AR::run()
{
	while (1)
	{
		if (frame_left.empty() || frame_right.empty())
		{
			cv::waitKey(100);
			continue;
		}

		cv::remap(frame_left, frame_rectify_left, mx1, my1, CV_INTER_LINEAR);
		cv::remap(frame_right, frame_rectify_right, mx2, my2, CV_INTER_LINEAR);

		PTAM_ptr->update_frame(frame_rectify_left.data, frame_rectify_right.data, false);

		//imshow("left", frame_rectify_left);
		//imshow("right", frame_rectify_right);
		//waitKey(100);

		if (checkFinish())
			break;
	}

	setFinish();
}

void SLAM_AR::requestFinish()
{
	unique_lock<mutex> lock(mMutexFinish);
	mbFinishRequested = true;
}

bool SLAM_AR::checkFinish()
{
	unique_lock<mutex> lock(mMutexFinish);
	return mbFinishRequested;
}

void SLAM_AR::setFinish()
{
	unique_lock<mutex> lock(mMutexFinish);
	mbFinished = true;
}

bool SLAM_AR::isFinished()
{
	unique_lock<mutex> lock(mMutexFinish);
	return mbFinished;
}

void SLAM_AR::requestStart()
{
	unique_lock<mutex> lock(mMutex);
	PTAM_ptr->start_track();
}

void SLAM_AR::requestReset()
{
	unique_lock<mutex> lock(mMutex);
	PTAM_ptr->reset();
}

double* SLAM_AR::getRotation()
{
	unique_lock<mutex> lock(mMutex);
	return PTAM_ptr->mRotation;
}

double* SLAM_AR::getTranslation()
{
	unique_lock<mutex> lock(mMutex);
	return PTAM_ptr->mTranslation;
}

int SLAM_AR::getTrackingQuality()
{
	unique_lock<mutex> lock(mMutex);
	return PTAM_ptr->get_tracking_quality();
}