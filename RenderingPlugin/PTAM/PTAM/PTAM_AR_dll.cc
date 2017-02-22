#include "PTAM_AR_dll.h"
#include "ATANCamera.h"
#include "MapMaker.h"
#include "Tracker.h"
#include <fstream>
#include <string>
#include <opencv2\opencv.hpp>


#ifdef UNITY_PLUGIN
#include <Windows.h>
#endif

using namespace CVD;
using namespace std;
using namespace GVars3;

PTAM_AR::PTAM_AR()
{


}

bool PTAM_AR::init()
{
	msg = -1;
	mVideoSize = ImageRef(OPENCV_VIDEO_W, OPENCV_VIDEO_H);
#if TRACK_IMAGE_W != OPENCV_VIDEO_W
	mimFrameBW.resize(ImageRef(TRACK_IMAGE_W,TRACK_IMAGE_H));
	mimFrameBW_right.resize(ImageRef(TRACK_IMAGE_W, TRACK_IMAGE_H));
#else
	mimFrameBW.resize(mVideoSize);
	mimFrameBW_right.resize(mVideoSize);
#endif
	
	mimFrameRGB.resize(mVideoSize);
	mimFrameRGB_right.resize(mVideoSize);


	//for dll use only, dll part takes no "settings.cfg" should read camera.cfg alone;
	std::ifstream File_in("./camera.cfg");
	if(!File_in.is_open())
	{
		cout<<"can't read camera.cfg"<<endl;
		return false;
	}
	std::string key,value;
	std::string line;
	while(std::getline(File_in,line))
	{
		std::stringstream line_to_read(line);
		std::getline(line_to_read, key, '[');
		std::getline(line_to_read, value, ']');
	}

	if(key != "Camera.Parameters=")
	{
		cout<<"Camera.parameters not match"<<endl;
		return false;
	}

	std::stringstream value_to_read(value);
	float p1,p2,p3,p4,p5;

	value_to_read>>p1>>p2>>p3>>p4>>p5;

	File_in.close();


	Vector<NUMTRACKERCAMPARAMETERS> vTest;

	vTest = makeVector(p1,p2,p3,p4,p5);

	//vTest = GV3::get<Vector<NUMTRACKERCAMPARAMETERS> >("Camera.Parameters", ATANCamera::mvDefaultParams, HIDDEN);
	//mpCamera = new ATANCamera("Camera");
	mpCamera = new ATANCamera("Camera",p1,p2,p3,p4,p5);
	//Vector<2> v2;
	//if(v2==v2) ;
	if(vTest == ATANCamera::mvDefaultParams)
    {
		cout << endl;
		cout << "! Camera.Parameters is not set, need to run the CameraCalibrator tool" << endl;
		cout << "  and/or put the Camera.Parameters= line into the appropriate .cfg file." << endl;
		return false;
    }
 //// 
	mpMap = new Map;
	mpMapMaker = new MapMaker(*mpMap, *mpCamera);
	mpTracker = new Tracker(ImageRef(TRACK_IMAGE_W, TRACK_IMAGE_H), *mpCamera, *mpMap, *mpMapMaker);
	return true;
}

//void PTAM_AR::processs_frame(bool bDraw)
//{
//	mVideoSource_ptr->GetAndFillFrameBWandRGB(mimFrameBW, mimFrameBW_right, mimFrameRGB, mimFrameRGB_right);
//
//
//	mpTracker->TrackFrame(mimFrameBW, mimFrameBW_right, bDraw);
//
//
//	mTranslation[0] = mpTracker->GetCurrentPose().get_translation()[0];
//	mTranslation[1] = mpTracker->GetCurrentPose().get_translation()[1];
//	mTranslation[2] = mpTracker->GetCurrentPose().get_translation()[2];
//
//	mRotation[0] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[0];
//	mRotation[1] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[1];
//	mRotation[2] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[2];
//	mRotation[3] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[3];
//	mRotation[4] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[4];
//	mRotation[5] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[5];
//	mRotation[6] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[6];
//	mRotation[7] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[7];
//	mRotation[8] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[8];
//
//}



void conversionNB(cv::Mat& frame, Image<CVD::byte> &imBW){
	//Mat clone = frame.clone();
	//Mat_<Vec3b>& frame_p = (Mat_<Vec3b>&)clone;
	//for (int i = 0; i < frame.rows; i++){
	//	for (int j = 0; j < frame.cols; j++){	
	//	imBW[i][j] = (frame_p(i,j)[0] + frame_p(i,j)[1] + frame_p(i,j)[2]) / 3;
	//	}
	//}

#ifdef DEBUG_PRINT
	double tic = (double)cvGetTickCount();
#endif

	cv::Mat cvd_image(frame.rows, frame.cols, CV_8UC1, (unsigned char*)imBW.data());
	cv::Mat frame_dst;
	cvtColor(frame, frame_dst, CV_BGR2GRAY);
	//cv::imshow("toCVDImage", frame_dst);
	//cv::waitKey();
	frame_dst.copyTo(cvd_image);

#ifdef DEBUG_PRINT
	double toc = (double)cvGetTickCount();
	double detectionTime = (toc - tic) / ((double)cvGetTickFrequency() * 1000);
	cout << "mat to cvd frame in:  " << detectionTime << endl;
#endif
}

#ifdef UNITY_PLUGIN
void GetAndFillFrameBW(CVD::Image<CVD::byte> &imL_BW, CVD::Image<CVD::byte> &imR_BW, unsigned char* image_data_BGR_left, unsigned char* image_data_BGR_right)
{
	cv::Mat frame_rectify(OPENCV_VIDEO_H, OPENCV_VIDEO_W, CV_8UC3, (unsigned char*)image_data_BGR_left);
	cv::Mat frame_rectify_down, frame_rectify_right_down;
#ifdef SHOW_CVIMAGE
	cv::imshow("src", frame_rectify);
	cv::waitKey(30);
#endif

#if TRACK_IMAGE_W != OPENCV_VIDEO_W
	cv::resize(frame_rectify, frame_rectify_down, cv::Size(TRACK_IMAGE_W, TRACK_IMAGE_H));
	conversionNB(frame_rectify_down, imL_BW);
#else
	conversionNB(frame_rectify, imL_BW);
#endif


	cv::Mat frame_rectify_right(OPENCV_VIDEO_H, OPENCV_VIDEO_W, CV_8UC3, (unsigned char*)image_data_BGR_right);

#ifdef SHOW_CVIMAGE
	cv::imshow("src_right", frame_rectify_right);
	cv::waitKey(30);
#endif


#if TRACK_IMAGE_W != OPENCV_VIDEO_W
	resize(frame_rectify_right, frame_rectify_right_down, cv::Size(TRACK_IMAGE_W, TRACK_IMAGE_H));
	conversionNB(frame_rectify_right_down, imR_BW);
#else
	conversionNB(frame_rectify_right, imR_BW);
#endif

}
#endif


#ifdef UNITY_PLUGIN
void PTAM_AR::update_frame(unsigned char* image_data_BGR_left, unsigned char* image_data_BGR_right, bool bDraw)
{
	double tic=(double)cvGetTickCount();


	//mVideoSource_ptr->GetAndFillFrameBWandRGB(mimFrameBW, mimFrameBW_right, mimFrameRGB, mimFrameRGB_right, image_data_RGBA_left, image_data_RGBA_right);

	GetAndFillFrameBW(mimFrameBW, mimFrameBW_right, image_data_BGR_left, image_data_BGR_right);




	double toc=(double)cvGetTickCount();
	double detectionTime = (toc-tic)/((double) cvGetTickFrequency()*1000);


	//msg = mpTracker->m_msg;
	//msg = detectionTime;

	mpTracker->TrackFrame(mimFrameBW, mimFrameBW_right, bDraw);
	
	msg = mpTracker->m_msg;

	mTranslation[0] = mpTracker->GetCurrentPose().get_translation()[0];
	mTranslation[1] = mpTracker->GetCurrentPose().get_translation()[1];
	mTranslation[2] = mpTracker->GetCurrentPose().get_translation()[2];

	mRotation[0] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[0];
	mRotation[1] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[1];
	mRotation[2] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[2];
	mRotation[3] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[3];
	mRotation[4] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[4];
	mRotation[5] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[5];
	mRotation[6] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[6];
	mRotation[7] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[7];
	mRotation[8] = mpTracker->GetCurrentPose().get_rotation().get_matrix().my_data[8];

}
#endif

PTAM_AR::~PTAM_AR()
{
}

#ifdef UNITY_PLUGIN
void PTAM_AR::start_track()
{
	mpTracker->mbUserPressedSpacebar = true;
}

void PTAM_AR::reset()
{
	mpTracker->Reset();
}


int PTAM_AR::get_tracking_quality()
{
	if (mpTracker->mTrackingQuality == Tracker::BAD)
		return 0;
	else if (mpTracker->mTrackingQuality == Tracker::DODGY)
		return 1;
	else if (mpTracker->mTrackingQuality == Tracker::GOOD)
		return 2;
}
#endif

//#ifdef UNITY_PLUGIN
//PTAM_AR* create_PTAM()
//{
//	PTAM_AR* PTAM_ptr = new PTAM_AR();
//	if(PTAM_ptr->init())
//		return PTAM_ptr;
//	return NULL;
//}
//
////void process_frame(PTAM_AR* PTAM_ptr)
////{
////	PTAM_ptr->processs_frame(false);
////}
//
//void update_frame(PTAM_AR* PTAM_ptr, unsigned char* img_bgr_left, unsigned char* img_bgr_right)
//{
//	PTAM_ptr->update_frame(img_bgr_left, img_bgr_right, false);
//}
//
//double* get_translation(PTAM_AR* PTAM_ptr)
//{
//
//	return PTAM_ptr->mTranslation;
//}
//
//double get_message(PTAM_AR* PTAM_ptr)
//{
//	return PTAM_ptr->msg;
//}
//
//int get_tracking_quality(PTAM_AR* PTAM_ptr)
//{
//	if(PTAM_ptr->mpTracker->mTrackingQuality == Tracker::BAD)
//		return 0;
//	else if(PTAM_ptr->mpTracker->mTrackingQuality == Tracker::DODGY)
//		return 1;
//	else if(PTAM_ptr->mpTracker->mTrackingQuality == Tracker::GOOD)
//		return 2;
//}
//
//void reset_tracking(PTAM_AR* PTAM_ptr)
//{
//	PTAM_ptr->mpTracker->Reset();
//}
//
//double* get_rotation(PTAM_AR* PTAM_ptr)
//{
//
//	return PTAM_ptr->mRotation;
//}
//
//void init_stereo(PTAM_AR* PTAM_ptr)
//{
//	
//	PTAM_ptr->mpTracker->mbUserPressedSpacebar = true;
//
//}
//
//void delte_PTAM(PTAM_AR* PTAM_ptr)
//{
//	delete PTAM_ptr;
//}
//#endif