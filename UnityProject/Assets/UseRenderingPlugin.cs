// on OpenGL ES there is no way to query texture extents from native texture id
#if (UNITY_IPHONE || UNITY_ANDROID) && !UNITY_EDITOR
	#define UNITY_GLES_RENDERER
#endif


using UnityEngine;
using System;
using System.Collections;
using System.Runtime.InteropServices;


public class UseRenderingPlugin : MonoBehaviour
{
	// Native plugin rendering events are only called if a plugin is used
	// by some script. This means we have to DllImport at least
	// one function in some active script.
	// For this example, we'll call into plugin's SetTimeFromUnity
	// function and pass the current time so the plugin can animate.

#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport ("RenderingPlugin")]
#endif
	private static extern void SetTimeFromUnity(float t);

	//native function for open web cam.
#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport ("RenderingPlugin")]
#endif
	private static extern Boolean OpenWebCam(int index_0, int index_1);

	//native function for init a SLAM system.
	#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
	#else
	[DllImport ("RenderingPlugin", EntryPoint="init_SLAM_AR")]
	#endif
	private static extern Boolean init_SLAM_AR();

	//native function call for destroying web cam
#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport ("RenderingPlugin")]
#endif
	private static extern void DestroyWebCam();

//native function for get remap parameters for shader remap
#if UNITY_IPHONE && !UNITY_EDITOR
[DllImport ("__Internal")]
#else
	[DllImport ("RenderingPlugin", EntryPoint="get_map_x1")]
#endif
	private static extern IntPtr get_map_x1();

	//native function for get remap parameters for shader remap
#if UNITY_IPHONE && !UNITY_EDITOR
[DllImport ("__Internal")]
#else
[DllImport ("RenderingPlugin", EntryPoint="get_map_y1")]
#endif
private static extern IntPtr get_map_y1();

	//native function for requesting start slam tracking
	#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
	#else
	[DllImport ("RenderingPlugin", EntryPoint="start_slam")]
	#endif
	private static extern void start_slam();

	//native function for requesting reset slam
	#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
	#else
	[DllImport ("RenderingPlugin", EntryPoint="reset_slam")]
	#endif
	private static extern void reset_slam();

	//getting translation 
	#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
	#else
	[DllImport("RenderingPlugin",EntryPoint="get_translation")]
	#endif
	private static extern IntPtr get_translation();


	//getting rotation
	#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
	#else
	[DllImport("RenderingPlugin",EntryPoint="get_rotation")]
	#endif
	private static extern IntPtr get_rotation();

	//getting the tracking quality
	//0 bad, 1 dogy, 3 good
	#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
	#else
	[DllImport("RenderingPlugin",EntryPoint="get_tracking_state")]
	#endif
	private static extern int get_tracking_state();


	// We'll also pass native pointer to a texture in Unity.
	// The plugin will fill texture data from native code.
#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport ("RenderingPlugin")]
#endif
#if UNITY_GLES_RENDERER
	private static extern void SetTextureFromUnity(System.IntPtr texture, int w, int h);
#else
	private static extern void SetTextureFromUnity(System.IntPtr texture);
#endif


#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("RenderingPlugin")]
#endif
	private static extern void SetUnityStreamingAssetsPath([MarshalAs(UnmanagedType.LPStr)] string path);


#if UNITY_IPHONE && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("RenderingPlugin")]
#endif
	private static extern IntPtr GetRenderEventFunc();

	private int width = 1080;
	private int height = 1080;

	private Texture2D texture;
	private float[] map_x1;
	private float[] map_y1;

	private double[] Position = new double[3];
	private double[] Rotation = new double[9];
	private Matrix4x4 W2C_matrix;

	public Boolean init_success = false;
	public UseRenderingPlugin_right render_sc_right;

	private GameObject StereoCamera;
	private GameObject ArObject;
	
	IEnumerator Start()
	{
		StereoCamera = GameObject.Find ("StereoCamera");
		ArObject = GameObject.Find ("Sphere");

		if (OpenWebCam (1, 0) && init_SLAM_AR()) {
			Debug.Log("init success!");

			//making a new texture to shader
			texture = new Texture2D (width, height, TextureFormat.RGFloat, false);
			Material mat = GetComponent<Renderer>().material;
			mat.SetTexture ("_Texture2", texture);
			
			//copying the calib params from opencv dll
			map_x1 = new float[width * height];
			map_y1 = new float[width * height];

			IntPtr ptr =  get_map_x1 ();
			Marshal.Copy (ptr, map_x1, 0, width * height);

			ptr = get_map_y1 ();
			Marshal.Copy (ptr, map_y1, 0, width * height);

//			Debug.Log("map x1 0: "+map_x1[1000]);
//			Debug.Log("map x1 1: "+map_x1[1001]);
//			Debug.Log("map x1 2: "+map_x1[2]);
//			Debug.Log("map x1 3: "+map_x1[3]);
//			Debug.Log("map y1 0: "+map_y1[1000]);
//			Debug.Log("map y1 1: "+map_y1[1001]);
//			Debug.Log("map y1 2: "+map_y1[2]);
//			Debug.Log("map y1 3: "+map_y1[3]);

			for (int i=0; i<width; ++i) {
				for(int j=0; j<height; ++j){
					Color color;
					color.r = map_x1[j*width+i]/(float)width;
					color.g = map_y1[j*width+i]/(float)height;
					//				color.r = ((float)i-0.0f)/(float)width;
					//				color.g = ((float)j-0.0f)/(float)height;
					color.b = 0.0f;
					color.a = 1.0f;
					texture.SetPixel(i,j,color);
				}
			}
			
			texture.Apply ();

			init_success = true;

			render_sc_right.enabled = true;
		} else {
			Debug.Log("init false");
		}

		SetUnityStreamingAssetsPath(Application.streamingAssetsPath);

		CreateTextureAndPassToPlugin();
		yield return StartCoroutine("CallPluginAtEndOfFrames");
	}

	private void CreateTextureAndPassToPlugin()
	{
		// Create a texture
		Texture2D tex = new Texture2D(width,height,TextureFormat.ARGB32,false);
		tex.wrapMode = TextureWrapMode.Clamp;
		// Set point filtering just so we can see the pixels clearly
		tex.filterMode = FilterMode.Bilinear;
		// Call Apply() so it's actually uploaded to the GPU
		tex.Apply();

		// Set texture onto our matrial
		GetComponent<Renderer>().material.mainTexture = tex;

		// Pass texture pointer to the plugin
	#if UNITY_GLES_RENDERER
		SetTextureFromUnity (tex.GetNativeTexturePtr(), tex.width, tex.height);
	#else
		SetTextureFromUnity (tex.GetNativeTexturePtr());
	#endif
	}


	//reform a left handed system transform matrix from right handed
	public static Matrix4x4 LHMatrixFromRHMatrix(Matrix4x4 rhm)
	{
		Matrix4x4 lhm = new Matrix4x4();;

		// Column 0.
		lhm[0, 0] =  rhm[0, 0];
		lhm[1, 0] =  rhm[1, 0];
		lhm[2, 0] = -rhm[2, 0];
		lhm[3, 0] =  rhm[3, 0];

		// Column 1.
		lhm[0, 1] =  rhm[0, 1];
		lhm[1, 1] =  rhm[1, 1];
		lhm[2, 1] = -rhm[2, 1];
		lhm[3, 1] =  rhm[3, 1];

		// Column 2.
		lhm[0, 2] = -rhm[0, 2];
		lhm[1, 2] = -rhm[1, 2];
		lhm[2, 2] =  rhm[2, 2];
		lhm[3, 2] = -rhm[3, 2];

		// Column 3.
		lhm[0, 3] =  rhm[0, 3];
		lhm[1, 3] =  rhm[1, 3];
		lhm[2, 3] = -rhm[2, 3];
		lhm[3, 3] =  rhm[3, 3];

		return lhm;
	}

	//get position from transform matrix
	public static Vector3 PositionFromMatrix(Matrix4x4 m)
	{
		return m.GetColumn(3);
	}

	//get rotation quaternion from matrix
	public static Quaternion QuaternionFromMatrix(Matrix4x4 m)
	{
		// Trap the case where the matrix passed in has an invalid rotation submatrix.
		if (m.GetColumn(2) == Vector4.zero) {
			Debug.Log("QuaternionFromMatrix got zero matrix.");
			return Quaternion.identity;
		}
		return Quaternion.LookRotation(m.GetColumn(2), m.GetColumn(1));
	}

	private IEnumerator CallPluginAtEndOfFrames()
	{
		while (true) {

//			StereoCamera.transform.Rotate(Vector3.up * 50f * Time.deltaTime);

			float time1 = Time.realtimeSinceStartup;

			// Wait until all frame rendering is done
			yield return new WaitForEndOfFrame();

			// Set time for the plugin
			SetTimeFromUnity (Time.timeSinceLevelLoad);

			// Issue a plugin event with arbitrary integer identifier.
			// The plugin can distinguish between different
			// things it needs to do based on this ID.
			// For our simple plugin, it does not matter which ID we pass here.
			GL.IssuePluginEvent(GetRenderEventFunc(), 0);

			float time2 = Time.realtimeSinceStartup;
			float interval = time2 - time1;
//			Debug.Log("time_left: "+interval*1000+"ms.");

			int tracking_quality = get_tracking_state ();
//			Debug.Log("tracking quality:  "+tracking_quality);

			if (tracking_quality != 0) {
				ArObject.SetActive (true);

				//get the translation and rotation
				IntPtr PositionPtr = get_translation ();
				Marshal.Copy (PositionPtr, Position, 0, 3);
//			Debug.Log("position 0 :"+Position[0]);
//			Debug.Log("position 1 :"+Position[1]);
//			Debug.Log("position 2 :"+Position[2]);
//
				IntPtr RotationPtr = get_rotation ();
				Marshal.Copy (RotationPtr, Rotation, 0, 9);

				W2C_matrix.m00 = (float)Rotation [0];
				W2C_matrix.m01 = (float)Rotation [1];
				W2C_matrix.m02 = (float)Rotation [2];
				W2C_matrix.m03 = (float)Position [0];
				W2C_matrix.m10 = (float)Rotation [3];
				W2C_matrix.m11 = (float)Rotation [4];
				W2C_matrix.m12 = (float)Rotation [5];
				W2C_matrix.m13 = (float)Position [1];
				W2C_matrix.m20 = (float)Rotation [6];
				W2C_matrix.m21 = (float)Rotation [7];
				W2C_matrix.m22 = (float)Rotation [8];
				W2C_matrix.m23 = (float)Position [2];
				W2C_matrix.m30 = 0;
				W2C_matrix.m31 = 0;
				W2C_matrix.m32 = 0;
				W2C_matrix.m33 = 1;

				Matrix4x4 transformationMatrix = LHMatrixFromRHMatrix (W2C_matrix);
				Matrix4x4 pose = transformationMatrix.inverse;

				Vector3 arPosition = PositionFromMatrix (pose);
				Quaternion arRotation = QuaternionFromMatrix (pose);

				StereoCamera.transform.localPosition = arPosition;
				StereoCamera.transform.localRotation = arRotation;
				StereoCamera.transform.Rotate (Vector3.up * 180f);
				StereoCamera.transform.Rotate (Vector3.forward * 180f);
			}
        }
	}

	void OnApplicationQuit()
	{
		DestroyWebCam ();
		Debug.Log("quit");
	}

	public void OnClickInit()
	{
		Debug.Log ("click!click!click!click!click!click!click!click!click!click!");
		start_slam ();
	}

	public void OnClickReset()
	{
		Debug.Log ("reset!");
		reset_slam ();
	}
}
