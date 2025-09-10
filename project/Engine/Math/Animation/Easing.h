#pragma once

#include <numbers>

// カスのライフハック
constexpr r32 PI_H = std::numbers::pi_v<r32> *0.5f;
constexpr r32 PI = std::numbers::pi_v<r32>;
constexpr r32 PI2 = std::numbers::pi_v<r32> *2.0f;


namespace Easing {

namespace In {

r32 Sine(r32 t) noexcept;
r32 Quad(r32 t) noexcept;
r32 Cubic(r32 t) noexcept;
r32 Quart(r32 t) noexcept;
r32 Quint(r32 t) noexcept;
r32 Expo(r32 t) noexcept;
r32 Circ(r32 t) noexcept;
r32 Back(r32 t) noexcept;
r32 Elastic(r32 t) noexcept;
r32 Bounce(r32 t) noexcept;

};

namespace Out {

r32 Sine(r32 t) noexcept;
r32 Quad(r32 t) noexcept;
r32 Cubic(r32 t) noexcept;
r32 Quart(r32 t) noexcept;
r32 Quint(r32 t) noexcept;
r32 Expo(r32 t) noexcept;
r32 Circ(r32 t) noexcept;
r32 Back(r32 t) noexcept;
r32 Elastic(r32 t) noexcept;
r32 Bounce(r32 t) noexcept;

};

namespace InOut {

r32 Sine(r32 t) noexcept;
r32 Quad(r32 t) noexcept;
r32 Cubic(r32 t) noexcept;
r32 Quart(r32 t) noexcept;
r32 Quint(r32 t) noexcept;
r32 Expo(r32 t) noexcept;
r32 Circ(r32 t) noexcept;
r32 Back(r32 t) noexcept;
r32 Elastic(r32 t) noexcept;
r32 Bounce(r32 t) noexcept;

};

};
