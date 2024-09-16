FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive

RUN apt -y update
RUN apt install -y --no-install-recommends ninja-build \
                libssl-dev \
                libcurl4-openssl-dev \
                python3 \
                libpython3-dev \
                python3-pip \
                libblas-dev \
                liblapack-dev \
                gcc-11 \
                g++-11 \
                git \
                cmake \
                wget \
                lsb-release \
                software-properties-common \
                gpg \
                libzstd-dev

RUN ln -sf /usr/bin/gcc-11 /usr/bin/gcc && \
    ln -sf /usr/bin/g++-11 /usr/bin/g++ && \
    ln -sf /usr/bin/gcc-11 /usr/bin/cc && \
    ln -sf /usr/bin/g++-11 /usr/bin/c++

WORKDIR /home/dev
RUN git clone https://github.com/danclaudino/xacc.git
WORKDIR /home/dev/xacc/build
RUN git fetch origin
RUN git checkout origin/ibm_update
RUN cmake -GNinja /home/dev/xacc -DXACC_BUILD_TESTS=TRUE -DXACC_REMOTE_ACCELERATORS=ON -DQIREE_MINIMAL_BUILD=ON
RUN cmake --build . --target install
RUN ctest --output-on-failure

WORKDIR /home/dev/llvm
RUN wget https://apt.llvm.org/llvm.sh && chmod +x llvm.sh && ./llvm.sh 17

WORKDIR /home/dev
RUN git clone https://github.com/ORNL-QCI/qiree.git
WORKDIR /home/dev/qiree/build
RUN cmake -GNinja /home/dev/qiree -DCMAKE_PREFIX_PATH=$HOME/.xacc
RUN cmake --build .
