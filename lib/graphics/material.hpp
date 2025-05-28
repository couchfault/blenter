//
// Created by James Pickering on 9/2/24.
//

#include <glm/glm.hpp>

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Materials {
    static const Material emerald;
    static const Material jade;
    static const Material obsidian;
    static const Material pearl;
    static const Material ruby;
    static const Material turquoise;
    static const Material brass;
    static const Material bronze;
    static const Material chrome;
    static const Material copper;
    static const Material gold;
    static const Material silver;
    static const Material blackPlastic;
    static const Material cyanPlastic;
    static const Material greenPlastic;
    static const Material redPlastic;
    static const Material whitePlastic;
    static const Material yellowPlastic;
    static const Material blackRubber;
    static const Material cyanRubber;
    static const Material greenRubber;
    static const Material redRubber;
    static const Material whiteRubber;
    static const Material yellowRubber;
};
