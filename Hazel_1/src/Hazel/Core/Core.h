#pragma once

#include <memory>

#ifdef HZ_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)//__declspec(dllexport)从dll中导出 函数、变量、类
	#else
		#define HAZEL_API __declspec(dllimport)//dllimport从dll中导入
	#endif
#else
	#define HAZEL_API
#endif
#else
	#error "Hazel only supports Windows!"
#endif

//使用预定义宏进行平台检测
//#ifdef _WIN32
//	/* Windows x64/x86 */
//	#ifdef _WIN64
//		/* Windows x64 */
//		#define HZ_PLATFORM_WINDOWS
//	#else
//		/* Windows x86 */
//		#error "x86 Builds are not supported!"
//	#endif
//#elif defined(__APPLE__) || defined(__MACH__)
//	#include <TargetConditionals.h>
//	/* TARGET_OS_MAC 标志存在于所有平台上，
//	 * 因此我们必须对所有平台进行检查（按照此顺序）
//	 * 以确保我们运行于苹果 MAC 系统上，
//	 * 而不是其他苹果平台上 */
//	#if TARGET_OS_SIMULATOR == 1
//		#error "IOS simulator is not supported!"
//	#elif TARGET_OS_IPHONE == 1
//		#define HZ_PLATFORM_IOS
//		#error "IOS is not supported!"
//	#elif TARGET_OS_MAC == 1
//		#define HZ_PLATFORM_MACOS
//		#error "MacOS is not supported!"
//	#else
//		#error "Unknown Apple platform!"
//	#endif
///* 我们需要在检查 __linux__ 之前检查 __ANDROID__
// * 因为 andorid 是基于 linux 内核开发的
// * 所以它已经定义了 __linux__ */
//#elif defined(__ANDROID__)
//	#define HZ_PLATFORM_ANDROID
//	#error "Android is not supported!"
//#elif defined(__linux__)
//	#define HZ_PLATFORM_LINUX
//	#error "Linux is not supported!"
//#else
//	/* 未知 编译器/平台 */
//	#error "Unknown platform!"
//#endif // 平台检测结束

#ifdef HZ_DUBUG
	#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if (!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define HZ_CORE_ASSERT(x, ...) { if (!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

//std::bind 允许将成员函数指针 &fn 和对象实例 this 绑定在一起，创建一个可以独立调用的可调用对象
//成员函数指针和普通的函数指针不同，成员函数指针需要一个对象实例来调用
//成员函数指针和 std::function 不兼容，std::function 期望一个可调用对象，所以成员函数指针需要使用 std::bind 创建一个新的可调用对象
//std::placeholders::_1 将 this 作为绑定函数的第一个输入变量
#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {

	//用 Hazel::Scope<> 包装 std::unique_ptr<>
	template<typename T>
	using Scope = std::unique_ptr<T>;

	//用 Hazel::Ref<> 包装 std::shared_ptr<>
	template<typename T>
	using Ref = std::shared_ptr<T>;
}