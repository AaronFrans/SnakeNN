#ifndef UTILS
#define UTILS

namespace Utils 
{
	void ShowVectorVals(std::wstring label, std::vector<float> &vector)
	{
		OutputDebugString(label.c_str());

		for (auto& item : vector)
		{
			std::wstring itemInfo{};
			itemInfo +=  std::to_wstring(item) + L"  \n";
			OutputDebugString(itemInfo.c_str());
		}
	}

}


#endif

