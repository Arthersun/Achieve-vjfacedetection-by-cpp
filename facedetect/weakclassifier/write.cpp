#include "pch.h"
#include <iostream>
#include "Class.h"
#include <vector>
#include <string>
#include <fstream>	

using namespace std;
using namespace cv;

void write(string path,int xmin,int xmax,int ymin,int ymax,double mode,int pos,double haar,int id ) {

	ofstream outfile;
	outfile.open(path, ios::app);
	cout << "writing..." << id << endl;
	outfile << xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<" "<<mode<<" "<<pos<<" "<<haar<<" "<<id<<endl;
	outfile.close();
}

void writefinalclassifier(string path, int xmin, int xmax, int ymin, int ymax, double mode, double threshold, double min_error, int p)
{
	ofstream outfile1;
	outfile1.open(path, ios::app);
	cout << "writing strong classifier..." << endl;
	outfile1<< xmin << " " << xmax << " " << ymin << " " << ymax << " " << mode << " " << threshold << " " << min_error << " " << p << endl;
	outfile1.close();
}