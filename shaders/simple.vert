#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out VS_OUT {
    vec2 TexCoord;
    vec3 LightDirTS;
    vec3 ViewDirTS;
} vs_out;

uniform mat4 mvp;
uniform mat4 mview;
uniform mat3 nmat;

// view space 기준의 light position
uniform vec4 LightPos;

void main()
{
    vec4 P = mview * vec4(pos, 1.0);

    vec3 N = normalize(nmat * vertexNormal);
    vec3 T = normalize(nmat * vertexTangent);

    // T를 N에 직교하도록 보정
    T = normalize(T - dot(T, N) * N);

    // B 재계산
    vec3 B = normalize(cross(N, T));

    // TBN: tangent space -> view space
    mat3 TBN = mat3(T, B, N);

    // 교수님 자료의 tangentMatrix:
    // view space -> tangent space
    mat3 tangentMatrix = transpose(TBN);

    vs_out.TexCoord = vertexTexCoord;

    // 교수님 자료 그대로:
    // L = normalize(tangentMatrix * (LightPos.xyz - P.xyz))
    vs_out.LightDirTS = tangentMatrix * (LightPos.xyz - P.xyz);

    // V = normalize(tangentMatrix * (-P.xyz))
    vs_out.ViewDirTS = tangentMatrix * (-P.xyz);

    gl_Position = mvp * vec4(pos, 1.0);
}