#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;

struct VertexInput {
    float3 position [[ attribute(0) ]];
    float2 uv [[attribute(1)]];
};

struct GlobalParam {
    float4x4 projection;
    float4x4 view;
};

struct LocalParam {
    float4x4 model;
};

struct VertexOutput {
    float4 position[[position]];
    float2 uv;
};

vertex VertexOutput vertex_main(
    VertexInput vert[[stage_in]]
    , constant GlobalParam& GlobalArgument[[buffer(1)]]
    , constant LocalParam& LocalArgument[[buffer(2)]]
    , uint vertexId[[vertex_id]]
){
    VertexOutput output;
    output.position = GlobalArgument.projection * GlobalArgument.view * LocalArgument.model * float4( vert.position, 1.0f );
    output.uv = vert.uv;
    return output;
}