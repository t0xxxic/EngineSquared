#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include <nlohmann/json.hpp>

namespace ES::Plugin::Object::Component {
/**
 * Component used to represent a 3D transformation for a game object.
 * It can be used as a 2D transformation.
 * This transformation is represented by position, scale, and rotation.
 */
struct Transform {
    /**
     * Position of the entity
     */
    glm::vec3 position;
    /**
     * Scale of the entity
     */
    glm::vec3 scale;
    /**
     * Rotation of the entity
     * Represents a rotation around the origin, represented as a quaternion.
     */
    glm::quat rotation;

    /**
     * \param   position    position of the entity. Default is (0, 0, 0).
     * \param   scale       scale of the entity. Default is (1, 1, 1).
     * \param   rotation    rotation of the entity. Default is identity (no rotation).
     */
    Transform(glm::vec3 position = glm::vec3(0), glm::vec3 scale = glm::vec3(1),
              glm::quat rotation = glm::quat(1, 0, 0, 0))
        : position(position), scale(scale), rotation(rotation)
    {
    }

    // Getters
    inline const glm::vec3 &getPosition() const { return position; }
    inline const glm::vec3 &getScale() const { return scale; }
    inline const glm::quat &getRotation() const { return rotation; }

    // Setters
    void setPosition(const glm::vec3 &newPosition) { position = newPosition; }
    void setPosition(float x, float y, float z) { position = glm::vec3(x, y, z); }
    void setScale(const glm::vec3 &newScale) { scale = newScale; }
    void setScale(float x, float y, float z) { scale = glm::vec3(x, y, z); }
    void setRotation(const glm::quat &newRotation) { rotation = newRotation; }
    void setRotation(float x, float y, float z, float w) { rotation = glm::quat(w, x, y, z); }

    /**
     * Create the transformation matrix for this transform component.
     *
     * \return  transformation matrix that combines the position, scale, and rotation of the entity.
     */
    glm::mat4 getTransformationMatrix() const
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        return translation * rotationMatrix * scaleMatrix;
    }

    /**
     * Serialize the Transform to JSON
     */
    nlohmann::json serialize() const
    {
        return nlohmann::json{
            {"position", {position.x, position.y, position.z}            },
            {"scale",    {scale.x, scale.y, scale.z}                     },
            {"rotation", {rotation.x, rotation.y, rotation.z, rotation.w}}
        };
    }

    /**
     * Deserialize a Transform from JSON
     */
    static Transform deserialize(const nlohmann::json &json)
    {
        if (!json.contains("position") || !json["position"].is_array() || json["position"].size() != 3 ||
            !json["position"][0].is_number() || !json["position"][1].is_number() || !json["position"][2].is_number())
            throw std::invalid_argument("Invalid JSON for Transform deserialization : position");

        if (!json.contains("scale") || !json["scale"].is_array() || json["scale"].size() != 3 ||
            !json["scale"][0].is_number() || !json["scale"][1].is_number() || !json["scale"][2].is_number())
            throw std::invalid_argument("Invalid JSON for Transform deserialization : scale");

        if (!json.contains("rotation") || !json["rotation"].is_array() || json["rotation"].size() != 4 ||
            !json["rotation"][0].is_number() || !json["rotation"][1].is_number() || !json["rotation"][2].is_number() ||
            !json["rotation"][3].is_number())
            throw std::invalid_argument("Invalid JSON for Transform deserialization : rotation");

        glm::vec3 position{json["position"][0].get<float>(), json["position"][1].get<float>(),
                           json["position"][2].get<float>()};

        glm::vec3 scale{json["scale"][0].get<float>(), json["scale"][1].get<float>(), json["scale"][2].get<float>()};

        glm::quat rotation{json["rotation"][3].get<float>(), json["rotation"][0].get<float>(),
                           json["rotation"][1].get<float>(), json["rotation"][2].get<float>()};

        return Transform{position, scale, rotation};
    }
};
} // namespace ES::Plugin::Object::Component
