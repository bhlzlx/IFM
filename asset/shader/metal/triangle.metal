#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;

struct VertexInput {
    float3 position [[ attribute(0) ]];
    float2 uv [[attribute(1)]];
};

struct GlobalParam {
    float4x4 view;
    float4x4 projection;
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
    , constant GlobalParam& vp[[buffer(1)]]
    , constant LocalParam& m[[buffer(2)]]
    , short vertexId[[vertex_id]]
){
    VertexOutput output;
    output.position = vp.projection * vp.view * float4( vert.position, 1.0f );
    output.uv = vert.uv;
    return output;
}

fragment float4 fragment_main(
    VertexOutput fragIn[[stage_in]]
    , sampler samBase[[sampler(0)]]
    , texture2d<half> texBase [[ texture(0) ]])
){
    const half4 color = texBase.sample(samBase, fragIn.uv);
    return float4(color);
}