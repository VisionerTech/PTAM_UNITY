#pragma once

#ifndef __VIDEOSOURCE_H
#define __VIDEOSOURCE_H

#define OPENCV 1
#if OPENCV
#endif

#if OPENCV
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "PTAM\PTAM\definition.h"

////image size for ar mod rendering
////current PTAM-demo machine camera setup is flipped 90.
//#define OPENCV_VIDEO_W 1080
//#define OPENCV_VIDEO_H 1080
//
////image size for tracking, usually smaller for faster frame rate.
//#define TRACK_IMAGE_W 540
//#define TRACK_IMAGE_H 540
//
////#define LOCAL_VIDEO
//#ifdef LOCAL_VIDEO
////#define FRAME_BY_FRAME
//#endif
//
////camera fliped 90 for current 
////#define CAMERA_FLIP


class VideoSource
{
public:
	//members
	cv::VideoCapture cap;
	cv::VideoCapture cap_right;

	cv::Mat src;
	cv::Mat src_right;

	cv::Mat src_flip;
	cv::Mat src_flip_right;

	cv::Mat frame_rectify;
	cv::Mat frame_rectify_right;
	
#if TRACK_IMAGE_W != OPENCV_VIDEO_W
	cv::Mat frame_rectify_down;
	cv::Mat frame_rectify_right_down;
#endif

	cv::Mat mx1, my1, mx2, my2;
	cv::Mat mx1_f, my1_f, mx2_f, my2_f;

	//public functions
	VideoSource();
	bool open_webcam(int index_0, int index_1);
	bool read_calib();
	~VideoSource();
	cv::Mat get_left_rgba();
	cv::Mat get_right_rgba();

};


#endif


#endif