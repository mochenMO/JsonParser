#include "moJson.h"


using namespace moJson;


Json::Json() :m_type(json_null)
{

}

Json::Json(bool _value) : m_type(json_bool)
{
	m_value.m_bool = _value;
}

Json::Json(int _value) : m_type(json_int)
{
	m_value.m_int = _value;
}

Json::Json(double _value) : m_type(json_double)
{
	m_value.m_double = _value;
}


Json::Json(const char* _value) : m_type(json_string)
{
	m_value.m_string = new std::string(_value);
}


Json::Json(std::string _value) : m_type(json_string)
{
	m_value.m_string = new std::string(_value);
}


Json::Json(Type _type) : m_type(_type)
{
	switch (_type)
	{
	case Json::json_null:
		break;
	case Json::json_bool:
		m_value.m_bool = true;
		break;
	case Json::json_int:
		m_value.m_int = 0;
		break;
	case Json::json_double:
		m_value.m_double = 0.0;
		break;
	case Json::json_string:
		m_value.m_string = new std::string();
		break;
	case Json::json_array:
		m_value.m_array = new std::vector<Json>();
		break;
	case Json::json_object:
		m_value.m_object = new std::unordered_map<std::string, Json>();
		break;
	default:
		break;
	}
}


//Json& Json::assisted_deep_copy(const Json& _other)
//{
//	m_type = _other.m_type;
//	switch (m_type)
//	{
//	case Json::json_null:
//		break;
//	case Json::json_bool:
//		m_value.m_bool = _other.m_value.m_bool;
//		break;
//	case Json::json_int:
//		m_value.m_int = _other.m_value.m_int;
//		break;
//	case Json::json_double:
//		m_value.m_double = _other.m_value.m_double;
//		break;
//	case Json::json_string:
//		m_value.m_string = new std::string(_other.m_value.m_string->c_str());
//		break;
//	case Json::json_array:
//	{
//		int _size = _other.m_value.m_array->size();
//		m_value.m_array = new std::vector<Json>(_size);
//		for (int i = 0; i < _size; i++) {
//			m_value.m_array->at(i) = assisted_deep_copy(_other.m_value.m_array->at(i));
//		}
//		break;
//	}
//	case Json::json_object:
//	{
//		m_value.m_object = new std::unordered_map<std::string, Json>();
//		for(auto it = m_value.m_object->begin(); it!= m_value.m_object->end(); ++it){
//			(*(m_value.m_object))[it->first] = assisted_deep_copy(it->second);    // 不能用at(),因为如果key不存在会报异常
//		}
//		break;
//	}
//	default:
//		break;
//	}
//	return *this;
//}


void Json::assisted_deep_copy(const Json& _other)
{
	m_type = _other.m_type;
	switch (m_type)
	{
	case Json::json_null:
		break;
	case Json::json_bool:
		m_value.m_bool = _other.m_value.m_bool;
		break;
	case Json::json_int:
		m_value.m_int = _other.m_value.m_int;
		break;
	case Json::json_double:
		m_value.m_double = _other.m_value.m_double;
		break;
	case Json::json_string:
		m_value.m_string = new std::string(_other.m_value.m_string->c_str());
		break;
	case Json::json_array:
	{
		int size = _other.m_value.m_array->size();
		m_value.m_array = new std::vector<Json>(size);
		for (int i = 0; i < size; i++) {
			m_value.m_array->at(i).assisted_deep_copy(_other.m_value.m_array->at(i));   
		}
		break;
	}
	case Json::json_object:
	{
		m_value.m_object = new std::unordered_map<std::string, Json>();
		for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it) {
			(*(m_value.m_object))[it->first].assisted_deep_copy(it->second);    // 不能用at(),因为如果key不存在会报异常

			// 先(*(m_value.m_object))[it->first]创建空间，再assisted_deep_copy();
		}
		break;
	}
	default:
		break;
	}
}

Json::Json(const Json& _other)
{
	//*this = assisted_deep_copy(_other);
	assisted_deep_copy(_other);
}



void Json::operator=(const Json& _other)
{
	clear();
	//*this = assisted_deep_copy(_other);
	assisted_deep_copy(_other);
}


Json::Json(Json&& _other) noexcept
{
	////*this = assisted_deep_copy(_other);
	//assisted_deep_copy(_other);
	//_other.~Json();


	// >>>
	m_type = _other.m_type;
	m_value = _other.m_value;
	_other.m_type = json_null;       // 赋值为json_null，转移控制权限
}

void Json::operator=(Json&& _other) noexcept
{
	//clear();
	////*this = assisted_deep_copy(_other);
	//assisted_deep_copy(_other);
	//_other.~Json(); 


	
	// >>>
	clear();
	m_type = _other.m_type;
	m_value = _other.m_value;
	_other.m_type = json_null;       // 赋值为json_null，转移控制权限
}


void Json::clear()
{
	switch(m_type)
	{
	case Json::json_null:
		break;
	case Json::json_bool:
		m_value.m_bool = true;
		break;
	case Json::json_int:
		m_value.m_int = 0;
		break;
	case Json::json_double:
		m_value.m_double = 0.0;
		break;
	case Json::json_string:
		delete m_value.m_string;
		break;
	case Json::json_array:
	{
		for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); ++it) {
			it->clear();
		}
		delete m_value.m_array;
		break;
	}
	case Json::json_object:
	{
		for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it) {
			it->second.clear();
		}
		delete m_value.m_object;
		break;
	}
	default:
		break;
	}
	m_type = json_null;    // 把m_type赋值为json_null
}


Json::~Json()
{
	clear();
}



Json& Json::operator[](size_t _index)
{
	if (m_type != json_array) {
		clear();
		m_type = json_array;
		m_value.m_array = new std::vector<Json>();
	}
	if (_index < 0) {
		throw new std::logic_error("json_array index < 0");
	}
	size_t size = m_value.m_array->size();
	if (_index >= size) {
		for (size_t i = size; i <= _index; ++i) {
			m_value.m_array->push_back(Json{});
		}
	}
	return m_value.m_array->at(_index);
}


void Json::append(const Json& _other)
{
	if (m_type != json_array) {
		clear();
		m_type = json_array;
		m_value.m_array = new std::vector<Json>();
	}
	m_value.m_array->push_back(_other);   // 这里调用了深拷贝
}

void Json::append(Json&& _other) noexcept
{
	if (m_type != json_array) {
		clear();
		m_type = json_array;
		m_value.m_array = new std::vector<Json>();
	}
	m_value.m_array->push_back(std::move(_other));    // 用std::move进行移动构造函数
}


Json& Json::operator[](const char* _key)
{
	return (*this)[std::string(_key)];
}

Json& Json::operator[](std::string _key)
{
	if (m_type != json_object) {
		clear();
		m_type = json_object;
		m_value.m_object = new std::unordered_map<std::string, Json>();
	}
	return (*(m_value.m_object))[_key];
}


//Json::operator bool()
//{
//	if (m_type != json_bool) {
//		throw new std::logic_error("type error, not bool value");
//	}
//	return m_value.m_bool;
//}
//
//Json::operator int() {
//	if (m_type != json_int) {
//		throw new std::logic_error("type error, not int value");
//	}
//	return m_value.m_int;
//}
//
//Json::operator double() {
//	if (m_type != json_double) {
//		throw new std::logic_error("type error, not double value");
//	}
//	return m_value.m_double;
//}
//
//Json::operator std::string() {
//	if (m_type != json_string) {
//		throw new std::logic_error("type error, not string value");
//	}
//	return *(m_value.m_string);   // 要解引用
//}


Json::operator bool&()
{
	if (m_type != json_bool) {
		throw new std::logic_error("type error, not bool value");
	}
	return m_value.m_bool;
}

Json::operator int&() {
	if (m_type != json_int) {
		throw new std::logic_error("type error, not int value");
	}
	return m_value.m_int;
}

Json::operator double&() {
	if (m_type != json_double) {
		throw new std::logic_error("type error, not double value");
	}
	return m_value.m_double;
}

Json::operator std::string&() {
	if (m_type != json_string) {
		throw new std::logic_error("type error, not string value");
	}
	return *(m_value.m_string);   // 要解引用
}


std::string Json::str()
{
	std::stringstream ss;
	switch (m_type)
	{
	case Json::json_null:
		ss << "null";
		break;
	case Json::json_bool:
		ss << (m_value.m_bool ? "true" : "false");
		break;
	case Json::json_int:
		ss << m_value.m_int;     // <<函数重载了int的版本
		break; 
	case Json::json_double:
		ss << m_value.m_double;  // <<函数重载了double的版本,注意：这里有精度限制,如果要更高精度可以用 #include <iomanip>中的  ss << std::setprecision(m_value.m_double) << std::fixed << 8; 
		break;
	case Json::json_string:
		ss << "\"" << *(m_value.m_string) << "\"";   // 注意要解引用
		break;
	case Json::json_array:
	{
		ss << "[";
		for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); ++it) {
			if (it != m_value.m_array->begin()) {
				ss << ",";
			}
			ss << it->str();
			
		}
		ss << "]";
		break;
	}
	case Json::json_object:
	{
		ss << "{";
		for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it) {
			if (it != m_value.m_object->begin()) {
				ss << ",";
			}
			ss << "\"" << it->first << "\"" << ":" << it->second.str();
		}
		ss << "}";
		break;
	}
	default:
		break;
	}
	return ss.str();
}


bool Json::get_bool() 
{
	if (m_type != json_bool) {
		throw new std::logic_error("type error,not bool value");
	}
	return m_value.m_bool;
}


int Json::get_int() 
{
	if (m_type != json_int) {
		throw new std::logic_error("type error,not int value");
	}
	return m_value.m_int;
}


double Json::get_double()
{
	if (m_type != json_double) {
		throw new std::logic_error("type error,not double value");
	}
	return m_value.m_double;
}

std::string& Json::get_string()
{
	if (m_type != json_string) {
		throw new std::logic_error("type error,not string value");
	}
	return *(m_value.m_string);
}

std::vector<Json>& Json::get_array() 
{
	if (m_type != json_array) {
		throw new std::logic_error("type error,not array value");
	}
	return *(m_value.m_array);
}

std::unordered_map<std::string, Json>& Json::get_object()
{
	if (m_type != json_object) {
		throw new std::logic_error("type error,not object value");
	}
	return *(m_value.m_object);
}


bool Json::is_null() { return m_type == json_null; }
bool Json::is_bool() { return m_type == json_bool; }
bool Json::is_int() { return m_type == json_int; }
bool Json::is_double() { return m_type == json_double; }
bool Json::is_string() { return m_type == json_string; }
bool Json::is_array() { return m_type == json_array; }
bool Json::is_object() { return m_type == json_object; }



bool Json::has_value(size_t _index)
{
	if (m_type != json_array) {
		throw new std::logic_error("type error,not array value");
	}
	return (_index< 0 || _index > m_value.m_array->size());
}


bool Json::has_value(const char* _key) {
	return has_value(std::string(_key));
}

bool Json::has_value(std::string _key) {
	if (m_type != json_object) {
		throw new std::logic_error("type error,not object value");
	}
	return (m_value.m_object->find(_key) != m_value.m_object->end());
}


void Json::remove(size_t _index)
{
	if (m_type != json_array) {
		throw new std::logic_error("type error,not array value");
	}
	if (_index < 0 || _index > m_value.m_array->size()) {
		throw new std::logic_error("array index is incorrect");
	}
	((m_value.m_array)->at(_index)).clear();        // 注意这里是Json中的clear()
	(m_value.m_array)->erase((m_value.m_array)->begin() + _index);
}


void Json::remove(const char* _key)
{
	remove(std::string(_key));
}


void Json::remove(std::string _key)
{
	if (m_type != json_object) {
		throw new std::logic_error("type error, not object value");
	}
	auto it = (m_value.m_object)->find(_key);
	if (it == (m_value.m_object)->end()) {
		throw new std::logic_error("not find object key");
	}
	(*(m_value.m_object))[_key].clear();  // 注意这里是Json中的clear()
	(m_value.m_object)->erase(_key);
}




// ========================================================================
// class JsonParser


JsonParser::JsonParser() : m_str(""), m_index(0) 
{

}

JsonParser::JsonParser(const char* _str) : m_str(_str), m_index(0)
{

}

JsonParser::JsonParser(std::string _str) : m_str(_str), m_index(0)
{

}

void JsonParser::load_str(const char* _str)
{
	m_str = _str;    // std::string都重载好了
}
	 
void JsonParser::load_str(std::string _str)
{
	m_str = _str; // std::string都重载好了
}



char JsonParser::get_next_token()   // 注意\0是交由parser函数处理????
{
	while (m_str[m_index] == ' ' || m_str[m_index] == '\n' || m_str[m_index] == '\r' || m_str[m_index] == '\t') {
		++m_index;
	}
	return m_str[m_index++];   // 注意：这里是先return m_str[m_index] 再 m_index+1

	/*// 操作说明
	*
	*              t e s t s t r i n g 
	*                    | |
	* new get the char---+ +---- m_index
	*
	*/
}


Json JsonParser::parse_null()     // 为什么返回值而不是引用？ 
{
	--m_index;
	if (m_str.compare(m_index, 4, "null") == 0) {     // 参数：起始下标，字符串长度，比较字符串
		m_index += 4;
		return Json();
	}
	throw new std::logic_error("parse json_null error");  // 这里包括了字符串的越界情况   
}


Json JsonParser::parse_bool()
{
	--m_index;
	if (m_str.compare(m_index, 4, "true") == 0) {
		m_index += 4;
		return Json(true);
	}
	if (m_str.compare(m_index, 5, "false") == 0) {
		m_index += 5;
		return Json(false);
	}
	throw new std::logic_error("parse json_bool error");
}


Json JsonParser::parse_number()
{
	// 在C++中科学计数法的数存在double中
	// -123.445e-2    -123.445e+2    123.445E2  123.445E+

	--m_index;
	size_t pos = m_index;
	if (m_str[m_index] == '-') {    // 注意：json标准中正数开头不能加+
		++m_index;
	}
	while (m_str[m_index] >= '0' && m_str[m_index] <= '9') {
		++m_index;
	}
	if (m_str[m_index] != '.') {    // 在科学计数法中'.'在'e'或'E'的前面
		return Json(std::atoi(m_str.c_str() + pos));
	}
	++m_index;    // 注意：这里m_index要加1
	while (m_str[m_index] >= '0' && m_str[m_index] <= '9') {
		++m_index;
	}
	if (m_str[m_index] == 'e' || m_str[m_index] == 'E') {
		++m_index;    // 注意：这里m_index要加1
		if (m_str[m_index] == '-' || m_str[m_index] == '+') {
			++m_index;
		}
		if (m_str[m_index] <= '0' || m_str[m_index] >= '9') {  // e'或'E'后面必须有一个数
			throw new std::logic_error("parse json_number error");
		}
		while (m_str[m_index] >= '0' && m_str[m_index] <= '9') {
			++m_index;
		}
		return Json(std::atof(m_str.c_str() + pos));
	}
	return Json(std::atof(m_str.c_str() + pos));
}



std::string JsonParser::parse_string()
{
	// "string"    "\"string\""    标准规定json_string不能以\"开头
	// {"\"qwer\"":1234}  ==>  {"\\"qwer\\"":1234}   标准规定json_string中的转义字符\要转换成\\,即要保留\
	// 总结：如果有转义字符就保留它，注意该函数不会去真正处理转义字符,如\u\b

	size_t pos = m_index;
	char ch = m_str[m_index];
	if (ch == '"') {
		++m_index;
		return "";
	}
	while (true) {
		m_index = m_str.find('"', m_index);    // 参数m_index用于设置起始下标
		if (m_index == std::string::npos) {
			throw new std::logic_error("parse json_string error");
		}
		if (m_str[m_index - 1] != '\\') {
			break;
		}
		++m_index;
	}
	++m_index; 
	return m_str.substr(pos, m_index - pos - 1);
}


Json JsonParser::parse_array()
{
	Json arr(Json::json_array);
	while (m_str[m_index] == ' ' || m_str[m_index] == '\n' || m_str[m_index] == '\r' || m_str[m_index] == '\t') {
		++m_index;
	}
	char ch = m_str[m_index];
	if (ch == ']') {
		++m_index;
		return arr;
	}
	while (true) {
		arr.append(std::move(parse()));
		ch = get_next_token();
		if (ch == ']') {
			break;
		}
		if (ch != ',') {
			throw new std::logic_error("parse json_array error");
		}
	}
	return arr;
}


//Json JsonParser::parse_array()
//{
//	Json arr(Json::json_array);
//	char ch = get_next_token();
//	if (ch != ']') {
//		while (true) {
//			arr.append(std::move(parse()));
//			ch = get_token();
//			++m_index;
//			if (ch == ']') {
//				break;
//			}
//			if (ch != ',') {
//				throw new std::logic_error("parse json_array error");
//			}
//		}
//	}
//	return arr;
//}


Json JsonParser::parse_object()
{ 
	Json obj(Json::json_object);
	std::string key;
	while (m_str[m_index] == ' ' || m_str[m_index] == '\n' || m_str[m_index] == '\r' || m_str[m_index] == '\t') {
		++m_index;
	}
	char ch = m_str[m_index];
	if (ch == '}') {
		++m_index;
		return obj;
	}
	while (true) {
		ch = get_next_token();
		if (ch != '"') {
			throw new std::logic_error("parse json_object error");
		}
		key = parse_string();
		ch = get_next_token();
		if (ch != ':') {
			throw new std::logic_error("parse json_object error");
		}
		obj[key] = std::move(parse());
		ch = get_next_token();
		if (ch == '}') {
			break;
		}
		if (ch != ',') {
			throw new std::logic_error("parse json_object error");
		}
	}
	return obj;
}


Json JsonParser::parse()
{
	char ch = get_next_token();
	switch (ch)
	{
	case 'n':
		return parse_null();
	case 't':
	case 'f':
		return parse_bool();
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return parse_number();
	case '"':
		return Json(parse_string());
	case '[':
		return parse_array();
	case '{':
		return parse_object();
	default:
		break;
	}
	throw new std::logic_error("unexpector char");    // \0用于报错退出
}













