## PTAM in UNITY

this is the PTAM stereo visual studio project for VisionerTech VMG-PROV. It is used to sense the environment and localize the headset.

## Requirement:

1.  Recommended specs: Intel Core i5-4460/8G RAM/GTX 660/at least two USB3.0/
2.  Windows x64 version.(tested on win7/win10)

## Installation

1.  Download and install the  Visual Studio 2012, the download address is here: https://www.microsoft.com/zh-cn/download/details.aspx?id=30682

2.  Download and install OpenCV(version 2.4.X) as:
http://docs.opencv.org/2.4.11/doc/tutorials/introduction/windows_install/windows_install.html

3.  Open "PTAM_UNITY/RenderingPlugin/VisualStudio2013/RenderingPlugin.sln", then change settings for visual studio project linked with OpenCV:
http://docs.opencv.org/2.4.11/doc/tutorials/introduction/windows_visual_studio_Opencv/windows_visual_studio_Opencv.html

## How to Run

1.  Build "/RenderingPlugin/VisualStudio2013/RenderingPlugin.sln" release version, RenderingPlugin.dll will copy to corresponding unity folder automatically.
2.  Put camera parameters got from stereo_calib or stereo_calib_executable to "/UnityProject/save_param/"
3.  As in stereo_seethrough, use Unity open "/UnityProject/" folder. The StereoCamera object is made of two cameras. Every camera has its own plane object, which images got from real VMG PROV camera are rendered on. Connect UseRenderingPlugin.cs and UseRenderingPlugin_right.cs scripts with left plane and right plane. Then a ball as an AR object are shown in foreground.
![alt text](https://github.com/VisionerTech/PTAM_UNITY/blob/master/readme_image/unity.png "unity")
4. Put the cameras towards a plane target. Then double click "scene" and  click run afterwards.
![alt text](https://github.com/VisionerTech/PTAM_UNITY/blob/master/readme_image/plane.png "plane")
5.  If the left image and right image are swop——left plane showing the right camera image and right plane showing the left camera image, we should change UseRenderingPlugin.cs script from:

      private static extern Boolean OpenWebCam(int index_0, int index_1);
to:

      private static extern Boolean OpenWebCam(int index_1, int index_o);
