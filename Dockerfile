FROM ubuntu:bionic

RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake python3-pip

WORKDIR /hardware-effects
COPY requirements.txt requirements.txt
RUN pip3 install -r requirements.txt

COPY . .
RUN mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j
