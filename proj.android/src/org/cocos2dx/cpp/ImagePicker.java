package org.cocos2dx.cpp;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.UUID;

import org.cocos2dx.lib.Cocos2dxHelper;

import android.R;
import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.MediaStore.Audio.Media;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.ImageView;

public class ImagePicker {
    
    public static final int NONE = 0;
    public static final int PHOTOHRAPH = 1;
    public static final int PHOTOZOOM = 2;
    public static final int PHOTORESOULT = 3;
    public static final String IMAGE_UNSPECIFIED = "image/*";
    
    private static ImagePicker instance = null;
    private static Activity activity = null;
    
    public static native void onImageSaved(String path);
    
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
        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(Environment.getExternalStorageDirectory(),"/cc_puzzles.jpg")));
        activity.startActivityForResult(intent, PHOTOHRAPH);
    }
    
    //回调
    public void onActivityResult(int requestCode,int resultCode,Intent data)
    {
        if (resultCode == NONE)
            return;
        
        // 拍照
        if (requestCode == PHOTOHRAPH)
        {
            File picture = new File(Environment.getExternalStorageDirectory(),"/cc_puzzles.jpg");
            startPhotoZoom(Uri.fromFile(picture));
        }
        
        if (data == null)
            return;
        
        // 读取相册缩放图片
        if (requestCode == PHOTOZOOM)
        {
            startPhotoZoom(data.getData());
        }
        
        // 处理结果
        if (requestCode == PHOTORESOULT)
        {
            Bundle extras = data.getExtras();
            if (extras != null) {
                Bitmap photo = extras.getParcelable("data");
                ByteArrayOutputStream stream = new ByteArrayOutputStream();
                photo.compress(Bitmap.CompressFormat.JPEG, 75, stream);
                
                // XXX/@ci_8888-8888-8888-8888.jpg
                //String path = activity.getFilesDir() + "/@ci_" + UUID.randomUUID().toString() + ".jpg";
                String fileName = UUID.randomUUID().toString() + ".jpg";
                String path = Environment.getExternalStorageDirectory()+"/" + fileName;
                saveMyBitmap(path, photo);
                
                // 通知C++层已保存图片 并返回路径
                Log.e("ImagePicker", "图片已经保存，通知c++层，");
                onImageSaved(path);
            }
        }
    }
    
    public void startPhotoZoom(Uri uri)
    {
        Intent intent = new Intent("com.android.camera.action.CROP");
        intent.setDataAndType(uri, IMAGE_UNSPECIFIED);
        intent.putExtra("crop", "true");
        intent.putExtra("aspectX", 1);
        intent.putExtra("aspectY", 1);
        intent.putExtra("outputX", 350);	//大小
        intent.putExtra("outputY", 350);
        intent.putExtra("return-data", true);
        activity.startActivityForResult(intent, PHOTORESOULT);
    }
    
    
    public void saveMyBitmap(String path,Bitmap photo)
    {
        File f = new File(path);
        try {
            f.createNewFile();
        } catch (IOException e) {
        }
        FileOutputStream fOut = null;
        try {
            fOut = new FileOutputStream(f);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        photo.compress(Bitmap.CompressFormat.JPEG, 70, fOut);
        try {
            fOut.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            fOut.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
}
