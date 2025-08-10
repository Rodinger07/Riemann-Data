FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libflint-dev \
    libarb-dev \
    libfftw3-dev \
    libeigen3-dev \
    nvidia-cuda-toolkit \
    sagemath \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install numpy scipy

# Compilar FPLLL
RUN git clone https://github.com/fplll/fplll && \
    cd fplll && \
    ./autogen.sh && \
    ./configure && \
    make && \
    make install

WORKDIR /app
COPY . .
RUN make all

#requirements.txt:

arb
flint
fftw3
eigen
cuda-toolkit
sagemath
numpy
scipy

