//
//  BufferMTL.cpp
//  RendererMTL
//
//  Created by kusugawa on 2019/3/24.
//  Copyright © 2019年 kusugawa. All rights reserved.
//

#include "BufferMTL.h"
#include "ContextMTL.h"

namespace Ks {
    
    IStaticVertexBuffer* ContextMTL::createStaticVertexBuffer( void* _data, size_t _size ) {
        BufferMTL* buffer = BufferMTL::createBuffer( _size, _data, BufferUsageStaticVBO);
        SVBOMTL* vbo = new SVBOMTL(std::move(*buffer));
        delete buffer;
        return vbo;
    }
    
    IDynamicVertexBuffer* ContextMTL::createDynamicVertexBuffer( size_t _size ) {
        BufferMTL* buffer = BufferMTL::createBuffer( _size,nullptr,BufferUsageDynamicVBO);
        DVBOMTL* vbo = new DVBOMTL(std::move(*buffer));
        delete buffer;
        return vbo;
    }
    
    IIndexBuffer* ContextMTL::createIndexBuffer( void* _data, size_t _size ) {
        BufferMTL* buffer = BufferMTL::createBuffer( _size, _data, BufferUsageDynamicVBO);
        IBOMTL* ibo = new IBOMTL(std::move(*buffer));
        delete buffer;
        return ibo;
    }
    
    void IBOMTL::setData( const void * _data, size_t _size, size_t _offset) {
        m_buffer.setDataCPUAccess(_data, _size, _offset);
    }
    
    void DVBOMTL::setData( const void * _data, size_t _size, size_t _offset) {
        m_buffer.setDataCPUAccess(_data, _size, _offset);
    }
    
    void SVBOMTL::setData( const void * _data, size_t _size, size_t _offset) {
        m_buffer.setDataGPUAccess( _data, _size, _offset);
    }
    
    void SVBOMTL::release() {
        delete this;
    }
    
    size_t SVBOMTL::getSize(){
        return m_buffer.buffer().length;
    }
    BufferType SVBOMTL::getType(){
        return BufferType::SVBO;
    }
    
    BufferMTL* BufferMTL::createBuffer( size_t _size, const void * _data, BufferUsageFlagBits _usage  ) {
        GetContext(context);
        //
        MTLResourceOptions options = 0;
        switch (_usage) {
            case BufferUsageStaticVBO:
            case BufferUsageIBO:
            case BufferUsageSSBO:
                options = MTLResourceStorageModePrivate; // static vbo should store in Graphics Card Video Memory
                break;
            case BufferUsageUniform:
            case BufferUsageDynamicVBO:
                options = MTLResourceStorageModeShared;
                break;
            default:
                break;
        }
        id<MTLBuffer> bufferMtl = nil;
        if( _data )
            bufferMtl = [context->getDevice() newBufferWithBytes:_data length:_size options:options];
        else
            bufferMtl = [context->getDevice() newBufferWithLength: _size options:options];
        BufferMTL* buffer= new BufferMTL( bufferMtl );
        //context->getUploadQueue().uploadBuffer( buffer, 0, _data, _size);
        buffer->m_usage = _usage;
        return buffer;
    }
    
    void BufferMTL::setData( const void * _data, size_t _length, size_t _offset ){
        switch (m_usage) {
            case BufferUsageStaticVBO:
            case BufferUsageIBO:
            case BufferUsageSSBO:
                setDataGPUAccess( _data, _length, _offset);
                break;
            case BufferUsageUniform:
            case BufferUsageDynamicVBO:
                setDataCPUAccess( _data, _length, _offset);
                break;
            default:
                break;
        }
    }
    
    void BufferMTL::setDataCPUAccess( const void * _data, size_t _length, size_t _offset ) {
        memcpy((uint8_t*)m_buffer.contents + _offset, _data, _length);
    }
    void BufferMTL::setDataGPUAccess( const void * _data, size_t _length, size_t _offset ) {
        GetContext(context)
        context->getGraphicsQueue().updateBuffer( this, _offset, _data, _length);
    }
}
