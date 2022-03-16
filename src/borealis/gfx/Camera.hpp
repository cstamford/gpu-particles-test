#pragma once

#include <borealis/gfx/Gfx.hpp>
#include <borealis/util/Math.hpp>

namespace br::gfx
{
    CameraId camera_make();
    void camera_set_transform(const CameraId id, math::Mat3Ref transform);
}
