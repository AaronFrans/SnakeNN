#include "TrainingData.h"

TrainingData::TrainingData(const std::wstring filename)
{
	m_TrainingDatafile.open(filename.c_str());
}

void TrainingData::GetTopology(std::vector<unsigned>& topology)
{
	std::wstring line;
	std::wstring label;

	std::getline(m_TrainingDatafile, line);
	std::wstringstream ss(line);
	ss >> label;
	if (IsEof() || label.compare(L"topology:") != 0)
	{
		abort();
	}

	while (!ss.eof())
	{
		unsigned n;
		ss >> n;
		topology.push_back(n);
	}

	return;
}

unsigned TrainingData::GetNextInputs(std::vector<float>& inputs)
{
	inputs.clear();

	std::wstring line;
	std::getline(m_TrainingDatafile, line);

	std::wstringstream ss(line);

	std::wstring label;

	ss >> label;

	if (label.compare(L"in:") == 0)
	{
		float oneValue;
		while (ss >> oneValue)
		{
			inputs.push_back(oneValue);
		}
	}

	return static_cast<int>(inputs.size());
}

unsigned TrainingData::GetTargetOutputs(std::vector<float>& outputs)
{

	outputs.clear();

	std::wstring line;
	std::getline(m_TrainingDatafile, line);

	std::wstringstream ss(line);

	std::wstring label;

	ss >> label;

	if (label.compare(L"out:") == 0)
	{
		float oneValue;
		while (ss >> oneValue)
		{
			outputs.push_back(oneValue);
		}
	}

	return static_cast<int>(outputs.size());
}
