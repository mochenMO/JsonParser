
/*// 项目说明
* 1.版本
* (1).C++11
*
* 2.缺陷
* (1).没有用C++17中的string_view，造成很多字符串的拷贝，空间上的有所浪费
* (2).没有专门设计和实现const class 的相关函数
* (3).没有专门设置nocepect 和 inline
* (4).用int代替了size_t,因为int类型字面值0，优先隐式转换成const char*而不是size_t,从而导致调用时多个函数参数冲突
* (5).没有对\u和\b的转义字符，进行专门的处理
*
* 3.待完善的功能
* (1).解决部分缺陷问题,版本升到C++17
* (2).增强Json::str()的功能,包括提高写入的double的精度
* (3).补充operator=函数,现在的operator=函数会调用构造函数,效率有点低
* (3).在Json中实现一个parse()函数,简化操作不用每次都显式的创建一个JsonParser再parse了
* (4).文件的读写
* (5).序列化与反序列化
* (6).添加日志系统
* (7).实现SAX和COM风格的解析API(COM风格就是现在写的代码,其最大的特点是解析整个文件,COM风格适用于要经常修改的场景;
*	  SAX风格就是以事件为导向,只解析指定的部分,SAX风格只能进行读取得操作,不能修改,删除或添加节点)
* 
* (8).operator[]真的需要动态性吗,用 if(m_type != json_array){throw std::logic_error("type error, not json_object value");}
*	  会不会更好呢？要不要专门写一个find函数？(查看测试用例！！！) (解决方法，operator[]只有修改权限，没有创建权限)
*	  通常operator[]有修改权限,没有创建的权限。
*/

/*// Json规范
* (1).在json_object中同级的键值，不允许有相同的key
* (2).空的json_array即[]是没有0下标的，因为json_array一个元素都没有
* 
* 
*/

/*// STL规范
* (1).STL容器不能存引用,通常存的值,特殊情况存指针。
*
*
*
*/

/*// 下一个目标
* (1).文件的读写,定点的局部读写,startPos,endPos
* (2).序列化与反序列化
* (3).版本升到C++17
* (4).何时把m_index归零,怎么把m_index归零
* 
* 
*/

/*// 说明
* (1).如果只是只读,或者进行操作查询,建议用JsonParser的operator[]它速度很快，且不会改变原数据，查询失败则报错
* 如果用Json中的operator[],由于其极高的灵活性或创建权限,有时即使查询失败也不会报错,且会不小心创建新的对象
*
*
*/


#pragma once
#ifndef _MOJSON_H_
#define _MOJSON_H_


#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <stack>


namespace moJson
{
	

class Json    // 大小是16字节
{
public:
	enum Type
	{
		json_null = 0,
		json_bool,
		json_int,
		json_double,
		json_string,
		json_array,
		json_object,
	};
private:
	union Value
	{
		bool m_bool;
		int m_int;
		double m_double;
		std::string* m_string;
		std::vector<Json>* m_array;
		std::unordered_map<std::string, Json>* m_object;
	};

	Type m_type;
	Value m_value; 

public:
	Json();
	Json(bool _value);
	Json(int _value);
	Json(double _value);
	Json(const char* _value);
	Json(std::string _value);
	Json(Type _type);

	void assisted_deep_copy(const Json& _other);
	Json(const Json& _other);
	void operator=(const Json& _other);
	
	Json(Json&& _other) noexcept;                
	void operator=(Json&& _other) noexcept;      

	void operator=(bool _value);
	void operator=(int _value);
	void operator=(double _value);
	void operator=(const char* _value);
	void operator=(std::string _value);

	void clear(); 
	~Json();

	Json& operator[](int _index);          // []有自动扩容的功能
	void append(const Json& _other);       // 注意是const类型
	void append(Json&& _other) noexcept;

	Json& operator[](const char* _key);  // []可以添加新的键值对
	Json& operator[](std::string _key);  // []可以添加新的键值对
	void insert(const char* _key, const Json& _other);
	void insert(std::string _key, const Json& _other);
	void insert(const char* _key, Json&& _other);
	void insert(std::string _key, Json&& _other);

	operator bool&();   // 类型转换运算符重载, operator T&(); 包括引用和值，两种类型的转换
	operator int&();
	operator double&();
	operator std::string&();

	std::string to_string();

	bool get_bool();
	int get_int();
	double get_double();
	std::string& get_string();
	std::vector<Json>& get_array();
	std::unordered_map<std::string,Json>& get_object();

	bool is_null();
	bool is_bool();
	bool is_int();
	bool is_double();
	bool is_string();
	bool is_array();
	bool is_object();

	bool has_value(int _index);
	bool has_value(const char* _key);
	bool has_value(std::string _key);

	void remove(int _index);
	void remove(const char* _key);
	void remove(std::string _key);
};




class JsonParser
{
private:
	std::string m_str;  // 没必要用指针或者引用，避免不必要的内存申请
	int m_index;
	
public:
	JsonParser();
	JsonParser(const char* _str);
	JsonParser(std::string _str);
	
	JsonParser(const JsonParser&) = default; // 拷贝和移动操作都让系统默认实现
	JsonParser& operator=(const JsonParser&) = default;
	JsonParser(JsonParser&&) = default; 
	JsonParser& operator=(JsonParser&&) = default;

	void load_str(const char* _str);
	void load_str(std::string _str);

	char get_next_token();
	Json parse_null();     
	Json parse_bool();
	Json parse_number();
	std::string parse_string();  // 为什么返回string而不是Json,因为key是string便于操作
	Json parse_array();
	Json parse_object();

	Json parse();


	JsonParser& operator[](const char* _key);
	JsonParser& operator[](std::string _key);
	JsonParser& operator[](int _index);
};






};








#endif // !_MOJSON_H_









