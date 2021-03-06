#pragma once

#ifdef _WIN32

#pragma warning(disable:4251)

#ifdef KS_DYNAMIC_LINK
	#define KS_API_DECL _declspec(dllexport)
#elif defined KS_STATIC_LINK
	#define KS_API_DECL __declspec(dllimport)
#else
	#define KS_API_DECL
#endif
#else
#define KS_API_DECL 
#endif

#ifndef KS_JSON
#define KS_JSON(...)
#endif

#include "KsDefine.h"
#include <memory>
#define GL_GLEXT_PROTOTYPES
namespace Ks{

	static const uint32_t MaxRenderTarget = 4;
    static const uint32_t MaxVertexAttribute = 16;
	static const uint32_t MaxVertexBufferBinding = 8;
	static const uint32_t UniformChunkSize = 1024 * 512; // 512KB
	static const uint32_t MaxFlightCount = 3;

    template < class T >
    struct Point {
        T x;
        T y;
    };

    template < class T >
    struct Size {
        T width;
        T height;
    };

    template < class T >
    struct Size3D {
        T width;
        T height;
        T depth;
    };

    template< class T >
    struct Rect {
        Point<T> origin;
        Size<T> size;
    };

	template< class T >
	struct Offset2D {
		T x, y;
	};

	template< class T >
	struct Offset3D {
		T x, y, z;
	};

    struct Viewport {
        float x,y;
        float width,height;
        float zNear,zFar;
    };

    typedef Rect<int> Scissor;
	/*
	struct ImageRegion {
		uint32_t			baseMipLevel;
		uint32_t			mipLevelCount;
		Offset3D<uint32_t>	offset;
		Size3D<uint32_t>	size;
	};*/
	// Graphics API Command interface
	// update only a part of one slice

	struct TextureRegion {
		uint32_t mipLevel; // mip map level
		// for texture 2d, baseLayer must be 0
		// for texture 2d array, baseLayer is the index
		// for texture cube, baseLayer is from 0 ~ 5
		// for texture cube array, baseLayer is ( index * 6 + face )
		// for texture 3d, baseLayer is ( depth the destination layer )
		// for texture 3d array, baseLayer is ( {texture depth} * index + depth of the destination layer )
		uint32_t baseLayer;
		// for texture 2d, offset.z must be 0
		Offset3D<uint32_t> offset;
		// for texture 2d, size.depth must be 1
		Size3D<uint32_t> size;
	};

    enum DeviceType {
        Software = 0,
        GLES3,
        Metal,
        Vulkan,
		DX12,
        OpenGLCore
    };

    enum ShaderModuleType {
        VertexShader = 0,
        FragmentShader,
        ComputeShader
    };

	enum KsFormat : uint8_t {
		KsInvalidFormat,
		// 32 bit
		KsRGBA8888_UNORM,
		KsBGRA8888_UNORM,
		KsRGBA8888_SNORM,
		// 16 bit
		KsRGB565_PACKED,
		KsRGBA5551_PACKED,
		// Float type
		KsRGBA_F16,
		KsRGBA_F32,
		// Depth stencil type
		KsDepth24FStencil8,
		KsDepth32F,
		KsDepth32FStencil8,
		// compressed type
		KsETC2_LINEAR_RGBA,
		KsEAC_RG11_UNORM,
		KsBC1_LINEAR_RGBA,
		KsBC3_LINEAR_RGBA,
		KsPVRTC_LINEAR_RGBA,
	};

    enum PolygonMode :uint8_t {
        PMPoint = 0,
        PMLine,
        PMFill
    };

    enum TopologyMode :uint8_t {
        TMPoints = 0,
        TMLineStrip,
        TMLineList,
		TMTriangleStrip,
		TMTriangleList,
		TMTriangleFan,
		TMCount,
    };

    enum CullMode :uint8_t {
        None = 0,
        Back = 1,
        Front = 2,
		FrontAndBack = 3
    };

    enum WindingMode : uint8_t {
        Clockwise = 0,
        CounterClockwise = 1,
    };

    enum CompareFunction : uint8_t {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        GreaterEqual,
        Always,
    };

    enum BlendFactor :uint8_t {
        Zero,
        One,
        SourceColor,
        InvertSourceColor,
        SourceAlpha,
        InvertSourceAlpha,
		SourceAlphaSat,
        //
        DestinationColor,
        InvertDestinationColor,
        DestinationAlpha,
        InvertDestinationAlpha,
    };

    enum BlendOperation :uint8_t {
        BlendOpAdd,
        BlendOpSubtract,
        BlendOpRevsubtract,
    };

    enum StencilOperation :uint8_t {
        StencilOpKeep,
        StencilOpZero,
        StencilOpReplace,
        StencilOpIncrSat,
        StencilOpDecrSat,
        StencilOpInvert,
        StencilOpInc,
        StencilOpDec
    };

    enum AddressMode :uint8_t {
        AddressModeWrap,
        AddressModeClamp,
        AddressModeMirror,
    };

    enum TextureFilter :uint8_t {
        TexFilterNone,
        TexFilterPoint,
        TexFilterLinear
    };

    enum TextureCompareMode :uint8_t {
        RefNone = 0,
        RefToTexture = 1
    };

	enum VertexType :uint8_t {
		VertexTypeFloat1,
		VertexTypeFloat2,
		VertexTypeFloat3,
		VertexTypeFloat4,
		VertexTypeHalf2,
		VertexTypeHalf4,
		VertexTypeUByte4,
		VertexTypeUByte4N,
	};

	enum TextureType :uint8_t {
		Texture1D,
		Texture2D,
		Texture2DArray,
		TextureCube,
		TextureCubeArray,
		Texture3D
	};

	enum TextureUsageFlagBits :uint8_t {
		TextureUsageNone = 0x0,
		TextureUsageTransferSource = 0x1,
		TextureUsageTransferDestination = 0x2,
		TextureUsageSampled = 0x4,
		TextureUsageStorage = 0x8,
		TextureUsageColorAttachment = 0x10,
		TextureUsageDepthStencilAttachment = 0x20,
		TextureUsageTransientAttachment = 0x40,
		TextureUsageInputAttachment = 0x80
	};

	enum AttachmentOutputUsageBits : uint8_t {
		NextPassColor,
		NextPassDepthStencil,
		Sampling,
		Present,
	};

	enum BufferType :uint8_t {
		SVBO, // stable vertex buffer object
		TVBO, // transient vertex buffer object
		IBO, // index buffer object
		UBO, // uniform buffer object
		SSBO, // shader storage buffer objects
		TBO, // Texel buffer object
	};

	enum RTLoadAction :uint8_t {
		Keep,
		Clear,
		DontCare,
	};

#pragma pack(push)
#pragma pack(1)
    struct SamplerState {
        AddressMode u : 8;
        AddressMode v : 8;
        AddressMode w : 8;
        TextureFilter min : 8;
        TextureFilter mag : 8;
        TextureFilter mip :8 ;
        //
        TextureCompareMode compareMode : 8;
        CompareFunction compareFunction : 8;
        bool operator < ( const SamplerState& _ss ) const {
            return *(uint64_t*)this < *(uint64_t*)&_ss;
        }
    };
#pragma pack(pop)

    struct VertexAttribueDescription {
        uint32_t    bufferIndex;
        uint32_t    offset;
        VertexType  type;
        VertexAttribueDescription() : 
            bufferIndex(0),
            offset(0),
            type(VertexTypeFloat1){
            }
		KS_JSON( bufferIndex, offset, type )
    };

    struct VertexBufferDescription {
        uint32_t stride;
        uint32_t instanceMode;
        uint32_t rate;
        VertexBufferDescription():
        stride(0),
        instanceMode(0),
        rate(1){
        }
		KS_JSON( stride, instanceMode, rate )
    };

    struct PipelineVertexDescription {
        uint32_t                    vertexAttributeCount;
        VertexAttribueDescription   vertexAttributes[MaxVertexAttribute];
        uint32_t                    vertexBufferCount;
        VertexBufferDescription     vertexBuffers[MaxVertexBufferBinding];
        PipelineVertexDescription(): vertexAttributeCount(0), vertexBufferCount(0) {
        }
		KS_JSON( vertexAttributeCount, vertexAttributes, vertexBufferCount, vertexBuffers )
    };

	struct DepthState {
		uint8_t             depthWriteEnable = 1;
		uint8_t             depthTestEnable = 1;
		CompareFunction     depthFunction = CompareFunction::LessEqual;
		KS_JSON(depthWriteEnable, depthTestEnable, depthFunction)
	};

	struct BlendState {
		uint8_t             blendEnable = 1;
		BlendFactor         blendSource = SourceAlpha;
		BlendFactor         blendDestination = InvertSourceAlpha;
		BlendOperation      blendOperation = BlendOpAdd;
		KS_JSON(blendEnable, blendSource, blendDestination, blendOperation)
	};

	struct StencilState {
		uint8_t             stencilEnable = 0;
		StencilOperation    stencilFail = StencilOpKeep;
		StencilOperation    stencilZFail = StencilOpKeep;
		StencilOperation    stencilPass = StencilOpKeep;
		uint8_t             twoSideStencil = 0;
		StencilOperation    stencilFailCCW = StencilOpKeep;
		StencilOperation    stencilZFailCCW = StencilOpKeep;
		StencilOperation    stencilPassCCW = StencilOpKeep;
		CompareFunction     stencilFunction = Greater;
		uint32_t            stencilMask = 0xffffffff;
		KS_JSON(stencilEnable, stencilFail, stencilZFail, stencilPass, twoSideStencil, stencilFailCCW, stencilZFailCCW, stencilPassCCW, stencilFunction, stencilMask )
	};

	static const uint8_t MaskRed = 1;
	static const uint8_t MaskGreen = 2;
	static const uint8_t MaskBlue = 4;
	static const uint8_t MaskAlpha = 8;

    struct RenderState {
		uint8_t				writeMask;
		CullMode            cullMode = CullMode::None;
		WindingMode         windingMode = Clockwise;
		uint8_t             scissorEnable = 1;
		DepthState			depthState;
		BlendState			blendState;
		StencilState		stencilState;
		KS_JSON(writeMask, cullMode, windingMode, scissorEnable, depthState, blendState, stencilState )
        //float               constantBias = 0.0f;
        //float               slopeScaleBias = 0.0f;
        //
        //PolygonMode         polygonMode = PMFill;
        //TopologyMode        topologyMode = TMTriangleList;
    };

	typedef uint8_t TextureUsageFlags;

    struct TextureDescription {
		TextureType type;
        KsFormat format;
        uint32_t mipmapLevel;
		//
        uint32_t width;
        uint32_t height;
        uint32_t depth;
    };

    

	struct ArgumentDescription {
		const char** uboNames;
		int uboCount;
		const char** samplerNames;
		int samplerCount;
	};

	

	// RenderPassDescription describe the 
	// load action
#pragma pack( push, 1 )
	struct AttachmentDescription {
		KsFormat format;
		RTLoadAction loadAction;
		AttachmentOutputUsageBits usage;
		KS_JSON(format, loadAction, usage)
	};
	struct RenderPassDescription {
		// render pass behavior
		uint32_t colorAttachmentCount;
		// framebuffer description
		AttachmentDescription colorAttachment[MaxRenderTarget];
		AttachmentDescription depthStencil;
		//
		bool operator < (const RenderPassDescription& _desc) const {
			return memcmp(this, &_desc, sizeof(RenderPassDescription)) < 0;
		}
		KS_JSON(colorAttachmentCount, colorAttachment, depthStencil)
	};
#pragma pack (pop)

	struct RpClear {
		struct color4 {
			float r, g, b, a;
		} colors[MaxRenderTarget];
		float depth;
		int stencil;
	};

	struct PipelineDescription {
		// shader
		char            vertexShader[64];
		char            fragmentShader[64];
		RenderPassDescription renderPassDescription;
		RenderState     renderState;
		PipelineVertexDescription vertexLayout;
		//
		KS_JSON(vertexShader, fragmentShader, renderPassDescription, renderState, vertexLayout)
	};


	class IRenderPass;
	class IContext;

	class KS_API_DECL IView {
	public:
		virtual void resize(uint32_t _width, uint32_t _height) = 0;
		virtual bool beginFrame() = 0;
		virtual void endFrame() = 0;
		//
		virtual IRenderPass* getRenderPass() = 0;
		virtual IContext* getContext() = 0;
		virtual void release() = 0;
	};

    class KS_API_DECL ITexture {
	public:
        virtual const TextureDescription& getDesc() const = 0;
        virtual void setSubData( const void * _data, size_t _length, const TextureRegion& _region ) = 0;
		virtual void setSubData(const void * _data, size_t _length, const TextureRegion& _region, uint32_t _mipCount ) = 0;
        virtual void release() = 0;
    };

    class KS_API_DECL IBuffer {
	protected:
		BufferType m_type;
    public:
		IBuffer(BufferType _type) : m_type(_type) {}
		//
        virtual size_t getSize() = 0;
		virtual void setData(const void * _data, size_t _size, size_t _offset) = 0;
		virtual void release() = 0;
		// don't re-implement this method
		virtual BufferType getType() final {
			return m_type;
		}
    };

    class KS_API_DECL IAttachment {
    public:
        //virtual void resize( uint32_t _width, uint32_t _height ) = 0;
        virtual const ITexture* getTexture() const = 0;
		virtual void release() = 0;
		virtual KsFormat getFormat() const = 0;
    };
    //
    class KS_API_DECL IRenderPass {
    private:
    public:
        virtual bool begin() = 0;
        virtual void end() = 0;
		virtual void release() = 0;
		virtual void setClear( const RpClear& _clear ) = 0;
		virtual void resize( uint32_t _width, uint32_t _height ) = 0;
    };

	typedef union {
		// opengl slot
		uint64_t oglSlot;
		// vulkan slot
		struct {
			uint32_t vkSet;
			uint32_t vkBinding;
		};
	} SamplerSlot ;

	typedef union {
		// opengl slot
		uint64_t oglSlot;
		// vulkan slot
		struct {
			uint16_t vkSet;
			uint16_t vkUBOIndex;
			uint16_t vkUBOOffset;
			uint16_t vkUBOSize; // unused : default 0
		};
	} UniformSlot;

	class KS_API_DECL IArgument {
	private:
	public:
		virtual void bind() = 0;
		virtual UniformSlot getUniformSlot(const char * _name) = 0;
		virtual SamplerSlot getSamplerSlot(const char * _name) = 0;
		//
		virtual void setSampler(SamplerSlot _slot, const SamplerState& _sampler, const ITexture* _texture) = 0;
		virtual void setUniform(UniformSlot _slot, const void * _data, size_t _size) = 0;
		//virtual void setSampler(SamplerSlot _slot, const SamplerState& _ss, IAttachment* _attachment) = 0;
		virtual void release() = 0;
	};

    class KS_API_DECL IPipeline {
    private:
    public:
		virtual void begin() = 0;
		virtual void end() = 0;
		virtual void setViewport(const Viewport& _viewport) = 0;
		virtual void setScissor(const Scissor& _scissor) = 0;


		virtual void bindVertexBuffer(uint32_t _index, IBuffer* _vertexBuffer, uint32_t _offset) = 0;
		virtual void draw(TopologyMode _pmode, uint32_t _offset, uint32_t _vertexCount) = 0;
		virtual void drawIndexed(TopologyMode _pmode, IBuffer* _indexBuffer, uint32_t _indexOffset, uint32_t _indexCount) = 0;
		virtual void drawInstanced(TopologyMode _pmode, uint32_t _vertexOffset, uint32_t _vertexCount, uint32_t _instanceCount) = 0;
		virtual void drawInstanced(TopologyMode _pmode, IBuffer* _indexBuffer, uint32_t _indexOffset, uint32_t _indexCount, uint32_t _instanceCount) = 0;
		//
		virtual void setPolygonOffset(float _constantBias, float _slopeScaleBias) = 0;
		//
		virtual IArgument* createArgument(const ArgumentDescription& _desc) = 0;
		//virtual IArgument* createArgument(uint32_t _setId) = 0;
		//
		virtual void setShaderCache(const void* _data, size_t _size, size_t _offset) = 0;
		//
		virtual const PipelineDescription& getDescription() = 0;
		virtual void release() = 0;
    };

	class ArchieveProtocol;
	typedef ArchieveProtocol IArch;

    class KS_API_DECL IContext {
	public:
		virtual bool initialize( Ks::IArch* _arch, void* _wnd ) = 0;
        virtual IView* createView(void* _nativeWindow) = 0;
		virtual IBuffer* createStableVBO(void* _data, size_t _size) = 0;
		virtual IBuffer* createTransientVBO(size_t _size) = 0;
		virtual IBuffer* createIndexBuffer(void* _data, size_t _size) = 0;
		//virtual IUniformBuffer* createUniformBuffer(size_t _size) = 0;
        virtual ITexture* createTexture(const TextureDescription& _desc, TextureUsageFlags _usage = TextureUsageNone ) = 0;
		virtual ITexture* createTextureDDS( const void* _data, size_t _length ) = 0;
		virtual ITexture* createTextureKTX(const void* _data, size_t _length) = 0;
		virtual IAttachment* createAttachment(KsFormat _format,uint32_t _width, uint32_t _height ) = 0;
        virtual IRenderPass* createRenderPass( const RenderPassDescription& _desc, IAttachment** _colorAttachments, IAttachment* _depthStencil ) = 0;
        virtual IPipeline* createPipeline( const PipelineDescription& _desc ) = 0;
		virtual KsFormat swapchainFormat() const = 0;
    };

	extern "C" {
		KS_API_DECL IContext* GetContextES3();
		KS_API_DECL IContext* GetContextVulkan();
		KS_API_DECL IContext* GetContextMetal();
		KS_API_DECL IContext* GetContextDX12();
	}	
 }