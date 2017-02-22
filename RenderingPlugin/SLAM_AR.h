#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <mutex>
#include "PTAM\PTAM\PTAM_AR_dll.h"

using namespace cv;
using namespace std;

extern "C"
{
	class SLAM_AR{
	public:
		SLAM_AR();
		~SLAM_AR();

		PTAM_AR* PTAM_ptr;

		//remap Mats.
		cv::Mat mx1, my1, mx2, my2;

		//frame
		cv::Mat frame_left, frame_right;
		//frame downsample
		cv::Mat frame_down_left, frame_down_right;
		//frame rectify
		cv::Mat frame_rectify_left, frame_rectify_right;

		//mutex and locks
		std::mutex mMutex;
		std::mutex mMutexFinish;
		bool mbFinishRequested;
		bool mbFinished;

		//init function
		bool init();

		//update sourse image from rendering thread(camera source as well)
		void update(Mat& pSrc, Mat& pSrc_right);
		void run();
		//stop the SLAM thread
		void requestFinish();
		bool checkFinish();
		void setFinish();
		bool isFinished();
		//request start slam.
		void requestStart();
		//reset slam.
		void requestReset();
		//get translation and rotation
		double* getTranslation();
		double* getRotation();
		int getTrackingQuality();
	};
}