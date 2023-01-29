#include "gpu.h"
#include "Resources/resource_manager.h"
#include <spdlog/spdlog.h>

GPU GPU::gpu;

GPU::GPU()
{
    static bool created = false;

    if (created)
    {
        spdlog::error("only one gpu can be created");
        return;
    }
    created = true;
}

void GPU::init()
{
    default_cube = loadMesh(ResourceManager::createCubeMesh());
    default_quad = loadMesh(ResourceManager::createQuadMesh());
    default_ui_quad = loadMesh(ResourceManager::createQuadMesh(glm::vec2(0, 1), glm::vec2(1, 0), true));
}

RID GPU::createVertexArray()
{
    VertexArray vao;
    vao.create();
    vertex_arrays.push_back(vao);
    return RID(vertex_arrays.size() - 1); // index of last element
}

RID GPU::createVertexArray(RID vertex_buffer, std::vector<VertexAttribs> attribs, bool AOS)
{
    vertex_arrays.emplace_back(vertex_buffers[vertex_buffer], attribs, AOS);
    return RID(vertex_arrays.size() - 1); // index of last element
}

void GPU::linkAttribs(RID vertex_array, RID vertex_buffer, std::vector<VertexAttribs> attribs, bool AOS)
{
    vertex_arrays[vertex_array].linkAttribs(vertex_buffers[vertex_buffer], attribs, AOS);
}

void GPU::linkIndexBuffer(RID vertex_array, RID index_buffer)
{
    vertex_arrays[vertex_array].linkIndexBuffer(index_buffers[index_buffer]);
}

void GPU::setMode(RID vertex_array, Mesh::Primitive mode)
{
    vertex_arrays[vertex_array].setMode(mode);
}

RID GPU::createVertexBuffer(const void *data, uint32_t size, GLenum usage)
{
    vertex_buffers.emplace_back(data, size, usage);
    return RID(vertex_buffers.size() - 1); // index of last element
}

RID GPU::createIndexBuffer(const uint32_t *data, uint32_t size, GLenum usage)
{
    index_buffers.emplace_back(data, size, usage);
    return RID(index_buffers.size() - 1); // index of last element
}

RID GPU::createIndexBuffer(const uint16_t *data, uint32_t size, GLenum usage)
{
    index_buffers.emplace_back(data, size, usage);
    return RID(index_buffers.size() - 1); // index of last element
}

RID GPU::loadMesh(const Mesh &mesh)
{
    RID vbo = createVertexBuffer(mesh.getVertices().data(), mesh.getVertices().size() * sizeof(Vertex));
    RID ibo = createIndexBuffer(mesh.getIndices().data(), mesh.getIndices().size() * sizeof(uint32_t));

    RID vao = createVertexArray(vbo, VertexAttribs::POS_NORM_TEX);
    linkIndexBuffer(vao, ibo);
    setMode(vao, mesh.getMode());

    return vao;
}
RID GPU::loadMesh(const UniMesh &mesh)
{
    RID vao = createVertexArray();
    setMode(vao, mesh.mode);

    RID ibo = createIndexBuffer(mesh.indices.data(), mesh.indices.size() * sizeof(uint32_t));
    linkIndexBuffer(vao, ibo);

    if (mesh.positions.size() != 0)
    {
        RID vbo = createVertexBuffer(mesh.positions.data(), mesh.positions.size() * sizeof(Vertex));
        linkAttribs(vao, vbo, {VertexAttribs::POSITION}, false);
    }

    if (mesh.normals.size() != 0)
    {
        RID vbo = createVertexBuffer(mesh.normals.data(), mesh.normals.size() * sizeof(Vertex));
        linkAttribs(vao, vbo, {VertexAttribs::NORMAL}, false);
    }

    if (mesh.color.size() != 0)
    {
        RID vbo = createVertexBuffer(mesh.color.data(), mesh.color.size() * sizeof(Vertex));
        linkAttribs(vao, vbo, {VertexAttribs::COLOR}, false);
    }

    if (mesh.texture_st0.size() != 0)
    {
        RID vbo = createVertexBuffer(mesh.texture_st0.data(), mesh.texture_st0.size() * sizeof(Vertex));
        linkAttribs(vao, vbo, {VertexAttribs::TEXTURE_ST0}, false);
    }

    if (mesh.texture_st1.size() != 0)
    {
        RID vbo = createVertexBuffer(mesh.texture_st1.data(), mesh.texture_st1.size() * sizeof(Vertex));
        linkAttribs(vao, vbo, {VertexAttribs::TEXTURE_ST1}, false);
    }

    if (mesh.positions2d.size() != 0)
    {
        RID vbo = createVertexBuffer(mesh.positions2d.data(), mesh.positions2d.size() * sizeof(Vertex));
        linkAttribs(vao, vbo, {VertexAttribs::POSITION2D}, false);
    }

    if (mesh.color4.size() != 0)
    {
        RID vbo = createVertexBuffer(mesh.color4.data(), mesh.color4.size() * sizeof(Vertex));
        linkAttribs(vao, vbo, {VertexAttribs::COLOR4}, false);
    }

    return vao;
}

RID GPU::loadMesh(const Mesh2d &mesh)
{
    RID vbo = createVertexBuffer(mesh.getVertices().data(), mesh.getVertices().size() * sizeof(Vertex2d));
    RID ibo = createIndexBuffer(mesh.getIndices().data(), mesh.getIndices().size() * sizeof(uint32_t));

    RID vao = createVertexArray(vbo, VertexAttribs::POS2D_TEX);
    linkIndexBuffer(vao, ibo);
    setMode(vao, mesh.getMode());

    return vao;
}

RID GPU::createTexture(const Image &image, uint32_t mipmap_levels, Texture::InternalFormat in_f)
{
    textures.emplace_back(image, mipmap_levels, in_f);
    return textures.size() - 1;
}

RID GPU::createTexture(const char *file, uint32_t mipmap_levels, Texture::InternalFormat in_f)
{
    textures.emplace_back(file, mipmap_levels, in_f);
    return textures.size() - 1;
}

RID GPU::createCubeMap(const std::array<Image, 6> &images, uint32_t mipmap_levels, bool gamma_correcred)
{
    cube_maps.emplace_back(images, mipmap_levels, gamma_correcred);
    return cube_maps.size() - 1;
}

RID GPU::createCubeMap(const std::array<const char *, 6> &files, uint32_t mipmap_levels, bool gamma_correcred)
{
    cube_maps.emplace_back(files, mipmap_levels, gamma_correcred);
    return cube_maps.size() - 1;
}

GPU::~GPU()
{
    for (const auto &vertex_array : vertex_arrays)
        vertex_array.destroy();

    for (const auto &vertex_buffer : vertex_buffers)
        vertex_buffer.destroy();

    for (const auto &index_buffer : index_buffers)
        index_buffer.destroy();

    for (const auto &texture : textures)
        texture.destroy();
}