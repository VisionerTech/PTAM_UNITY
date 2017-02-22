//class encapsule only the tracking and mapping things.
//NO RENDER things 
//also for dll export


#include <cvd/image.h>
#include <cvd/rgb.h>
#include <cvd/byte.h>
#include "definition.h"

//#ifdef PTAMDLL_EXPORT
//#define PTAMDLL_API __declspec(dllexport) 
//#else
//#define PTAMDLL_API __declspec(dllimport) 
//#endif

class ATANCamera;
class Map;
class MapMaker;
class Tracker;


extern "C"
{
	class PTAM_AR{
	public:

		CVD::ImageRef mVideoSize;
		CVD::Image<CVD::Rgb<CVD::byte> > mimFrameRGB;
		CVD::Image<CVD::byte> mimFrameBW;

		CVD::Image<CVD::byte> mimFrameBW_right;
		CVD::Image<CVD::Rgb<CVD::byte>> mimFrameRGB_right;

		Map *mpMap; 
		MapMaker *mpMapMaker; 
		Tracker *mpTracker; 
		ATANCamera *mpCamera;

		double mTranslation[3];
		double mRotation[9];

		double msg;

		PTAM_AR();
		~PTAM_AR();

		bool init();

		//void processs_frame(bool bDraw);
		//for unity plugin, unity open webcam and pass in RGBA data, this function rectify the image data and as input of PTAM frame
#ifdef UNITY_PLUGIN
		void update_frame(unsigned char* image_data_BGR_left, unsigned char* image_data_BGR_right, bool bDraw);
		void start_track();
		void reset();
		int get_tracking_quality();
#endif

	};
#ifdef UNITY_PLUGIN
	// PTAM_AR* _cdecl create_PTAM();
	////PTAMDLL_API void process_frame(PTAM_AR* PTAM_ptr);
	// void update_frame(PTAM_AR* PTAM_ptr, unsigned char* img_bgr_left, unsigned char* img_bgr_right);
	// void init_stereo(PTAM_AR* PTAM_ptr);
	// //double* get_translation(PTAM_AR* PTAM_ptr);
	// //double* get_rotation(PTAM_AR* PTAM_ptr);
	// void delte_PTAM(PTAM_AR* PTAM_ptr);
	////return some message for debug;
	// double get_message(PTAM_AR* PTAM_ptr);
	// int get_tracking_quality(PTAM_AR* PTAM_ptr);
	// void reset_tracking(PTAM_AR* PTAM_ptr);

	//
	//create_PTAM
	//delete_PTAM
	//init_stereo
	//update_frame(PTAM*, unsigned char* left, unsigned char* right, int frame_width, int frame_height)
	//get_translation
	//get_rotation
#endif

}

