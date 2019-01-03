#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>
using namespace std;
using namespace cv;

//四种mode即四种特征
double haar_calculate(double mode, int xmax, int ymax, int xmin, int ymin,double **p)
{
			if (mode == 1)		//特征类型为a
			{
				int middlex = (xmin + xmax) / 2;
				double label_white = p[middlex][ymax] + p[xmin][ymin] - p[middlex][ymin] - p[xmin][ymax];
				double label_black = p[middlex][ymin] + p[xmax][ymax] - p[middlex][ymax] - p[xmax][ymin];
				double haar_a = label_white - label_black;
				return haar_a;
			}
			if (mode == 2)		//特征类型为b
			{
				int middley = (ymin + ymax) / 2;
				double label_white = p[xmax][ymax] + p[xmin][middley] - p[xmax][middley] - p[xmin][ymax];
				double label_black = p[xmin][ymin] + p[xmax][middley] - p[xmin][middley] - p[xmax][ymin];
				double haar_b = label_white - label_black;
				return haar_b;
			}
			if (mode == 3)		//特征类型为c
			{
				int middlex1 = xmin*2/3 + xmax / 3;
				int middlex2 = xmin / 3 + xmax * 2 / 3;
				double label_white = p[middlex1][ymax] + p[xmin][ymin] - p[middlex1][ymin] - p[xmin][ymax] 
								   + p[xmax][ymax]+ p[middlex2][ymin] - p[xmax][ymin] - p[middlex2][ymax];
				double label_black = p[middlex1][ymin] + p[middlex2][ymax] - p[middlex1][ymax] - p[middlex2][ymin];
				double haar_c = label_white - label_black;
				return haar_c;
			}
			if (mode == 4)		//特征类型为d
			{
				int middlex = (xmin + xmax) / 2;
				int middley = (ymin + ymax) / 2;
				double label_white = p[middlex][middley] + p[xmin][ymin] - p[xmin][middley] - p[middlex][ymin]
								   + p[xmax][ymax] + p[middlex][middley] - p[middlex][ymax] - p[xmax][middley];
				double label_black = p[middlex][ymin] + p[xmax][middley] - p[xmax][ymin] - p[middlex][middley]
								   + p[xmin][middley] + p[middlex][ymax] - p[middlex][middley] - p[xmin][ymax];
				double haar_d = label_white - label_black;
				return haar_d;
			}
	return 0;
}