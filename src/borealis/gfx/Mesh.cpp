#include <borealis/gfx/Mesh.hpp>
#include <borealis/gfx/impl/State.hpp>

using namespace br::gfx::impl;

namespace br::gfx
{
    MeshId mesh_make()
    {
        return _meshes.next();
    }

    void mesh_set_geometry(const MeshId id, const math::Vec2* positions, const uint16_t positions_len, bool dynamic_draw)
    {
        Mesh& mesh = _meshes[id];

        if (mesh.vao == INVALID_IDX)
        {
            glCreateBuffers(1, &mesh.vbo);
            glCreateVertexArrays(1, &mesh.vao);
            glEnableVertexArrayAttrib(mesh.vao, 0);
            glVertexArrayVertexBuffer(mesh.vao, 0, mesh.vbo, 0, sizeof(math::Vec2));
            glVertexArrayAttribFormat(mesh.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(mesh.vao, 0, 0);
        }

        glNamedBufferData(mesh.vbo, sizeof(math::Vec2) * positions_len, positions, dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

    void mesh_set_indices(const MeshId id, const uint16_t indices, const uint16_t indices_len)
    {

    }
}
