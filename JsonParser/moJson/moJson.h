#pragma once
#ifndef _MOJSON_H_
#define _MOJSON_H_



#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>



/*// ��Ŀ˵��
* ���⣺
* 1.����ʶ���ѧ������������
*	12.2e-2   =>  0.122
*   12.3e3    =>  12300
*   12.23e    =>  ����
*
* 2.��\�ַ�������һЩ����
*
* 3.string �� const char* �Ĵ�����������C++17�� std::string_view
*
* 4.�����õ���ǳ��������ʵ�ʽ��������
*
* 5.û��ʵ��RAII���ƣ�Ҫ�ֶ����ٶ���
*
*/



/*// ��Ŀ˵�� 
* 1.
*
* 2.ȱ��
* (1).��Json������ʵ����Json�ľ���������ܣ�������һЩûʵ�֡�����������
* (2).û����C++17�е�string_view����ɺܶ��ַ����Ŀ������ռ��ϵ������˷�
* (3).û����ƺ�ʵ��const class ����غ���
* (4).û������nocepect �� inline
*/



namespace moJson
{
	

class Json    // ��С��16�ֽ�
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
	
	Json(Json&& _other) noexcept;                // �����⣿������
	void operator=(Json&& _other) noexcept;      // �����⣿������

	void clear(); 
	~Json();

	Json& operator[](size_t _index);       // []���Զ����ݵĹ���
	void append(const Json& _other);    // ע����const����
	void append(Json&& _other) noexcept;

	Json& operator[](const char* _key);  // []���������µļ�ֵ��
	Json& operator[](std::string _key);  // []���������µļ�ֵ��

	//operator bool();   // ����ת�����������
	//operator int();
	//operator double();
	//operator std::string();

	operator bool&();   // ����ת�����������
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

	bool has_value(size_t _index);
	bool has_value(const char* _key);
	bool has_value(std::string _key);

	void remove(size_t _index);
	void remove(const char* _key);
	void remove(std::string _key);
};




class JsonParser
{
private:
	std::string m_str;  // û��Ҫ��ָ��������ã����ⲻ��Ҫ���ڴ�����
	size_t m_index;
	
public:
	JsonParser();
	JsonParser(const char* _str);
	JsonParser(std::string _str);
	
	JsonParser(const JsonParser&) = default; // �������ƶ���������ϵͳĬ��ʵ��
	JsonParser& operator=(const JsonParser&) = default;
	JsonParser(JsonParser&&) = default; 
	JsonParser& operator=(JsonParser&&) = default;

	void load_str(const char* _str);
	void load_str(std::string _str);

	char get_next_token();
	Json parse_null();     // Ϊʲô����ֵ���������ã� 
	Json parse_bool();
	Json parse_number();
	// std::string parse_string_escape();  // Ϊʲô����string������Json,��Ϊkey��string���ڲ���
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




/*
* 
* ���һ����������ȱ�ݣ�������������ŵ������ܹ���ȥʱ���������У�������������Ϊ
* ע�����ȱ�ݣ����ᱨ�����쳣��
* 
* ��
* jp.parse(R(""""));   ����string("")
* 
* ����Ҫ��֤������������ȷ�ԣ����������������ȷ������ֻ�вο���ֵ
* 
*/



#endif // !_MOJSON_H_







