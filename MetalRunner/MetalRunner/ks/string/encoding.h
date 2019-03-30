#ifndef __APPUI_STRING_H__
#define __APPUI_STRING_H__

#ifdef _WIN32
#include <yvals.h>
#endif

// 用于创建 Unicode(16) 字符串
#ifdef _WIN32
#define USTR16( STR ) L##STR
#else
#define USTR16( STR ) u##STR
#endif
#include <stdint.h>

namespace uiv2
{
	// msvc10		: wchar_t	text[] = WSTR16("Hello,World!");
	// gcc/clang	: char16_t	text[] = WSTR16("Hello,World!");

	//************************************
	// Method:    utf82ucsle
	// FullName:  appui::utf82ucsle
	// Access:    public 
	// Returns:   uint32
	// Qualifier: UTF8 到 Unicode转换方法，渲染文字时用
	// Parameter: const char * _pUTF8	utf8 数据
	// Parameter: uint32 _nDataLen		utf8 长度
	// Parameter: char * _pUnicode		unicode输出buff
	// Parameter: uint32 _nBufferSize	输出buff的长度
	//************************************
	uint32_t utf82ucsle(const char* _pUTF8, uint32_t _nDataLen, char** _ppUnicode );
	uint32_t ucsle2utf8( const char * _pUnic, size_t _nDataLen, char ** _ppUTF );
	uint32_t ucsle2gbk(const char * _pUnic, size_t _nDataLen, char ** _ppAscii);
	uint32_t gbk2utf8(const char * pAscii, size_t _nDataLen, char ** _ppUTF);
	void clear_conv();
	//uint32 utf82ucsle(const char* _pUTF8, uint32 _nDataLen, char* _pUnicode, uint32 _nBufferSize);
	//uint32 ucsle2utf8( const char * _pUnic, size_t _nDataLen, char * _pUTF, size_t _nBufferSize );
	//uint32 ucsle2gb2312( const char * _pUnic, uint32 _nDataLen, char * _pAscii, uint32 _nBufferSize);

	// 从网上抄的一段代码改的
	// 用于unicode字串查找，作用类似于 strstr/wcsstr
	char16_t* ucsstr( const char16_t *s1, const char16_t *s2 );
	// 计算unicode长度
	int ucslen( const char16_t* str );
	// 复制字串
	void ucsncpy( char16_t * _pDest, const char16_t *_pSrc, size_t _nCount);

}

#endif

