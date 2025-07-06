#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {

	class  Input
	{
	public:
		//inline �᳢����ÿ�����øú����ĵط�������ֱ�Ӳ��루����չ����
		//inline ��һ�����õ��Ż�����������Ƶ��������С�����ĺ���
		//�������������������inline �ᵼ�´������ͣ��Ӷ�Ӱ�컺��Ч��
		//�� inline ֻ���������������һ�ֽ��飬inline ʹ�ò������������ܻ���� inline ����
		//
		//static ��̬��Ա��������ʾ��Щ���������౾������������κ��ض�����ֻ�����þ�̬��������ˣ����ǿ���ͨ������ֱ�ӵ��ã�������Ҫ�������ʵ��
		//������ std::bind �����ã����� std::bind �Ǵ�����һ���ɵ��õĳ�Ա������֧�ְ����������Ĳ�������������ʱ��̬��ָ����Щ����
		//���ṹ�����޹��캯����ȫ�Ǿ�̬��Ա��������ô�ýṹ������������ namespace�������� :: ���þ�̬��Ա����
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		//std::pair �൱�ڴ���һ��ӵ��������Ա�����Ľṹ�壬����ʹ��{ ��}��ֵҲ����ʹ�� std::make_pair() ��ֵ���ڲ�>,<,==�ȶ���ֱ��ʹ��
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};

}