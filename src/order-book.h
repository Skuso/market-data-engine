#pragma once
#include <cstdint>
#include <functional>  // std::greater
#include <map>
#include <optional>
#include "types.hpp"

// Domain types — move these here (out of main.cpp) so book, parser,
// and tests all share one definition. This header is JSON-free and
// network-free by design: the book is testable in total isolation.


class OrderBook {
public:
    // Snapshot semantics: discard everything, rebuild from scratch.
    // Caller hands us already-parsed levels one at a time via set_level
    // after calling clear() — OR you may prefer apply_snapshot taking
    // a vector of (Price, Size) pairs per side. Your call; pick the
    // shape that makes the wiring code read cleanly, and be able to
    // say why.
    void clear();

    // Delta semantics (per your docs reading): size REPLACES the level's
    // total. Size 0 must ERASE the level — your first unit test.
    void set_level(Side side, Price price, Size size);

    // Empty optional when that side of the book is empty — same
    // "absence is a checkable value" philosophy as parse_fixed_point.
    std::optional<Price> best_bid() const;
    std::optional<Price> best_ask() const;

    // Size resting at an exact price; 0 or nullopt if level absent —
    // decide which and be consistent with your erase-on-zero rule.
    std::optional<Size> size_at(Side side, Price price) const;

    // How many price levels each side currently holds (your zero-erase
    // test will assert on this).
    std::size_t depth(Side side) const;

    // Has a snapshot been applied yet? Deltas arriving before a
    // snapshot are the "unready state" question from last message —
    // representing it beats pretending it can't happen.
    bool ready() const;

private:
    // THE core decision, made concrete: bids iterate best-first via a
    // descending comparator; asks ascend naturally. Best price on each
    // side is therefore begin() — O(1) reads of the thing read most.
    std::map<Price, Size, std::greater<Price>> bids_;
    std::map<Price, Size>                      asks_;

    bool ready_ = false;
};