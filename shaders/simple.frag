#version 450

in VS_OUT {
    vec2 TexCoord;
    vec3 LightDirTS;
    vec3 ViewDirTS;
} fs_in;

out vec4 Fragcolor;

struct LightInfo {
    vec3 Ia;
    vec3 Id;
    vec3 Is;
};

struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shiness;
};

uniform LightInfo Light;
uniform MaterialInfo Material;

uniform sampler2D ColorTex;
uniform sampler2D NormalMapTex;

void main()
{
    // color map
    vec3 KdTex = texture(ColorTex, fs_in.TexCoord).rgb;

    // normal map: [0,1] -> [-1,1]
    vec3 N = texture(NormalMapTex, fs_in.TexCoord).rgb;
    N = normalize(N * 2.0 - 1.0);

    // 이미 tangent space로 넘어온 L, V
    vec3 L = normalize(fs_in.LightDirTS);
    vec3 V = normalize(fs_in.ViewDirTS);

    // Blinn-Phong
    vec3 H = normalize(L + V);

    float diff = max(dot(L, N), 0.0);
    float spec = pow(max(dot(N, H), 0.0), Material.Shiness);

    vec3 ambientColor  = Light.Ia * Material.Ka * KdTex;
    vec3 diffuseColor  = Light.Id * Material.Kd * KdTex * diff;
    vec3 specularColor = Light.Is * Material.Ks * spec;

    vec3 finalColor = ambientColor + diffuseColor + specularColor;

    Fragcolor = vec4(finalColor, 1.0);
}