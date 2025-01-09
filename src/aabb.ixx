module;

#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <limits>

export module aabb;

import renderable;
import outline;

export class AxisAlignedBoundingBox {
public:
    Outline* outline = nullptr;

    AxisAlignedBoundingBox()
    {
        // the bounding box of no geometry
		min_ = glm::vec2(std::numeric_limits<float>::max());
		max_ = glm::vec2(std::numeric_limits<float>::lowest());
    }
    AxisAlignedBoundingBox(const glm::vec2& min, const glm::vec2& max)
        : min_(min), max_(max) {}

    ~AxisAlignedBoundingBox() {
        if (outline != nullptr)
        {
            delete outline;
        }
    }

    static AxisAlignedBoundingBox compute_bounding_box(Renderable &rd) {
        glm::vec2 min(std::numeric_limits<float>::max());
        glm::vec2 max(std::numeric_limits<float>::lowest());

        for (const auto& point : rd.core) {
            min = glm::min(min, point);
            max = glm::max(max, point);
        }

        return AxisAlignedBoundingBox(min, max);
    }

    static AxisAlignedBoundingBox compute_bounding_box(const std::vector<AxisAlignedBoundingBox>& boxes) {
        glm::vec2 min(std::numeric_limits<float>::max());
        glm::vec2 max(std::numeric_limits<float>::lowest());

        for (const auto& box : boxes) {
            min = glm::min(min, box.min_);
            max = glm::max(max, box.max_);
        }

        return AxisAlignedBoundingBox(min, max);
    }

    bool intersects(const AxisAlignedBoundingBox& other) const {
        return !(max_.x < other.min_.x || min_.x > other.max_.x ||
                 max_.y < other.min_.y || min_.y > other.max_.y);
    }


    glm::vec2 min_;
    glm::vec2 max_;
};