#pragma once

#include <memory>

#ifdef HZ_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)//__declspec(dllexport)��dll�е��� ��������������
	#else
		#define HAZEL_API __declspec(dllimport)//dllimport��dll�е���
	#endif
#else
	#define HAZEL_API
#endif
#else
	#error "Hazel only supports Windows!"
#endif

//ʹ��Ԥ��������ƽ̨���
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
//	/* TARGET_OS_MAC ��־����������ƽ̨�ϣ�
//	 * ������Ǳ��������ƽ̨���м�飨���մ�˳��
//	 * ��ȷ������������ƻ�� MAC ϵͳ�ϣ�
//	 * ����������ƻ��ƽ̨�� */
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
///* ������Ҫ�ڼ�� __linux__ ֮ǰ��� __ANDROID__
// * ��Ϊ andorid �ǻ��� linux �ں˿�����
// * �������Ѿ������� __linux__ */
//#elif defined(__ANDROID__)
//	#define HZ_PLATFORM_ANDROID
//	#error "Android is not supported!"
//#elif defined(__linux__)
//	#define HZ_PLATFORM_LINUX
//	#error "Linux is not supported!"
//#else
//	/* δ֪ ������/ƽ̨ */
//	#error "Unknown platform!"
//#endif // ƽ̨������

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

//std::bind ������Ա����ָ�� &fn �Ͷ���ʵ�� this ����һ�𣬴���һ�����Զ������õĿɵ��ö���
//��Ա����ָ�����ͨ�ĺ���ָ�벻ͬ����Ա����ָ����Ҫһ������ʵ��������
//��Ա����ָ��� std::function �����ݣ�std::function ����һ���ɵ��ö������Գ�Ա����ָ����Ҫʹ�� std::bind ����һ���µĿɵ��ö���
//std::placeholders::_1 �� this ��Ϊ�󶨺����ĵ�һ���������
#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {

	//�� Hazel::Scope<> ��װ std::unique_ptr<>
	template<typename T>
	using Scope = std::unique_ptr<T>;

	//�� Hazel::Ref<> ��װ std::shared_ptr<>
	template<typename T>
	using Ref = std::shared_ptr<T>;
}