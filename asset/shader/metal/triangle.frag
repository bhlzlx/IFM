#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;

struct VertexOutput {
    float4 position[[position]];
    float2 uv;
};

fragment float4 fragment_main(
    VertexOutput fragIn[[stage_in]]
    , sampler samBase[[sampler(0)]]
    , texture2d<half> texBase [[ texture(0) ]])
{
    const half4 color = texBase.sample(samBase, fragIn.uv);
    return float4(color);
}