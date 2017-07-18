// MyTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "CvxText.h"
#include <opencv2/opencv.hpp>

#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp> 
#include "facedetect-dll.h"

//#pragma comment(lib,"libfacedetect.lib")
#pragma comment(lib,"libfacedetect-x64.lib")

//define the buffer size. Do not change the size!
#define DETECT_BUFFER_SIZE 0x20000

using namespace std;
using namespace cv;
using namespace cv::ml;

using boost::filesystem::directory_iterator;
using boost::filesystem::path;

static void recursive_dir(const path& dir, std::vector<path>& files)
{
	directory_iterator end;
	for (directory_iterator pos(dir); pos != end; ++pos)
	{
		if (is_directory(*pos))
		{
			recursive_dir(*pos, files);
		}
		else
		{
			string ext = boost::filesystem::extension(*pos);
			transform(ext.begin(), ext.end(), ext.begin(), tolower);
			if (boost::filesystem::is_regular_file(*pos) && (ext == ".jpg" || ext == ".png" || ext == ".bmp"))
			{
				files.push_back(pos->path());
			}
		}
	}
}

void MNIST2JPG()
{
	string train_test_image[2] = { "./mnist_lmdb/train-images.idx3-ubyte", "./mnist_lmdb/t10k-images.idx3-ubyte" };
	string train_test_label[2] = { "./mnist_lmdb/train-labels.idx1-ubyte","./mnist_lmdb/t10k-labels.idx1-ubyte" };
	int label_num[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	string dir[2] = { "./train/","./test/" };
	for (int iter = 0; iter < 2; iter++)
	{
		//读取label  
		ifstream fin_label(train_test_label[iter], ios::binary);
		vector<int> label;

		int magic_number;
		fin_label.read((char *)(&magic_number), sizeof(magic_number));
		int number_items;
		fin_label.read((char *)(&number_items), sizeof(number_items));
		while (!fin_label.eof())
		{
			char label_tmp;
			fin_label.read((char *)&label_tmp, sizeof(label_tmp));
			label.push_back(label_tmp);
			//std::cout << label_tmp << std::endl;
		}		
		//读取图片  
		vector<Mat> image;
		int width = 28, height = 28;
		ifstream fin_image(train_test_image[iter], ios::binary);
		int magic_number1;
		fin_image.read((char *)(&magic_number1), sizeof(magic_number1));
		int number_images;
		fin_image.read((char *)(&number_images), sizeof(number_images));
		int num_rows;
		fin_image.read((char *)(&num_rows), sizeof(num_rows));
		int num_columns;
		fin_image.read((char *)(&num_columns), sizeof(num_columns));
		while (!fin_image.eof())
		{
			unsigned char tmp;
			Mat image_tmp(width, height, CV_8UC1);
			for (int r = 0; r < image_tmp.rows; r++)
			{
				for (int c = 0; c < image_tmp.cols; c++)
				{
					fin_image.read((char *)&tmp, sizeof(tmp));
					image_tmp.at<uchar>(r, c) = tmp;
				}
			}
			image.push_back(image_tmp);

		}

		for (int i = 0; i < label.size(); i++)
		{
			char clabel[10];
			sprintf_s(clabel, "%d", label[i]);
			string slabel = clabel;
			char clabel_num[10];
			sprintf_s(clabel_num, "%d", label_num[label[i]]);
			string slabel_num = clabel_num;

			string name = dir[iter] + slabel + "_" + slabel_num + ".jpg";
			imwrite(name, image[i]);
			label_num[label[i]]++;
			cout << name << endl;
		}

	}
}

Mat srcimage;
Point previousPoint;

static void on_Mouse(int event, int x, int y, int flags, void*)
{

	if (event == EVENT_LBUTTONUP || !(flags&EVENT_FLAG_LBUTTON))
	{
		previousPoint = Point(-1, -1);
	}
	else
		if (event == EVENT_LBUTTONDOWN)
		{
			previousPoint = Point(x, y);
		}
		else if (event == EVENT_MOUSEMOVE || (flags&EVENT_FLAG_LBUTTON))
		{
			Point pt(x, y);
			if (previousPoint.x<0)
			{
				previousPoint = pt;
			}
			line(srcimage, previousPoint, pt, Scalar(255), 16, 8, 0);
			previousPoint = pt;
			imshow("result", srcimage);
		}

}
#if 1
void CreateOCRSamples()
{
	//Mat3b srcImage(32, 32, Vec3b(0, 0, 0));
	Mat srcImage = imread("vcartest/template.png");
	//imshow("Test", srcImage);
	//waitKey(0);

	CvxText text("vcartest/fonts/msyhbd.ttf");
	float transparent = 1;

	CvScalar size(31, 1, 1, 0);   //设置字体的大小/空白比例/间隔比较/旋转角度  
	text.setFont(NULL, &size, NULL, &transparent);
	IplImage* img = &(IplImage)srcImage;

	int txtPosX = 1;
	int txtPosY = 30;

	string showText = "大";
	text.putText(img, showText.c_str(), cv::Point(txtPosX, txtPosY), CV_RGB(255, 255, 255));
	cv::cvtColor(srcImage, srcImage, CV_BGR2GRAY);
	imwrite("vcartest/test.jpg", srcImage);
	imshow("Test", srcImage);
	waitKey(0);
}
#endif

void cutLogoFromHK()
{
	string filelist = "files.txt";
	string image_path;
	ifstream fin(filelist);
	int icounter = 0;
	cout << filelist << endl;
	while (fin >> image_path)
	{
		if (image_path.length() < 40 || image_path.find("_") == -1) {
			continue;
		}
		string file_name = image_path.substr(image_path.find_last_of("/\\") + 1);
		int ipos1 = file_name.find("_");
		int ipos2 = file_name.find("_", ipos1 + 1);
		string strplate = file_name.substr(ipos1 + 1, ipos2 - ipos1-1);
		cout << strplate << endl;
		int ichars = strplate.length() + 2;
		if (strplate.compare("无车牌") == 0) {
			ichars = strplate.length();
			//cout << "NoPlate  " << ichars << endl;
		}
		//cout << "Plate  " << ichars << endl;

		int iwid = 360;
		Rect logoRect = Rect(16*(ichars+14), 0, iwid, 32);

		Mat vcarImg = imread(image_path);
		Mat roiImg = vcarImg(logoRect);

		icounter++;

		for (int i = 0; i < 10; i++) {
			char bufstr[256];
			Rect subRect = Rect(32 * i, 0, 32, 32);
			Mat roiImgSub = roiImg(subRect);
			cv::Mat imBintestRoi;
			cv::Mat imGraytestRoi;
			cv::cvtColor(roiImgSub, imGraytestRoi, CV_BGR2GRAY);
			cv::threshold(imGraytestRoi, imBintestRoi, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			int iNon = countNonZero(imBintestRoi);
			//cout << iNon << endl;
			if (iNon < 20) {
				if (i > 1) {
					Mat roiLogoImg = roiImg(Rect(0, 0, 32 * i, 32));
					sprintf(bufstr, ".\\vcartest\\out\\A_%d.jpg", icounter);
					imwrite(bufstr, roiLogoImg);
				}
				break;
			}
			
			sprintf(bufstr, ".\\vcartest\\out\\C_%d_%d.jpg", icounter, i);
			imwrite(bufstr, roiImgSub);
			//imshow("Test1", imBintestRoi);
			//imshow("Test", roiImgSub);
			//waitKey(0);
		}
	}
}
#if 0
void prepareSampleLabel() {
	//Process image to extract contour
	Mat thr, gray, con;
	Mat src = imread("logoall.png", 1);
	cvtColor(src, gray, CV_BGR2GRAY);
	threshold(gray, thr, 200, 255, THRESH_BINARY); //Threshold to find contour
	thr.copyTo(con);

	// Create sample and label data
	vector< vector <Point> > contours; // Vector for storing contour
	vector< Vec4i > hierarchy;
	Mat sample;
	Mat response_array;
	findContours(con, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Find contour

	for (int i = 0; i< contours.size(); i = hierarchy[i][0]) // iterate through first hierarchy level contours
	{
		Rect r = boundingRect(contours[i]); //Find bounding rect for each contour
		rectangle(src, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(0, 0, 255), 2, 8, 0);
		Mat ROI = thr(r); //Crop the image
		Mat tmp1, tmp2;
		resize(ROI, tmp1, Size(10, 10), 0, 0, INTER_LINEAR); //resize to 10X10
		tmp1.convertTo(tmp2, CV_32FC1); //convert to float
		sample.push_back(tmp2.reshape(1, 1)); // Store  sample data
		imshow("src", src);
		int c = waitKey(0); // Read corresponding label for contour from keyoard
		c -= 0x30;     // Convert ascii to intiger value
		response_array.push_back(c); // Store label to a mat
		rectangle(src, Point(r.x, r.y), Point(r.x + r.width, r.y + r.height), Scalar(0, 255, 0), 2, 8, 0);
	}

	// Store the data to file
	Mat response, tmp;
	tmp = response_array.reshape(1, 1); //make continuous
	tmp.convertTo(response, CV_32FC1); // Convert  to float

	FileStorage Data("TrainingData.yml", FileStorage::WRITE); // Store the sample data in a file
	Data << "data" << sample;
	Data.release();

	FileStorage Label("LabelData.yml", FileStorage::WRITE); // Store the label data in a file
	Label << "label" << response;
	Label.release();
	cout << "Training and Label data created successfully....!! " << endl;

	imshow("src", src);
	waitKey();
}

void trainAndTest() {
	Mat thr, gray, con;
	Mat src = imread("dig.png", 1);
	cvtColor(src, gray, CV_BGR2GRAY);
	threshold(gray, thr, 200, 255, THRESH_BINARY_INV); // Threshold to create input
	thr.copyTo(con);


	// Read stored sample and label for training
	Mat sample;
	Mat response, tmp;
	FileStorage Data("TrainingData.yml", FileStorage::READ); // Read traing data to a Mat
	Data["data"] >> sample;
	Data.release();

	FileStorage Label("LabelData.yml", FileStorage::READ); // Read label data to a Mat
	Label["label"] >> response;
	Label.release();


	
	Ptr<TrainData> tdata = prepare_train_data(sample, response, ntrain_samples);
	Ptr<KNearest> model = KNearest::create();
	model->setDefaultK(K);
	model->setIsClassifier(true);
	model->train(tdata);

	KNearest knn;
	knn.train(sample, response); // Train with sample and responses
	cout << "Training compleated.....!!" << endl;

	vector< vector <Point> > contours; // Vector for storing contour
	vector< Vec4i > hierarchy;

	//Create input sample by contour finding and cropping
	findContours(con, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	Mat dst(src.rows, src.cols, CV_8UC3, Scalar::all(0));

	for (int i = 0; i< contours.size(); i = hierarchy[i][0]) // iterate through each contour for first hierarchy level .
	{
		Rect r = boundingRect(contours[i]);
		Mat ROI = thr(r);
		Mat tmp1, tmp2;
		resize(ROI, tmp1, Size(10, 10), 0, 0, INTER_LINEAR);
		tmp1.convertTo(tmp2, CV_32FC1);
		float p = knn.find_nearest(tmp2.reshape(1, 1), 1);
		char name[4];
		sprintf(name, "%d", (int)p);
		putText(dst, name, Point(r.x, r.y + r.height), 0, 1, Scalar(0, 255, 0), 2, 8);
	}

	imshow("src", src);
	imshow("dst", dst);
	imwrite("dest.jpg", dst);
	waitKey();
}
#endif

// 读取文件数据
bool read_num_class_data(const string& filename, int var_count, Mat* _data, Mat* _responses)
{
	const int M = 1024;
	char buf[M + 2];

	Mat el_ptr(1, var_count, CV_32F);
	int i;
	vector<int> responses;

	_data->release();
	_responses->release();
	FILE *f;
	fopen_s(&f, filename.c_str(), "rt");
	if (!f)
	{
		cout << "Could not read the database " << filename << endl;
		return false;
	}

	for (;;)
	{
		char* ptr;
		if (!fgets(buf, M, f) || !strchr(buf, ','))
			break;
		responses.push_back((int)buf[0]);
		ptr = buf + 2;
		for (i = 0; i < var_count; i++)
		{
			int n = 0;
			sscanf_s(ptr, "%f%n", &el_ptr.at<float>(i), &n);
			ptr += n + 1;
		}
		if (i < var_count)
			break;
		_data->push_back(el_ptr);
	}
	fclose(f);
	Mat(responses).copyTo(*_responses);
	return true;
}

//准备训练数据
Ptr<TrainData> prepare_train_data(const Mat& data, const Mat& responses, int ntrain_samples)
{
	Mat sample_idx = Mat::zeros(1, data.rows, CV_8U);
	Mat train_samples = sample_idx.colRange(0, ntrain_samples);
	train_samples.setTo(Scalar::all(1));

	int nvars = data.cols;
	Mat var_type(nvars + 1, 1, CV_8U);
	var_type.setTo(Scalar::all(VAR_ORDERED));
	var_type.at<uchar>(nvars) = VAR_CATEGORICAL;

	return TrainData::create(data, ROW_SAMPLE, responses,
		noArray(), sample_idx, noArray(), var_type);
}
//分类预测
void test_and_save_classifier(const Ptr<StatModel>& model, const Mat& data, const Mat& responses,
	int ntrain_samples, int rdelta)
{
	int i, nsamples_all = data.rows;
	double train_hr = 0, test_hr = 0;

	// compute prediction error on train and test data
	for (i = 0; i < nsamples_all; i++)
	{
		Mat sample = data.row(i);

		float r = model->predict(sample);
		r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;

		if (i < ntrain_samples)
			train_hr += r;
		else
			test_hr += r;
	}

	test_hr /= nsamples_all - ntrain_samples;
	train_hr = ntrain_samples > 0 ? train_hr / ntrain_samples : 1.;

	printf("Recognition rate: train = %.1f%%, test = %.1f%%\n",
		train_hr*100., test_hr*100.);
}

//K最近邻分类
bool build_knearest_classifier(const string& data_filename, int K)
{
	Mat data;
	Mat responses;
	read_num_class_data(data_filename, 16, &data, &responses);
	int nsamples_all = data.rows;
	int ntrain_samples = (int)(nsamples_all*0.8);

	Ptr<TrainData> tdata = prepare_train_data(data, responses, ntrain_samples);
	Ptr<KNearest> model = KNearest::create();
	model->setDefaultK(K);
	model->setIsClassifier(true);
	model->train(tdata);

	test_and_save_classifier(model, data, responses, ntrain_samples, 0);
	return true;
}

//svm分类
bool build_svm_classifier(const string& data_filename)
{
	Mat data;
	Mat responses;
	read_num_class_data(data_filename, 16, &data, &responses);

	int nsamples_all = data.rows;
	int ntrain_samples = (int)(nsamples_all*0.8);

	Ptr<SVM> model;
	Ptr<TrainData> tdata = prepare_train_data(data, responses, ntrain_samples);
	model = SVM::create();
	model->setType(SVM::C_SVC);
	model->setKernel(SVM::LINEAR);
	model->setC(1);
	model->train(tdata);

	test_and_save_classifier(model, data, responses, ntrain_samples, 0);
	return true;
}

int hanziSize = 28;

void _knn_train_test(const string& trainimages, const string& testimages)
{
	int num = 10296;
	int size = hanziSize * hanziSize;
	Mat trainData = Mat(Size(size, num), CV_32FC1);
	Mat responces = Mat(Size(1, num), CV_32FC1);

	int ipos1;
	int ipos2;
	int lab = 0;
	string slab;
	string imagefile;
	ifstream fin(trainimages);
	int counter = 0;
	while (fin >> imagefile)
	{
		ipos1 = imagefile.find_last_of("\\");
		ipos2 = imagefile.find("_", ipos1+1);
		slab = imagefile.substr(ipos1 + 1, ipos2 - ipos1 - 1);
		lab = atoi(slab.c_str());
		//cout << "Lab: " << slab<<","<<lab << endl;

		Mat mat = imread(imagefile, 0);
		resize(mat, mat, Size(hanziSize, hanziSize));
		mat.convertTo(mat, CV_32FC1);
		mat = mat.reshape(1, 1);
		for (int k = 0; k<size; k++)
		{
			trainData.at<float>(counter*size + k) = mat.at<float>(k);
		}
		responces.at<float>(counter) = lab;
		counter++;
	}

	Ptr<TrainData> trainingData;
	Ptr<KNearest> kclassifier = KNearest::create();

	trainingData = TrainData::create(trainData, SampleTypes::ROW_SAMPLE, responces);

	kclassifier->setIsClassifier(true);
	kclassifier->setAlgorithmType(KNearest::Types::BRUTE_FORCE);
	kclassifier->setDefaultK(10);

	kclassifier->train(trainingData);

	counter = 0;
	string imagefile2;
	ifstream fin2(testimages);

	while (fin2 >> imagefile2)
	{
		ipos1 = imagefile2.find_last_of("\\");
		ipos2 = imagefile2.find("_", ipos1 + 1);
		slab = imagefile2.substr(ipos1 + 1, ipos2 - ipos1 - 1);
		lab = atoi(slab.c_str());
		cout << "Lab: " << slab << "," << lab << endl;

		Mat matTest = imread(imagefile2, 0);
		resize(matTest, matTest, Size(hanziSize, hanziSize));
		matTest.convertTo(matTest, CV_32FC1);
		matTest = matTest.reshape(1, 1);
		Mat matResults;
		kclassifier->findNearest(matTest, kclassifier->getDefaultK(), matResults);

		counter++;
	}

	

	//Just checking the settings
	cout << "Training data: " << endl
		<< "getNSamples\t" << trainingData->getNSamples() << endl
		<< "getSamples\n" << trainingData->getSamples() << endl
		<< endl;

	cout << "Classifier :" << endl
		<< "kclassifier->getDefaultK(): " << kclassifier->getDefaultK() << endl
		<< "kclassifier->getIsClassifier()   : " << kclassifier->getIsClassifier() << endl
		<< "kclassifier->getAlgorithmType(): " << kclassifier->getAlgorithmType() << endl
		<< endl;

	//knn.train(trainData, responces);
}

int main(int argc, char** argv)
{
#if 1
	vector<path> imageFiles;
	recursive_dir(argv[1], imageFiles);
	cout << "Files: " << imageFiles.size() << endl;
	string imagePath;
	Mat tempimg = cv::imread("C:\\vcode\\codeimage\\temp.png");
	Mat imageROI = tempimg(cv::Rect(4, 0, 20, 28));

	for (int i = 0; i < imageFiles.size(); i++) {
		imagePath = imageFiles[i].string();
		cout << imagePath << endl;
		Mat imgsrc = cv::imread(imagePath);
		imgsrc(Rect(0, 0, 20, 28)).copyTo(imageROI);
		cv::Mat imBintestRoi;
		cv::Mat imGraytestRoi;
		cv::cvtColor(tempimg, imGraytestRoi, CV_BGR2GRAY);
		cv::threshold(imGraytestRoi, imBintestRoi, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
		//cv::resize(imBintestRoi, imBintestRoi, cv::Size(24, 24), (0, 0), (0, 0), cv::INTER_LINEAR);
		cv::imwrite(imagePath, imBintestRoi);
		imshow("Out", imBintestRoi);
		char key = (char)waitKey(10);
		// Exit this loop on escape:
		if (key == 27)
			break;
	}
#endif

#if 0
	bool isCam = false;
	if (argc == 3) {
		cout << "Load from Camera." << endl;
		isCam = true;
	}
	else {
		cout << "Load from files" << endl;
	}

	vector<path> imageFiles;
	VideoCapture cap;
	if (isCam) {
		int camId = atoi(argv[2]);
		cap.open(camId);
		// Check if we can use this device at all:
		if (!cap.isOpened()) {
			cerr << "Capture Device ID " << camId << "cannot be opened." << endl;
			return -1;
		}
	}
	else {
		string imagesFolder = argv[1];
		boost::filesystem::path dir(imagesFolder);
		if (!exists(dir) || !boost::filesystem::is_directory(dir))
		{
			cout << "workdir is incorrect." << endl;
			return 0;
		}
		recursive_dir(dir, imageFiles);
		cout << "Files: " << imageFiles.size() << endl;
	}

	int * pResults = NULL;
	//pBuffer is used in the detection functions.
	//If you call functions in multiple threads, please create one buffer for each thread!
	unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
	if (!pBuffer)
	{
		fprintf(stderr, "Can not alloc buffer.\n");
		return -1;
	}
	int doLandmark = 1;
	int n = 0;
	string imagePath;
	for (;;)
	{
		try {
			Mat img;
			if (isCam) {
				Mat frame;
				cap >> frame;
				// Clone the current frame:
				img = frame.clone();
			}
			else {
				imagePath = imageFiles[n].string();
				n++;
				Mat imgsrc = cv::imread(imagePath);
				//cv::resize(imgsrc, img, cv::Size(480, 640), (0, 0), (0, 0), cv::INTER_LINEAR);
				img = imgsrc;
			}
						
			Mat gray;
			cvtColor(img, gray, CV_BGR2GRAY); //转换成灰度图，因为harr特征从灰度图中提取

			///////////////////////////////////////////
			// frontal face detection / 68 landmark detection
			// it's fast, but cannot detect side view faces
			//////////////////////////////////////////
			//!!! The input image must be a gray one (single-channel)
			//!!! DO NOT RELEASE pResults !!!
			pResults = facedetect_frontal_surveillance(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows,
				(int)gray.step, 1.2f, 2, 32, 0, doLandmark);

			//printf("%d faces detected.\n", (pResults ? *pResults : 0));
			Mat result_frontal = img.clone();
			//print the detection results
			for (int i = 0; i < (pResults ? *pResults : 0); i++)
			{
				short * p = ((short*)(pResults + 1)) + 142 * i;
				int x = p[0];
				int y = p[1];
				int w = p[2];
				int h = p[3];
				int neighbors = p[4];
				int angle = p[5];
								
				if (doLandmark)
				{
					rectangle(result_frontal, Rect(x, y, w, h), Scalar(255, 255, 0), 1);
					for (int j = 0; j < 68; j++)
						circle(result_frontal, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 255));
				}
				//printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
				Mat roiImg = img(Rect(x-10, y-5, w+20, h+10)).clone();
				Mat outImg;
				resize(roiImg, outImg, Size(224, 224), 0, 0, 3);
				//Mat roiImg = img(Rect(x, y, w, h));
				char strbuf[256];
				if (!isCam) {
					sprintf(strbuf, "%s.jpg", imagePath.substr(0, imagePath.length()-4).c_str());
				}
				else {
					sprintf(strbuf, "myfaces\\1_%d_%d.jpg", n, i);
				}				
				imwrite(strbuf, outImg);
				cout << "File: " << imagePath << endl;
			}
			if (doLandmark)
			{
				imshow("Results_frontal", result_frontal);
				char key = (char)waitKey(1);
				// Exit this loop on escape:
				if (key == 27)
					break;
			}
			if (!isCam) {
				if (n >= imageFiles.size()) {
					break;
				}
			}
		}
		catch (cv::Exception& e) {
			const char* err_msg = e.what();
			//cout << "CV Error: " << err_msg << endl;
			//waitKey(2000);
		}
	}
	cout << "All Over." << endl;
#endif


#if 0
	_knn_train_test("C:\\Caffe\\caffe-windows\\examples\\ocr_chinese\\knntrain.txt", "C:\\Caffe\\caffe-windows\\examples\\ocr_chinese\\knntest.txt");
#endif

#if 0
	string data_filename = "E:/opencv/opencv/sources/samples/data/letter-recognition.data";  //字母数据
	cout << "K最近邻分类：" << endl;
	build_knearest_classifier(data_filename, 10);
#endif

#if 0
	cutLogoFromHK();
#endif
	
#if 0
	CreateOCRSamples();
#endif

#if 0
	MNIST2JPG();
#endif

	return 0;
}