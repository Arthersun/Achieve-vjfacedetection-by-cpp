#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/calib3d/calib3d.hpp>
#include "integralimg.cpp"
#include "haar_calculate.cpp"
#include "Class.h"
#include "explain_txt.cpp"
#include <vector>
#include <string>
#include "write.cpp"
#include <algorithm>
#include "classifier_threshold_select.cpp"

//调整得到求解强分类器的迭代次数,例如，5表示最后得到的强分类器有5行,迭代次数要小于文件数目
const int itertime = 5;			

//调整训练与否，训练的输出文档是filename,重新导入图片或数据需要把他置为1，得到filename文档后把他置为0，可以直接读取文档节省时间。
const int trainornot = 0;		

//总体的feature输出路径
const char * filename = "1.txt";

//精炼后的feature输出路径
const char * fifeafilename = "2.txt";

//样本数据文档的存储路径
const String path = "training1.txt";

//设定开始的点，因为最开始的点基本上都不是有效变量，所以算了也没用，而且很浪费时间，这里默认是8，就是从第八个点开始算
//可以根据实际需要进行调整
const int startx = 8;
const int starty = 8;

//输出文档，存强分类器的文档
const char* printfilename = "3.txt";


using namespace std;
using namespace cv;

vector<feature> fea;
vector<finalclassifier> fina;
vector<weightx> wei;
vector<feature> fifea;


//savefeature函数用于把各个值存入高级数组
void savefeature(int x, int y, int x1, int y1, double haar, double mode, int pos,int id) {
	feature fe;
	fe.pos = pos;
	fe.haar = haar;
	fe.mode = mode;
	fe.xmax = x1;
	fe.xmin = x;
	fe.ymax = y1;
	fe.ymin = y;
	fe.id = id;
	std::cout << "已存入数组" << fe.mode << " " << fe.xmin << " " << fe.xmax << " " << fe.ymin << " " << fe.ymax << " " << fe.haar << " " << fe.pos<<" "<<fe.id << endl;
	fea.push_back(fe);
}

//判断是否是眼睛
int judge_ture(int x, int y, int x1, int y1, double lx, double ly, double rx, double ry)
{
	double m = 0.096;
	int pos;
	if (x >= lx * m - 1 && x1 <= rx * m&& y >= ly * m - 1 && y1 <= ry * m)
		pos = 1;
	else
		pos = 0;
	return pos;
}

//把弱分类器降序排序
bool SortByhaar(const weakclassifier &d1, const weakclassifier &d2)//容器的比较函数
{
	return (d1.haar > d2.haar);//降序排列   
}

//同步两个高级数组的权重
void syncweight()
{
	vector<weightx>::iterator m;
	vector<feature>::iterator itre;
	double weightsum = 0;
	for (m = wei.begin(); m != wei.end(); m++)
	{
		for (itre = fifea.begin(); itre != fifea.end(); itre++)
		{
			if ((*m).id == (*itre).id)
				(*itre).weight=(*m).weight;
		}
	}
	cout << "同步权重成功!" << endl;
}

//更新权重
void updateweight()
{
	vector<feature>::iterator itre;
	vector<finalclassifier>::iterator fitre;
	vector<weightx>::iterator weix;
	vector<int> m;
	vector<int>::iterator me;
	double hx;
	fitre = fina.end()-1;
	int poss;
	for (itre = fifea.begin(); itre != fifea.end(); itre++)
	{
		if ((*itre).xmin == (*fitre).xmin && (*itre).xmax == (*fitre).xmax && (*itre).ymin == (*fitre).ymin && (*itre).ymax == (*fitre).ymax && (*itre).mode == (*fitre).mode)
		{
			if ((*itre).haar * (*fitre).p < (*fitre).p * (*fitre).threshold)	//弱分类器公式，if pf(x)<p*threshold,hx=1; else hx = 0;
				poss = 1;
			else
				poss = 0;
			if (poss == (*itre).pos)			//如果预测正确，权重减小；否则不变
			{									//注意这个权重是样本的权重。不是该特征的权重
				bool flag=true;
				if (m.begin() == m.end())
				{
					m.push_back((*itre).id);
				}
				else {
					for (me = m.begin(); me != m.end(); me++)
					{
						if ((*itre).id == (*me))
							flag = false;
					}
					if (flag)
						m.push_back((*itre).id);
				}
			}
		}
	}

	for (weix = wei.begin();weix!=wei.end();weix++)
	{
		for(me=m.begin();me!=m.end();me++)			//只要有错就改变整个样本的权重
			if ((*weix).id == (*me))
			{
				(*weix).weight = (*weix).weight * (*fitre).min_error / (1 - (*fitre).min_error);
				cout << "样本权重改变为" << (*weix).weight << endl;
			}
	}
}

void train()
{
	vector<sam> sample_list;
	vector<sam>::iterator it = sample_list.begin();
	sample_list = explain_txt(path);

	//从txt文件中解释到的文件名，然后用文件名寻址一下
	//获取像素点，然后构成积分图；
	//使用积分图进行特征计算

	int id = 0;
	for (it = sample_list.begin(); it != sample_list.end(); it++)
	{
		string filename = (*it).filename;
		sam face;
		double **p;		//定义二维数组

		p = face_integral_img1(filename);		//返回积分图

		//比如说做一个眼睛和面颊的分类器,眼睛面颊的分类器类型为haar_b
		//(xmin,ymin)=(8,10),(xmax,ymax) = (17,14)
		//把积分图传进去，就能计算出haar值


		int pos;

		weightx idx;
		for (int x = startx; x < 23; x++)
			for (int y = starty; y < 23; y++)
				for (int x1 = x + 1; x1 < 24; x1++)
				{
					for (int y1 = x + 1; y1 < 24; y1++)
					{
						//给是否是真赋值；
						pos = judge_ture(x, y, x1, y1, (*it).left_eyex, (*it).left_eyey, (*it).right_eyex, (*it).right_eyey);

						//if (pos == 1)
						//	cout << filename << " " << x << " " << y << " " << x1 << " " << y1 << " " << endl;

						idx.id = id;
						wei.push_back(idx);

						double middlex = (x1 - x) % 2;
						double middley = (y1 - y) % 2;

						//下面判断的目的是在积分图中小数不能获取值
						if (middlex > 0 && middley > 0)
						{
							continue;
						}
						else if (middlex > 0 && (x1 - x) % 3 != 0)
						{
							double haar2 = haar_calculate(2, x1, y1, x, y, p);
							savefeature(x, y, x1, y1, haar2, 2, pos,id);
						}
						else if ((x1 - x) % 3 == 0)
						{
							double haar3 = haar_calculate(3, x1, y1, x, y, p);
							savefeature(x, y, x1, y1, haar3, 3, pos,id);
							double haar2 = haar_calculate(2, x1, y1, x, y, p);
							savefeature(x, y, x1, y1, haar2, 2, pos,id);
						}
						else if (middley > 0)
						{
							double haar1 = haar_calculate(1, x1, y1, x, y, p);
							savefeature(x, y, x1, y1, haar1, 2, pos,id);
							double haar3 = haar_calculate(3, x1, y1, x, y, p);
							savefeature(x, y, x1, y1, haar3, 3, pos,id);
						}
						else if (middley == 0 && middlex == 0)
						{
							double haar1 = haar_calculate(1, x1, y1, x, y, p);
							savefeature(x, y, x1, y1, haar1, 1, pos,id);
							double haar2 = haar_calculate(2, x1, y1, x, y, p);
							savefeature(x, y, x1, y1, haar2, 2, pos,id);
							double haar4 = haar_calculate(4, x1, y1, x, y, p);
							savefeature(x, y, x1, y1, haar4, 4, pos,id);
						}


					}
				}
		id++;
		//一定要释放数组内存，不然的话内存泄露
		for (int i = 0; i < 24; i++)
			std::free(p[i]);
		std::free(p);
	}

	//删除"1.txt"文件，写高级数组到文件中
	if (remove(filename) == 0)
		cout << "删除"<<filename<<"文件成功" << endl;
	vector<feature>::iterator itr;
	for (itr = fea.begin(); itr != fea.end(); itr++)
		write(filename, (*itr).xmin, (*itr).xmax, (*itr).ymin, (*itr).ymax, (*itr).mode, (*itr).pos, (*itr).haar,(*itr).id);
}


//舍去无效特征, 改进了一下，之前的运算时间太长了，没有效果。
void tofifea()
{
	vector<feature>::iterator itr;
	vector<jinglian> jin;

	vector<jinglian>::iterator m;
for (itr = fea.begin(); itr != fea.end(); itr++)
{
	if ((*itr).pos == 1)
	{
		if (jin.begin() == jin.end())
		{
			jinglian a;
			a.xmin = (*itr).xmin;
			a.xmax = (*itr).xmax;
			a.ymin = (*itr).ymin;
			a.ymax = (*itr).ymax;
			a.mode = (*itr).mode;
			jin.push_back(a);
		}
		else {
			bool fla = true;
			for (m = jin.begin(); m != jin.end(); m++)
			{
				if ((*m).mode == (*itr).mode && (*m).xmin == (*itr).xmin && (*m).xmax == (*itr).xmax && (*m).ymin == (*itr).ymin && (*m).ymax == (*itr).ymax)
				{
					fla = false;
				}
			}
			if (fla)
			{
				jinglian a;
				a.xmin = (*itr).xmin;
				a.xmax = (*itr).xmax;
				a.ymin = (*itr).ymin;
				a.ymax = (*itr).ymax;
				a.mode = (*itr).mode;
				jin.push_back(a);
			}
		}
	}
}

for (m = jin.begin(); m != jin.end(); m++)
{
	for (itr = fea.begin(); itr != fea.end(); itr++)
	{
		if ((*m).mode == (*itr).mode && (*m).xmin == (*itr).xmin && (*m).xmax == (*itr).xmax && (*m).ymin == (*itr).ymin && (*m).ymax == (*itr).ymax)
		{
			feature mp;
			mp.haar = (*itr).haar;
			mp.id = (*itr).id;
			mp.mode = (*itr).mode;
			mp.pos = (*itr).pos;
			mp.weight = (*itr).weight;
			mp.xmax = (*itr).xmax;
			mp.xmin = (*itr).xmin;
			mp.ymax = (*itr).ymax;
			mp.ymin = (*itr).ymin;
			fifea.push_back(mp);
		}
	}
}

if(remove(fifeafilename))
	cout << "删除文件" << fifeafilename << "成功！" << endl;

cout << "开始写文件。" << endl;

for (itr = fifea.begin(); itr != fifea.end(); itr++)
{
	write(fifeafilename, (*itr).xmin, (*itr).xmax, (*itr).ymin, (*itr).ymax, (*itr).mode, (*itr).pos, (*itr).haar, (*itr).id);
}
}


int main()
{
	if (trainornot == 1)
	{
		train();	//训练
		tofifea();	//舍去无用特征，精炼一下
	}
	
	//从文件中读出
	if (trainornot == 0)
	{
		fifea = read_txt(fifeafilename);
	}

	//计算正负样本数
	vector<feature>::iterator feap;
	vector<weightx>::iterator weix;
	int l = 0;


	if (wei.size() == 0)
	{
		weightx weigh;
		for (feap = fifea.begin(); feap != fifea.end(); feap++)
		{
			if (feap == fifea.begin())
			{
				weigh.id = (*feap).id;
				wei.push_back(weigh);
			}
			else
			{
				bool flge = true;
				for (weix = wei.begin(); weix != wei.end(); weix++)
				{
					if ((*weix).id == (*feap).id)
						flge = false;
				}
				if (flge)
				{
					weigh.id = (*feap).id;
					wei.push_back(weigh);
				}
			}
		}
	}

	l = wei.size();
	std::cout << "样本数为" << l << endl;

	//初始化权重
	for (weix = wei.begin(); weix != wei.end(); weix++)
	{
		(*weix).weight = 1 / (double)l;
	}
	syncweight();
	std::cout << "初始化完成，每个样本权重为" << 1 / (double)l << endl;

	vector<strongclassifier> strong;
	strongclassifier same;
	finalclassifier finalc;

	for (int iter = 1; iter < itertime; iter++)
	{
		//从第八个像素点开始能有效提高运行速度，在我的程序中因为前几个像素点没有有效变量，我直接从第八个点开始了
		for (int x = startx; x < 23; x++)
			for (int y = starty; y < 23; y++)
				for (int x1 = x + 1; x1 < 24; x1++)
					for (int y1 = y + 1; y1 < 24; y1++)
						for (int mode = 1; mode <= 4; mode++)
						{
							vector<weakclassifier> weak;
							vector<weakclassifier>::iterator wea;

							weakclassifier a;

							for (feap = fifea.begin(); feap != fifea.end(); feap++)
							{
								if ((*feap).xmin == x && (*feap).xmax == x1 && (*feap).ymin == y && (*feap).ymax == y1 && (*feap).mode == mode)
								{
									a.haar = (*feap).haar;
									a.pos = (*feap).pos;
									a.weight = (*feap).weight;
									weak.push_back(a);
									cout << "压入弱分类器中" << endl;
								}
							}
							if (weak.size() == 0)
								continue;
							std::sort(weak.begin(), weak.end(), SortByhaar);
							double min_error = 1;
							double threshold = 1;
							int p = 1;
							threshold = classifier_threshold_select(weak, min_error, p);
							same.xmin = x;
							same.xmax = x1;
							same.ymin = y;
							same.ymax = y1;
							same.mode = mode;
							same.threshold = threshold;
							same.min_error = min_error;
							same.p = p;
							weak.clear();
							strong.push_back(same);
						}
		vector<strongclassifier>::iterator str;
		double merror = 1;
		double thisthreshold;
		for (str = strong.begin(); str != strong.end(); str++)
		{
			merror = min(merror, (*str).min_error);
		}
		for (str = strong.begin(); str != strong.end(); str++)
		{
			if (merror == (*str).min_error)
			{
				thisthreshold = (*str).threshold;
				break;
			}
		}
		std::cout << "第" << iter << "次迭代,选出的弱分类器是" << "xmin=" << (*str).xmin << ",xmax=" << (*str).xmax << "ymin=" << (*str).ymin << ",ymax=" << (*str).ymax << endl;
		std::cout << "最小误差为" << merror << ",阈值为" << thisthreshold << endl;
		finalc.min_error = merror;
		finalc.threshold = thisthreshold;
		finalc.mode = (*str).mode;
		finalc.xmin = (*str).xmin;
		finalc.xmax = (*str).xmax;
		finalc.ymin = (*str).ymin;
		finalc.ymax = (*str).ymax;
		finalc.p = (*str).p;
		fina.push_back(finalc);		//把分类器压入最后的vector中进行保存
		strong.erase(str);		//删除这个分类器特征

		//更新权重
		updateweight();

		//标准化权重
		double weightsum = 0;
		for (weix = wei.begin(); weix != wei.end(); weix++)
		{
			weightsum += (*weix).weight;
		}

		for (weix = wei.begin(); weix != wei.end(); weix++)
		{
			(*weix).weight = (*weix).weight / weightsum;
		}

		//同步权重
		syncweight();
	}
	vector<finalclassifier>::iterator finalct;
	if (remove(printfilename))
		cout << "删除文件"<<printfilename<<"成功" <<endl;
	for (finalct = fina.begin(); finalct != fina.end(); finalct++)
	{
		writefinalclassifier(printfilename, (*finalct).xmin, (*finalct).xmax, (*finalct).ymin, (*finalct).ymax, (*finalct).mode, (*finalct).threshold, (*finalct).min_error, (*finalct).p);
	}
}
	

