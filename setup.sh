#!/bin/bash
sudo apt update
sudo apt upgrade

sudo apt install -y \
    build-essential \
    cmake \
    ninja-build \
    ccache \
    zlib1g \
    git \
    wget

echo "# latest
deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy main
deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy main
# 18
deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main
deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main
# 17
deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-17 main
deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-17 main
# 16
deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main
deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main
" | sudo tee -a /etc/apt/sources.list.d/llvm.list > /dev/null

wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | sudo tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc

sudo apt update
sudo apt install -y \
    libllvm17 \
    llvm-17 \
    llvm-17-dev \
    lld-17 \
    lldb-17 \
    clang-17