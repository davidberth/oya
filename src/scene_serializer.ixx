// src/scene_serializer.ixx
module;

#include <fstream>
#include <vector>
#include <glm/glm.hpp>

export module scene_serializer;
import node;
import scene;

export class SceneSerializer {
public:
    // version number to handle format changes
    static constexpr uint32_t CURRENT_VERSION = 1;

    static void serialize(const Scene& scene, const std::string& filepath) {
        std::ofstream file(filepath, std::ios::binary);
        write_header(file);
        serialize_node(file, scene.root);
    }

    static void deserialize(Scene& scene, const std::string& filepath) {
        //scene.root = deserialize_node(file, nullptr);
        //scene.setup();
        //scene.setup_shader();
        std::ifstream file(filepath, std::ios::binary);
        verify_header(file);

        scene.root = deserialize_node(file, nullptr);
        // scene.create_world();
        scene.setup();
        scene.setup_shader();
        
    }

private:
    static void write_header(std::ofstream& file) {
        file.write(reinterpret_cast<const char*>(&CURRENT_VERSION), sizeof(CURRENT_VERSION));
    }

    static void verify_header(std::ifstream& file) {
        uint32_t version;
        file.read(reinterpret_cast<char*>(&version), sizeof(version));
        if (version != CURRENT_VERSION) {
            throw std::runtime_error("Incompatible scene file version");
        }
    }

    static void serialize_node(std::ofstream& file, const Node* node) {
        // write node properties
        uint32_t outline_size = static_cast<uint32_t>(node->polygon.core.size());
        file.write(reinterpret_cast<const char*>(&outline_size), sizeof(outline_size));
        file.write(reinterpret_cast<const char*>(node->polygon.core.data()), outline_size * sizeof(glm::vec2));
        file.write(reinterpret_cast<const char*>(&node->polygon.color), sizeof(glm::vec3));
        file.write(reinterpret_cast<const char*>(&node->polygon.outline_color), sizeof(glm::vec3));
        file.write(reinterpret_cast<const char*>(&node->polygon.outline_width), sizeof(float));
        file.write(reinterpret_cast<const char*>(&node->rotate_delta), sizeof(float));
        file.write(reinterpret_cast<const char*>(&node->angle), sizeof(float));

        // write children
        uint32_t child_count = static_cast<uint32_t>(node->children.size());
        file.write(reinterpret_cast<const char*>(&child_count), sizeof(child_count));
        for (const Node* child : node->children) {
            serialize_node(file, child);
        }
    }

    static Node* deserialize_node(std::ifstream& file, Node* parent) {
        Node* node = new Node();

        // read node properties
        uint32_t outline_size;
        file.read(reinterpret_cast<char*>(&outline_size), sizeof(outline_size));
        node->polygon.core.resize(outline_size);
        file.read(reinterpret_cast<char*>(node->polygon.core.data()), outline_size * sizeof(glm::vec2));
        file.read(reinterpret_cast<char*>(&node->polygon.color), sizeof(glm::vec3));
        file.read(reinterpret_cast<char*>(&node->polygon.outline_color), sizeof(glm::vec3));
        file.read(reinterpret_cast<char*>(&node->polygon.outline_width), sizeof(float));
        file.read(reinterpret_cast<char*>(&node->rotate_delta), sizeof(float));
        file.read(reinterpret_cast<char*>(&node->angle), sizeof(float));

        node->parent = parent;
        node->setup_polygon();
        node->centroid = node->polygon.compute_centroid();
        
        // read children
        uint32_t child_count;
        file.read(reinterpret_cast<char*>(&child_count), sizeof(child_count));
        for (uint32_t i = 0; i < child_count; i++) {
            Node* child = deserialize_node(file, node);
            node->children.push_back(child);
        }

        return node;
    }
};

export SceneSerializer scene_serializer;