FROM ubuntu:26.04

ARG TARGETARCH

# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages and clean up
RUN apt-get update && \
    apt-get install -y \
    zsh \
    build-essential \
    git \
    curl \
    ca-certificates \
    vim \
    gcc \
    g++ \
    gdb \
    libomp-dev \
    qemu-user \
    libc6-armel-cross libc6-dev-armel-cross binutils-arm-linux-gnueabi libncurses5-dev bison flex libssl-dev bc \
    gcc-arm-linux-gnueabi \
    gcc-arm-none-eabi \
    gdb-multiarch \
    net-tools \
    && apt-get clean \
    && apt-get autoremove -y --purge \
    && rm -rf /var/lib/apt/lists/* \
    && mkdir -p /workspace /opt/resources \
    && git clone https://github.com/remzi-arpacidusseau/ostep-code /opt/resources/ostep-code \
    && git clone https://github.com/remzi-arpacidusseau/ostep-homework /opt/resources/ostep-homework

# Copy resources files
COPY resources/ /opt/resources/

# Copy misc configuration files
COPY miscs/.zshrc /root/.zshrc

WORKDIR /workspace

CMD ["/bin/zsh"]
