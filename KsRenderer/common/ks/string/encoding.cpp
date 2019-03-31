#include "encoding.h"
#include <stdio.h>
#include <memory.h>
#include <cassert>

#ifdef __APPLE__
#import <Foundation/Foundation.h>
#else
#ifdef _WIN32
#include <windows.h>
#else
extern "C"{
}
#endif
#endif

#include <vector>

namespace uiv2
{
	char16_t* ucsstr( const char16_t *s1, const char16_t *s2 )
	{
		int n;
		if (*s2)
		{
			while (*s1)
			{
				for (n=0; *(s1 + n) == *(s2 + n); n++)
				{
					if (!*(s2 + n + 1))
						return (char16_t *)s1;
				}
				s1++;
			}
			return NULL;
		}
		else
		{
			return (char16_t *)s1;
		}
	}

	int ucslen( const char16_t* str )
	{
		if ( !str )
		{
			return 0;
		}
		int length = 0;  
		while ( *str++ )  
			++length;  
		return ( length );
	}

	void ucsncpy( char16_t * _pDest, const char16_t *_pSrc, size_t _nCount )
	{
		memcpy( _pDest, _pSrc, sizeof(char16_t) * _nCount);
	}

// 编码转换使用的缓存
static std::vector<unsigned char >	CONV_BUFF;

void adjust_conv_buffer( size_t size )
{
	if (CONV_BUFF.size() < size)
	{
		CONV_BUFF.resize(size);
	}
}

void clear_conv()
{
	CONV_BUFF.clear();
	CONV_BUFF.shrink_to_fit();
}
    
#if __APPLE__
// NSString Cocoa 版本
    unsigned int utf82ucsle(const char* _pUTF8, unsigned int _nDataLen, char** _ppUnicode )
    {
        NSString * strUtf8 = [[NSString alloc] initWithBytes:(char*)_pUTF8
                                                     length:_nDataLen
                                                   encoding:NSUTF8StringEncoding];
        NSData * data = [strUtf8 dataUsingEncoding:NSUTF16LittleEndianStringEncoding];
        if ( nil == data) {
            *_ppUnicode[0] = 0;
            return 0;
        }
        adjust_conv_buffer(data.length + 2);
        CONV_BUFF.assign(data.length + 2,0);
        memcpy( CONV_BUFF.data(), data.bytes, data.length);
        *_ppUnicode = (char*)CONV_BUFF.data();
        return (unsigned int)data.length + 2;
    }
    
    unsigned int ucsle2utf8( const char * _pUnic, size_t _nDataLen, char ** _ppUTF )
    {
        NSString * ucsStr = [[NSString alloc] initWithBytes:(char*)_pUnic
                                                     length:_nDataLen
                                                   encoding:NSUTF16LittleEndianStringEncoding];
        if (nil == ucsStr) {
            return 0;
        }
        NSData * data = [ucsStr dataUsingEncoding:NSUTF8StringEncoding];
        if (nil == data) {
            return 0;
        }
        
        adjust_conv_buffer(data.length + 1);
        CONV_BUFF.assign(data.length + 1,0);
        memcpy( CONV_BUFF.data(), data.bytes, data.length);
        *_ppUTF = (char*)CONV_BUFF.data();
        return (unsigned int)(data.length + 1);
    }
    
    unsigned int gbk2utf8(const char * pAscii, size_t _nDataLen, char ** _ppUTF)
    {
        NSStringEncoding enc = CFStringConvertEncodingToNSStringEncoding (kCFStringEncodingGB_18030_2000);
        NSString * ucsStr = [[NSString alloc] initWithBytes:(char*)pAscii
                                                     length:_nDataLen
                                                   encoding:enc];
        if (nil == ucsStr) {
            return 0;
        }
        NSData * data = [ucsStr dataUsingEncoding:NSUTF8StringEncoding];
        if (nil == data) {
            return 0;
        }
        
        adjust_conv_buffer(data.length + 1);
        CONV_BUFF.assign(data.length + 1,0);
        memcpy( CONV_BUFF.data(), data.bytes, data.length);
        *_ppUTF = (char*)CONV_BUFF.data();
        return (unsigned int)(data.length + 1);
    }
#endif
#ifdef _WIN32
// Win32 Api 版本
	uint32_t utf82ucsle(const char* _pUTF8, uint32_t _nDataLen, char** _ppUnicode )
	{
		int bytes = ::MultiByteToWideChar( CP_UTF8, 0, _pUTF8, _nDataLen, NULL, 0) * 2;
		adjust_conv_buffer(bytes + 2);
		CONV_BUFF.assign(bytes + 2, 0);
		MultiByteToWideChar( CP_UTF8, 0, _pUTF8, _nDataLen , (LPWSTR)CONV_BUFF.data(), bytes );
		*_ppUnicode = (char*)CONV_BUFF.data();
		return bytes+2;
	}

	uint32_t ucsle2utf8( const char * _pUnic, size_t _nDataLen, char ** _ppUTF )
	{
		int bytes = ::WideCharToMultiByte( CP_UTF8, 0, (LPCWSTR)_pUnic, (int)_nDataLen/2, NULL, 0, NULL, NULL );
		adjust_conv_buffer(bytes+1);
		CONV_BUFF.assign(bytes+1, 0);
		::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)_pUnic, (int)_nDataLen/2, (LPSTR)CONV_BUFF.data(), bytes, NULL, NULL);
		*_ppUTF = (char*)CONV_BUFF.data();
		return bytes+1;
	}

	uint32_t ucsle2gbk( const char * _pUnic, size_t _nDataLen, char ** _ppAscii )
	{
		int bytes = ::WideCharToMultiByte( CP_OEMCP, 0,(LPCWSTR)_pUnic, (int)_nDataLen/2, NULL, 0, NULL, NULL);
		adjust_conv_buffer(bytes+1);
		CONV_BUFF.assign(bytes+1, 0);
		::WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)_pUnic, (int)_nDataLen, (LPSTR)CONV_BUFF.data(), bytes, NULL, NULL);
		*_ppAscii = (char*)CONV_BUFF.data();
		return bytes+1 ;
	}
	uint32_t gbk2utf8(const char * pAscii, size_t _nDataLen, char ** _ppUTF)
	{
		int rst = MultiByteToWideChar(CP_ACP, 0, (char*)pAscii, (int)_nDataLen, NULL, 0)+1;
		wchar_t* str_unicode = new wchar_t[rst];
		memset(str_unicode, 0, (rst) * sizeof(wchar_t));
		rst = MultiByteToWideChar(CP_ACP, 0, (char*)pAscii, (int)_nDataLen, str_unicode, rst);

		int bytes = WideCharToMultiByte(CP_UTF8, 0, str_unicode, -1, NULL, 0, NULL, NULL)+1;
		adjust_conv_buffer(bytes+1);
		CONV_BUFF.assign(bytes+1, 0);
		::WideCharToMultiByte(CP_UTF8, 0, str_unicode, -1, (LPSTR)CONV_BUFF.data(), bytes, NULL, NULL);
		delete[] str_unicode;
		*_ppUTF = (char*)CONV_BUFF.data();
		return bytes+1;
	}

#endif
#if __ANDROID__

	static bool checkUtf8FollowChar(const uint8_t* src, int _size)
	{
		while (_size-- > 0)
		{
			const uint_t s = *++src;
			if (s < 128 || s >= 192)
				return false;
		}
		return true;
	}
	uint32_t utf82ucsle(const char* _pUTF8, uint32_t _nDataLen, char** _ppUnicode )
	{
		// utf8转unicode内码大小极端情况下内存占用是原来的2倍
		
		//char * utf8 = const_cast<char*>(_pUTF8);
		
		adjust_conv_buffer(_nDataLen * 2 + 2);
		
		const uint8_t *src = (const uint8_t*)_pUTF8;
		const uint8_t *src_end = src + _nDataLen;
		uint8_t *desc = (uint8_t*)CONV_BUFF.data();
		while (src < src_end)
		{
			uint16_t d = 0;
			const uint8_t s = *src;
			if (s < 128)
			{
				d = s;
				++src;
			}
			else if (s < 192)
				break;
			else if (s < 224)
			{
				if (src + 1 < src_end && checkUtf8FollowChar(src, 1))
				{
					d = ((s & 0x1f) << 6) + (src[1] & 0x3f);
					src += 2;
				}
				else
					break;
			}
			else if (s < 248)
			{
				if (src + 2 < src_end && checkUtf8FollowChar(src, 2))
				{
					d = ((s & 0xf) << 12) + ((src[1] & 0x3f) << 6) + (src[2] & 0x3f);
					src += 3;
				}
				else
					break;
			}
			else
				break;

			*(desc++) = d & 0xff;
			*(desc++) = (d & 0xff00) >> 8;
		}

		*desc = 0;
		*(desc + 1) = 0;
		*_ppUnicode = (char*)CONV_BUFF.data();
		return desc - (uint8_t*)(*_ppUnicode) + 2;
	}

	uint32_t ucsle2utf8( const char * _pUnic, size_t _nDataLen, char ** _ppUTF )
	{
		if (_nDataLen & 0x1)
			return 0;

		// unicode转utf8内码大小极端情况下内存占用是原来的1.5倍
		uint32_t origSize = _nDataLen*1.5 + 1;
		adjust_conv_buffer(origSize);
		const uint16_t *src = (const uint16_t*)_pUnic;
		const uint16_t *src_end = (const uint16_t*)(_pUnic + _nDataLen);
		uint8_t *desc = (uint8_t*)CONV_BUFF.data();
		while (src < src_end)
		{
			const uint16_t s = *(src++);
			if (s < 128)
			{
				*(desc++) = (uint8_t)s;
			}
			else if (s < 0x800)
			{
				*desc = 0xe0 | (s & 0x7c0);
				*(desc + 1) = 0xc0 | (s & 0x3f);
			}
		}
		*desc = 0;
		*_ppUTF = (char*)CONV_BUFF.data();
		return desc - (uint8_t*)(*_ppUTF) + 1;
	}
	
	uint32_t gbk2utf8(const char * _gbk, size_t _nDataLen, char ** _ppUTF)
	{
		assert(false && "not support gbk2utf8");
		return 0;
	}
#endif

}
