#pragma once

#include <glm/vec2.hpp>
#include <nlohmann/json.hpp>

#include "AssetID.hpp"
#include "Color.hpp"
#include "Math.hpp"

namespace ES::Plugin::UI::Component {
/**
 * @brief A struct representing a 2D sprite
 */
struct Sprite2D {
    /**
     * @brief The ID of the texture to use
     */
    ES::Plugin::Object::Utils::AssetID textureID = ES::Plugin::Object::Utils::NULL_ASSET_ID;
    /**
     * @brief The source rectangle of the texture
     *
     * The source rectangle is the part of the texture to use.
     * The position is the upper left corner of the rectangle.
     * The size is the width and height of the rectangle.
     */
    ES::Plugin::Math::Rect sourceRect = {
        {0, 0},
        {0, 0}
    };
    /**
     * @brief Origin of the sprite, used like a pivot
     */
    glm::vec2 origin = {0, 0};
    /**
     * @brief The depth of the sprite
     */
    int zIndex = 0;
    /**
     * @brief The color of the sprite
     */
    ES::Plugin::Colors::Utils::Color color;

    /**
     * @brief Serialize the Sprite2D to JSON
     */
    nlohmann::json serialize() const
    {
        return nlohmann::json{
            {"textureID",  textureID                           },
            {"sourceRect",
             {{"position", {sourceRect.position.x, sourceRect.position.y}},
              {"size", {sourceRect.size.x, sourceRect.size.y}}}},
            {"origin",     {origin.x, origin.y}                },
            {"zIndex",     zIndex                              },
            {"color",      {color.r, color.g, color.b, color.a}}
        };
    }

    /**
     * @brief Deserialize a Sprite2D from JSON
     */
    static Sprite2D deserialize(const nlohmann::json &json)
    {
        if (!json.contains("textureID") || !json.contains("sourceRect") || !json.contains("origin") ||
            !json.contains("zIndex") || !json.contains("color"))
            throw std::invalid_argument("Invalid JSON for Sprite2D deserialization");

        ES::Plugin::Object::Utils::AssetID textureID = json["textureID"].get<ES::Plugin::Object::Utils::AssetID>();

        if (!json["sourceRect"].contains("position") || !json["sourceRect"]["position"].is_array() ||
            json["sourceRect"]["position"].size() != 2 || !json["sourceRect"].contains("size") ||
            !json["sourceRect"]["size"].is_array() || json["sourceRect"]["size"].size() != 2)
            throw std::invalid_argument("Invalid JSON for Sprite2D deserialization");

        glm::vec2 position{json["sourceRect"]["position"][0].get<float>(),
                           json["sourceRect"]["position"][1].get<float>()};
        glm::vec2 size{json["sourceRect"]["size"][0].get<float>(), json["sourceRect"]["size"][1].get<float>()};

        glm::vec2 origin{json["origin"][0].get<float>(), json["origin"][1].get<float>()};

        int zIndex = json["zIndex"].get<int>();

        ES::Plugin::Colors::Utils::Color color{
            json["color"][0].get<unsigned char>(), json["color"][1].get<unsigned char>(),
            json["color"][2].get<unsigned char>(), json["color"][3].get<unsigned char>()};

        return Sprite2D{
            textureID,
            {position, size},
            origin,
            zIndex,
            color
        };
    }
};
} // namespace ES::Plugin::UI::Component
