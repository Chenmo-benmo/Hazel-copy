#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {

	class  Input
	{
	public:
		//inline 会尝试在每个调用该函数的地方将代码直接插入（内联展开）
		//inline 是一个有用的优化方案，用于频繁调用且小体量的函数
		//如果包含大体量函数，inline 会导致代码膨胀，从而影响缓存效率
		//但 inline 只是向编译器发出的一种建议，inline 使用不当编译器可能会忽略 inline 建议
		//
		//static 静态成员函数，表示这些函数属于类本身，而不是类的任何特定对象，只能引用静态变量。因此，它们可以通过类名直接调用，而不需要创建类的实例
		//类似于 std::bind 的作用，不过 std::bind 是创建了一个可调用的成员函数，支持绑定任意数量的参数，并在运行时动态地指向这些参数
		//当结构体内无构造函数且全是静态成员函数，那么该结构体作用类似于 namespace，可以用 :: 调用静态成员函数
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		//std::pair 相当于创建一个拥有两个成员变量的结构体，可以使用{ ，}赋值也可以使用 std::make_pair() 赋值，内部>,<,==等都可直接使用
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