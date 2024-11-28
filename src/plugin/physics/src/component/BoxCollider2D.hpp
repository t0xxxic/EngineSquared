#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace ES::Plugin::Physics::Component {

/**
 * Basic collider used to check collisions through CollisionChecker system
 */
struct BoxCollider2D {
    /**
     * The size of the box
     */
    glm::vec2 size;

    /**
     * Serialize the BoxCollider2D to JSON
     */
    nlohmann::json serialize() const
    {
        return nlohmann::json{
            {"size", {size.x, size.y}}
        };
    }

    /**
     * Deserialize a BoxCollider2D from JSON
     */
    static BoxCollider2D deserialize(const nlohmann::json &json)
    {
        if (!json.contains("size") || !json["size"].is_array() || json["size"].size() != 2 ||
            !json["size"][0].is_number() || !json["size"][1].is_number())
            throw std::invalid_argument("Invalid JSON for BoxCollider2D deserialization");

        glm::vec2 size{json["size"][0].get<float>(), json["size"][1].get<float>()};

        return BoxCollider2D{size};
    }
};
} // namespace ES::Plugin::Physics::Component
