
#include "caffe_wrapper.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp> 

#include "ExtractFeature_.h"

using namespace CaffeUtil;

using boost::filesystem::directory_iterator;
using boost::filesystem::path;

map<int, string> lab_1;
map<string, string> lab_2;
map<string, string> lab_3;
int _Debug = 0;

void initLabelsMap()
{
	//, MG����
	lab_1.insert(pair<int, string>(0, "����"));
	lab_1.insert(map<int, string>::value_type(10, "����"));
	lab_1.insert(map<int, string>::value_type(106, "Ծ��"));
	lab_1.insert(map<int, string>::value_type(108, "�ƺ�"));
	lab_1.insert(map<int, string>::value_type(113, "��ʱ��"));
	lab_1.insert(map<int, string>::value_type(114, "ʱ������"));
	lab_1.insert(map<int, string>::value_type(115, "�ݱ�"));
	lab_1.insert(map<int, string>::value_type(121, "JEEP"));
	lab_1.insert(map<int, string>::value_type(123, "·��"));
	lab_1.insert(map<int, string>::value_type(128, "����"));
	lab_1.insert(map<int, string>::value_type(132, "��������"));
	lab_1.insert(map<int, string>::value_type(137, "MAN����"));
	lab_1.insert(map<int, string>::value_type(140, "����"));
	lab_1.insert(map<int, string>::value_type(142, "½��"));
	lab_1.insert(map<int, string>::value_type(144, "�����ͳ�"));
	lab_1.insert(map<int, string>::value_type(147, "ŷ��"));
	lab_1.insert(map<int, string>::value_type(148, "����"));
	lab_1.insert(map<int, string>::value_type(15, "�ղ�"));
	lab_1.insert(map<int, string>::value_type(158, "ک��"));
	lab_1.insert(map<int, string>::value_type(160, "����"));
	lab_1.insert(map<int, string>::value_type(166, "���ǽ�"));
	lab_1.insert(map<int, string>::value_type(168, "Ұ��"));
	lab_1.insert(map<int, string>::value_type(17, "�µ�"));
	lab_1.insert(map<int, string>::value_type(170, "����"));
	lab_1.insert(map<int, string>::value_type(174, "�ɻ���"));
	lab_1.insert(map<int, string>::value_type(177, "�ڱ�"));
	lab_1.insert(map<int, string>::value_type(178, "�ƿ�"));
	lab_1.insert(map<int, string>::value_type(179, "��������"));
	lab_1.insert(map<int, string>::value_type(186, "Ϋ��"));
	lab_1.insert(map<int, string>::value_type(187, "��ϲ"));
	lab_1.insert(map<int, string>::value_type(193, "��������"));
	lab_1.insert(map<int, string>::value_type(200, "����"));
	lab_1.insert(map<int, string>::value_type(204, "��̨"));
	lab_1.insert(map<int, string>::value_type(206, "GMC"));
	lab_1.insert(map<int, string>::value_type(209, "ĵ��"));
	lab_1.insert(map<int, string>::value_type(212, "��������"));
	lab_1.insert(map<int, string>::value_type(218, "���ֿͳ�"));
	lab_1.insert(map<int, string>::value_type(220, "����ͳ�"));
	lab_1.insert(map<int, string>::value_type(222, "�칤"));
	lab_1.insert(map<int, string>::value_type(225, "ũ�ó�"));
	lab_1.insert(map<int, string>::value_type(227, "����"));
	lab_1.insert(map<int, string>::value_type(229, "��������"));
	lab_1.insert(map<int, string>::value_type(23, "��������"));
	lab_1.insert(map<int, string>::value_type(231, "�����ͳ�"));
	lab_1.insert(map<int, string>::value_type(232, "����ͳ�"));
	lab_1.insert(map<int, string>::value_type(238, "����"));
	lab_1.insert(map<int, string>::value_type(240, "�ɶ��ͳ�"));
	lab_1.insert(map<int, string>::value_type(242, "����"));
	lab_1.insert(map<int, string>::value_type(246, "����"));
	lab_1.insert(map<int, string>::value_type(247, "����"));
	lab_1.insert(map<int, string>::value_type(248, "����"));
	lab_1.insert(map<int, string>::value_type(249, "��������"));
	lab_1.insert(map<int, string>::value_type(253, "��ɯ����"));
	lab_1.insert(map<int, string>::value_type(256, "��ۿͳ�"));
	lab_1.insert(map<int, string>::value_type(257, "ʵ���ͳ�"));
	lab_1.insert(map<int, string>::value_type(261, "����������"));
	lab_1.insert(map<int, string>::value_type(262, "̫���ͳ�"));
	lab_1.insert(map<int, string>::value_type(264, "��¡�ͳ�"));
	lab_1.insert(map<int, string>::value_type(266, "�����ͳ�"));
	lab_1.insert(map<int, string>::value_type(267, "®ɽ��"));
	lab_1.insert(map<int, string>::value_type(269, "�����"));
	lab_1.insert(map<int, string>::value_type(272, "���ֳ�"));
	lab_1.insert(map<int, string>::value_type(28, "����"));
	lab_1.insert(map<int, string>::value_type(29, "����"));
	lab_1.insert(map<int, string>::value_type(3, "��̩"));
	lab_1.insert(map<int, string>::value_type(31, "���ǿͳ�"));
	lab_1.insert(map<int, string>::value_type(32, "����"));
	lab_1.insert(map<int, string>::value_type(33, "��������"));
	lab_1.insert(map<int, string>::value_type(4, "���"));
	lab_1.insert(map<int, string>::value_type(42, "�ֶ���"));
	lab_1.insert(map<int, string>::value_type(46, "������"));
	lab_1.insert(map<int, string>::value_type(47, "�ۺ�"));
	lab_1.insert(map<int, string>::value_type(5, "����˹��"));
	lab_1.insert(map<int, string>::value_type(51, "���ǵ�"));
	lab_1.insert(map<int, string>::value_type(52, "��ľ"));
	lab_1.insert(map<int, string>::value_type(68, "ʮͨ"));
	lab_1.insert(map<int, string>::value_type(7, "���Դ�"));
	lab_1.insert(map<int, string>::value_type(71, "����"));
	lab_1.insert(map<int, string>::value_type(76, "��ά��"));
	lab_1.insert(map<int, string>::value_type(78, "����"));
	lab_1.insert(map<int, string>::value_type(81, "�ʹ�"));
	lab_1.insert(map<int, string>::value_type(84, "JMC"));
	lab_1.insert(map<int, string>::value_type(90, "����"));
	lab_1.insert(map<int, string>::value_type(92, "���Ž���"));
	lab_1.insert(map<int, string>::value_type(96, "���ݽ���"));
	lab_1.insert(map<int, string>::value_type(99, "��ͨ"));

	lab_2.insert(map<string, string>::value_type("103_223", "��������"));
	lab_2.insert(map<string, string>::value_type("103_72", "�����ؿ�"));
	lab_2.insert(map<string, string>::value_type("104_33", "��������"));
	lab_2.insert(map<string, string>::value_type("104_43", "����ɭ����"));
	lab_2.insert(map<string, string>::value_type("12_9", "����"));
	lab_2.insert(map<string, string>::value_type("129_130", "��������"));
	lab_2.insert(map<string, string>::value_type("129_275", "����"));
	lab_2.insert(map<string, string>::value_type("13_105", "��������"));
	lab_2.insert(map<string, string>::value_type("13_14", "����"));
	lab_2.insert(map<string, string>::value_type("13_18", "����"));
	lab_2.insert(map<string, string>::value_type("13_20", "����"));
	lab_2.insert(map<string, string>::value_type("13_25", "������"));
	lab_2.insert(map<string, string>::value_type("135_25", "�����ͳ�"));
	lab_2.insert(map<string, string>::value_type("135_42", "���ֿͳ�"));
	lab_2.insert(map<string, string>::value_type("180_181", "˫������"));
	lab_2.insert(map<string, string>::value_type("180_25", "˫��"));
	lab_2.insert(map<string, string>::value_type("191_75", "������Ұ"));
	lab_2.insert(map<string, string>::value_type("191_97", "�����Ʊ�"));
	lab_2.insert(map<string, string>::value_type("192_17", "�°³�"));
	lab_2.insert(map<string, string>::value_type("192_2", "�´������"));
	lab_2.insert(map<string, string>::value_type("192_243", "����;"));
	lab_2.insert(map<string, string>::value_type("192_28", "������"));
	lab_2.insert(map<string, string>::value_type("192_88", "�¿�"));
	lab_2.insert(map<string, string>::value_type("2_18", "���"));
	lab_2.insert(map<string, string>::value_type("2_211", "��������"));
	lab_2.insert(map<string, string>::value_type("2_259", "������"));
	lab_2.insert(map<string, string>::value_type("2_3", "����"));
	lab_2.insert(map<string, string>::value_type("2_99", "����ͳ�"));
	lab_2.insert(map<string, string>::value_type("21_22", "ѩ����"));
	lab_2.insert(map<string, string>::value_type("21_24", "ѩ����"));
	lab_2.insert(map<string, string>::value_type("214_215", "���޿ͳ�"));
	lab_2.insert(map<string, string>::value_type("214_23", "��������"));
	lab_2.insert(map<string, string>::value_type("217_100", "��ͨ�ͳ�"));
	lab_2.insert(map<string, string>::value_type("217_242", "��������"));
	lab_2.insert(map<string, string>::value_type("237_243", "������"));
	lab_2.insert(map<string, string>::value_type("237_42", "���ֿͳ�"));
	lab_2.insert(map<string, string>::value_type("260_168", "��Ұ����"));
	lab_2.insert(map<string, string>::value_type("260_7", "����ͳ�"));
	lab_2.insert(map<string, string>::value_type("34_181", "����"));
	lab_2.insert(map<string, string>::value_type("34_271", "���ֳ�"));
	lab_2.insert(map<string, string>::value_type("34_35", "����"));
	lab_2.insert(map<string, string>::value_type("34_74", "��һ�ع�"));
	lab_2.insert(map<string, string>::value_type("36_230", "˹������"));
	lab_2.insert(map<string, string>::value_type("36_262", "˹̫������"));
	lab_2.insert(map<string, string>::value_type("36_37", "˹�´�"));
	lab_2.insert(map<string, string>::value_type("36_60", "˹��³"));
	lab_2.insert(map<string, string>::value_type("38_17", "��������"));
	lab_2.insert(map<string, string>::value_type("38_58", "����"));
	lab_2.insert(map<string, string>::value_type("40_100", "��ͨ�ͳ�"));
	lab_2.insert(map<string, string>::value_type("40_101", "�й�����"));
	lab_2.insert(map<string, string>::value_type("40_134", "�пͻ���"));
	lab_2.insert(map<string, string>::value_type("40_145", "��˳"));
	lab_2.insert(map<string, string>::value_type("40_169", "����"));
	lab_2.insert(map<string, string>::value_type("40_2", "�д�����"));
	lab_2.insert(map<string, string>::value_type("40_41", "�л�"));
	lab_2.insert(map<string, string>::value_type("41_149", "��������"));
	lab_2.insert(map<string, string>::value_type("41_35", "��������"));
	lab_2.insert(map<string, string>::value_type("41_89", "��̩"));
	lab_2.insert(map<string, string>::value_type("44_5", "�׿���˹"));
	lab_2.insert(map<string, string>::value_type("44_83", "��ŵ"));
	lab_2.insert(map<string, string>::value_type("49_223", "������"));
	lab_2.insert(map<string, string>::value_type("49_42", "��������"));
	lab_2.insert(map<string, string>::value_type("49_50", "����"));
	lab_2.insert(map<string, string>::value_type("49_69", "����"));
	lab_2.insert(map<string, string>::value_type("54_55", "��"));
	lab_2.insert(map<string, string>::value_type("56_98", "����"));
	lab_2.insert(map<string, string>::value_type("57_136", "��������"));
	lab_2.insert(map<string, string>::value_type("57_215", "������"));
	lab_2.insert(map<string, string>::value_type("57_35", "����"));
	lab_2.insert(map<string, string>::value_type("57_68", "��ʮ��"));
	lab_2.insert(map<string, string>::value_type("58_181", "��������"));
	lab_2.insert(map<string, string>::value_type("58_69", "����"));
	lab_2.insert(map<string, string>::value_type("6_138", "����"));
	lab_2.insert(map<string, string>::value_type("6_25", "����"));
	lab_2.insert(map<string, string>::value_type("62_46", "Ӣ�����"));
	lab_2.insert(map<string, string>::value_type("62_63", "Ӣ��"));
	lab_2.insert(map<string, string>::value_type("62_9", "Ӣ��"));
	lab_2.insert(map<string, string>::value_type("64_12", "����"));
	lab_2.insert(map<string, string>::value_type("64_58", "�����ͳ�"));
	lab_2.insert(map<string, string>::value_type("64_65", "����"));
	lab_2.insert(map<string, string>::value_type("66_155", "����HAVAL"));
	lab_2.insert(map<string, string>::value_type("69_138", "�Ͽ�"));
	lab_2.insert(map<string, string>::value_type("70_184", "��Դ�ͳ�"));
	lab_2.insert(map<string, string>::value_type("70_88", "�����ͳ�"));
	lab_2.insert(map<string, string>::value_type("88_18", "��������"));
	lab_2.insert(map<string, string>::value_type("88_7", "����"));
	lab_2.insert(map<string, string>::value_type("94_234", "���Ŀͳ�"));
	lab_2.insert(map<string, string>::value_type("94_56", "�Ϻ�����"));

	lab_3.insert(map<string, string>::value_type("72_73_162", "����SMART"));
	lab_3.insert(map<string, string>::value_type("6_7_152", "����MINI"));
	lab_3.insert(map<string, string>::value_type("103_144_225", "����ũ��"));
	lab_3.insert(map<string, string>::value_type("103_144_75", "��������"));
	lab_3.insert(map<string, string>::value_type("103_144_75", "�����������쳧"));
	lab_3.insert(map<string, string>::value_type("103_75_33", "��������"));
	lab_3.insert(map<string, string>::value_type("103_75_207", "��������"));
	lab_3.insert(map<string, string>::value_type("8_9_195", "����˼��"));
	lab_3.insert(map<string, string>::value_type("13_9_114", "����ʱ��"));
	lab_3.insert(map<string, string>::value_type("66_67_182", "��������"));
	lab_3.insert(map<string, string>::value_type("56_7_143", "����֣��"));
	lab_3.insert(map<string, string>::value_type("38_39_118", "����ȫ��ӥ"));
	lab_3.insert(map<string, string>::value_type("58_59_70", "��������"));
	lab_3.insert(map<string, string>::value_type("58_59_26", "�����ִ��ͳ�"));
	lab_3.insert(map<string, string>::value_type("58_52_227", "��������"));
	lab_3.insert(map<string, string>::value_type("58_52_150", "����Ԧʤ"));
	lab_3.insert(map<string, string>::value_type("109_110_71", "�Ͽ�����"));
	lab_3.insert(map<string, string>::value_type("109_110_64", "�Ͽ��"));
	lab_3.insert(map<string, string>::value_type("109_110_3", "�Ͽ���̩"));
	lab_3.insert(map<string, string>::value_type("30_31_132", "���ǰ���"));
	lab_3.insert(map<string, string>::value_type("94_75_2", "������ͨ"));
	lab_3.insert(map<string, string>::value_type("94_75_258", "��������"));
	lab_3.insert(map<string, string>::value_type("26_27_201", "�ִ��Ͷ�˹����"));
	lab_3.insert(map<string, string>::value_type("235_236_58", "���ӽ��ͳ�"));
	lab_3.insert(map<string, string>::value_type("235_236_134", "���ӿͳ�"));
	lab_3.insert(map<string, string>::value_type("74_75_72", "һ������"));
	lab_3.insert(map<string, string>::value_type("183_184_67", "��Դ�ɵ�"));
	lab_3.insert(map<string, string>::value_type("183_184_75", "��Դ����"));
	lab_3.insert(map<string, string>::value_type("64_70_72", "��������"));
	lab_3.insert(map<string, string>::value_type("64_70_197", "�����а���"));
	lab_3.insert(map<string, string>::value_type("64_70_54", "������ţ��"));
	lab_3.insert(map<string, string>::value_type("64_70_111", "��������"));
	lab_3.insert(map<string, string>::value_type("64_70_126", "�����ݶ�"));
	lab_3.insert(map<string, string>::value_type("8_9", "����"));
	lab_3.insert(map<string, string>::value_type("13_9", "����"));
	lab_3.insert(map<string, string>::value_type("66_67", "����"));
	lab_3.insert(map<string, string>::value_type("56_7", "����"));
	lab_3.insert(map<string, string>::value_type("38_39", "����"));
	lab_3.insert(map<string, string>::value_type("58_59", "����"));
	lab_3.insert(map<string, string>::value_type("30_31", "����"));
	lab_3.insert(map<string, string>::value_type("26_27", "�ִ�"));
	lab_3.insert(map<string, string>::value_type("74_75", "һ��"));
	lab_3.insert(map<string, string>::value_type("64_70", "����"));
	lab_3.insert(map<string, string>::value_type("6_7", "����"));
	lab_3.insert(map<string, string>::value_type("72_73", "����"));
}
template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

int jpg_is_over(const char* jpgfile) {
	int iret = 1;
	FILE *jfp;
	short flag;
	short EOI = 0xD9FF;

	if (jfp = fopen(jpgfile, "r+b")) {
		fseek(jfp, -2L, 2);
		fread(&flag, sizeof(short), 2, jfp);
		if (flag != EOI) {
			fseek(jfp, -1L, 1);
			unsigned char newByte = 0xD9;
			fwrite(&newByte, sizeof(newByte), 1, jfp);
			iret = 0;
		}
		fclose(jfp);
	}

	return iret;
}

void mysplit(const std::string& src, const std::string& delim, std::vector<std::string>* dstArr)
{
	size_t last = 0;
	size_t index = src.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		dstArr->push_back(src.substr(last, index - last));
		last = index + 1;
		index = src.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		dstArr->push_back(src.substr(last, index - last));
	}
}

void MNIST2JPG()
{
	string train_test_image[2] = { "./mnist_lmdb/train-images.idx3-ubyte", "./mnist_lmdb/t10k-images.idx3-ubyte" };
	string train_test_label[2] = { "./mnist_lmdb/train-labels.idx1-ubyte","./mnist_lmdb/t10k-labels.idx1-ubyte" };
	int label_num[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	string dir[2] = { "./mnist_lmdb/train/","./mnist_lmdb/test/" };
	for (int iter = 0; iter < 2; iter++)
	{
		//��ȡlabel  
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
			std::cout << label_tmp << std::endl;
		}		
		//��ȡͼƬ  
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
		}

	}
}

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
			if (boost::filesystem::is_regular_file(*pos) && ((boost::filesystem::extension(*pos) == ".jpg") || (boost::filesystem::extension(*pos) == ".png")))
			{
				files.push_back(pos->path());
			}
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
///////////////////////////////////////////////////////////////////////
// recog vehicle logo
// imread�п��ܻῨ������������ͼƬ�޷�����
// �����ƶ�ͼƬ�ĳ����ű������caffeuser�Ĺ�ʱ��⣬��ʱ��kill���Ұ��ļ�����

void HKLogoRecog(Classifier classifier, vector<path> images, string srcDir, string dstDir)
{
	for(int n = 0; n < images.size(); n++)
	{
		try {			
			string vlogo = "";
			char targfile[256];

			string imagePath = images[n].string();
			cout << "File: " << imagePath << endl;
			if (imagePath.length() < 40 || imagePath.find("_") == -1) {
				continue;
			}
			int ipos = imagePath.find_last_of("/\\");
			string fileName = imagePath.substr(ipos + 1);
			string curFilePath = imagePath.substr(0, ipos);
			if (_Debug) {
				cout << "FilePath: [" << curFilePath << "] " << "FileName: [" << fileName << "] " << endl;
			}
			vector<string> fileFields;
			//mysplit(fileName, "_", &fileFields);
			fileFields = split(fileName, '_');
			if (_Debug) {
				cout << "fileFields: [" << fileFields.size() << "] " << endl;
			}
			if (fileFields.size() != 6) {
				std::cerr << "Check filename: " << imagePath << endl;
				continue;
			}

			string strplate = fileFields[1];

			int ichars = strplate.length() + 2;
			if (strplate.compare("�޳���") == 0) {
				ichars = strplate.length();
			}

			vector<int> logolabs;
			int iwid = 360;
			Rect logoRect = Rect(16 * (ichars + 14), 0, iwid, 32);
			if (_Debug) {
				cout << "Plate: [" << strplate << "] " << endl;
				if (jpg_is_over(imagePath.c_str()) != 0) {
					cout << "File is not jpg format." << endl;
					//rename(imagePath.c_str(), (imagePath+".err").c_str());
				}
			}

			Mat vcarImg = cv::imread(imagePath, 0);
			Mat roiImg = vcarImg(logoRect);
			if (_Debug) {
				imshow("roiImg", roiImg);
				waitKey(1);
			}
			for (int i = 0; i < 3; i++) {
				char bufstr[256];
				Rect subRect = Rect(32 * i, 0, 32, 32);
				Mat roiImgSub = roiImg(subRect);
				if (_Debug) {
					imshow("roiImgSub", roiImgSub);
					waitKey(1);
				}				
				if (i > 1) {
					cv::Mat imBintestRoi;
					cv::threshold(roiImgSub, imBintestRoi, 50, 255, CV_THRESH_BINARY);
					int iNon = countNonZero(imBintestRoi);
					if (_Debug) {
						cout << "NonZero: [" << i << "] " << iNon << endl;
						imshow("imBintestRoi", imBintestRoi);
						waitKey(1);
					}

					if (iNon < 20) {
						char key2[128];
						sprintf(key2, "%d_%d", logolabs[0], logolabs[1]);
						map<string, string>::iterator it3 = lab_3.find(key2);
						if (it3 == lab_3.end()) {
							cout << "***Error1, Not Foud.@lab_3: " << key2 << endl;
						}
						else {
							vlogo = it3->second;
						}
						break;
					}
				}

				CHECK(!roiImgSub.empty()) << "Unable to decode image " << std::endl;
#if 0
				std::vector<Prediction> predictions = classifier.PredictEx(roiImgSub, 5);
				for (size_t j = 0; j < predictions.size(); ++j) {
					Prediction p = predictions[j];
					//std::cout << std::fixed << std::setprecision(4) << p.second << " - \"" << p.first << "\"" << std::endl;
					if (j == 0) {
						sprintf(bufstr, ".\\vcartest\\result\\%s_%d_%d.jpg", p.first.c_str(), icounter, i);
						imwrite(bufstr, roiImgSub);
						break;
					}
				}
#else
				int  prediction = classifier.Predict(roiImgSub);
				//std::cout << "prediction: [" << prediction << "], "<< classifier.getLabel(prediction) << std::endl;

				if (i == 0) {
					map<int, string>::iterator it1 = lab_1.find(prediction);
					if (it1 == lab_1.end()) {
						logolabs.push_back(prediction);
					}
					else {
						vlogo = it1->second;
						break;
					}
				}
				else if (i == 1) {
					char key1[128];
					sprintf(key1, "%d_%d", logolabs[0], prediction);
					map<string, string>::iterator it2 = lab_2.find(key1);
					if (it2 == lab_2.end()) {
						logolabs.push_back(prediction);
					}
					else {
						vlogo = it2->second;
						break;
					}
				}
				else if (i == 2) {
					char key2[128];
					sprintf(key2, "%d_%d_%d", logolabs[0], logolabs[1], prediction);
					map<string, string>::iterator it3 = lab_3.find(key2);
					if (it3 == lab_3.end()) {
						cout << "***Error2, Not Foud.@lab_3: " << key2 << endl;
						logolabs.push_back(prediction);
					}
					else {
						vlogo = it3->second;
					}
				}

				//sprintf(bufstr, ".\\vcartest\\result\\%s_%d_%d.jpg", labs[prediction].c_str(), icounter, i);
				//imwrite(bufstr, roiImgSub);
#endif
			}
			if (vlogo.length() == 0) {
				int icnt = logolabs.size() > 2 ? 2 : logolabs.size();
				for (int x = 0; x < icnt; x++) {
					vlogo += classifier.getLabel(logolabs[x]);
				}
				cout << "Recog Recog: " << vlogo << endl;
			}
			// move file
			string newDstDir = dstDir;
			// create dest directories
			if (curFilePath.length() > srcDir.length()) {
				newDstDir += curFilePath.substr(srcDir.length(), curFilePath.length() - srcDir.length());
				if (_Debug) {
					cout << "newDstDir: [" << newDstDir << "] " << endl;
				}
				boost::filesystem::path newDstPath(newDstDir);
				if (!exists(newDstPath))
				{
					cout << "Create New Folder:" << newDstDir << endl;;
					boost::filesystem::create_directories(newDstPath);
				}
			}

			sprintf(targfile, "%s\\%s_%s_%s_%s_%s_%s_%s", newDstDir.c_str(), fileFields[0].c_str(), fileFields[1].c_str(), vlogo.c_str(), fileFields[2].c_str(), fileFields[3].c_str(), fileFields[4].c_str(), fileFields[5].c_str());
			if (rename(imagePath.c_str(), targfile) != 0) {
				cout << "File Rename Failed: " << imagePath << "->" << targfile << endl;
				boost::filesystem::path dstPath(targfile);
				if (exists(dstPath))
				{					
					if (!remove(targfile)) {
						cout << "File Duplicated.[Remove]" << endl;
						rename(imagePath.c_str(), targfile);
					}
				}
			}
		}
		catch (cv::Exception& e) {
			const char* err_msg = e.what();
			cout << "CV Error: " << err_msg << endl;
		}
	}
	cout << "All Over." << endl;
}

void HKLogoRecogTemp(string srcDir)
{
	int icounter = 0;
	std::string TIMEARR[] = { "20170612160242357", "20170612154029461", "20170612152517145", "20170612151035173", "20170612144610097", "20170612142001914", "20170612140023266", "20170612133809353", "20170612130354073", "20170612123004555", "20170612120013592", "20170612113548233", "20170612110251766", "20170612102953174" };
	int iArrFrom = 13;
	vector<string> labs;
	string strLabTxt;
	ifstream finLab("label.txt");
	//std::stringstream sbuffer;
	//sbuffer << finLab.rdbuf();
	//string labelsString(sbuffer.str());
	//cout << "Lab: " << labelsString << endl;
	while (finLab >> strLabTxt)
	{
		labs.push_back(strLabTxt);
	}

	vector<path> imageFiles;
	boost::filesystem::path dir(srcDir);

	if (!exists(dir) || !boost::filesystem::is_directory(dir))
	{
		cout << "workdir is incorrect." << endl;
		return;
	}
	recursive_dir(dir, imageFiles);

	int istart = 245; //��
	for (int n = 0; n < imageFiles.size(); n++)
	{
		string image_path = imageFiles[n].string();
		if (image_path.length() < 40 || image_path.find("_") == -1) {
			continue;
		}

		string fileName = image_path.substr(image_path.find_last_of("/\\") + 1);
		int ipos1 = fileName.find("_");
		string strtime = fileName.substr(0, ipos1);
		cout << "Curr: " << strtime << endl;
		if (strtime >= TIMEARR[iArrFrom]) {
			cout << "Curr: " << strtime <<  " *****Next*****: " << TIMEARR[iArrFrom] << endl;
			iArrFrom--;
			istart -= 20;
			icounter = 0;
		}

		if (iArrFrom == 6) {	//����...
			istart = 95;
		}

		Rect logoRect = Rect(0, 0, 960, 32);

		Mat vcarImg = imread(image_path);
		Mat roiImg = vcarImg(logoRect);

		icounter++;
		if (icounter > 120) {
			cout << "-------------" << endl;
			continue;
		}
		int ioff = 0;
		int iflag = 0;
		int isecstart = istart;
		int ilab = 0;
		for (int i = 0; i < 20; i++) {
			char bufstr[256];		
			if (i == 10) {
				ioff = 32 * 3;
				iflag = 1;
				isecstart = istart - 20;				
			}
			ilab = isecstart + i;

			if (iArrFrom <= 6) {	//����...
				ilab = istart + i;
			}
			if (iArrFrom == 1) {	//����...
				ilab = i;
				if (i > 14) {
					ilab = 275;
				}
			}
			if (iArrFrom == 0) {	//����
				ilab = 255+i;
			}

			Rect subRect = Rect(32 * i+ ioff, 0, 32, 32);
			Mat roiImgSub = roiImg(subRect);
			//imshow("tmpimg", roiImgSub);
			//waitKey(1);
			cv::Mat imBintestRoi;
			cv::Mat imGraytestRoi;
			cv::cvtColor(roiImgSub, imGraytestRoi, CV_BGR2GRAY);

			//cvtColor(roiImgSub, roiImgSub, CV_BGR2GRAY);
			//bitwise_xor(imgTmp, Scalar(255), imgTmp);
			CHECK(!roiImgSub.empty()) << "Unable to decode image " << std::endl;
			if (icounter <= 100) {
				sprintf(bufstr, ".\\vcartest\\out\\train\\%d\\%d_%d_%s.jpg", ilab, ilab, icounter, strtime.substr(9, 9).c_str());
			}
			else if(icounter <= 120) {
				sprintf(bufstr, ".\\vcartest\\out\\test\\%d\\%d_%d_%s.jpg", ilab, ilab, icounter-100, strtime.substr(9, 9).c_str());
			}			
			imwrite(bufstr, imGraytestRoi);

			//imshow("Test1", imBintestRoi);
			//imshow("Test", roiImgSub);
			//waitKey(0);
		}
	}
}

int main(int argc, char** argv)
{
#if 0
	MNIST2JPG();
#endif

#if 0
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " image" << std::endl;
		return 1;
	}
	Caffe_Predefine();
	vector<float> res = GetSimilarFaces(argv[1]);
	int i = 0;
	while (i<res.size())
	{
		cout << "Similar: "<<res[i++] << endl;
	}
	/*
	Mat lena = imread(argv[1]);
	if (!lena.empty())
	{
		int i = 0;
		vector<float> print = ExtractFeature(lena);
		while (i<print.size())
		{
			cout << print[i++] << endl;
		}
	}
	imshow("Extract feature", lena);
	waitKey(0);
	*/
#endif

#if 0
	caffe_wrapper *cw = new caffe_wrapper();
	string input = "0.jpg";
	string model = "lenet_iter_10000.caffemodel";
	string def = "lenet.prototxt";
	cw->run_test(input, model, def);
#endif

#if 0
	if (argc < 3) {
		std::cerr << "Usage: " << argv[0]
			<< " src_dir"
			<< " dst_dir" << std::endl;
		return 1;
	}
	::google::InitGoogleLogging(argv[0]);
	string srcDir = argv[1];
	string dstDir = argv[2];
	if (argc == 4) {
		_Debug = atoi(argv[3]);
	}	
	vector<path> images;
	boost::filesystem::path dir(srcDir);

	if (!exists(dir) || !boost::filesystem::is_directory(dir))
	{
		cout << "src_dir is incorrect." << endl;
		return 1;
	}
	boost::filesystem::path dirDst(dstDir);
	if (!exists(dirDst) || !boost::filesystem::is_directory(dirDst))
	{
		cout << "dst_dir is incorrect." << endl;
		return 1;
	}
	if (srcDir[srcDir.length() - 1] == '\\') {
		srcDir = srcDir.substr(0, srcDir.length() - 1);
	}
	if (dstDir[dstDir.length() - 1] == '\\') {
		dstDir = dstDir.substr(0, dstDir.length() - 1);
	}
	initLabelsMap();

	Classifier classifier("lenet.prototxt", "lenet_iter_10000.caffemodel", "label.txt");
	time_t start = 0, end = 0;

	while (true) {		
		recursive_dir(dir, images);
		if (images.size() == 0) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
			cout << "Waiting..." << endl;
			continue;
		}
		time(&start);
		HKLogoRecog(classifier, images, srcDir, dstDir);
		images.clear();
		time(&end);
		cout << "��ʱ��" << (end - start) << "��" << endl;
	}	
	//HKLogoRecogTemp(srcDir);
#endif

#if 1

	//string srcDir = argv[1];
	if (argc < 5) {
		cout << "Usage: " << argv[0] << " deploy.prototxt caffemodel label test_folder [0/1](debug on/off)" << endl;
		return 0;
	}
	if (argc == 6) {
		_Debug = atoi(argv[5]);
	}

	vector<path> imageFiles;
	string imagesFolder = argv[4];
	boost::filesystem::path dir(imagesFolder);

	if (!exists(dir) || !boost::filesystem::is_directory(dir))
	{
		cout << "workdir is incorrect." << endl;
		return 0;
	}
	recursive_dir(dir, imageFiles);

	string model_file = argv[1]; //"lenet.prototxt";
	string trained_file = argv[2]; //"lenet_iter_10000.caffemodel";
	string label_file = argv[3]; //"label.txt"
	Classifier classifier(model_file, trained_file, label_file);
	
	std::cout << "start recog.(press ESC to end.)" << endl;

	int ia = 0;
	int ib = 0;
	while (1)
	{
		char c = (char)waitKey();
		if (c == 27)
			break;

		cv::Mat imgTmp;

		std::cout << imageFiles[ia] << std::endl;
		imgTmp = imread(imageFiles[ia].string());
		if (imgTmp.empty()) {
			continue;
		}

		cv::imshow("inputImg", imgTmp);
		cv::moveWindow("inputImg", 100, 100);

		//resize(imgTmp, imgTmp, Size(28,28), CV_INTER_CUBIC);
		cvtColor(imgTmp, imgTmp, CV_BGR2GRAY);
		//bitwise_xor(imgTmp, Scalar(255), imgTmp);
		CHECK(!imgTmp.empty()) << "Unable to decode image " << std::endl;

		std::vector<Prediction> predictions = classifier.PredictEx(imgTmp, 5);
		/* Print the top N predictions. */
		for (size_t i = 0; i < predictions.size(); ++i) {
			Prediction p = predictions[i];
			std::cout << std::fixed << std::setprecision(4) << p.second << " - \"" << p.first << "\"" << std::endl;
		}

		ia++;
		if (ia >= imageFiles.size()) {
			cout << "all done." << endl;
			break;
		}
	}

	waitKey();

#endif

	return 0;
}