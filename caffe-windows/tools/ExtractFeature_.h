
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

std::vector<float> GetSimilarFaces(std::string testfile);
std::vector<float> ExtractFeature(Mat FaceROI);//给一个图片 返回一个vector<float>容器
void Caffe_Predefine();