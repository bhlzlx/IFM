//
//  KsJpDecl.h
//  make `json` parsing more easy!
//
//  Created by kusugawa on 2019/4/1.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#pragma once

#include <map>
#include <vector>
#include <cstdint>
#include <string>
#include <typeinfo>
#include <typeindex>

#ifdef KS_JSON
#undef KS_JSON
#endif

extern std::map< size_t, std::map< std::string, uint32_t > > KsJsonEnumTable;

#define PRIVATE_ARGS_GLUE(x, y) x y

#define PRIVATE_MACRO_VAR_ARGS_IMPL_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9, _10, N, ...) N
#define PRIVATE_MACRO_VAR_ARGS_IMPL(args) PRIVATE_MACRO_VAR_ARGS_IMPL_COUNT args
#define COUNT_MACRO_VAR_ARGS(...) PRIVATE_MACRO_VAR_ARGS_IMPL((__VA_ARGS__,10, 9,8,7,6,5,4,3,2,1,0))

#define PRIVATE_MACRO_CHOOSE_HELPER2(M,count)  M##count
#define PRIVATE_MACRO_CHOOSE_HELPER1(M,count) PRIVATE_MACRO_CHOOSE_HELPER2(M,count)
#define PRIVATE_MACRO_CHOOSE_HELPER(M,count)   PRIVATE_MACRO_CHOOSE_HELPER1(M,count)

#define INVOKE_VAR_MACRO(M,...) PRIVATE_ARGS_GLUE(PRIVATE_MACRO_CHOOSE_HELPER(M,COUNT_MACRO_VAR_ARGS(__VA_ARGS__)), (__VA_ARGS__))


#define DEFINE_JSON_X_BEGIN public:
#define DEFINE_JSON_X_END

#define DEFINE_JSON_SAVE_FUNC_BEGIN std::string serialize() {\
rapidjson::Document doc;\
serializeObject(doc, doc.GetAllocator());\
rapidjson::StringBuffer buffer;\
rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);\
doc.Accept(writer);\
writer.Flush();\
return std::string(buffer.GetString());\
}\
\
bool serializeObject( rapidjson::Value& _value, rapidjson::Document::AllocatorType& _allocator ) {\
	_value.SetObject();\

#define DEFINE_JSON_SAVE_FUNC_END return true; }
#define SITEM__(item) rapidjson::Value v##item;\
WriteMember(v##item, item, _allocator);\
_value.AddMember( #item, v##item, _allocator );\

// 枚举
template < class T >
typename std::enable_if< std::is_enum< typename T >::value, bool >::type WriteItemAllType(rapidjson::Value& _object, T& _type, rapidjson::Document::AllocatorType& _allocator) {
	auto it1 = KsJsonEnumTable.find(typeid(T).hash_code());
	if (it1 != KsJsonEnumTable.end()) {
		for (auto& p : it1->second) {
			if (p.second == (uint32_t)_type) {
				_object.SetString(p.first.c_str(), p.first.length());
				return true;
			}
		}
	}
	_object = std::move(rapidjson::Value(unsigned(_type)));
	//_object.setUint((unsigned int)_type);
	return true;
}

// 非枚举
template < class T >
typename std::enable_if< !std::is_enum< typename T >::value, bool >::type WriteItemAllType(rapidjson::Value& _object, T& _type, rapidjson::Document::AllocatorType& _allocator) {
	return WriteItem(_object, _type, _allocator);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template < class T >
bool WriteItem(rapidjson::Value& _object, T& _type, rapidjson::Document::AllocatorType& _allocator) {
	return _type.serializeObject(_object, _allocator);
}

template <>
bool WriteItem<float>(rapidjson::Value& _object, float& _type, rapidjson::Document::AllocatorType& _allocator) {
	_object.SetFloat(_type);
	return true;
}

template <>
bool WriteItem<uint8_t>(rapidjson::Value& _object, uint8_t& _type, rapidjson::Document::AllocatorType& _allocator) {
	_object.SetUint(_type);
	return true;
}

template <>
bool WriteItem<int>(rapidjson::Value& _object, int& _type, rapidjson::Document::AllocatorType& _allocator) {
	_object.SetInt(_type); return true;
}

template <>
bool WriteItem<uint32_t>(rapidjson::Value& _object, uint32_t& _type, rapidjson::Document::AllocatorType& _allocator) {
	_object.SetUint(_type); return true;
}

template <>
bool WriteItem<uint64_t>(rapidjson::Value& _object, uint64_t& _type, rapidjson::Document::AllocatorType& _allocator) {
	_object.SetUint64(_type); return true;
}

template <>
bool WriteItem<int64_t>(rapidjson::Value& _object, int64_t& _type, rapidjson::Document::AllocatorType& _allocator) {
	_object.SetInt64(_type); return true;
}

template <>
bool WriteItem<std::string>(rapidjson::Value& _object, std::string& _type, rapidjson::Document::AllocatorType& _allocator) {
	_object.SetString( _type.c_str(), _type.length()); return true;
}

template <>
bool WriteItem<bool>(rapidjson::Value& _object, bool& _type, rapidjson::Document::AllocatorType& _allocator) {
	_object.SetBool(_type); return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class T >
bool WriteMember_(rapidjson::Value& _object, T& _type, rapidjson::Document::AllocatorType& _allocator) {
	return WriteItemAllType(_object, _type, _allocator);
}

template< class T, const size_t N >
bool WriteMember_(rapidjson::Value& _object, T(&_type)[N], rapidjson::Document::AllocatorType& _allocator) {
	_object.SetArray();
	rapidjson::Value::Array& arr = _object.GetArray();
	arr.Reserve(N, _allocator);
	for ( auto& ele : _type ) {
		rapidjson::Value obj;
		WriteItemAllType(obj, ele, _allocator);
		arr.PushBack(obj, _allocator);
	}
	return true;
}

template< const size_t N >
bool WriteMember_(rapidjson::Value& _object, char(&_type)[N], rapidjson::Document::AllocatorType& _allocator) {
	_object.SetString(&_type[0], strlen(&_type[0]));
	return true;
}

template< class T >
bool WriteMember_(rapidjson::Value& _object, std::vector<T>& _type, rapidjson::Document::AllocatorType& _allocator) {
	_object.SetArray();
	rapidjson::Value::Array& arr = _object.GetArray();
	arr.Reserve(_type.size(), _allocator);
	for (auto& ele : _type) {
		rapidjson::Value obj;
		WriteItemAllType(obj, ele, _allocator);
		arr.PushBack(obj, _allocator);
	}
	return true;
}

template< class T>
bool WriteMember(rapidjson::Value& _object, T& _type, rapidjson::Document::AllocatorType& _allocator ){
	return WriteMember_(_object, _type, _allocator);
}




#define DEFINE_JSON_PARSE_FUNC_BEGIN void parse( const char * _text ) {\
rapidjson::Document doc;\
doc.Parse(_text);\
if(doc.HasParseError()) return; parse(doc.GetObject()); }\
void parse(rapidjson::Value::Object& _object) {\

#define PITEM__( item ) \
	auto& iter##item = _object.FindMember(#item);\
	if ( iter##item != _object.end()) {\
	ParseAttribute(iter##item->value, item);}\


#define DEFINE_JSON_PARSE_FUNC_END }

template < class T >
typename std::enable_if< std::is_enum< typename T >::value, bool >::type ParseJsonAllSupportType(rapidjson::Value& _object, T& _value) {
	if (_object.IsString()) {
		auto it1 = KsJsonEnumTable.find(typeid(T).hash_code());
		if (it1 != KsJsonEnumTable.end()) {
			auto it2 = it1->second.find(std::string(_object.GetString()));
			if (it2 != it1->second.end()) {
				_value = (T)it2->second;
				return true;
			} else {
				return false;
			}
		}else{
			return false;
		}
	}
	else
	{
		_value = (T)_object.GetInt();
		return true;
	}
}

template < class T >
typename std::enable_if< !std::is_enum<typename T>::value, bool >::type ParseJsonAllSupportType(rapidjson::Value& _object, T& _value) {
	return ParseJsonItem(_object, _value);
}


template < class T >
bool ParseJsonItem(rapidjson::Value& _object, T& _value) {
	auto obj = _object.GetObject();
	_value.parse(obj);
	return true;
}

template <>
bool ParseJsonItem<uint8_t>(rapidjson::Value& _object, uint8_t& _value) {
	_value = _object.GetInt();
	return true;
}

template<>
bool ParseJsonItem<float>(rapidjson::Value& _object, float& _value) {
	_value = _object.GetFloat();
	return true;
}

template<>
bool ParseJsonItem<int>(rapidjson::Value& _object, int& _value) {
	_value = _object.GetInt();
	return true;
}

template<>
bool ParseJsonItem<uint32_t>(rapidjson::Value& _object, uint32_t& _value) {
	_value = _object.GetUint();
	return true;
}

template<>
bool ParseJsonItem<uint64_t>(rapidjson::Value& _object, uint64_t& _value) {
	_value = _object.GetUint64();
	return true;
}

template<>
bool ParseJsonItem<int64_t>(rapidjson::Value& _object, int64_t& _value) {
	_value = _object.GetInt64();
	return true;
}

template<>
bool ParseJsonItem<std::string>(rapidjson::Value& _object, std::string& _value) {
	_value = _object.GetString();
	return true;
}

template<>
bool ParseJsonItem<bool>(rapidjson::Value& _object, bool& _value) {
	_value = _object.GetBool();
	return true;
}

template< class T >
bool ParseMember_(rapidjson::Value& _object, T& _type) {
	return ParseJsonAllSupportType(_object, _type);
}

template< class T, const size_t N >
bool ParseMember_(rapidjson::Value& _object, T(&_type)[N]) {
	if (_object.IsArray()) {
		auto& arr = _object.GetArray();
		for (size_t i = 0; i < arr.Size() && i < N; ++i) {
			ParseJsonAllSupportType(arr[i], _type[i]);
		}
		return true;
	}
	return false;
}

template< const size_t N >
bool ParseMember_(rapidjson::Value& _object, char(&_type)[N]) {
	if (_object.IsString()) {
		strncpy(_type, _object.GetString(), N - 1);
		return true;
	}
	return false;
}

template< class T >
bool ParseMember_(rapidjson::Value& _object, std::vector<T>& _type) {
	if (_object.IsArray()) {
		auto& arr = _object.GetArray();
		for (auto& item : arr) {
			_type.resize(_type.size() + 1);
			ParseJsonAllSupportType(item, _type.back());
		}
		return true;
	}
	return false;
}

template< class T >
bool ParseAttribute(rapidjson::Value& _object, T& _type) {
	return ParseMember_(_object, _type);
}

#define PITEMO(X) PITEM__(X)
#define PITEMS1( a ) PITEMO(a)
#define PITEMS2( a, b) PITEMS1(a) PITEMS1(b) 
#define PITEMS3( a, b, c ) PITEMS1(a) PITEMS2(b,c)
#define PITEMS4( a,b,c,d ) PITEMS1(a) PITEMS3(b,c,d)
#define PITEMS5( a,b,c,d,e )  PITEMS1(a) PITEMS4( b,c,d,e )
#define PITEMS6( a,b,c,d,e,f)  PITEMS1(a) PITEMS5( b,c,d,e,f)
#define PITEMS7( a,b,c,d,e,f,g)  PITEMS1(a) PITEMS6(b,c,d,e,f,g)
#define PITEMS8( a,b,c,d,e,f,g,h)  PITEMS1(a) PITEMS7(b,c,d,e,f,g,h)
#define PITEMS9( a,b,c,d,e,f,g,h,i)  PITEMS1(a) PITEMS8(b,c,d,e,f,g,h,i)
#define PITEMS10(a,b,c,d,e,f,g,h,i,j)  PITEMS1(a) PITEMS9(b,c,d,e,f,g,h,i,j)

#define SITEMO(X) SITEM__(X)
#define SITEMS1( a ) SITEMO(a)
#define SITEMS2( a, b) SITEMS1(a) SITEMS1(b) 
#define SITEMS3( a, b, c ) SITEMS1(a) SITEMS2(b,c)
#define SITEMS4( a,b,c,d ) SITEMS1(a) SITEMS3(b,c,d)
#define SITEMS5( a,b,c,d,e )  SITEMS1(a) SITEMS4( b,c,d,e )
#define SITEMS6( a,b,c,d,e,f)  SITEMS1(a) SITEMS5( b,c,d,e,f)
#define SITEMS7( a,b,c,d,e,f,g)  SITEMS1(a) SITEMS6(b,c,d,e,f,g)
#define SITEMS8( a,b,c,d,e,f,g,h)  SITEMS1(a) SITEMS7(b,c,d,e,f,g,h)
#define SITEMS9( a,b,c,d,e,f,g,h,i)  SITEMS1(a) SITEMS8(b,c,d,e,f,g,h,i)
#define SITEMS10(a,b,c,d,e,f,g,h,i,j)  SITEMS1(a) SITEMS9(b,c,d,e,f,g,h,i,j)

#define DEFINE_JSON_X_PARSE_ATTRS( ... ) INVOKE_VAR_MACRO(PITEMS,__VA_ARGS__)
#define DEFINE_JSON_X_SAVE_ATTRS( ... ) INVOKE_VAR_MACRO(SITEMS,__VA_ARGS__)
#define DEFINE_JSON_X_PARSE(...) DEFINE_JSON_PARSE_FUNC_BEGIN DEFINE_JSON_X_PARSE_ATTRS(__VA_ARGS__) DEFINE_JSON_PARSE_FUNC_END
#define DEFINE_JSON_X_SAVE(...) DEFINE_JSON_SAVE_FUNC_BEGIN DEFINE_JSON_X_SAVE_ATTRS(__VA_ARGS__) DEFINE_JSON_SAVE_FUNC_END

#define KS_JSON(...) DEFINE_JSON_X_BEGIN  DEFINE_JSON_X_PARSE(__VA_ARGS__)  DEFINE_JSON_X_SAVE(__VA_ARGS__)  DEFINE_JSON_X_END