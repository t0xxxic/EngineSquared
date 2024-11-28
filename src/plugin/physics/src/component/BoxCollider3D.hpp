#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace ES::Plugin::Physics::Component {

/**
 * Basic collider used to check collisions through DetectABABCollisions system
 */
struct BoxCollider3D {
    /**
     * The size of the box
     */
    glm::vec3 size;

    /**
     * Serialize the BoxCollider3D to JSON
     */
    nlohmann::json serialize() const
    {
        return nlohmann::json{
            {"size", {size.x, size.y, size.z}}
        };
    }

    /**
     * Deserialize a BoxCollider3D from JSON
     */
    static BoxCollider3D deserialize(const nlohmann::json &json)
    {
        if (!json.contains("size") || !json["size"].is_array() || json["size"].size() != 3 ||
            !json["size"][0].is_number() || !json["size"][1].is_number() || !json["size"][2].is_number())
            throw std::invalid_argument("Invalid JSON for BoxCollider3D deserialization");

        glm::vec3 size{json["size"][0].get<float>(), json["size"][1].get<float>(), json["size"][2].get<float>()};

        return BoxCollider3D{size};
    }
};
} // namespace ES::Plugin::Physics::Component
