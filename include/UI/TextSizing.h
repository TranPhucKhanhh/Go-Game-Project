#pragma once
#include <SFML/Graphics/Text.hpp>
#include <algorithm>
#include <cmath>

namespace ui_detail {
inline float textScaleToFit(const sf::Text& text, sf::Vector2f size, float ratio) {
    const auto bounds = text.getLocalBounds().size;
    if (!std::isfinite(ratio) || size.x <= 0.f || size.y <= 0.f ||
        bounds.x <= 0.f || bounds.y <= 0.f) return 0.f;
    ratio = std::clamp(ratio, 0.f, 1.f);
    return std::min(size.x * ratio / bounds.x, size.y * ratio / bounds.y);
}

inline unsigned int fittedCharacterSize(const sf::Text& text, sf::Vector2f size, float ratio) {
    sf::Text probe = text;
    probe.setCharacterSize(64);
    unsigned int candidate = static_cast<unsigned int>(std::floor(64.f * textScaleToFit(probe, size, ratio)));
    // Font hinting can change rounded bounds; check the actual candidate before using it.
    while (candidate > 0) {
        probe.setCharacterSize(candidate);
        if (textScaleToFit(probe, size, ratio) >= 1.f) break;
        --candidate;
    }
    return candidate;
}
}
