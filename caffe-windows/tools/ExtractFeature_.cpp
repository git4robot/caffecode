#include "ExtractFeature_.h"
#include "caffe_net_memorylayer.h"

#if 0
namespace caffe
{
	extern INSTANTIATE_CLASS(InputLayer);
	extern INSTANTIATE_CLASS(InnerProductLayer);
	extern INSTANTIATE_CLASS(DropoutLayer);
	extern INSTANTIATE_CLASS(ConvolutionLayer);
	REGISTER_LAYER_CLASS(Convolution);
	extern INSTANTIATE_CLASS(ReLULayer);
	REGISTER_LAYER_CLASS(ReLU);
	extern INSTANTIATE_CLASS(PoolingLayer);
	REGISTER_LAYER_CLASS(Pooling);
	extern INSTANTIATE_CLASS(LRNLayer);
	REGISTER_LAYER_CLASS(LRN);
	extern INSTANTIATE_CLASS(SoftmaxLayer);
	REGISTER_LAYER_CLASS(Softmax);
	extern INSTANTIATE_CLASS(MemoryDataLayer);
}
#endif

template <typename Dtype>
caffe::Net<Dtype>* Net_Init_Load(std::string param_file, std::string pretrained_param_file, caffe::Phase phase)
{
	caffe::Net<Dtype>* net(new caffe::Net<Dtype>("vgg_extract_feature_memorydata.prototxt", caffe::TEST));
	net->CopyTrainedLayersFrom("lenet_iter_10000.caffemodel");
	return net;
}

void Caffe_Predefine()//when our code begining run must add it
{
	//caffe::Caffe::set_mode(caffe::Caffe::GPU);
	net = Net_Init_Load<float>("vgg_extract_feature_memorydata.prototxt", "VGG_FACE.caffemodel", caffe::TEST);
	memory_layer = (caffe::MemoryDataLayer<float> *)net->layers()[0].get();
}

float cosine_similarity(std::vector<float> A, std::vector<float> B)
{
	if (A.size() != B.size()) {
		return 0.0;
	}
	float dot = 0.0, denom_a = 0.0, denom_b = 0.0;
	for (unsigned int i = 0u; i < A.size(); ++i) {
		dot += A[i] * B[i];
		denom_a += A[i] * A[i];
		denom_b += B[i] * B[i];
	}
	return dot / (sqrt(denom_a) * sqrt(denom_b));
}

std::vector<float> ExtractFeature(Mat FaceROI)
{
	//caffe::Caffe::set_mode(caffe::Caffe::GPU);
	std::vector<Mat> test;
	std::vector<int> testLabel;
	std::vector<float> test_vector;
	test.push_back(FaceROI);
	testLabel.push_back(0);
	memory_layer->AddMatVector(test, testLabel);// memory_layer and net , must be define be a global variable.
	test.clear(); testLabel.clear();
	std::vector<caffe::Blob<float>*> input_vec;
	net->Forward(input_vec);
	boost::shared_ptr<caffe::Blob<float>> fc8 = net->blob_by_name("fc7");	//fc8 <->num=2622, fc7<->num=4096
	int test_num = 0;
	while (test_num < 4096)
	{
		test_vector.push_back(fc8->data_at(0, test_num++, 1, 1));
	}
	return test_vector;
}

std::vector<float> GetSimilarFaces(std::string testfile) 
{
	std::vector<float> res;
	Mat testFace = imread(testfile);
	if (testFace.empty())
	{
		std::cout << "Error, Image Empty." << endl;
		return res;
	}
	vector<float> testFaceFeatures = ExtractFeature(testFace);
	for (int i = 0; i < 1; i++) {
		float regScores = 0.0;
		for (int j = 0; j < 5; j++) {
			char buf[126];
			sprintf(buf, ".\\regist_pic\\%d\\%d_%d.jpg", i, i, j);
			std::cout << buf << endl;
			Mat regFaceImg = imread(buf);
			vector<float> regFaceFeatures = ExtractFeature(regFaceImg);
			float similarScore = cosine_similarity(testFaceFeatures, regFaceFeatures);
			regScores += similarScore;
			std::cout << "Score: "<< similarScore << endl;

			//sprintf(buf, ".\\regist_pic\\%d\\%d_%d_cps.bin", i, i, j);
			//std::ofstream output_file(buf);
			//std::ostream_iterator<float> output_iterator(output_file, "\n");
			//std::copy(regFaceFeatures.begin(), regFaceFeatures.end(), output_iterator);
		}
		res.push_back(regScores / 5.0);
	}

	return res;
}