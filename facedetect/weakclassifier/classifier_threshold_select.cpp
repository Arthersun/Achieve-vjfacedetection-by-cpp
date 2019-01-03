#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>
#include "class.h"

using namespace std;
using namespace cv;

//const int sample = 1000;  //假设1000个样本


double classifier_threshold_select(vector<weakclassifier> weak,double &min_error,int &p)
{
	double splus = 0;	//当前的积极样本总重
	double ssub = 0;	//当前的消极样本总重
	double tplus = 0;
	double tsub = 0;
	vector<weakclassifier>::iterator it = weak.begin();

	for (it = weak.begin(); it != weak.end(); it++)
	{
		if ((*it).pos == 1)
			tplus += (*it).weight;
		else
			tsub += (*it).weight;
	}

	//遍历一遍把全部误差取每个样本的误差都算出来
	for (it = weak.begin(); it!=weak.end(); it++)
	{

		if ((*it).pos == 1)
			splus += (*it).weight;
		else
			ssub += (*it).weight;
		(*it).e = min(splus + tsub - ssub, ssub + tplus - splus);
	}

	//找到最小误差对应的值，即为threshold
	double threshold;
	for (it = weak.begin(); it != weak.end(); it++)
	{
		min_error = min(min_error, (*it).e);
	}
	for (it = weak.begin(); it != weak.end(); it++)
	{
		if (min_error == (*it).e)
		{
			threshold = (*it).haar;
			break;
		}
	}
	//找到不等式极性p的正负
	int pplus=0;
	int psub=0;
	for (it = weak.begin(); it != weak.end(); it++)
	{
		if ((*it).haar < threshold)
		{
			pplus++;
		}
		else
			psub++;
	}
	if (pplus > psub)
		p = -1;
	else
		p = 1;
	cout << "该分类器最小误差为" << min_error << endl;
	cout << "threshold:" << threshold;
	return threshold;
}