module;

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "loguru.hpp"

export module scene_loader;

export class SceneLoader {
public:
    struct NodeData {
        std::string type;
        std::vector<glm::vec2> vertices;
        glm::vec3 fillColor{ 0.5f, 0.5f, 0.5f };    // default gray fill
        glm::vec3 outlineColor{ 0.3f, 0.3f, 0.3f }; // default dark outline
        float outlineWidth{ 0.02f };                  // default width
        int depth{ 0 };                              // indentation level
        std::vector<std::shared_ptr<NodeData>> children;
    };

    std::vector<std::shared_ptr<NodeData>> loadFromFile(const std::string& filename) {
        std::vector<std::shared_ptr<NodeData>> nodes;
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open scene file: " + filename);
        }

        std::string line;
        std::vector<std::shared_ptr<NodeData>> nodeStack;

        while (std::getline(file, line)) {
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') {
                continue;
            }

            // Calculate indentation level
            int depth = 0;
            while (depth < line.length() && line[depth] == ' ') {
                depth++;
            }
            depth /= 4; // Assuming 4 spaces per indentation level

            // Parse the node
            auto node = parseLine(line.substr(depth * 4));
            node->depth = depth;

            // Handle hierarchy
            if (depth == 0) {
                nodes.push_back(node);
                nodeStack = { node };
            }
            else {
                while (nodeStack.size() > depth) {
                    nodeStack.pop_back();
                }
                if (nodeStack.empty()) {
                    throw std::runtime_error("Invalid indentation level");
                }
                nodeStack.back()->children.push_back(node);
                nodeStack.push_back(node);
            }
        }

        return nodes;
    }

private:
    std::shared_ptr<NodeData> parseLine(const std::string& line) {
        auto node = std::make_shared<NodeData>();
        std::istringstream iss(line);

        // Read node type
        iss >> node->type;

        std::string token;
        while (iss >> token) {
            if (token == "vertices") {
                // Read vertices until we hit another keyword or end
                std::string vertex;
                while (iss >> vertex) {
                    if (vertex[0] != '(') break;

                    // Parse vertex coordinates
                    float x, y;
                    if (sscanf(vertex.c_str(), "(%f,%f)", &x, &y) == 2) {
                        node->vertices.push_back(glm::vec2(x, y));
                    }
                }
                if (!vertex.empty() && vertex[0] != '(') {
                    token = vertex;
                    
                }
            }
            if (token == "fill_color") {
                std::string color;
                if (iss >> color) {
                    parseColor(color, node->fillColor);
                }
            }
            if (token == "outline_color") {
                std::string color;
                if (iss >> color) {
                    parseColor(color, node->outlineColor);
                }
            }
            if (token == "outline_width") {
                std::string width;
                if (iss >> width) {
                    if (width[0] == '(' && width[width.length() - 1] == ')') {
                        node->outlineWidth = std::stof(width.substr(1, width.length() - 2));
                    }
                }
            }
        }

        return node;
    }

    void parseColor(const std::string& colorStr, glm::vec3& color) {
        float r, g, b;
        if (sscanf(colorStr.c_str(), "(%f,%f,%f)", &r, &g, &b) == 3) {
            color = glm::vec3(r, g, b);
        }
    }
};

