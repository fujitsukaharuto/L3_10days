#include "EasingIhara.h"

#include <cmath>
#include <numbers>

// イージング用定数
static const float c1 = 1.70158f;
static const float c2 = c1 * 1.525f;
static const float c3 = c1 + 1.0f;
static const float c4 = (2.0f * std::numbers::pi_v<float>) / 3.0f;
static const float c5 = (2.0f * std::numbers::pi_v<float>) / 4.5f;

static const float n1 = 7.5625f;
static const float d1 = 2.75f;

float EasingIhara::Apply(EasingType type, float t) {
	switch (type) {
	case EasingType::Linear:          return EaseLinear(t);

	case EasingType::EaseInSine:        return EaseInSine(t);
	case EasingType::EaseOutSine:       return EaseOutSine(t);
	case EasingType::EaseInOutSine:     return EaseInOutSine(t);

	case EasingType::EaseInQuad:        return EaseInQuad(t);
	case EasingType::EaseOutQuad:       return EaseOutQuad(t);
	case EasingType::EaseInOutQuad:     return EaseInOutQuad(t);

	case EasingType::EaseInCubic:       return EaseInCubic(t);
	case EasingType::EaseOutCubic:      return EaseOutCubic(t);
	case EasingType::EaseInOutCubic:    return EaseInOutCubic(t);

	case EasingType::EaseInQuart:       return EaseInQuart(t);
	case EasingType::EaseOutQuart:      return EaseOutQuart(t);
	case EasingType::EaseInOutQuart:    return EaseInOutQuart(t);

	case EasingType::EaseInQuint:       return EaseInQuint(t);
	case EasingType::EaseOutQuint:      return EaseOutQuint(t);
	case EasingType::EaseInOutQuint:    return EaseInOutQuint(t);

	case EasingType::EaseInExpo:        return EaseInExpo(t);
	case EasingType::EaseOutExpo:       return EaseOutExpo(t);
	case EasingType::EaseInOutExpo:     return EaseInOutExpo(t);

	case EasingType::EaseInCirc:        return EaseInCirc(t);
	case EasingType::EaseOutCirc:       return EaseOutCirc(t);
	case EasingType::EaseInOutCirc:     return EaseInOutCirc(t);

	case EasingType::EaseInBack:        return EaseInBack(t);
	case EasingType::EaseOutBack:       return EaseOutBack(t);
	case EasingType::EaseInOutBack:     return EaseInOutBack(t);

	case EasingType::EaseInElastic:     return EaseInElastic(t);
	case EasingType::EaseOutElastic:    return EaseOutElastic(t);
	case EasingType::EaseInOutElastic:  return EaseInOutElastic(t);

	case EasingType::EaseInBounce:      return EaseInBounce(t);
	case EasingType::EaseOutBounce:     return EaseOutBounce(t);
	case EasingType::EaseInOutBounce:   return EaseInOutBounce(t);

	default:                            return t;
	}
}

float EasingIhara::EaseLinear(float t) {
	return t;
}

// ───────── Sine ─────────
float EasingIhara::EaseInSine(float t) {
	return 1.0f - std::cos((t * std::numbers::pi_v<float>) / 2.0f);
}

float EasingIhara::EaseOutSine(float t) {
	return std::sin((t * std::numbers::pi_v<float>) / 2.0f);
}

float EasingIhara::EaseInOutSine(float t) {
	return -(std::cos(std::numbers::pi_v<float> *t) - 1.0f) * 0.5f;
}

// ───────── Quad ─────────
float EasingIhara::EaseInQuad(float t) {
	return t * t;
}

float EasingIhara::EaseOutQuad(float t) {
	return 1.0f - (1.0f - t) * (1.0f - t);
}

float EasingIhara::EaseInOutQuad(float t) {
	return (t < 0.5f) ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) * 0.5f;
}

// ───────── Cubic ─────────
float EasingIhara::EaseInCubic(float t) {
	return t * t * t;
}

float EasingIhara::EaseOutCubic(float t) {
	return 1.0f - std::pow(1.0f - t, 3.0f);
}

float EasingIhara::EaseInOutCubic(float t) {
	return (t < 0.5f) ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) * 0.5f;
}

// ───────── Quart ─────────
float EasingIhara::EaseInQuart(float t) {
	return t * t * t * t;
}

float EasingIhara::EaseOutQuart(float t) {
	return 1.0f - std::pow(1.0f - t, 4.0f);
}

float EasingIhara::EaseInOutQuart(float t) {
	return (t < 0.5f) ? 8.0f * std::pow(t, 4.0f) : 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) * 0.5f;
}

// ───────── Quint ─────────
float EasingIhara::EaseInQuint(float t) {
	return t * t * t * t * t;
}

float EasingIhara::EaseOutQuint(float t) {
	return 1.0f - std::pow(1.0f - t, 5.0f);
}

float EasingIhara::EaseInOutQuint(float t) {
	return (t < 0.5f) ? 16.0f * std::pow(t, 5.0f) : 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) * 0.5f;
}

// ───────── Expo ─────────
float EasingIhara::EaseInExpo(float t) {
	return (t == 0.0f) ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
}

float EasingIhara::EaseOutExpo(float t) {
	return (t == 1.0f) ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
}

float EasingIhara::EaseInOutExpo(float t) {
	if (t == 0.0f) return 0.0f;
	if (t == 1.0f) return 1.0f;
	return (t < 0.5f)
		? std::pow(2.0f, 20.0f * t - 10.0f) * 0.5f
		: (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) * 0.5f;
}

// ───────── Circ ─────────
float EasingIhara::EaseInCirc(float t) {
	return 1.0f - std::sqrt(1.0f - t * t);
}

float EasingIhara::EaseOutCirc(float t) {
	return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
}

float EasingIhara::EaseInOutCirc(float t) {
	return (t < 0.5f)
		? (1.0f - std::sqrt(1.0f - 4.0f * t * t)) * 0.5f
		: (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) * 0.5f;
}

// ───────── Back ─────────
float EasingIhara::EaseInBack(float t) {
	return c3 * t * t * t - c1 * t * t;
}

float EasingIhara::EaseOutBack(float t) {
	return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
}

float EasingIhara::EaseInOutBack(float t) {
	return (t < 0.5f)
		? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) * 0.5f
		: (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) * 0.5f;
}

// ───────── Elastic ─────────
float EasingIhara::EaseInElastic(float t) {
	if (t == 0.0f || t == 1.0f) return t;
	return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
}

float EasingIhara::EaseOutElastic(float t) {
	if (t == 0.0f || t == 1.0f) return t;
	return  std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float EasingIhara::EaseInOutElastic(float t) {
	if (t == 0.0f || t == 1.0f) return t;
	return (t < 0.5f)
		? -0.5f * std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)
		: 0.5f * std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5) + 1.0f;
}

// ───────── Bounce ─────────
static float EaseOutBounceInternal(float t) {
	if (t < 1.0f / d1) {
		return n1 * t * t;
	} else if (t < 2.0f / d1) {
		t -= 1.5f / d1;
		return n1 * t * t + 0.75f;
	} else if (t < 2.5f / d1) {
		t -= 2.25f / d1;
		return n1 * t * t + 0.9375f;
	} else {
		t -= 2.625f / d1;
		return n1 * t * t + 0.984375f;
	}
}

float EasingIhara::EaseOutBounce(float t) {
	return EaseOutBounceInternal(t);
}

float EasingIhara::EaseInBounce(float t) {
	return 1.0f - EaseOutBounceInternal(1.0f - t);
}

float EasingIhara::EaseInOutBounce(float t) {
	return (t < 0.5f)
		? (1.0f - EaseOutBounceInternal(1.0f - 2.0f * t)) * 0.5f
		: (1.0f + EaseOutBounceInternal(2.0f * t - 1.0f)) * 0.5f;
}