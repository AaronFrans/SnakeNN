#include <cassert>
#include <fstream>
#include <filesystem>
#include "Network.h"

Network::Network(const std::vector<unsigned>& topology, bool isDrawn)
{
	m_IsDrawn = isDrawn;
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

	m_Snake = std::make_unique<Snake>(40, 40);

}

void Network::UpdateSnake()
{
	//when allive run trough nn and update direction
	//give state to nn

	if (m_IsDrawn)
	{
		Outputs targets{};

		targets = m_Snake->GetBestDir();

		FeedForward(m_Snake->GetState());

		Outputs results{};

		GetResults(results);

		//sample from results

		m_Snake->UpdateDir(results);


		BackProp(targets);
		//check direction for + point

		m_Snake->Update();
	}
	else
	{
		for (int i = 0; i < 100; i++)
		{

			Outputs targets{};

			targets = m_Snake->GetBestDir();

			FeedForward(m_Snake->GetState());

			Outputs results{};

			GetResults(results);

			//sample from results

			m_Snake->UpdateDir(results);


			BackProp(targets);
			//check direction for + point

			m_Snake->Update();
			if (m_Snake->IsDead())
				break;
		}
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

void Network::FeedForward(const Inputs& input)
{
	std::vector<float> inputs{};
	input.ToVector(inputs);
	FeedForward(inputs);
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

void Network::BackProp(const Outputs& targets)
{

	std::vector<float> outputs{};
	targets.ToVector(outputs);
	BackProp(outputs);
}

void Network::GetResults(std::vector<float>& results) const
{
	results.clear();

	const Layer& outputLayer = m_Layers.back();

	for (unsigned neuron = 0; neuron < outputLayer.size() - 1; ++neuron)
		results.push_back(outputLayer[neuron].GetOutput());
}

void Network::GetResults(Outputs& output) const
{
	const Layer& outputLayer = m_Layers.back();

	output = Outputs::FromLayer(outputLayer);
}

void Network::SerializeLayers(std::wstring path, Network* n)
{
	std::filesystem::path extension{ std::filesystem::path(path).extension() };
	if (extension == "")
	{
		path += L".snn";
	}
	else if (extension == ".")
	{
		path += L"snn";
	}
	std::wofstream networkFile{};

	networkFile.open(path);

	for (auto& layer : n->m_Layers)
	{
		networkFile << "Layer:" << std::endl;
		for (auto& neuron : layer)
		{
			networkFile << "Neuron:" << std::endl;
			networkFile << "Output: " << neuron.GetOutput() << std::endl;
			networkFile << "OutputWeights:" << std::endl;
			for (auto& outputsWeight : neuron.GetOutputWeights())
			{
				networkFile << "Weight: " << outputsWeight.Weight << std::endl;
				networkFile << "DeltaWeight: " << outputsWeight.DeltaWeight << std::endl;
			}
			networkFile << "MyIndex: " << neuron.GetIndex() << std::endl;
			networkFile << "Gradient: " << neuron.GetGradient() << std::endl;
		}
	}

	if (networkFile.is_open())
	{
		networkFile.close();
	}
}

void Network::DeserializeLayers(std::wstring path, std::vector<Layer>& layers)
{
	std::wifstream networkFile{};

	networkFile.open(path);

	std::wstring line;
	while (!networkFile.eof())
	{
		std::getline(networkFile, line);

		std::wstringstream ss{ line };

		std::wstring label;

		ss >> label;

		if (label.compare(L"Layer:") == 0)
		{
			layers.push_back(Layer{});
			continue;
		}
		if (label.compare(L"Neuron:") == 0)
		{
			layers.back().push_back(Neuron{});
			continue;
		}
		if (label.compare(L"Output:") == 0)
		{
			float output;
			ss >> output;
			layers.back().back().SetOutput(output);
			continue;
		}
		if (label.compare(L"OutputWeights:") == 0)
		{

			std::vector<Connection> connections{};
			std::getline(networkFile, line);

			ss.str(line);
			ss.clear();

			label.erase();

			ss >> label;
			while (label.compare(L"Weight:") == 0)
			{
				Connection connection{};

				float weight;
				ss >> weight;
				connection.Weight = weight;

				std::getline(networkFile, line);

				ss.str(line);
				ss.clear();

				label.erase();;

				ss >> label;
				if (label.compare(L"DeltaWeight:") == 0)
				{
					float deltaWeight;
					ss >> deltaWeight;
					connection.DeltaWeight = deltaWeight;

					connections.push_back(connection);
				}

				std::getline(networkFile, line);

				ss.str(line);
				ss.clear();

				label.erase();
				ss >> label;
			}
			layers.back().back().SetOutputWeights(connections);
			//no continue because index is in label
		}
		if (label.compare(L"MyIndex:") == 0)
		{
			int index;
			ss >> index;
			layers.back().back().SetIndex(index);
			continue;
		}
		if (label.compare(L"Gradient:") == 0)
		{
			float gradient;
			ss >> gradient;
			layers.back().back().SetGradient(gradient);
		}
	}

	networkFile.close();
}
