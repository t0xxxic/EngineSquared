#pragma once

#include "AssetID.hpp"
#include <nlohmann/json.hpp>

namespace ES::Plugin::Object::Component {
/**
 * Component used to identify a mesh within a game object. MeshID refers to an id of a mesh that should be inside of a
 * AssetsManager.
 */
struct MeshID {
    /**
     * Id refering to a mesh.
     */
    ES::Plugin::Object::Utils::AssetID meshID;

    /**
     * Serialize the MeshID to JSON
     */
    nlohmann::json serialize() const
    {
        return nlohmann::json{
            {"meshID", meshID}
        };
    }

    /**
     * Deserialize a MeshID from JSON
     */
    static MeshID deserialize(const nlohmann::json &json)
    {
        if (!json.contains("meshID") || !json["meshID"].is_number())
            throw std::invalid_argument("Invalid JSON for MeshID deserialization");

        Object::Utils::AssetID meshID = json["meshID"].get<Object::Utils::AssetID>();

        return MeshID{meshID};
    }
};
} // namespace ES::Plugin::Object::Component
