#include "io.h"
#include "../string/path.h"

namespace Ks
{
    class StandardFile: public IOProtocol
    {
        friend class StandardArchieve;
    private:
        FILE* _handle = nullptr;
		size_t _size = 0;
        //
        StandardFile()
        {
        }
    public:

		virtual size_t size()
		{
			return _size;
		}

        virtual bool readable()
        {
            return true;
        }

        virtual bool writable()
        {
            return true;
        }
        
        virtual bool seekable()
        {
            return true;
        }

        virtual size_t read( size_t _bytes, IOProtocol* out_ )
        {
            char chunk[64];
            size_t bytesLeft = _bytes;
            size_t roundRead = 0;
            do
            {
                roundRead = bytesLeft > sizeof(chunk) ? sizeof(chunk) : bytesLeft;
                size_t readReal = fread( chunk, 1, sizeof(chunk), _handle);
                bytesLeft -= readReal;
                out_->write( readReal, chunk );
                if( readReal != roundRead )
                    break;
            }
            while( bytesLeft );
            return _bytes - bytesLeft;
        }

        virtual size_t read( size_t _bytes, void* out_ )
        {
            return fread( out_, 1, _bytes, _handle );
        }

        virtual size_t write( size_t _bytes, IOProtocol* _in )
        {
            char chunk[64];
            size_t bytesLeft = _bytes;
            size_t roundWrite = 0;
            do
            {
                roundWrite = bytesLeft > sizeof(chunk) ? sizeof(chunk) : bytesLeft;
                size_t writeReal = _in->read( roundWrite, chunk );
                bytesLeft -= writeReal;
                fwrite( chunk, 1, writeReal, _handle );
                if( writeReal != roundWrite )
                    break;
            }
            while( bytesLeft );
            return _bytes - bytesLeft;
        }

        virtual size_t write( size_t _bytes, const void* _in )
        {
            return fwrite(_in, 1, _bytes, _handle);
        }

        virtual size_t tell()
        {
            return ftell(_handle);
        }

        virtual bool seek( SeekFlag _flag, long _offset )
        {
            return fseek( _handle, _offset, _flag) != 0;
        }
        //
        virtual void release()
        {
            if( _handle )
                fclose( _handle );
            delete this;
        }
    };

    class MemoryIO: public IOProtocol
    {
		friend IOProtocol* CreateMemoryBuffer( void *, size_t, MemoryFreeCB _cb );
		friend IOProtocol* CreateMemoryBuffer(size_t _length, MemoryFreeCB _cb);
    private:
        void* _raw;
        long _size;
        long _position;
        MemoryFreeCB _destructor = nullptr;
    public:
        MemoryIO()
        {
            _raw = nullptr;
            _size = 0;
            _position = 0;
        }
        virtual bool readable()
        {
            return true;
        }

        virtual bool writable()
        {
            return true;
        }
        
        virtual bool seekable()
        {
            return true;
        }

        virtual size_t read( size_t _bytes, IOProtocol* out_ )
        {
            char chunk[64];
            size_t bytesLeft = _bytes;
            size_t roundRead = 0;
            do
            {
                size_t memLeft = (_size - _position);
                roundRead = bytesLeft > sizeof(chunk) ? sizeof(chunk) : bytesLeft;
                size_t readReal = roundRead > memLeft ? memLeft : roundRead;
                memcpy( chunk, (char*)_raw + _position, readReal );
                _position += readReal;
                bytesLeft -= readReal;
                out_->write( readReal, chunk );
                if( readReal != roundRead )
                    break;
            }
            while( bytesLeft );
            return _bytes - bytesLeft;
        }

        virtual size_t read( size_t _bytes, void* out_ )
        {
            size_t memLeft = (_size - _position);
            size_t readReal = _bytes > memLeft ? memLeft : _bytes; 
            memcpy( out_, (char*)_raw + _position, readReal);
            return readReal;
        }

        virtual size_t write( size_t _bytes, IOProtocol* _in )
        {
            size_t sizeWrite = (_size - _position);
            sizeWrite = sizeWrite > _bytes? _bytes:sizeWrite;
            auto nRead = _in->read( sizeWrite,  (char*)_raw + _position);
            _position += nRead;
            return nRead;
        }

        virtual size_t write( size_t _bytes, const void* _in )
        {
            size_t sizeWrite = (_size - _position);
            sizeWrite = sizeWrite > _bytes? _bytes:sizeWrite;
            memcpy( (char*)_raw + _position, _in, sizeWrite );
            _position+=sizeWrite;
            return sizeWrite;
        }

        virtual size_t tell()
        {
            return _position;
        }

        virtual bool seek( SeekFlag _flag, long _offset )
        {
            switch( _flag)
            {
                case SeekFlag::SeekCur:
                    _position += _offset;
                    break;
                case SeekFlag::SeekEnd:
                    _position = _size + _offset;
                    break;
                case SeekFlag::SeekSet:
                    _position = _offset;
            }

            if(_position < 0 )
            {
                _position = 0;
            }
            else if( _position > _size )
            {
                _position  = _size;
            }
            return true;
        }

		virtual size_t size()
		{
			return _size;
		}

		virtual const void* constData() const { 
			return this->_raw; 
		}
        //
        virtual void release()
        {
            if( _destructor )
                _destructor( _raw );
            delete this;
        }
    };

    class StandardArchieve: public ArchieveProtocol
    {
    friend ArchieveProtocol* CreateStdArchieve( const std::string& _path );
    private:
        std::string _root;
        //
        virtual IOProtocol* open( const std::string& _path ) override;
        //
        virtual void release() override;
        //
    };

    IOProtocol* StandardArchieve::open( const std::string& _path )
    {
        std::string fullpath = _root;
        fullpath.append( _path );
        auto path = FormatFilePath( fullpath );
        auto fh = fopen( path.c_str(), "rb+" );
        if( !fh )
            return nullptr;
		fseek(fh, 0, SEEK_SET);
		size_t set = ftell(fh);
		fseek(fh, 0, SEEK_END);
		size_t size = ftell(fh) - set;
		fseek(fh, 0, SEEK_SET);
        StandardFile* file = new StandardFile();
		file->_size = size;
        file->_handle = fh;
        return file;
    }

    void StandardArchieve::release()
    {
        delete this;
    }

    ArchieveProtocol* CreateStdArchieve( const std::string& _path )
    {
        StandardArchieve* arch = new StandardArchieve();
        arch->_root = FormatFilePath(_path);
        arch->_root.push_back('/');
        return arch;
    }

	IOProtocol* CreateMemoryBuffer(void * _ptr, size_t _length, IOProtocol::MemoryFreeCB _freeCB )
	{
		MemoryIO* buffer = new MemoryIO();
		buffer->_raw = _ptr;
		buffer->_destructor = _freeCB;
		buffer->_size = _length;
		buffer->_position = 0;
		return buffer;
	}

	IOProtocol* CreateMemoryBuffer( size_t _length, IOProtocol::MemoryFreeCB _freeCB)
	{
		MemoryIO* buffer = new MemoryIO();
		void * _ptr = malloc(_length);
		buffer->_raw = _ptr;
		buffer->_destructor = _freeCB;
		buffer->_size = _length;
		buffer->_position = 0;
		return buffer;
	}

    bool TextReader::openFile( Ks::ArchieveProtocol* _arch, const std::string& _filepath)
    {
        if (m_textMemory)
            m_textMemory->release();
        auto file = _arch->open(_filepath.c_str());
        if (!file)
            return false;
        m_textMemory = Ks::CreateMemoryBuffer(file->size() + 1);
        m_textMemory->write(file->size(), file);
        m_textMemory->write(1, "\0");
        file->release();
        //
        return true;
    }
    const char * TextReader::getText()
    {
        return (const char*)m_textMemory->constData();
    }

}
