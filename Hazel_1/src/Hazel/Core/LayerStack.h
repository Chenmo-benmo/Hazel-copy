#pragma once

#include "Hazel/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

	class  LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline int GetLayerLength() const { return m_Layers.size(); }

		//提供了 Layer* 的开始和结束迭代器
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		//std::vector<Layer*>::iterator m_LayerInsert; //指向 Layer* 的迭代器
		unsigned int m_LayerInsertIndex = 0;
	};
}