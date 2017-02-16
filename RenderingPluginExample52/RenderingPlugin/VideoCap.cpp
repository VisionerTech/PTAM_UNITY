/*
* Autor : flankechen (flanke.chen@visionertech.com flankechen@gmail.com)
* this is the implementation based on opencv. and capturing video source data for Unity Native rendering plugin
* 
*
* INSTALLATION :
* - make sure you have correctly installed opencv(I work with 2.4.11)
* - 
* - 
LINKFLAGS = 
*	- 
* - Compile the project
* - Enjoy !
*
* Notice this code define two constants for the image width and height (OPENCV_VIDEO_W and OPENCV_VIDEO_H)
*/


#include "VideoCap.h"

using namespace std;
using namespace cv;

//defalut constructor
VideoSource::VideoSource()
{

}

//constructor index_0/1 for left and right camera
bool VideoSource::open_webcam(int index_0, int index_1)
{
	cap.open(index_0);
	cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
	cap.set(CV_CAP_PROP_FRAME_WIDTH, OPENCV_VIDEO_W);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, OPENCV_VIDEO_H);

	cap_right.open(index_1);
	cap_right.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
	cap_right.set(CV_CAP_PROP_FRAME_WIDTH, OPENCV_VIDEO_W);
	cap_right.set(CV_CAP_PROP_FRAME_HEIGHT, OPENCV_VIDEO_H);

	if (cap.isOpened() && cap_right.isOpened())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool VideoSource::read_calib()
{
#ifdef CAMERA_FLIP
	mx1.create(OPENCV_VIDEO_W, OPENCV_VIDEO_H, CV_16S);
	my1.create(OPENCV_VIDEO_W, OPENCV_VIDEO_H, CV_16S);
	mx2.create(OPENCV_VIDEO_W, OPENCV_VIDEO_H, CV_16S);
	my2.create(OPENCV_VIDEO_W, OPENCV_VIDEO_H, CV_16S);
#endif

	FileStorage fs("./save_param/calib_para.yml", CV_STORAGE_READ);
	if (fs.isOpened())
	{
		fs["MX1"] >> mx1;
		fs["MX2"] >> mx2;
		fs["MY1"] >> my1;
		fs["MY2"] >> my2;

		cv::convertMaps(mx1, my1, mx1_f, my1_f, CV_32FC1);
		cv::convertMaps(mx2, my2, mx2_f, my2_f, CV_32FC1);

		fs.release();
		return true;
	}
	else return false;
	

	
}

//destructor
VideoSource::~VideoSource()
{
	cap.release();
	cap_right.release();
}


Mat VideoSource::get_left_rgba()
{
	cap >> src;
	//flip(src.t(), src, 0);
	//imshow("src_flip", src);
	//waitKey(30);
	//cv::remap(src, frame_rectify, mx1, my1, CV_INTER_LINEAR);
	//imshow("frame_rectify", frame_rectify);
	//waitKey(30);

	//flip(frame_rectify, frame_rectify, 1);
	cv::Mat src_rgba;
	cv::cvtColor(src, src_rgba, CV_BGR2RGBA);
	return src_rgba;
}

Mat VideoSource::get_right_rgba()
{
	cap_right >> src_right;
	//flip(src_right.t(), src_right, 0);

	//cv::remap(src_right, frame_rectify_right, mx2, my2, CV_INTER_LINEAR);

	//flip(frame_rectify_right, frame_rectify_right, 1);
	cv::Mat src_rgba;
	cv::cvtColor(src_right, src_rgba, CV_BGR2RGBA);
	return src_rgba;
}

