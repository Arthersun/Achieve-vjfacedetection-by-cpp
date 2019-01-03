# 人脸识别系统

这个识别系统来自于我大三上学期（2019）的科研项目开发，后续待完善

##### 版本详情：

1.0.0  积分图、求haar特征、求弱分类器、最后得到强分类器



## Introduction

程序使用opencv和vs2017进行开发（运行需要opencv环境，下载连接https://opencv.org/releases.html），编程语言是C++，需要进行一定的opencv环境配置（后面有介绍）。运行本程序后会得到一个存有强分类器各值的txt文档。通过查看该文档可以得到强分类器的计算公式。



## 使用说明

1.安装vs2017（低版本不确定可行）和opencv，在vs上配置环境。配置教程可见：

https://blog.csdn.net/weixin_39393712/article/details/79583274

2.运行weakclassifier.sln，编译运行即可。

3.参数调整，main.cpp最开始有一些指定目录的参数，注释说明的比较清楚了，按需调整。

4.图片保存在文件夹weakclassifier/lfw_5590中，标签保存在weakclassifier/training1.txt，标签说明为weakclassifier/readme.txt，数据集来自于Multi-Task Facial Landmark (MTFL) dataset v1.0 2014.10.28http://mmlab.ie.cuhk.edu.hk/archive/CNN_FacePoint.htm

