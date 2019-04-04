//
//  Triangle.cpp
//  MetalRunner
//
//  Created by pixelsoft on 2019/4/3.
//  Copyright © 2019 kusugawa. All rights reserved.
//

#include "Triangle.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cstring>
#include <string>

namespace Ks {
    
    float PlaneVertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    };
    
    uint16_t PlaneIndices[] = {
        0,1,2,0,2,3
    };
    
    const char * shaderFolders[] = {
        "ogles3", // software swift shader
        "ogles3", // opengl es3
        "metal", // apple metal
        "vulkan", // KHR vulkan
        "oglcore" // KHR OpenGL Core Profile
    };
    
    uint32_t BitmapData[] = {
        0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,
        0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,
        0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,
        0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,
        0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,
        0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,
        0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,
        0xff000000,0xff000000,0xffffffff,0xffffffff,0xff000000,0xff000000,0xffffffff,0xffffffff,
    };
    
    ///////////////////
    
    
    bool Triangle::initialize(void* _wnd, Ks::ArchieveProtocol* _archieve) {
        m_resInited = false;
        m_context = GetContextMetal();
        //
        m_archieve = _archieve;
        m_context->initialize(_archieve, _wnd);
        m_view = m_context->createView(_wnd);
        //
        initializeResources();
        //
        return true;
    }
    
    void Triangle::initializeResources() {
        //
        m_vbo = m_context->createStaticVertexBuffer(PlaneVertices, sizeof(PlaneVertices));
        m_ibo = m_context->createIndexBuffer(PlaneIndices, sizeof(PlaneIndices));
        TextureDescription texDesc; {
            texDesc.depth = 1;
            texDesc.format = m_context->swapchainFormat();
            texDesc.width = 8;
            texDesc.height = 8;
            texDesc.mipmapLevel = 1;
            texDesc.type = Texture2D;
        }
        /*
        auto file = m_archieve->open("texture/texture2D.ktx");
        auto mem = Ks::CreateMemoryBuffer(file->size());
        file->read(file->size(), mem);
        m_texture = m_context->createTextureKTX(mem->constData(), mem->size());
        mem->release();
        file->release();
        */
        m_texture = m_context->createTexture(texDesc);
        TextureRegion region;
        region.mipLevel = 0;
        region.baseLayer = 0;
        region.offset = {
        0, 0, 0
        };
        region.size = {
        8, 8, 1
        };
        m_texture->setSubData(BitmapData, sizeof(BitmapData), region);
        //
        PipelineDescription pipelineDesc = {};
        pipelineDesc.renderState.cullMode = None;
        pipelineDesc.renderState.blendEnable = 0;
        
        pipelineDesc.renderPassDescription.colorAttachmentCount = 1;
        pipelineDesc.renderPassDescription.colorAttachment[0] = {
            KsBGRA8888_UNORM, RTLoadAction::Clear, AttachmentOutputForPresent
        };
        pipelineDesc.renderPassDescription.depthStencil = {
            KsDepth32F, RTLoadAction::Clear, AttachmentOutputForNextPassDepthStencilAttachment
        };
        
        char shaderPath[256];
        sprintf( shaderPath, "/shader/%s/triangle.vert", shaderFolders[contextType()]);
        strcpy(pipelineDesc.vertexShader, shaderPath);
        sprintf(shaderPath, "/shader/%s/triangle.frag", shaderFolders[contextType()]);
        strcpy(pipelineDesc.fragmentShader, shaderPath);
        pipelineDesc.vertexLayout.vertexAttributeCount = 2;
        pipelineDesc.vertexLayout.vertexAttributes[0].bufferIndex = 0;
        pipelineDesc.vertexLayout.vertexAttributes[0].offset = 0;
        pipelineDesc.vertexLayout.vertexAttributes[0].type = VertexTypeFloat3;
        pipelineDesc.vertexLayout.vertexAttributes[1].bufferIndex = 0;
        pipelineDesc.vertexLayout.vertexAttributes[1].offset = sizeof(float) * 3;
        pipelineDesc.vertexLayout.vertexAttributes[1].type = VertexTypeFloat2;
        pipelineDesc.vertexLayout.vertexBufferCount = 1;
        pipelineDesc.vertexLayout.vertexBuffers[0].instanceMode = 0;
        pipelineDesc.vertexLayout.vertexBuffers[0].rate = 1;
        pipelineDesc.vertexLayout.vertexBuffers[0].stride = sizeof(float) * 5;
        
        m_pipeline = m_context->createPipeline(pipelineDesc);
        
        Viewport vp = {
            static_cast<float>(0),
            static_cast<float>(0),
            static_cast<float>(m_size.width),
            static_cast<float>(m_size.height),
            static_cast<float>(0.0f),
            static_cast<float>(1.0f)
        };
        Scissor sissor = {
            0,
            0,
            static_cast<int>(m_size.width),
            static_cast<int>(m_size.height)
        };
        
        m_pipeline->setViewport(vp);
        m_pipeline->setScissor(sissor);
        
        if (this->contextType() != Vulkan ) {
            ArgumentDescription globalArgumentDesc;
            ArgumentDescription localArgumentDesc;
            const char * samplerNames[] = {
                "samBase"
            };
            const char * globalBlockNames[] = {
                "GlobalArgument"
            };
            const char * localBlockNames[] = {
                "LocalArgument"
            };
            globalArgumentDesc.samplerCount = 1;
            globalArgumentDesc.samplerNames = samplerNames;
            globalArgumentDesc.uboCount = 1;
            globalArgumentDesc.uboNames = globalBlockNames;
            
            localArgumentDesc.samplerCount = 0;
            localArgumentDesc.samplerNames = nullptr;
            localArgumentDesc.uboCount = 1;
            localArgumentDesc.uboNames = localBlockNames;
            
            m_globalArgument = m_pipeline->createArgument(globalArgumentDesc);
            m_localArgument = m_pipeline->createArgument(localArgumentDesc);
        }
        else
        {
            m_globalArgument = m_pipeline->createArgument(0);
            m_localArgument = m_pipeline->createArgument(1);
        }
        
        DrawableDescription drawableDesc; {
            drawableDesc.bufferCount = 1;
            drawableDesc.buffers[0] = m_vbo;
            drawableDesc.indexBuffer = m_ibo;
        }
        m_drawable = m_pipeline->createDrawable(drawableDesc);
        m_samBaseSlot = m_globalArgument->getSamplerSlot("samBase");
        m_globalUBOSlot = m_globalArgument->getUniformSlot("GlobalArgument");
        m_localUBOSlot = m_localArgument->getUniformSlot("LocalArgument");
        
        Ks::SamplerState ss;
        m_globalArgument->setSampler(m_samBaseSlot, ss, m_texture);
    }
    
    void Triangle::resize(uint32_t _width, uint32_t _height) {
        m_size.width = _width;
        m_size.height = _height;
        m_viewport = {
            0.0f, 0.0f,
            (float)_width,(float)_height,
            0.0f, 1.0f
        };
        //
        m_scissor.origin = { 0, 0 };
        m_scissor.size = { (int)_width, (int)_height };
        
        m_view->resize(_width, _height);
        
        if (m_pipeline) {
            Viewport vp = {
                0, 0, (float)_width, (float)_height, 0.0f, 1.0f
            };
            Scissor scissor = {
                0, 0, (int)_width, (int)_height
            };
            
            m_pipeline->setViewport(vp);
            m_pipeline->setScissor(scissor);
        }
        
        RpClear rpClear; {
            rpClear.colors[0] = { 1.0f, 0.5f, 1.0f, 1.0f };
            rpClear.depth = 1.0f;
            rpClear.stencil = 0;
        }
        m_view->getRenderPass()->setClear(rpClear);
    }
    
    void Triangle::release() {
    }
    
    void Triangle::tick() {
        Ks::RpClear clear;
        clear.colors[0] = { 1.0f, 1.0f, 0.0f, 1.0f };
        clear.depth = 1.0f;
        clear.stencil = 0;
        m_view->beginFrame(); {
            struct {
                glm::mat4 proj;
                glm::mat4 view;
            } GlobalUBO;
            GlobalUBO.proj = glm::perspective<float>(90.0f, (float)m_size.width / (float)m_size.height, 0.01, 10);
            GlobalUBO.view = glm::lookAt(glm::vec3(-1, 1, -1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
            static float arc = 0.0f;
            arc += 0.02;
            glm::mat4 model = glm::rotate(glm::mat4(), arc, glm::vec3(0, 1, 0));
            
            m_globalArgument->setUniform(m_globalUBOSlot, &GlobalUBO, sizeof(GlobalUBO));
            m_localArgument->setUniform(m_localUBOSlot, &model, sizeof(model));
            m_view->getRenderPass()->begin(m_size.width, m_size.height);
            {
                m_pipeline->begin();
                {
                    m_globalArgument->bind();
                    m_localArgument->bind();
                    //
                    //m_pipeline->setViewport(m_viewport);
                    //m_pipeline->setScissor(m_scissor);
                    //
                    m_pipeline->drawIndexed( m_drawable,TMTriangleList, 6);
                    m_pipeline->end();
                }
            }
            m_view->getRenderPass()->end();
            m_view->endFrame();
        }
    }
    KsApplication* appppppppppppppppppppppppp = nullptr;
}



KsApplication* GetApplication() {
    if (!Ks::appppppppppppppppppppppppp) {
        Ks::appppppppppppppppppppppppp = new Ks::Triangle();
    }
    return Ks::appppppppppppppppppppppppp;
}

