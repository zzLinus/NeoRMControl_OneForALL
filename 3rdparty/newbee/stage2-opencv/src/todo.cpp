/*
在网上搜寻综合了很多算法，选择使用霍夫圆检测算法，去查找这些圆心，根据该圆心上模向量相交数量的多少，根据阈值进行最终判断。
首先对图像进行灰度处理和滤波器去噪声，简易化图像并且提高图像视觉质量，虽然均值滤波缺点易导致图像模糊，但在灰度图像下标靶轮廓十分明显，从而利于后续Canny的边缘检测。
HoughCircles	图像、输出的向量（圆的参数）、dp分辨率、minDist检测圆与圆最小距离，参数过小重叠圆，过大错过一些圆、minR/maxR为最小最大半径、param1是边缘检测器的上限阈值，越高检测边缘越多，param2圆心检测器在霍夫变换中使用的累加器阈值。这个参数越小，检测到的圆就越多

stage1：在实际检测中出现了标靶多环现象，增大了minDist数值和param2数值，并且设置的半径区间更小来框出标靶
发现标靶随即转动会出现椭圆形式，相对于圆的形式，并不能很好地框出每一帧旋转的标靶。会出现当标靶旋转成椭圆形不能很好地进行框取。
所以接着采用了椭圆的检测方法，但是椭圆相比较圆有五个参数，传统的Hough变换需要五维参数空间，利用椭圆的几何特征降低参数的维度。运用椭圆的几何性质，把hough参数空间减少到2维。

emm经过一系列操作,放弃了T_T霍夫变换，难崩，对于旋转起来的标靶检测一会出现一会消失，椭圆的ellipse函数在opencv4.6好像不适用，放弃此算法。

又找到一个查询图像轮廓的好方法！！！！

1、首先还是进行图像化的基础灰度、去噪声、边缘检测。
2、定义一个存放x\y坐标的二维动态数组来存储查找的轮廓
findContours这个方法来进行查找轮廓
只需要通过单次循环就可以遍历二维存储的轮廓，approx存储轮廓的近似多边形，然后计算轮廓近似多边形周长，approxPolyDP来通过点集对多边形近似，如果我设置的精度越高点集就越多。

我设置了很多点集，但是发现即便0.0000000000001和0.001 * perimeter取得的拟合效果差距不明显，仍旧是一个框住的折线图。查阅后发现，高斯滤波相比较均值滤波处理图像边缘时有更好地去噪声效果。woc，效果十分显著！！（兴奋）

在执行stage2初始发现不需要改动代码就可以实现，但是发现当随机小球通过标靶时，会导致标靶的轮廓线断开，导致轮廓识别出现问题。

查阅后发现了图像处理的膨胀与腐蚀。dilate膨胀就是在图像的边缘添加像素值，使得整体的像素值扩张，进而达到图像的膨胀效果。可以使得小球在经过标靶时，不会使得轮廓线断开，从而实现小球和标靶的独立框出。
*/

#include <opencv2/opencv.hpp>  
  
using namespace cv;
using namespace std;  
  
void solve(Mat &img) {

	//先转化成灰度图像  
	Mat grayImage;
	cvtColor(img,grayImage,COLOR_BGR2GRAY);	
	
	//均值滤波器去噪声
	Mat blurImage;
    	GaussianBlur(grayImage, blurImage, Size(13, 13), 0);
	
	//进行Canny边缘检测
	Mat edgesImage;
    	Canny(blurImage, edgesImage, 120, 50);
    	
    	//对图像进行彭泽处理
    	Mat dilatedImage;
    	dilate(edgesImage, dilatedImage, Mat());


    	// 查找轮廓
    	vector<vector<Point>> contours;
   	findContours(dilatedImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    	// 遍历轮廓使得轮廓逼近
    	for (size_t i = 0; i < contours.size(); i++) {
        	vector<Point> approx;
        	double perimeter = arcLength(contours[i], true);
        	approxPolyDP(contours[i], approx, 0.001 * perimeter, true);

        drawContours(img, vector<vector<Point>>{approx}, -1, Scalar(0, 0, 255), 2);
    }

}












/*
void solve(Mat &img) {

	//先转化成灰度图像  
	Mat grayImage;
	cvtColor(img,grayImage,COLOR_BGR2GRAY);	
	
	//滤波器去噪声
	Mat blurImage;
	blur(grayImage,blurImage,Size(10,10));
	
	//Canny边缘检测
	Mat edgesImage;
    	Canny(blurImage, edgesImage, 100, 50);

    	//HoughCircles检测圆
	vector<Vec3f> circles;
	int minDist = 30; 
    	int param1 = 100;     
    	int param2 = 50;
    	int minRadius = 10; 	 // 最小半径
    	int maxRadius = 100;	// 最大半径
    	HoughCircles(blurImage, circles, HOUGH_GRADIENT, 1, minDist,  param1, param2, minRadius, maxRadius);

	//HoughCircles绘制空心圆
    	for (size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(img, center, radius, Scalar(0, 0, 255),3); 
    }

}
*/
















