#ifndef NETWORK_CLASS
#define NETWORK_CLASS
#include <vector>

class Neuron;

typedef std::vector<Neuron> Layer;

class Network
{
public:

	Network(const std::vector<unsigned>&topology);


	void FeedForward(const std::vector<float>& input);
	void BackProp(const std::vector<float>& targets);
	void GetResults(std::vector<float>& results) const;

private:

	std::vector<Layer> m_Layers;
};
#endif

