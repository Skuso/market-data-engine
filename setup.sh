echo "Installing Market Data Engine dependencies..."

set -e #stop on first error

# Toolchain
sudo apt install -y build-essential cmake git clang-format gdb

# GitHub CLI
sudo apt install -y gh

# Project dependencies
sudo apt install -y libboost-all-dev libssl-dev nlohmann-json3-dev