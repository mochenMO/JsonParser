

/*// ��Ŀ˵��
* ���⣺
* 1.����ʶ���ѧ������������
*	12.2e-2   =>  0.122
*   12.3e3    =>  12300
*   12.23e    =>  ����
* 
* 2.��\�ַ�������һЩ����
* 
* 3.string �� const char* �Ĵ���������C++17�� std::string_view
* 
* 4.�����õ���ǳ��������ʵ�ʽ��������
* 
* 5.û��ʵ��RAII���ƣ�Ҫ�ֶ����ٶ���
* 
*/





#define _CRTDBG_MAP_ALLOC   // ʹ��ӡ��й©�������ϸ
#include <stdlib.h>  
#include <crtdbg.h>  


#include <iostream>
#include <string>
#include <fstream>
#include <string_view>

#include <windows.h>   // ���ڼ�������ʱ��

#include "moJson/moJson.h"




int main()
{
	{   // �޶�������ʹSTL�Զ��ͷţ���������ڴ��⺯����Ӱ�죨Ҳ����ר��дһ��test������

		std::ifstream fin("./test/test.json");
		std::stringstream ss;
		ss << fin.rdbuf();
		const std::string& str = ss.str();


		LARGE_INTEGER frequency, start, end;

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);

		int times = 10000;   // ���Դ�����������Ϊ10000�Σ��������̫�ٻ᲻׼ȷ

		for (int i = 0; i < times; i++) {
			moJson::JsonParser jp(str);
			moJson::Json json = jp.parse();
		}

		QueryPerformanceCounter(&end);
		double elapsed = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

		std::cout << elapsed / times << std::endl;    // ��λ����
	}

	_CrtDumpMemoryLeaks();
	return 0;
}






/*// ֪ʶ����
*  1.operator= 
*		operator= �ķ���ֵ����һ���ǵ����������ͣ�������ʵ��a=b=c�����Ȳ���
*		ͨ��operator= ��ʾ���¸�ֵ����˿�ͷҪ��clear()���ͷŶ���ԭ�е���Դ
* 
*  2.Ҫ֪��operator[]��at������
* 
*  3.��Ҫ�����������÷���ֵΪ�������ͣ�Ҫע��𷵻ؾֲ�������������
* 
*  4._CrtDumpMemoryLeaks()
*		�ڲ���ʱҪ��main��������ֶ�������������
*		��main�����е�����STL��Ӱ�������������ڲ���ʱҲҪ��main��������ֶ�������������������дһ��test������������{}�޶�������
*		���������ڴ��С�����ǲ�׼ȷ�ģ������ÿ��գ���������ڴ��С��׼ȷ
*		�����_CrtSetBreakAlloc()��λ������ֱ�Ӷ�λ��malloc���ļ��У���������STLû�ͷ�
* 
*  ������ʵ��һ��var
* 
*/