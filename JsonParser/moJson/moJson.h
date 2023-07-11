
/*// ��Ŀ˵��
* 1.�汾
* (1).C++11
*
* 2.ȱ��
* (1).û����C++17�е�string_view����ɺܶ��ַ����Ŀ������ռ��ϵ������˷�
* (2).û��ר����ƺ�ʵ��const class ����غ���
* (3).û��ר������nocepect �� inline
* (4).��int������size_t,��Ϊint��������ֵ0��������ʽת����const char*������size_t,�Ӷ����µ���ʱ�������������ͻ
* (5).û�ж�\u��\b��ת���ַ�������ר�ŵĴ���
*
* 3.�����ƵĹ���
* (1).�������ȱ������,�汾����C++17
* (2).��ǿJson::str()�Ĺ���,�������д���double�ľ���
* (3).����operator=����,���ڵ�operator=��������ù��캯��,Ч���е��
* (3).��Json��ʵ��һ��parse()����,�򻯲�������ÿ�ζ���ʽ�Ĵ���һ��JsonParser��parse��
* (4).�ļ��Ķ�д
* (5).���л��뷴���л�
* (6).�����־ϵͳ
* (7).ʵ��SAX��COM���Ľ���API(COM����������д�Ĵ���,�������ص��ǽ��������ļ�,COM���������Ҫ�����޸ĵĳ���;
*	  SAX���������¼�Ϊ����,ֻ����ָ���Ĳ���,SAX���ֻ�ܽ��ж�ȡ�ò���,�����޸�,ɾ������ӽڵ�)
* 
* (8).operator[]�����Ҫ��̬����,�� if(m_type != json_array){throw std::logic_error("type error, not json_object value");}
*	  �᲻������أ�Ҫ��Ҫר��дһ��find������(�鿴��������������) (���������operator[]ֻ���޸�Ȩ�ޣ�û�д���Ȩ��)
*	  ͨ��operator[]���޸�Ȩ��,û�д�����Ȩ�ޡ�
*/

/*// Json�淶
* (1).��json_object��ͬ���ļ�ֵ������������ͬ��key
* (2).�յ�json_array��[]��û��0�±�ģ���Ϊjson_arrayһ��Ԫ�ض�û��
* 
* 
*/

/*// STL�淶
* (1).STL�������ܴ�����,ͨ�����ֵ,���������ָ�롣
*
*
*
*/

/*// ��һ��Ŀ��
* (1).�ļ��Ķ�д,����ľֲ���д,startPos,endPos
* (2).���л��뷴���л�
* (3).�汾����C++17
* (4).��ʱ��m_index����,��ô��m_index����
* 
* 
*/

/*// ˵��
* (1).���ֻ��ֻ��,���߽��в�����ѯ,������JsonParser��operator[]���ٶȺܿ죬�Ҳ���ı�ԭ���ݣ���ѯʧ���򱨴�
* �����Json�е�operator[],�����伫�ߵ�����Ի򴴽�Ȩ��,��ʱ��ʹ��ѯʧ��Ҳ���ᱨ��,�һ᲻С�Ĵ����µĶ���
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

	Json& operator[](int _index);          // []���Զ����ݵĹ���
	void append(const Json& _other);       // ע����const����
	void append(Json&& _other) noexcept;

	Json& operator[](const char* _key);  // []��������µļ�ֵ��
	Json& operator[](std::string _key);  // []��������µļ�ֵ��
	void insert(const char* _key, const Json& _other);
	void insert(std::string _key, const Json& _other);
	void insert(const char* _key, Json&& _other);
	void insert(std::string _key, Json&& _other);

	operator bool&();   // ����ת�����������, operator T&(); �������ú�ֵ���������͵�ת��
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
	std::string m_str;  // û��Ҫ��ָ��������ã����ⲻ��Ҫ���ڴ�����
	int m_index;
	
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
	Json parse_null();     
	Json parse_bool();
	Json parse_number();
	std::string parse_string();  // Ϊʲô����string������Json,��Ϊkey��string���ڲ���
	Json parse_array();
	Json parse_object();

	Json parse();


	JsonParser& operator[](const char* _key);
	JsonParser& operator[](std::string _key);
	JsonParser& operator[](int _index);
};






};








#endif // !_MOJSON_H_









