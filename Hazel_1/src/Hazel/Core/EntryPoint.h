#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)//Èë¿Úµã
{
	Hazel::Log::Init();
	HZ_CORE_ERROR("Initialized Log!");
	int a = 5;
	HZ_CRITICAL("Hello! var = {0}", a);

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif 