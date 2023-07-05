#define _CRTDBG_MAP_ALLOC   // 使打印的泄漏报告更详细
#include <stdlib.h>         
#include <crtdbg.h>          
#include <windows.h>        // 用于计算运行时间

#include <iostream>
#include <string>
#include <fstream>

#include "moJson/moJson.h"



void test01()
{
	auto str = R"({
	  "null": null,
	  "bool": false,
	  "integer": 42,
	  "decimal": 3.14,
	  "negative": -10,
	  "scientific_positive": 2.99792458e8,
	  "scientific_negative_decimal": -1.23e-5,
	  "string": "Hello, world!",
	  "array": [1, "two", [3, "four"], {"key": "value"}],
	  "object": {
	    "number": 123,
	    "text": "example",
	    "nested_array": [77, 88, 99],
	    "nested_object": {"inner_key": "inner_value"}
	  }
	})";

	moJson::JsonParser jp(str);
	moJson::Json json = jp.parse();
	std::cout << json.str() << std::endl;

	bool v1 = json["bool"];
	int v2 = json["integer"];
	double v3 = json["decimal"];
	std::string v4 = json["string"];
	
	std::cout << v1 << std::endl;
	std::cout << v2 << std::endl;
	std::cout << v3 << std::endl;
	std::cout << v4 << std::endl;

	std::cout << json["array"][2][1].get_string() << std::endl;
	std::cout << json["array"][3]["key"].get_string() << std::endl;
	std::cout << json["object"]["nested_array"][2].get_int() << std::endl;
}


void test01()
{

	moJson::Json j1;
	moJson::Json j2 = false;
	moJson::Json j3 = 123;
	moJson::Json j4 = 12.344;
	moJson::Json j5 = "string";

	moJson::Json arr;
	
	// arr[0] = 123;


	//std::cout << json.str() << std::endl;

	//bool v1 = json["bool"];
	//int v2 = json["integer"];
	//double v3 = json["decimal"];
	//std::string v4 = json["string"];


	//std::cout << v1 << std::endl;
	//std::cout << v2 << std::endl;
	//std::cout << v3 << std::endl;
	//std::cout << v4 << std::endl;

	//std::cout << v1 << std::endl;
	//std::cout << v2 << std::endl;
	//std::cout << v3 << std::endl;
	//std::cout << v4 << std::endl;


}






int main()
{
	test01();


	{   // 限定作用域，使STL自动释放，避免其对内存检测函数的影响（也可以专门写一个test函数）

		std::ifstream fin("./test/test.json");
		std::stringstream ss;
		ss << fin.rdbuf();
		const std::string& str = ss.str();


		LARGE_INTEGER frequency, start, end;

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);

		int times = 1;   // 测试次数建议设置为10000次，如果次数太少会不准确

		for (int i = 0; i < times; i++) {
			moJson::JsonParser jp(str);
			moJson::Json json = jp.parse();
		}

		QueryPerformanceCounter(&end);
		double elapsed = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

		std::cout << "runTime: " << elapsed / times << std::endl;    // 单位是s秒
	}

	_CrtDumpMemoryLeaks();
	return 0;
}





