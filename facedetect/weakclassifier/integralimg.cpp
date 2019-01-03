
#include "Class.h"
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv; 

double**  face_integral_img1(String filename)
{
	Mat img = imread(filename);
	Mat image_gray;
	cvtColor(img, image_gray, CV_BGR2GRAY);//转为灰度图
	
	//把训练图片变为24*24的图像，方便积分法
	Mat dst = Mat::zeros(24, 24, CV_8UC1);
	resize(image_gray, dst, dst.size());


	double **p;		//定义二维数组
	p = (double **)malloc(sizeof(double *)*24);
	for (int i = 0; i < 24; i++)
		p[i] = (double *)malloc(sizeof(double)*24);


	for (int x = 0; x < 24; x++)
	{
		for (int y = 0; y < 24; y++)
		{
			Scalar intensity = dst.at<uchar>(x, y);
			double point = intensity.val[0];
			if (x == 0 && y == 0)
			{
				p[x][y] = point;
				//cout << p[x][y] << " ";
				continue;
			}
			if (x == 0)
				p[x][y] = p[x][y - 1] + point;
			else if (y == 0)
				p[x][y] = p[x - 1][y] + point;
			else
				p[x][y] = p[x][y - 1] + p[x - 1][y] - p[x - 1][y - 1] + point;
			//cout << p[x][y] << " ";
		}
		//cout << endl;
	}
	
	return p;
}