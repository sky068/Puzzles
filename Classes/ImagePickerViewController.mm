//
//  ImageViewController.m
//  Puzzles
//
//  Created by xujw on 15/12/9.
//
//

#import <Foundation/Foundation.h>
#import "ImagePickerViewController.h"

@interface ImagePickerViewController ()

@end

@implementation ImagePickerViewController

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewDidUnload
{
    [super viewDidUnload];
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

-(void)localPhoto
{
    NSLog(@"-(void)localPhoto");
    UIImagePickerController *picker = [[UIImagePickerController alloc]init];
    picker.delegate = self;
    picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    picker.allowsEditing = YES;
    [self presentViewController:picker animated:YES completion:^(void)
                                                                {
                                                                    NSLog(@"ImageViewController is presented");
                                                                }];
    [picker release];
}

-(void)takePhoto
{
    UIImagePickerControllerSourceType sourceType = UIImagePickerControllerSourceTypeCamera;
    if ([UIImagePickerController isSourceTypeAvailable:sourceType])
    {
        UIImagePickerController *picker = [[UIImagePickerController alloc]init];
        picker.delegate = self;
        //设置拍照后可以编辑
        picker.allowsEditing = YES;
        picker.sourceType = sourceType;
        [self presentViewController:picker animated:YES completion:^(void)
                                                                    {
                                                                    NSLog(@"ImageViewController is presented");
                                                                    }];
    }
    else
    {
        NSLog(@"模拟器无法使用相机，请在真机中调试");
    }
    NSLog(@"-(void)takePhoto");
}

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<NSString *,id> *)info
{
    NSString *type = [info objectForKey:@"UIImagePickerControllerMediaType"];
    if ([type isEqualToString:@"public.image"])
    {
        //先把图片转成NSData
        //原始图
//        UIImage *image = [info objectForKey:@"UIImagePickerControllerOriginalImage"];

        UIImage *image = [info objectForKey:@"UIImagePickerControllerEditedImage"];
        NSData *data;
        if (UIImagePNGRepresentation(image) == nil)
        {
            data = UIImageJPEGRepresentation(image, 1.0);
        }
        else
        {
            data = UIImagePNGRepresentation(image);
        }
        
        //图片保存路径
        //这里将图片放在沙盒的documents/image文件夹中
        NSString *documentsPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
        NSString *imgPath = [documentsPath stringByAppendingPathComponent:@"image"];
        
        //文件管理器
        NSFileManager *fileManager = [NSFileManager defaultManager];
        //生成唯一字符串
        NSString *uuid = [[NSUUID UUID]UUIDString];
        //生成文件名
        NSString *fileName = [NSString stringWithFormat:@"%@.png",uuid];
        
        //把刚刚由图片转成的data对象拷贝至沙盒中 并保存为xxxxx-xxxx-xxx...xxx.png
        /******保存之前最好先清空下，不然占用磁盘越来越大********/
        [fileManager removeItemAtPath:imgPath error:nil];
        /*************************************************/
        
        [fileManager createDirectoryAtPath:imgPath withIntermediateDirectories:YES attributes:nil error:nil];
        [fileManager createFileAtPath:[imgPath stringByAppendingPathComponent:fileName] contents:data attributes:nil];
        
        //得到选择后沙盒中图片的完整路径
        filePath = [[NSString alloc]initWithFormat:@"%@",[imgPath stringByAppendingPathComponent:fileName]];
        
        //关闭相册界面
        [picker dismissViewControllerAnimated:YES completion:^(void){}];
        
        //通知ImagePicker完成取图
        std::string strFilePath = [filePath UTF8String];
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("ImagePickerEvent",&strFilePath);
        
    }
}

-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    NSLog(@"您取消了照片选择");
    [picker dismissViewControllerAnimated:YES completion:^(void){}];
}

@end
















