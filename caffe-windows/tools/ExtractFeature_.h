
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

std::vector<float> GetSimilarFaces(std::string testfile);
std::vector<float> ExtractFeature(Mat FaceROI);//��һ��ͼƬ ����һ��vector<float>����
void Caffe_Predefine();