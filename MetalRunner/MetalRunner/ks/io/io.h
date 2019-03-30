#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <string>

namespace Ks
{
    enum SeekFlag
    {
        SeekCur = 0,
        SeekEnd,
        SeekSet
    };

    struct IOProtocol
    {
		typedef void(*MemoryFreeCB)(void *);
		//
        virtual size_t read( size_t _bytes, IOProtocol* out_ ) = 0;
        virtual size_t read( size_t _bytes, void* out_ ) = 0;
        virtual bool readable() = 0;
        
        virtual size_t write( size_t _bytes, IOProtocol* out_ ) = 0;
        virtual size_t write( size_t _bytes, const void* out_ ) = 0;
        virtual bool writable() = 0;

        virtual size_t tell() = 0;
        virtual bool seek( SeekFlag _flag, long _offset ) = 0;
        virtual bool seekable() = 0;

		virtual const void* constData() const { return nullptr; }

		virtual size_t size() = 0;

        virtual void release() = 0;

        virtual ~IOProtocol(){}
    };

    class ArchieveProtocol
    {
	public:
        // Open
        virtual IOProtocol* open( const std::string& _path ) = 0;
        // List
        // Delete
        // Create
        // Destroy
        virtual void release() = 0;

        virtual ~ArchieveProtocol(){}
    };

    ArchieveProtocol* CreateStdArchieve( const std::string& _path );
	IOProtocol* CreateMemoryBuffer(void * _ptr, size_t _length, IOProtocol::MemoryFreeCB _freeCB = nullptr);
	IOProtocol* CreateMemoryBuffer(size_t _length, IOProtocol::MemoryFreeCB _freeCB = [](void* _ptr) {
		free(_ptr); 
	});

    class TextReader
    {
    private:
        Ks::IOProtocol* m_textMemory;
    public:
        TextReader() :
            m_textMemory(nullptr)
        {}
        bool openFile(Ks::ArchieveProtocol* _arch, const std::string& _filepath);
        const char * getText();
        ~TextReader() {
            if (m_textMemory) {
                m_textMemory->release();
            }
        }
    };

	typedef ArchieveProtocol IArch;
}