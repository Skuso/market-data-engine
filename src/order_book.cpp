#include "order_book.h"

// Constructor implementation (if needed)
void OrderBook::clear() {
  // Implementation for clearing the order book
  bids_.clear();
  asks_.clear();
  ready_ = false;
}
void OrderBook::set_level(Side side, Price price, Size size) {
  // Implementation for setting a level in the order book
  if (side == Side::Bid) {
    if (size == 0) {
      bids_.erase(price);
    } else {
      bids_[price] = size;
    }
  } else {
    if (size == 0) {
      asks_.erase(price);
    } else {
      asks_[price] = size;
    }
  }
}
std::optional<Price> OrderBook::best_bid() const {
  // Implementation for retrieving the best bid price
  if (!bids_.empty()) {
    return bids_.begin()->first;  // Best bid is the highest price
  }
  return std::nullopt;  // No bids available
}
std::optional<Price> OrderBook::best_ask() const {
  // Implementation for retrieving the best ask price
  if (!asks_.empty()) {
    return asks_.begin()->first;  // Best ask is the lowest price
  }
  return std::nullopt;  // No asks available
}
std::optional<Size> OrderBook::size_at(Side side, Price price) const {
  // Implementation for retrieving the size at a specific price level
  if (side == Side::Bid) {
    auto it = bids_.find(price);
    if (it != bids_.end()) {
      return it->second;
    }
  } else {
    auto it = asks_.find(price);
    if (it != asks_.end()) {
      return it->second;
    }
  }
  return std::nullopt;  // Level not found
}
std::size_t OrderBook::depth(Side side) const {
  // Implementation for retrieving the depth of the order book
  if (side == Side::Bid) {
    return bids_.size();
  } else {
    return asks_.size();
  }
}
bool OrderBook::ready() const {
  // Implementation for checking if the order book is ready
  return ready_;  // Placeholder
}
