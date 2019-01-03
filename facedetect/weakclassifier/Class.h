#pragma once
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>
#include <string>
using namespace std;
using namespace cv;
struct sam {
	string filename;
	double left_eyex;
	double left_eyey;
	double right_eyex;
	double right_eyey;
};

struct feature {
	int id;
	int xmin;
	int ymin;
	int xmax;
	int ymax;
	double haar;
	double mode;
	int pos;
	double weight;
};

struct weakclassifier {
	double haar;
	int pos;
	double e;
	double weight;
};

struct strongclassifier {
	double threshold;
	double min_error;
	int xmin;
	int ymin;
	int xmax;
	int ymax;
	double mode;
	int p;
};

struct jinglian {
	int xmin;
	int ymin;
	int xmax;
	int ymax;
	double mode;
};

struct finalclassifier {
	double threshold;
	double min_error;
	int p;
	int xmin;
	int ymin;
	int xmax;
	int ymax;
	double mode;
};

struct weightx {
	int id;
	double weight;
};

class Class
{
public:
	Class();
	~Class();
};