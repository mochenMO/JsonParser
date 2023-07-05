#pragma once
#ifndef _MOJSON_H_
#define _MOJSON_H_


#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>



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
*/


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
	
	// Json& assisted_deep_copy(const Json& _other);

	void assisted_deep_copy(const Json& _other);
	Json(const Json& _other);
	void operator=(const Json& _other);
	
	Json(Json&& _other) noexcept;                // 有问题？？？？
	void operator=(Json&& _other) noexcept;      // 有问题？？？？

	void clear(); 
	~Json();

	Json& operator[](int _index);       // []有自动扩容的功能
	void append(const Json& _other);       // 注意是const类型
	void append(Json&& _other) noexcept;

	Json& operator[](const char* _key);  // []可以添加新的键值对
	Json& operator[](std::string _key);  // []可以添加新的键值对

	//operator bool();   // 类型转换运算符重载
	//operator int();
	//operator double();
	//operator std::string();

	operator bool&();   // 类型转换运算符重载
	operator int&();
	operator double&();
	operator std::string&();

	std::string str();

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
	Json parse_null();     // 为什么返回值而不是引用？ 
	Json parse_bool();
	Json parse_number();
	// std::string parse_string_escape();  // 为什么返回string而不是Json,因为key是string便于操作
	// std::string pase_string_unescaped();
	std::string parse_string();
	Json parse_array();
	Json parse_object();

	// Json parse_escape();
	// Json parse_unescaped();
	// Json parse(bool _escape = false);
	Json parse();
};








};








#endif // !_MOJSON_H_









