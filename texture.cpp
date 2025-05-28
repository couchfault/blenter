//
// Created by James Pickering on 9/1/24.
//

#include "texture.hpp"

#include <iostream>

#include <glad/glad.h>
#include <ostream>

#include <stb_image.h>

auto Texture::fromPng(const TextureLoadReq& req) -> unsigned int {
    auto width = int{};
    auto height = int{};
    auto numChannels = int{};

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    stbi_set_flip_vertically_on_load(req.flip);

    const auto data = stbi_load(req.path.c_str(), &width, &height, &numChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: \"" << req.path << "\"" << std::endl;
        stbi_image_free(data);
        return 0;
    }

    std::cout << req.path.c_str() << std::endl;
    std::cout << numChannels << std::endl;
    std::cout << req.hasAlpha << std::endl;

    auto id = (unsigned int){};

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, req.hasAlpha ? GL_RGBA : GL_RGB, width, height, 0, req.hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    if (req.repeat) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return id;
}
