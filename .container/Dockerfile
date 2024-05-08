# Dockerfile for practicing LLVM
# VMO Lab, 2024
FROM ubuntu:22.04

ENV TERM xterm-256color
ARG UID=1000
ARG GID=1000

RUN apt update
RUN apt upgrade -y
# Essentials
RUN apt install -y \
    sudo \
    software-properties-common \
    vim

RUN apt update
RUN apt upgrade -y
# Dependencies
RUN apt install -y \
    build-essential \
    net-tools \
    openssh-server \
    openssh-client \
    cmake \
    ninja-build \
    ccache \
    zlib1g \
    git \
    wget
RUN apt update
RUN apt upgrade -y

# Add llvm-apt archive.
RUN printf "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy main\n \
    deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy main\n \
    # 18\n \
    deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main\n \
    deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main\n \
    # 17\n \
    deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-17 main\n \
    deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-17 main\n \
    # 16\n \
    deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main\n \
    deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main\n \
    \n" > /etc/apt/sources.list.d/llvm.list
RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | sudo tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc

RUN apt update
RUN apt install -y \
    libllvm17 \
    llvm-17 \
    llvm-17-dev \
    lld-17 \
    lldb-17 \
    clang-17

COPY script/update-alternatives-llvm.sh /home/ubuntu/update-alternatives-llvm.sh
RUN chmod +x /home/ubuntu/update-alternatives-llvm.sh
RUN /bin/bash -c "/home/ubuntu/update-alternatives-llvm.sh 17 0"
RUN rm /home/ubuntu/update-alternatives-llvm.sh

# Auxiliary
RUN apt install -y \
    zlib1g-dev \
    libcurl4-openssl-dev \
    libzstd-dev

# Root P/W: ubuntu
RUN echo "root:ubuntu" | chpasswd
# ubuntu P/W: ubuntu
RUN groupadd -g $GID -o ubuntu
RUN useradd -m -u $UID -g $GID -s /bin/bash ubuntu && \
    usermod -aG sudo ubuntu
RUN echo "ubuntu:ubuntu" | chpasswd
RUN echo "%sudo ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
RUN sudo -k
RUN chown -R ubuntu /home/ubuntu/

USER ubuntu

WORKDIR /home/ubuntu
ENTRYPOINT tail -f /dev/null
EXPOSE 22