
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>
#include "Class.h"
#include <fstream>
#include <string>
#include <malloc.h>
#include <string.h>

using namespace std;
using namespace cv;


void split1(char *src, const char *separator, char **dest) {
	char *pNext;

	int count = 0;
	if (src == NULL || strlen(src) == 0)
		return;
	if (separator == NULL || strlen(separator) == 0)
		return;
	pNext = strtok(src, separator);
	while (pNext != NULL) {
		*dest++ = pNext;
		++count;
		pNext = strtok(NULL, separator);
	}
}


//c++什么玩意啊？split都没有 c语言的字符串函数真难用
vector<sam> explain_txt(String filename)
{
	ifstream in(filename);
	string line;
	vector<string> words;
	vector<sam> samp;
	sam a;
	if (in)
	{

		while(getline(in,line))
		{
			words.push_back(line);
		}
		char **dest = (char**)malloc(sizeof(10000));
		char buf[1000];
		vector<string>::iterator  p = words.begin();
		while (p != words.end())
		{
			split1(strcpy(buf, (*p).c_str()), " ", dest);
			a.filename = dest[0];
			a.left_eyex = atof(dest[1]);
			a.left_eyey = atof(dest[6]);
			a.right_eyex = atof(dest[2]);
			a.right_eyey = atof(dest[7]);
			//cout << dest[0] << " " << dest[1]<<endl;
			samp.push_back(a);
			p++;
		}
	}
	words.clear();
	return samp;
}

vector<feature> read_txt(string filename) {
	ifstream in(filename);
	string line;
	vector<string> words;
	vector<feature> fea;
	feature featu;
	if (in)
	{

		while (getline(in, line))
		{
			words.push_back(line);
		}
		char **dest = (char**)malloc(sizeof(10000));
		char buf[1000];
		vector<string>::iterator  p = words.begin();
		while (p != words.end())
		{
			split1(strcpy(buf, (*p).c_str()), " ", dest);
			featu.xmin = atoi(dest[0]);
			featu.xmax = atoi(dest[1]);
			featu.ymin = atoi(dest[2]);
			featu.ymax = atoi(dest[3]);
			featu.mode = atof(dest[4]);
			featu.pos = atoi(dest[5]);
			featu.haar = atof(dest[6]);
			featu.id = atoi(dest[7]);
			//cout << dest[0] << " " << dest[1]<<endl;
			fea.push_back(featu);
			p++;
		}
	}
	words.clear();
	return fea;
}