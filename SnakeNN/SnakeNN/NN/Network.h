#ifndef NETWORK_CLASS
#define NETWORK_CLASS
#include "framework.h"

#include <vector>
#include "Neuron.h"

class Neuron;

typedef std::vector<Neuron> Layer;

class Network
{
public:

	Network(const std::vector<unsigned>& topology);


	void FeedForward(const std::vector<float>& input);
	void BackProp(const std::vector<float>& targets);
	void GetResults(std::vector<float>& results) const;

	float GetRecentAverageError() const { return m_RecentAverageError; };


private:
	std::vector<Layer> m_Layers;

	float m_Error{};
	float m_RecentAverageError{};
	float m_RecentAverageSmoothingFactor{};
};
#endif

