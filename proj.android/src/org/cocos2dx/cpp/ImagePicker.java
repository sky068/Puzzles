package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileNotFoundException;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;

public class ImagePicker {
    
    public static final int NONE = 0;
    public static final int PHOTOHRAPH = 1;
    public static final int PHOTOZOOM = 2;
    public static final int PHOTORESOULT = 3;
    public static final String IMAGE_UNSPECIFIED = "image/*";
    
    private static ImagePicker instance = null;
    private static Activity activity = null;
    private static String TAG = "ImagePicker";
    
    public static native void onImageSaved(String path);
    
    //拍摄照片保存路径
    private static String savePath = Environment.getExternalStorageDirectory() +"/PuzzlesImg";
    private static String photoName = "";
    private static Uri imgUri = null;
    
    public static ImagePicker getInstance()
    {
        if(null == instance)
        {
            instance = new ImagePicker();
        }
        return instance;
    }
    
    //初始化
    public void init(Activity activity)
    {
        ImagePicker.activity = activity;
    }
    
    //打开相册
    static public void openPhoto()
    {
        Intent intent = new Intent(Intent.ACTION_PICK, null);
        intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, IMAGE_UNSPECIFIED);
        activity.startActivityForResult(intent, PHOTOZOOM);
    }
    
    //打开相机
    static public void openCamera()
    {
    	File destDir = new File(savePath);
    	if (!destDir.exists()) 
        {
    		destDir.mkdirs();
        }
    	photoName = "temp.jpg";
    	File file = new File(savePath + "/" + photoName);
    	imgUri = Uri.fromFile(file);
        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, imgUri);
        activity.startActivityForResult(intent, PHOTOHRAPH);
    }
    
    //回调
    public void onActivityResult(int requestCode,int resultCode,Intent data)
    {
        if (resultCode == NONE)
            return;
        
        // 拍照 不能使用data，因为没有返回是空的
        if (requestCode == PHOTOHRAPH)
        {
        	if (imgUri == null)
        	{
        		Log.e(TAG, "PHOTOHRAPH imgUri is null");
        		return;
			}
            startPhotoZoom(imgUri);
        }
        
        if (requestCode == PHOTOZOOM)
        {
        	 // 读取相册缩放图片
            if (data==null )
            {
            	Log.e(TAG, "data is null");
                return;
            }
            if (data.getData()==null) 
            {
            	Log.e(TAG, "data.getData() is null");
                return;
			}
            startPhotoZoom(data.getData());
        }
        
        // 处理结果
        if (requestCode == PHOTORESOULT)
        {
        	Bitmap bitmap = decodeUriAsBitmap(imgUri);
        	if (bitmap == null)
        	{
        		Log.e(TAG, "bitmap is null");
			}
        	Log.e("ImagePicker", "图片已经保存，通知c++层，");
        	onImageSaved(savePath + "/" + photoName);
        }
    }
    
    public void startPhotoZoom(Uri uri)
    {
    	photoName = System.currentTimeMillis() + ".jpg";
    	File file = new File(savePath,photoName);
    	imgUri = Uri.fromFile(file);
    	
        Intent intent = new Intent("com.android.camera.action.CROP");
        intent.setDataAndType(uri, IMAGE_UNSPECIFIED);
        intent.putExtra("crop", "true");
        intent.putExtra("aspectX", 1);
        intent.putExtra("aspectY", 1);
        intent.putExtra("outputX", 600);
        intent.putExtra("outputY", 600);
        intent.putExtra("return-data", false);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, imgUri);

        activity.startActivityForResult(intent, PHOTORESOULT);
    }
    
    private Bitmap decodeUriAsBitmap(Uri uri)
	{
		Bitmap bitmap = null;
		try {
			bitmap = BitmapFactory.decodeStream(activity.getContentResolver().openInputStream(uri));
		} catch (FileNotFoundException e) {
			// TODO: handle exception
			e.printStackTrace();
			return null;
		}
		
		return bitmap;
	}
    
}
