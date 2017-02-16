//image size for ar mod rendering
//current PTAM-demo machine camera setup is flipped 90. use the fliped resolution here.
#define OPENCV_VIDEO_W 1080
#define OPENCV_VIDEO_H 1080

//image size for tracking, usually smaller for faster frame rate.
#define TRACK_IMAGE_W 540
#define TRACK_IMAGE_H 540

//#define LOCAL_VIDEO
#ifdef LOCAL_VIDEO
//#define FRAME_BY_FRAME
#endif

//#define FRAME_BY_FRAME

//printing out debug logs
//#define DEBUG_PRINT

//opencv widow showing left and right image
//#define SHOW_CVIMAGE

//for unity dll, unity open webcam and pass in the image data.
#define UNITY_PLUGIN

//for camera set up, camera flip 90
//#define CAMERA_FLIP_90

//for stereo init number of good trails 
#define STEREO_INIT_TS 100

//for stereo feature pairs bundle adjust might goes infin loop
#define STEREO_INIT_BA_TS 50

//for how fast, from last keyframe to add new keyframes
#define LAST_KEYFRAME_DROP 10

//for ar drawing game mode, 
//#define MIKU
#define EYEGAME