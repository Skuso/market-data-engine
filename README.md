# market-data-engine

A real-time C++20 market data engine that consumes live cryptocurrency exchange feeds over TLS WebSockets, parses trades into exact fixed-point types, and maintains a live limit order book, built from scratch as a learning project, working toward a paper-trading system with honest fill simulation.

## Current capabilities

- Connects to the Coinbase Exchange WebSocket feed (`matches` channel) over TLS with certificate verification and SNI
- Parses live trade messages into typed `Trade` values
- Exact decimal arithmetic: prices and sizes parsed **directly from string to scaled `int64_t`** (×10⁸), never passing through floating point: see [Design notes](#design-notes)
- Limit order book with price-level aggregation (in progress, test-first)

## Building

Requires a Linux environment (developed on Ubuntu under WSL2), GCC 13+ and CMake 3.25+.

```bash
# Install toolchain and dependencies
./scripts/setup.sh

# Authenticate with GitHub (one-time, interactive)
gh auth login

# Build
cmake -B build
cmake --build build

# Run the feed client
./build/market-data-engine

# Run the tests
./build/order_book_test
```

### Dependencies

| Library | Role |
|---|---|
| Boost.Asio | Sockets, DNS resolution, TLS stream wrapper |
| Boost.Beast | WebSocket protocol on top of Asio |
| OpenSSL | TLS cryptography and certificate verification |
| nlohmann/json | JSON parsing of feed messages |
| Catch2 | Unit tests |

Later phases will add benchmarking/profiling tooling and possibly a faster JSON parser; dependencies are documented as they land, not before.

## Design notes

**Prices are integers.** Exchanges send prices as JSON *strings* because JSON numbers decay to IEEE-754 doubles, which cannot exactly represent most decimal fractions. This engine parses those strings directly into `int64_t` scaled by 10⁸ using `std::from_chars`, no `stod`, no double anywhere in the money path. Malformed input returns `std::optional` emptiness rather than throwing ("parse, don't validate").

**The order book is network-free.** `OrderBook` takes typed values and knows nothing about JSON or sockets, so it is unit-tested in total isolation with hand-crafted update sequences. Level updates use replace-semantics (a size *replaces* the level's total; size 0 erases the level), matching the Coinbase `level2` channel contract.

**Bids and asks are mirrored maps.** `std::map<Price, Size, std::greater<Price>>` for bids, ascending for asks — both sides expose their best price at `begin()`. Chosen for clarity first; measurement-driven optimization is a later phase, on purpose.

## Roadmap

- [x] **Phase 0 — Foundation:** CMake, strict warnings, tooling, repo hygiene
- [x] **Phase 1a — Live feed:** TLS WebSocket connection, subscription, message stream
- [x] **Phase 1b — Typed parsing:** fixed-point parser (`from_chars`, optional-based failure), Trade structs
- [ ] **Phase 1c — Resilience:** reconnect loop with exponential backoff, 30-minute unattended run
- [ ] **Phase 2 — Order book:** live L2 book, sequence-gap handling, 15+ unit tests *(in progress)*
- [ ] **Phase 3 — Analytics:** spread, microprice, imbalance, rolling VWAP/volatility in constant memory
- [ ] **Phase 4 — Concurrency:** network/processing threads, SPSC queue, sanitizer-clean
- [ ] **Phase 5 — Performance:** replay benchmarking, p50/p99 latency, profile-guided optimization
- [ ] **Phase 6 — Paper trading:** strategy interface, book-walking fill simulator, P&L and risk limits
- [ ] **Phase 7 — Polish:** architecture docs, benchmark numbers, demo

## Engineering log

Decisions, bugs, and lessons are recorded in [LOG.md](LOG.md) as they happen, including the ones that went wrong first.

<img width="1547" height="663" alt="image" src="https://github.com/user-attachments/assets/cb764f8e-2051-4496-a806-523a9c948628" />

<img width="345" height="632" alt="image" src="https://github.com/user-attachments/assets/83c9c5cf-d145-4689-8069-544d2b4103db" />

