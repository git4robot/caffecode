
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
	//, MG名爵
	lab_1.insert(pair<int, string>(0, "其它"));
	lab_1.insert(map<int, string>::value_type(10, "标致"));
	lab_1.insert(map<int, string>::value_type(106, "跃进"));
	lab_1.insert(map<int, string>::value_type(108, "黄海"));
	lab_1.insert(map<int, string>::value_type(113, "保时捷"));
	lab_1.insert(map<int, string>::value_type(114, "时代风菱"));
	lab_1.insert(map<int, string>::value_type(115, "捷豹"));
	lab_1.insert(map<int, string>::value_type(121, "JEEP"));
	lab_1.insert(map<int, string>::value_type(123, "路虎"));
	lab_1.insert(map<int, string>::value_type(128, "陕汽"));
	lab_1.insert(map<int, string>::value_type(132, "霸龙汽车"));
	lab_1.insert(map<int, string>::value_type(137, "MAN汽车"));
	lab_1.insert(map<int, string>::value_type(140, "传祺"));
	lab_1.insert(map<int, string>::value_type(142, "陆风"));
	lab_1.insert(map<int, string>::value_type(144, "京华客车"));
	lab_1.insert(map<int, string>::value_type(147, "欧宝"));
	lab_1.insert(map<int, string>::value_type(148, "开瑞"));
	lab_1.insert(map<int, string>::value_type(15, "日产"));
	lab_1.insert(map<int, string>::value_type(158, "讴歌"));
	lab_1.insert(map<int, string>::value_type(160, "启辰"));
	lab_1.insert(map<int, string>::value_type(166, "纳智捷"));
	lab_1.insert(map<int, string>::value_type(168, "野马"));
	lab_1.insert(map<int, string>::value_type(17, "奥迪"));
	lab_1.insert(map<int, string>::value_type(170, "理念"));
	lab_1.insert(map<int, string>::value_type(174, "松花江"));
	lab_1.insert(map<int, string>::value_type(177, "黑豹"));
	lab_1.insert(map<int, string>::value_type(178, "唐骏"));
	lab_1.insert(map<int, string>::value_type(179, "莲花汽车"));
	lab_1.insert(map<int, string>::value_type(186, "潍力"));
	lab_1.insert(map<int, string>::value_type(187, "派喜"));
	lab_1.insert(map<int, string>::value_type(193, "王牌重汽"));
	lab_1.insert(map<int, string>::value_type(200, "道奇"));
	lab_1.insert(map<int, string>::value_type(204, "燕台"));
	lab_1.insert(map<int, string>::value_type(206, "GMC"));
	lab_1.insert(map<int, string>::value_type(209, "牡丹"));
	lab_1.insert(map<int, string>::value_type(212, "联合汽车"));
	lab_1.insert(map<int, string>::value_type(218, "少林客车"));
	lab_1.insert(map<int, string>::value_type(220, "青年客车"));
	lab_1.insert(map<int, string>::value_type(222, "徐工"));
	lab_1.insert(map<int, string>::value_type(225, "农用车"));
	lab_1.insert(map<int, string>::value_type(227, "轻骑"));
	lab_1.insert(map<int, string>::value_type(229, "楚风汽车"));
	lab_1.insert(map<int, string>::value_type(23, "兰博基尼"));
	lab_1.insert(map<int, string>::value_type(231, "九龙客车"));
	lab_1.insert(map<int, string>::value_type(232, "友谊客车"));
	lab_1.insert(map<int, string>::value_type(238, "武夷"));
	lab_1.insert(map<int, string>::value_type(240, "成都客车"));
	lab_1.insert(map<int, string>::value_type(242, "天马"));
	lab_1.insert(map<int, string>::value_type(246, "观致"));
	lab_1.insert(map<int, string>::value_type(247, "宾利"));
	lab_1.insert(map<int, string>::value_type(248, "富奇"));
	lab_1.insert(map<int, string>::value_type(249, "航天汽车"));
	lab_1.insert(map<int, string>::value_type(253, "玛莎拉蒂"));
	lab_1.insert(map<int, string>::value_type(256, "舒驰客车"));
	lab_1.insert(map<int, string>::value_type(257, "实力客车"));
	lab_1.insert(map<int, string>::value_type(261, "法拉利汽车"));
	lab_1.insert(map<int, string>::value_type(262, "太湖客车"));
	lab_1.insert(map<int, string>::value_type(264, "常隆客车"));
	lab_1.insert(map<int, string>::value_type(266, "悦西客车"));
	lab_1.insert(map<int, string>::value_type(267, "庐山牌"));
	lab_1.insert(map<int, string>::value_type(269, "万丰牌"));
	lab_1.insert(map<int, string>::value_type(272, "二轮车"));
	lab_1.insert(map<int, string>::value_type(28, "奇瑞"));
	lab_1.insert(map<int, string>::value_type(29, "瑞麟"));
	lab_1.insert(map<int, string>::value_type(3, "众泰"));
	lab_1.insert(map<int, string>::value_type(31, "亚星客车"));
	lab_1.insert(map<int, string>::value_type(32, "荣威"));
	lab_1.insert(map<int, string>::value_type(33, "威麟汽车"));
	lab_1.insert(map<int, string>::value_type(4, "别克"));
	lab_1.insert(map<int, string>::value_type(42, "沃尔沃"));
	lab_1.insert(map<int, string>::value_type(46, "菲亚特"));
	lab_1.insert(map<int, string>::value_type(47, "帝豪"));
	lab_1.insert(map<int, string>::value_type(5, "克莱斯勒"));
	lab_1.insert(map<int, string>::value_type(51, "比亚迪"));
	lab_1.insert(map<int, string>::value_type(52, "铃木"));
	lab_1.insert(map<int, string>::value_type(68, "十通"));
	lab_1.insert(map<int, string>::value_type(7, "马自达"));
	lab_1.insert(map<int, string>::value_type(71, "夏利"));
	lab_1.insert(map<int, string>::value_type(76, "依维柯"));
	lab_1.insert(map<int, string>::value_type(78, "力帆"));
	lab_1.insert(map<int, string>::value_type(81, "皇冠"));
	lab_1.insert(map<int, string>::value_type(84, "JMC"));
	lab_1.insert(map<int, string>::value_type(90, "昌河"));
	lab_1.insert(map<int, string>::value_type(92, "厦门金龙"));
	lab_1.insert(map<int, string>::value_type(96, "苏州金龙"));
	lab_1.insert(map<int, string>::value_type(99, "宇通"));

	lab_2.insert(map<string, string>::value_type("103_223", "北方汽车"));
	lab_2.insert(map<string, string>::value_type("103_72", "北奔重卡"));
	lab_2.insert(map<string, string>::value_type("104_33", "卡威汽车"));
	lab_2.insert(map<string, string>::value_type("104_43", "卡尔森汽车"));
	lab_2.insert(map<string, string>::value_type("12_9", "丰田"));
	lab_2.insert(map<string, string>::value_type("129_130", "红岩汽车"));
	lab_2.insert(map<string, string>::value_type("129_275", "红旗"));
	lab_2.insert(map<string, string>::value_type("13_105", "福星汽车"));
	lab_2.insert(map<string, string>::value_type("13_14", "福特"));
	lab_2.insert(map<string, string>::value_type("13_18", "福迪"));
	lab_2.insert(map<string, string>::value_type("13_20", "福达"));
	lab_2.insert(map<string, string>::value_type("13_25", "福龙马"));
	lab_2.insert(map<string, string>::value_type("135_25", "申龙客车"));
	lab_2.insert(map<string, string>::value_type("135_42", "申沃客车"));
	lab_2.insert(map<string, string>::value_type("180_181", "双环汽车"));
	lab_2.insert(map<string, string>::value_type("180_25", "双龙"));
	lab_2.insert(map<string, string>::value_type("191_75", "广汽日野"));
	lab_2.insert(map<string, string>::value_type("191_97", "广州云豹"));
	lab_2.insert(map<string, string>::value_type("192_17", "新奥驰"));
	lab_2.insert(map<string, string>::value_type("192_2", "新大地汽车"));
	lab_2.insert(map<string, string>::value_type("192_243", "新雅途"));
	lab_2.insert(map<string, string>::value_type("192_28", "新奇瑞"));
	lab_2.insert(map<string, string>::value_type("192_88", "新凯"));
	lab_2.insert(map<string, string>::value_type("2_18", "大迪"));
	lab_2.insert(map<string, string>::value_type("2_211", "大运汽车"));
	lab_2.insert(map<string, string>::value_type("2_259", "大发汽车"));
	lab_2.insert(map<string, string>::value_type("2_3", "大众"));
	lab_2.insert(map<string, string>::value_type("2_99", "大宇客车"));
	lab_2.insert(map<string, string>::value_type("21_22", "雪佛兰"));
	lab_2.insert(map<string, string>::value_type("21_24", "雪铁龙"));
	lab_2.insert(map<string, string>::value_type("214_215", "春洲客车"));
	lab_2.insert(map<string, string>::value_type("214_23", "春兰汽车"));
	lab_2.insert(map<string, string>::value_type("217_100", "恒通客车"));
	lab_2.insert(map<string, string>::value_type("217_242", "恒天汽车"));
	lab_2.insert(map<string, string>::value_type("237_243", "西雅特"));
	lab_2.insert(map<string, string>::value_type("237_42", "西沃客车"));
	lab_2.insert(map<string, string>::value_type("260_168", "神野汽车"));
	lab_2.insert(map<string, string>::value_type("260_7", "神马客车"));
	lab_2.insert(map<string, string>::value_type("34_181", "三环"));
	lab_2.insert(map<string, string>::value_type("34_271", "三轮车"));
	lab_2.insert(map<string, string>::value_type("34_35", "三菱"));
	lab_2.insert(map<string, string>::value_type("34_74", "三一重工"));
	lab_2.insert(map<string, string>::value_type("36_230", "斯堪尼亚"));
	lab_2.insert(map<string, string>::value_type("36_262", "斯太尔汽车"));
	lab_2.insert(map<string, string>::value_type("36_37", "斯柯达"));
	lab_2.insert(map<string, string>::value_type("36_60", "斯巴鲁"));
	lab_2.insert(map<string, string>::value_type("38_17", "吉奥汽车"));
	lab_2.insert(map<string, string>::value_type("38_58", "吉江"));
	lab_2.insert(map<string, string>::value_type("40_100", "中通客车"));
	lab_2.insert(map<string, string>::value_type("40_101", "中国重汽"));
	lab_2.insert(map<string, string>::value_type("40_134", "中客华北"));
	lab_2.insert(map<string, string>::value_type("40_145", "中顺"));
	lab_2.insert(map<string, string>::value_type("40_169", "中兴"));
	lab_2.insert(map<string, string>::value_type("40_2", "中大汽车"));
	lab_2.insert(map<string, string>::value_type("40_41", "中华"));
	lab_2.insert(map<string, string>::value_type("41_149", "华普汽车"));
	lab_2.insert(map<string, string>::value_type("41_35", "华菱星马"));
	lab_2.insert(map<string, string>::value_type("41_89", "华泰"));
	lab_2.insert(map<string, string>::value_type("44_5", "雷克萨斯"));
	lab_2.insert(map<string, string>::value_type("44_83", "雷诺"));
	lab_2.insert(map<string, string>::value_type("49_223", "东方红"));
	lab_2.insert(map<string, string>::value_type("49_42", "东沃汽车"));
	lab_2.insert(map<string, string>::value_type("49_50", "东风"));
	lab_2.insert(map<string, string>::value_type("49_69", "东南"));
	lab_2.insert(map<string, string>::value_type("54_55", "金杯"));
	lab_2.insert(map<string, string>::value_type("56_98", "海格"));
	lab_2.insert(map<string, string>::value_type("57_136", "五征汽车"));
	lab_2.insert(map<string, string>::value_type("57_215", "五洲龙"));
	lab_2.insert(map<string, string>::value_type("57_35", "五菱"));
	lab_2.insert(map<string, string>::value_type("57_68", "五十铃"));
	lab_2.insert(map<string, string>::value_type("58_181", "江环货车"));
	lab_2.insert(map<string, string>::value_type("58_69", "江南"));
	lab_2.insert(map<string, string>::value_type("6_138", "宝骏"));
	lab_2.insert(map<string, string>::value_type("6_25", "宝龙"));
	lab_2.insert(map<string, string>::value_type("62_46", "英菲尼迪"));
	lab_2.insert(map<string, string>::value_type("62_63", "英伦"));
	lab_2.insert(map<string, string>::value_type("62_9", "英田"));
	lab_2.insert(map<string, string>::value_type("64_12", "长丰"));
	lab_2.insert(map<string, string>::value_type("64_58", "长江客车"));
	lab_2.insert(map<string, string>::value_type("64_65", "长城"));
	lab_2.insert(map<string, string>::value_type("66_155", "哈弗HAVAL"));
	lab_2.insert(map<string, string>::value_type("69_138", "南骏"));
	lab_2.insert(map<string, string>::value_type("70_184", "安源客车"));
	lab_2.insert(map<string, string>::value_type("70_88", "安凯客车"));
	lab_2.insert(map<string, string>::value_type("88_18", "凯迪拉克"));
	lab_2.insert(map<string, string>::value_type("88_7", "凯马"));
	lab_2.insert(map<string, string>::value_type("94_234", "上饶客车"));
	lab_2.insert(map<string, string>::value_type("94_56", "上海汇众"));

	lab_3.insert(map<string, string>::value_type("72_73_162", "奔驰SMART"));
	lab_3.insert(map<string, string>::value_type("6_7_152", "宝马MINI"));
	lab_3.insert(map<string, string>::value_type("103_144_225", "北京农用"));
	lab_3.insert(map<string, string>::value_type("103_144_75", "北京汽车"));
	lab_3.insert(map<string, string>::value_type("103_144_75", "北京汽车制造厂"));
	lab_3.insert(map<string, string>::value_type("103_75_33", "北汽威旺"));
	lab_3.insert(map<string, string>::value_type("103_75_207", "北汽银翔"));
	lab_3.insert(map<string, string>::value_type("8_9_195", "本田思铭"));
	lab_3.insert(map<string, string>::value_type("13_9_114", "福田时代"));
	lab_3.insert(map<string, string>::value_type("66_67_182", "哈飞赛豹"));
	lab_3.insert(map<string, string>::value_type("56_7_143", "海马郑州"));
	lab_3.insert(map<string, string>::value_type("38_39_118", "吉利全球鹰"));
	lab_3.insert(map<string, string>::value_type("58_59_70", "江淮安驰"));
	lab_3.insert(map<string, string>::value_type("58_59_26", "江淮现代客车"));
	lab_3.insert(map<string, string>::value_type("58_52_227", "江铃轻汽"));
	lab_3.insert(map<string, string>::value_type("58_52_150", "江铃驭胜"));
	lab_3.insert(map<string, string>::value_type("109_110_71", "老款夏利"));
	lab_3.insert(map<string, string>::value_type("109_110_64", "老款长城"));
	lab_3.insert(map<string, string>::value_type("109_110_3", "老款众泰"));
	lab_3.insert(map<string, string>::value_type("30_31_132", "起亚霸锐"));
	lab_3.insert(map<string, string>::value_type("94_75_2", "上汽大通"));
	lab_3.insert(map<string, string>::value_type("94_75_258", "上汽仪征"));
	lab_3.insert(map<string, string>::value_type("26_27_201", "现代劳恩斯酷派"));
	lab_3.insert(map<string, string>::value_type("235_236_58", "扬子江客车"));
	lab_3.insert(map<string, string>::value_type("235_236_134", "扬子客车"));
	lab_3.insert(map<string, string>::value_type("74_75_72", "一汽奔腾"));
	lab_3.insert(map<string, string>::value_type("183_184_67", "永源飞碟"));
	lab_3.insert(map<string, string>::value_type("183_184_75", "永源汽车"));
	lab_3.insert(map<string, string>::value_type("64_70_72", "长安奔奔"));
	lab_3.insert(map<string, string>::value_type("64_70_197", "长安谛艾仕"));
	lab_3.insert(map<string, string>::value_type("64_70_54", "长安金牛星"));
	lab_3.insert(map<string, string>::value_type("64_70_111", "长安商用"));
	lab_3.insert(map<string, string>::value_type("64_70_126", "长安逸动"));
	lab_3.insert(map<string, string>::value_type("8_9", "本田"));
	lab_3.insert(map<string, string>::value_type("13_9", "福田"));
	lab_3.insert(map<string, string>::value_type("66_67", "哈飞"));
	lab_3.insert(map<string, string>::value_type("56_7", "海马"));
	lab_3.insert(map<string, string>::value_type("38_39", "吉利"));
	lab_3.insert(map<string, string>::value_type("58_59", "江淮"));
	lab_3.insert(map<string, string>::value_type("30_31", "起亚"));
	lab_3.insert(map<string, string>::value_type("26_27", "现代"));
	lab_3.insert(map<string, string>::value_type("74_75", "一汽"));
	lab_3.insert(map<string, string>::value_type("64_70", "长安"));
	lab_3.insert(map<string, string>::value_type("6_7", "宝马"));
	lab_3.insert(map<string, string>::value_type("72_73", "奔驰"));
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
			std::cout << label_tmp << std::endl;
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
// imread有可能会卡死，导致其它图片无法处理。
// 需在移动图片的程序或脚本中添加caffeuser的工时检测，超时被kill，且把文件移走

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
			if (strplate.compare("无车牌") == 0) {
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

	int istart = 245; //锐
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

		if (iArrFrom == 6) {	//其它...
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

			if (iArrFrom <= 6) {	//其它...
				ilab = istart + i;
			}
			if (iArrFrom == 1) {	//其它...
				ilab = i;
				if (i > 14) {
					ilab = 275;
				}
			}
			if (iArrFrom == 0) {	//蒂舒
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
		cout << "计时：" << (end - start) << "秒" << endl;
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