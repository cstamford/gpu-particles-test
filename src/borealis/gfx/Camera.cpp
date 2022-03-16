#include <borealis/gfx/Camera.hpp>
#include <borealis/gfx/impl/State.hpp>

using namespace br::gfx::impl;

namespace br::gfx
{
    CameraId camera_make()
    {
        return _cameras.next();
    }

    void camera_set_transform(const CameraId id, math::Mat3Ref transform)
    {
        _cameras[id].transform = transform;
    }
}
