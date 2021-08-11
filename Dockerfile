FROM ubuntu:20.04

ARG X_USER=docker
ARG X_UID=1000
ARG X_GID=1000
ARG X_PASSWORD=1234

ENV TZ=Europe/Istanbul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime

RUN apt-get update && apt-get install -y \
    sudo \
    build-essential \
    software-properties-common \
    xz-utils \
    wget \
    curl \
    git \
    cmake

# install g++-10
RUN g++ --version \
    && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && apt-get update \
    && apt-get install -y gcc-10 g++-10
RUN g++-10 --version

# install clang-10
# https://apt.llvm.org/
RUN wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh 10
RUN clang-10 --version

# install Google Test
RUN git clone https://github.com/google/googletest.git /google-test \
    && mkdir -p /google-test/build \
    && cd /google-test/build \
    && cmake .. && make && make install \
    && cd / && rm -rf /google-test

# install Google Benchmark
RUN git clone https://github.com/google/benchmark.git /google-benchmark \
    && git clone https://github.com/google/googletest.git /google-benchmark/googletest \
    && mkdir -p /google-benchmark/build \
    && cd /google-benchmark/build \
    && cmake .. && make && make install \
    && cd / && rm -rf /google-benchmark

# add user so that directories created in container's volume have the correct owner
RUN useradd -m ${X_USER} --uid=${X_UID} \
    && echo "${X_USER}:${X_PASSWORD}" | chpasswd \
    && adduser ${X_USER} sudo

USER ${X_UID}:${X_GID}
WORKDIR /home/${X_USER}

CMD ["/bin/bash"]
