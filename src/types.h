#pragma once
#include <cstdint>

constexpr std::int64_t PRICE_SCALE = 100'000'000;

using Price = std::int64_t;
using Size = std::int64_t;

enum class Side { Bid, Ask };