#pragma once

// The game's button symbols drawn as the buttons of the gamepad in use
// (Controls > Button prompts; GameProfile::edited_textures and edit_texture).

#include "profile.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace d012 {

// The game's textures that hold button symbols, by their keys.
[[nodiscard]] std::span<const portablekit::EditedTexture> button_prompt_textures();

// Which of them is the game's icon sheet (SELECT, START, L, R and the face
// buttons), which the port's own dialogs draw from too (d012_dialogs.cpp).
inline constexpr std::size_t kButtonPromptIcons = 0u;

// Draws texture `index` of button_prompt_textures() again at four times its
// size, its symbols in the context's style; false for the PSP's own.
bool draw_button_prompts(std::size_t index, const std::uint32_t *pixels, std::uint32_t width, std::uint32_t height,
                         const portablekit::TextureEditContext &context, std::vector<std::uint32_t> &out,
                         std::uint32_t &out_width, std::uint32_t &out_height);

} // namespace d012
