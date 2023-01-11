#include <cassert>
#include "Network.h"

Network::Network(const std::vector<unsigned>& topology)
{
	unsigned numLayers = static_cast<int>(topology.size());

	for (unsigned currentLayer{ 0 }; currentLayer < numLayers; ++currentLayer)
	{
		m_Layers.push_back(Layer{});
		unsigned numOutputs{ currentLayer == topology.size() - 1 ? 0 : topology[currentLayer + 1] };


		//Per Layer fill with neurons
		//1 extra neuron for bias
		for (unsigned currentNeuron = 0; currentNeuron <= topology[currentLayer]; ++currentNeuron)
		{
			m_Layers.back().push_back(Neuron{ numOutputs, currentNeuron });
			OutputDebugString(L"New neuron!\n");
		}

		//set bias to 1.f

		m_Layers.back().back().SetOutput(1.f);
	}

}

void Network::FeedForward(const std::vector<float>& inputs)
{
	assert(inputs.size() == m_Layers[0].size() - 1);

	//assing the inputs to input neurons
	for (unsigned input = 0; input < inputs.size(); input++)
	{
		m_Layers[0][input].SetOutput(inputs[input]);
	}

	//go trough every neuron in every layer and feed them forward
	for (unsigned layer = 1; layer < m_Layers.size(); ++layer)
	{
		Layer& prevLayer = m_Layers[layer - 1];
		for (unsigned neuron = 0; neuron < m_Layers[layer].size() - 1; ++neuron)
		{
			m_Layers[layer][neuron].FeedForward(prevLayer);
		}
	}
}

void Network::BackProp(const std::vector<float>& targets)
{
	//Calculate error
	Layer& outputLayer = m_Layers.back();

	m_Error = 0.f;

	for (unsigned neuron = 0; neuron < outputLayer.size() - 1; ++neuron)
	{
		float delta = targets[neuron] - outputLayer[neuron].GetOutput();
		m_Error += delta * delta;
	}

	m_Error /= outputLayer.size() - 1;
	m_Error = sqrtf(m_Error);


	//recent average measurement
	m_RecentAverageError =
		(m_RecentAverageError * m_RecentAverageSmoothingFactor + m_Error)
		/ (m_RecentAverageSmoothingFactor + 1.f);



	//Calculate output layers gradients
	for (unsigned neuron = 0; neuron < outputLayer.size() - 1; ++neuron)
	{
		outputLayer[neuron].CalcOutputGradients(targets[neuron]);
	}

	//Calculate gradients on hidden layers

	for (unsigned layer = static_cast<int>(m_Layers.size() - 2); layer > 0; --layer)
	{
		Layer& hiddenLayer = m_Layers[layer];
		Layer& nextLayer = m_Layers[layer + 1];

		for (unsigned neuron = 0; neuron < hiddenLayer.size(); ++neuron)
		{
			hiddenLayer[neuron].CalcHiddenGradients(nextLayer);
		}
	}

	//for all layers from outputs to first hidden layer
	// update connections weights

	for (unsigned layerIndex = static_cast<int>(m_Layers.size() - 1); layerIndex > 0; --layerIndex)
	{
		Layer& layer = m_Layers[layerIndex];
		Layer& prevLayer = m_Layers[layerIndex - 1];

		for (unsigned neuron = 0; neuron < layer.size() - 1; ++neuron)
		{
			layer[neuron].UpdateInputWeights(prevLayer);
		}
	}

}

void Network::GetResults(std::vector<float>& results) const
{
	results.clear();

	const Layer& outputLayer = m_Layers.back();

	for (unsigned neuron = 0; neuron < outputLayer.size() - 1; ++neuron)
		results.push_back(outputLayer[neuron].GetOutput());
}
