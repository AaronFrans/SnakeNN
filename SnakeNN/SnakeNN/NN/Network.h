#ifndef NETWORK_CLASS
#define NETWORK_CLASS
#include "framework.h"

#include <vector>
#include <memory>
#include <string>
#include "Neuron.h"
#include "DataStruct.h"
#include "Snake\Snake.h"

class Neuron;

typedef std::vector<Neuron> Layer;

class Network
{
public:

	Network(const std::vector<unsigned>& topology, bool isDrawn = true);


	void UpdateSnake();

	void FeedForward(const std::vector<float>& input);
	void FeedForward(const Inputs& input);
	void BackProp(const std::vector<float>& targets);
	void BackProp(const Outputs& targets);
	void GetResults(std::vector<float>& results) const;
	void GetResults(Outputs& output) const;

	float GetRecentAverageError() const { return m_RecentAverageError; };

	Snake* GetSnake() const { return m_Snake.get(); };
	Snake* ResetSnake() { m_Snake.reset(); m_Snake = std::make_unique<Snake>(40, 40); return m_Snake.get(); };

	std::vector<Layer> GetLayerInfo()const { return m_Layers; };
	void SetLayerInfo(const std::vector<Layer> newLayers) { m_Layers = newLayers; };


	static void SerializeLayers(std::wstring path, Network* n);
	static void DeserializeLayers(std::wstring path, std::vector<Layer>& layers);

	bool IsDrawn() const { return m_IsDrawn; };
private:

	std::vector<Layer> m_Layers;


	std::unique_ptr<Snake> m_Snake;

	float m_Error{};
	float m_RecentAverageError{};
	float m_RecentAverageSmoothingFactor{};

	bool m_IsDrawn{};
};
#endif

