#define STB_IMAGE_IMPLEMENTATION
#include "../Texture/stb_image.h"

#include "MeshBs.h"
#include "bs_ears.h"
#include <iostream>
#include <cmath>

MeshBs::MeshBs() {
    setup();
}

static void computeTangentBasisIndexed(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec2>& uvs,
    const std::vector<glm::vec3>& normals,
    const unsigned int* indices,
    int indexCount,
    std::vector<glm::vec3>& tangents,
    std::vector<glm::vec3>& bitangents)
{
    tangents.assign(vertices.size(), glm::vec3(0.0f));
    bitangents.assign(vertices.size(), glm::vec3(0.0f));

    for (int i = 0; i < indexCount; i += 3) {
        unsigned int i0 = indices[i + 0];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        const glm::vec3& v0 = vertices[i0];
        const glm::vec3& v1 = vertices[i1];
        const glm::vec3& v2 = vertices[i2];

        const glm::vec2& uv0 = uvs[i0];
        const glm::vec2& uv1 = uvs[i1];
        const glm::vec2& uv2 = uvs[i2];

        glm::vec3 dPos1 = v1 - v0;
        glm::vec3 dPos2 = v2 - v0;
        glm::vec2 dUV1 = uv1 - uv0;
        glm::vec2 dUV2 = uv2 - uv0;

        float denom = dUV1.x * dUV2.y - dUV1.y * dUV2.x;
        if (std::fabs(denom) < 1e-8f) {
            continue;
        }

        float r = 1.0f / denom;
        glm::vec3 T = (dPos1 * dUV2.y - dPos2 * dUV1.y) * r;
        glm::vec3 B = (dPos2 * dUV1.x - dPos1 * dUV2.x) * r;

        tangents[i0] += T;
        tangents[i1] += T;
        tangents[i2] += T;

        bitangents[i0] += B;
        bitangents[i1] += B;
        bitangents[i2] += B;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        glm::vec3 N = glm::normalize(normals[i]);
        glm::vec3 T = tangents[i];

        if (glm::length(T) < 1e-6f) {
            T = glm::abs(N.y) < 0.9f ? glm::cross(glm::vec3(0, 1, 0), N)
                                     : glm::cross(glm::vec3(1, 0, 0), N);
        }

        T = glm::normalize(T - N * glm::dot(N, T));

        glm::vec3 B = bitangents[i];
        float handedness = (glm::dot(glm::cross(N, T), B) < 0.0f) ? -1.0f : 1.0f;
        B = glm::normalize(glm::cross(N, T) * handedness);

        tangents[i] = T;
        bitangents[i] = B;
    }
}

static bool loadTexture2D(const char* path, GLuint textureID, bool srgb)
{
    int width = 0, height = 0, channels = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(path, &width, &height, &channels, 0);

    if (!image) {
        std::cerr << "텍스처 로드 실패: " << path << std::endl;
        return false;
    }

    GLenum internalFmt = GL_RGB8;
    GLenum dataFmt = GL_RGB;

    if (channels == 4) {
        internalFmt = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
        dataFmt = GL_RGBA;
    }
    else if (channels == 3) {
        internalFmt = srgb ? GL_SRGB8 : GL_RGB8;
        dataFmt = GL_RGB;
    }
    else if (channels == 1) {
        internalFmt = GL_R8;
        dataFmt = GL_RED;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureStorage2D(textureID, 1, internalFmt, width, height);
    glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFmt, GL_UNSIGNED_BYTE, image);
    glGenerateTextureMipmap(textureID);

    stbi_image_free(image);
    return true;
}

void MeshBs::setup() {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    int vertexCount = sizeof(modelVertices) / sizeof(ModelVertex);
    int indexCount = sizeof(modelIndices) / sizeof(unsigned int);

    vertices.reserve(vertexCount);
    normals.reserve(vertexCount);
    uvs.reserve(vertexCount);

    for (int i = 0; i < vertexCount; i++) {
        const ModelVertex& v = modelVertices[i];
        vertices.push_back(v.position);
        normals.push_back(v.normal);
        uvs.push_back(v.uv);
    }

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    computeTangentBasisIndexed(vertices, uvs, normals, modelIndices, indexCount, tangents, bitangents);

    glCreateVertexArrays(1, &vao);

    // model vertex buffer: position / normal / uv
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(modelVertices), modelVertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(ModelVertex));

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(ModelVertex, position));
    glVertexArrayAttribBinding(vao, 0, 0);
    glEnableVertexArrayAttrib(vao, 0);

    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(ModelVertex, normal));
    glVertexArrayAttribBinding(vao, 1, 0);
    glEnableVertexArrayAttrib(vao, 1);

    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(ModelVertex, uv));
    glVertexArrayAttribBinding(vao, 2, 0);
    glEnableVertexArrayAttrib(vao, 2);

    // tangent buffer
    glCreateBuffers(1, &tangentVBO);
    glNamedBufferData(tangentVBO, tangents.size() * sizeof(glm::vec3), tangents.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vao, 1, tangentVBO, 0, sizeof(glm::vec3));
    glVertexArrayAttribFormat(vao, 3, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 3, 1);
    glEnableVertexArrayAttrib(vao, 3);

    // bitangent buffer
    glCreateBuffers(1, &bitangentVBO);
    glNamedBufferData(bitangentVBO, bitangents.size() * sizeof(glm::vec3), bitangents.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vao, 2, bitangentVBO, 0, sizeof(glm::vec3));
    glVertexArrayAttribFormat(vao, 4, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 4, 2);
    glEnableVertexArrayAttrib(vao, 4);

    // index buffer
    glCreateBuffers(1, &ibo);
    glNamedBufferData(ibo, sizeof(modelIndices), modelIndices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(vao, ibo);

    // 텍스처 좌표가 위아래 반대로 보이면 false를 true로 바꿔 테스트하세요.
    stbi_set_flip_vertically_on_load(false);

    glCreateTextures(GL_TEXTURE_2D, 1, &colorTexID);
    glTextureParameteri(colorTexID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(colorTexID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(colorTexID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(colorTexID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    loadTexture2D("Texture/ogre_diffuse.png", colorTexID, true);

    glCreateTextures(GL_TEXTURE_2D, 1, &normalMapTexID);
    glTextureParameteri(normalMapTexID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(normalMapTexID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(normalMapTexID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(normalMapTexID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    loadTexture2D("Texture/ogre_normalmap.png", normalMapTexID, false);
}

void MeshBs::draw() {
    glBindTextureUnit(0, colorTexID);
    glBindTextureUnit(1, normalMapTexID);

    glBindVertexArray(vao);

    GLsizei elementCount = static_cast<GLsizei>(sizeof(modelIndices) / sizeof(unsigned int));
    glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
