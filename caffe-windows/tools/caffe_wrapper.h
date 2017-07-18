#pragma once
/*
#ifdef CAFFE_WRAP
#define CWDLL_API __declspec(dllexport)
#else
#define CWDLL_API __declspec(dllimport)
#endif
*/
#include <string>
#include "caffe/caffe.hpp"
#include "caffe/util/io.hpp"
#include "caffe/blob.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "boost/smart_ptr/shared_ptr.hpp"
#include "caffe/layers/memory_data_layer.hpp"

// Caffe's required library
//#pragma comment(lib, "libcaffe.lib")

// enable namespace
using namespace std;
using namespace cv;
using namespace boost;
using namespace caffe;

namespace CaffeUtil
{
	/* Pair (label, confidence) representing a prediction. */
	typedef std::pair<string, float> Prediction;

	class Classifier {
	public:
		Classifier(const string& model_file, const string& trained_file, const string& label_file);

		int Predict(const cv::Mat& img);
		std::vector<Prediction> PredictEx(const cv::Mat& img, int N);
		std::string getLabel(int prediction);
	private:

		std::vector<float> Predict_(const cv::Mat& img);

		void WrapInputLayer(std::vector<cv::Mat>* input_channels);

		void Preprocess(const cv::Mat& img, std::vector<cv::Mat>* input_channels);

	private:
		caffe::shared_ptr<Net<float> > net_;
		cv::Size input_geometry_;
		int num_channels_;
		std::vector<string> labels_;
	};
}