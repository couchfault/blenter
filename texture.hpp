//
// Created by James Pickering on 9/1/24.
//

#pragma once

#include <string>

struct TextureLoadReq {
    std::string path;
    bool hasAlpha = true;
    bool flip = true;
    bool repeat = false;
};

struct Texture {
    static auto fromPng(const TextureLoadReq& req) -> unsigned int;
};
