#ifndef _MY_LIB_
#define _MY_LIB_

#ifdef MY_LINUX
	#define MYLIB_API		extern
#else
	#ifdef MYLIB_EXPORT
		#define MYLIB_API	__declspec(dllexport)
	#else
		#define MYLIB_API	__declspec(dllimport)
	#endif
#endif

MYLIB_API int my_add(int num1, int num2);
MYLIB_API int my_sub(int num1, int num2);

#endif  // _MY_LIB_