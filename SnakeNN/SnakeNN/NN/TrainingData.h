#ifndef TRAININGDATA_CLASS
#define TRAININGDATA_CLASS
#include <vector>
#include <fstream>
#include <sstream>
class TrainingData
{
public:
	TrainingData(const std::wstring filename);
	float IsEof(void) { return m_TrainingDatafile.eof(); };
	void GetTopology(std::vector<unsigned>& topology);
	unsigned GetNextInputs(std::vector<float>& inputs);
	unsigned GetTargetOutputs(std::vector<float>& outputs);

private:
	std::wifstream m_TrainingDatafile;
};

#endif
