#pragma once

namespace Hazel {

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{

		}

		//����Ķ���������Ҫ float ���͵ĳ���ʱ����������ᱻ�Զ�����
		//Timestep ts; ts����ֱ�ӷ���m_Time
		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMillionseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}