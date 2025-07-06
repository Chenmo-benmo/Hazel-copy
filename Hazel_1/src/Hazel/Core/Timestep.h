#pragma once

namespace Hazel {

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{

		}

		//当类的对象被用在需要 float 类型的场景时，此运算符会被自动调用
		//Timestep ts; ts可以直接返回m_Time
		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMillionseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}