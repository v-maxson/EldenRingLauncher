#pragma once


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#define main() WINAPI WinMain(							\
					_In_ HINSTANCE hInstance,			\
					_In_opt_ HINSTANCE hPrevInstance,	\
					_In_ LPSTR     lpCmdLine,			\
					_In_ int       nShowCmd				\
				)					
#elif __APPLE__ || __linux__
#define main() main()
#endif