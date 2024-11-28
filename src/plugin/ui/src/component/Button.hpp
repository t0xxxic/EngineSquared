#pragma once

#include <functional>
#include <optional>
#include <variant>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "Engine.hpp"

#include "AssetID.hpp"
#include "Color.hpp"

namespace ES::Plugin::UI::Component {
/**
 * @brief The different types of display for a button
 */
namespace DisplayType {
/**
 * @brief A struct representing the tint color of a button
 *
 * @param   imageID         The ID of the image to tint
 * @param   normalColor     The color to use when the button is in the normal state
 * @param   hoverColor      The color to use when the button is in the hover state
 * @param   pressedColor    The color to use when the button is in the pressed state
 */
struct TintColor {
    ES::Plugin::Object::Utils::AssetID imageID;
    ES::Plugin::Colors::Utils::Color normalColor;
    ES::Plugin::Colors::Utils::Color hoverColor;
    ES::Plugin::Colors::Utils::Color pressedColor;
    /**
     * Serialize the TintColor to JSON
     */
    nlohmann::json serialize() const
    {
        return nlohmann::json{
            {"imageID",      imageID                                                         },
            {"normalColor",  {normalColor.r, normalColor.g, normalColor.b, normalColor.a}    },
            {"hoverColor",   {hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a}        },
            {"pressedColor", {pressedColor.r, pressedColor.g, pressedColor.b, pressedColor.a}}
        };
    }
};

/**
 * @brief A struct representing the image of a button
 *
 * @param   normalImageID   The ID of the image to use when the button is in the normal state
 * @param   hoverImageID    The ID of the image to use when the button is in the hover state
 * @param   pressedImageID  The ID of the image to use when the button is in the pressed state
 */
struct Image {
    ES::Plugin::Object::Utils::AssetID normalImageID;
    ES::Plugin::Object::Utils::AssetID hoverImageID;
    ES::Plugin::Object::Utils::AssetID pressedImageID;

    /**
     * Serialize the Image to JSON
     */
    nlohmann::json serialize() const
    {
        return nlohmann::json{
            {"normalImageID",  normalImageID },
            {"hoverImageID",   hoverImageID  },
            {"pressedImageID", pressedImageID}
        };
    }
};

/**
 * @brief A variant representing the different types of display for a button
 */
using Variant = std::variant<TintColor, Image>;
}; // namespace DisplayType

/**
 * @brief A struct representing a button
 */
struct Button {
    /**
     * @brief The state of the button
     */
    enum class State {
        Normal,
        Hover,
        Pressed
    };
    /**
     * @brief The current state of the button
     */
    State state = State::Normal;
    /**
     * @brief The previous state of the button, used to check if the state has changed
     */
    State lastState = State::Normal;
    /**
     * @brief The display type of the button
     */
    DisplayType::Variant displayType;
    /**
     * @brief The function to call when the button is clicked
     */
    std::function<void(ES::Engine::Registry &)> onClick;
    /**
     * @brief The script to call when the button is clicked
     */
    std::optional<std::string> onClickScript;

    /**
     * Serialize the Button to JSON
     */
    nlohmann::json serialize() const
    {
        return nlohmann::json{
            {"state",       static_cast<int>(state)                              },
            {"lastState",   static_cast<int>(lastState)                          },
            {"displayType", std::visit([](auto &&arg) { return arg.serialize(); }, displayType)},
            {"onClick",             onClickScript.value_or("")                                                         }
        };
    }

    /**
     * Deserialize a Button from JSON
     */
    static Button deserialize(const nlohmann::json &json)
    {
        if (!json.contains("state") || !json.contains("lastState") || !json.contains("displayType") ||
            !json.contains("onClick"))
            throw std::invalid_argument("Invalid JSON for Button deserialization");

        State state = static_cast<State>(json["state"].get<int>());
        State lastState = static_cast<State>(json["lastState"].get<int>());
        DisplayType::Variant displayType;

        if (json["displayType"].contains("imageID"))
        {
            displayType =
                DisplayType::Image{json["displayType"]["normalImageID"].get<ES::Plugin::Object::Utils::AssetID>(),
                                   json["displayType"]["hoverImageID"].get<ES::Plugin::Object::Utils::AssetID>(),
                                   json["displayType"]["pressedImageID"].get<ES::Plugin::Object::Utils::AssetID>()};
        }
        else
        {
            Colors::Utils::Color normalColor{json["displayType"]["normalColor"][0].get<unsigned char>(),
                                             json["displayType"]["normalColor"][1].get<unsigned char>(),
                                             json["displayType"]["normalColor"][2].get<unsigned char>(),
                                             json["displayType"]["normalColor"][3].get<unsigned char>()};

            Colors::Utils::Color hoverColor{json["displayType"]["hoverColor"][0].get<unsigned char>(),
                                            json["displayType"]["hoverColor"][1].get<unsigned char>(),
                                            json["displayType"]["hoverColor"][2].get<unsigned char>(),
                                            json["displayType"]["hoverColor"][3].get<unsigned char>()};

            Colors::Utils::Color pressedColor{json["displayType"]["pressedColor"][0].get<unsigned char>(),
                                              json["displayType"]["pressedColor"][1].get<unsigned char>(),
                                              json["displayType"]["pressedColor"][2].get<unsigned char>(),
                                              json["displayType"]["pressedColor"][3].get<unsigned char>()};

            displayType =
                DisplayType::TintColor{json["displayType"]["imageID"].get<ES::Plugin::Object::Utils::AssetID>(),
                                       normalColor, hoverColor, pressedColor};
        }

        std::optional<std::string> onClickScript;

        if (json["onClick"].is_string())
            onClickScript = json["onClick"].get<std::string>();

        return Button{state, lastState, displayType, {}, onClickScript};
    }
};
}; // namespace ES::Plugin::UI::Component
