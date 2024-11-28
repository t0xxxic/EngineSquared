#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace ES::Plugin::Camera::Component {
/**
 * Component used to store the 3D camera data.
 */
struct Camera3D {
    /**
     * Field used to store the camera lookAt position.
     */
    glm::vec3 lookAt;

    /**
     * Field used to store the camera up vector.
     */
    glm::vec3 up;

    /**
     * Field used to store the camera field of view.
     */
    float fov;

    /**
     * Field used to store the camera aspect ratio.
     */
    float aspect;

    /**
     * Field used to store the camera near plane.
     */
    float nearPlane;

    /**
     * Field used to store the camera far plane.
     */
    float farPlane;

    /**
     * Serialize the Camera3D to JSON
     */
    nlohmann::json serialize() const
    {
        return nlohmann::json{
            {"lookAt",    {lookAt.x, lookAt.y, lookAt.z}},
            {"up",        {up.x, up.y, up.z}            },
            {"fov",       fov                           },
            {"aspect",    aspect                        },
            {"nearPlane", nearPlane                     },
            {"farPlane",  farPlane                      },
        };
    }

    /**
     * Deserialize a Camera3D from JSON
     */
    static Camera3D deserialize(const nlohmann::json &json)
    {
        if (!json.contains("lookAt") || !json["lookAt"].is_array() || json["lookAt"].size() != 3 ||
            !json["lookAt"][0].is_number() || !json["lookAt"][1].is_number() || !json["lookAt"][2].is_number())
            throw std::invalid_argument("Invalid JSON for Camera3D deserialization : lookAt");

        if (!json.contains("up") || !json["up"].is_array() || json["up"].size() != 3 || !json["up"][0].is_number() ||
            !json["up"][1].is_number() || !json["up"][2].is_number())
            throw std::invalid_argument("Invalid JSON for Camera3D deserialization : up");

        if (!json.contains("fov") || !json["fov"].is_number() || !json.contains("aspect") ||
            !json["aspect"].is_number() || !json.contains("nearPlane") || !json["nearPlane"].is_number() ||
            !json.contains("farPlane") || !json["farPlane"].is_number())
            throw std::invalid_argument("Invalid JSON for Camera3D deserialization : fov, aspect, nearPlane, farPlane");

        glm::vec3 lookAt = {json["lookAt"][0].get<float>(), json["lookAt"][1].get<float>(),
                            json["lookAt"][2].get<float>()};
        glm::vec3 up = {json["up"][0].get<float>(), json["up"][1].get<float>(), json["up"][2].get<float>()};
        float fov = json["fov"].get<float>();
        float aspect = json["aspect"].get<float>();
        float nearPlane = json["nearPlane"].get<float>();
        float farPlane = json["farPlane"].get<float>();

        return Camera3D{lookAt, up, fov, aspect, nearPlane, farPlane};
    }
};
} // namespace ES::Plugin::Camera::Component
