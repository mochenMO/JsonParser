

/*// 项目说明
* 问题：
* 1.不能识别科学技术法的数字
*	12.2e-2   =>  0.122
*   12.3e3    =>  12300
*   12.23e    =>  错误
* 
* 2.对\字符处理有一些问题
* 
* 3.string 和 const char* 的处理，可以用C++17的 std::string_view
* 
* 4.拷贝用的是浅拷贝，但实际建议用深拷贝
* 
* 5.没有实现RAII机制，要手动销毁对象
* 
*/





#define _CRTDBG_MAP_ALLOC   // 使打印的泄漏报告更详细
#include <stdlib.h>  
#include <crtdbg.h>  


#include <iostream>
#include <string>
#include <fstream>
#include <string_view>

#include <windows.h>   // 用于计算运行时间

#include "moJson/moJson.h"




int main()
{
	{   // 限定作用域，使STL自动释放，避免其对内存检测函数的影响（也可以专门写一个test函数）

		std::ifstream fin("./test/test.json");
		std::stringstream ss;
		ss << fin.rdbuf();
		const std::string& str = ss.str();


		LARGE_INTEGER frequency, start, end;

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);

		int times = 10000;   // 测试次数建议设置为10000次，如果次数太少会不准确

		for (int i = 0; i < times; i++) {
			moJson::JsonParser jp(str);
			moJson::Json json = jp.parse();
		}

		QueryPerformanceCounter(&end);
		double elapsed = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

		std::cout << elapsed / times << std::endl;    // 单位是秒
	}

	_CrtDumpMemoryLeaks();
	return 0;
}






/*// 知识整理
*  1.operator= 
*		operator= 的返回值，不一定非得是引用类型，除非想实现a=b=c的连等操作
*		通常operator= 表示重新赋值，因此开头要先clear()，释放对象原有的资源
* 
*  2.要知道operator[]和at的区别
* 
*  3.不要动不动就设置返回值为引用类型，要注意别返回局部变量的引用了
* 
*  4._CrtDumpMemoryLeaks()
*		在测试时要在main函数最后，手动调用析构函数
*		在main函数中的其他STL会影响最后结果，因此在测试时也要在main函数最后，手动调用析构函数。或则写一个test函数，或者用{}限定作用域
*		最后输出的内存大小可能是不准确的，建议用快照，它测出的内存大小更准确
*		如果用_CrtSetBreakAlloc()定位函数，直接定位到malloc库文件中，则大概率是STL没释放
* 
*  核心是实现一个var
* 
*/