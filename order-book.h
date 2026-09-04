#pragma once
#include <nlohmann/json.hpp>

class OrderBook {
public:
    OrderBook();
    void apply_update(const nlohmann::json& message);
    void apply_snapshot(const nlohmann::json& message);


private:
    void best_bid() const;
    void best_ask() const;
    
};