FROM ubuntu:latest
RUN apt-get update && apt install -y git cmake make g++ libgtk2.0-dev pkg-config build-essential

WORKDIR /opencv-build

RUN git clone --depth 1 "https://github.com/opencv/opencv.git"

RUN cd opencv && mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local && \
    make -j$(nproc) && \
    make install && \
    ldconfig

WORKDIR /proctoring

COPY . .

RUN make

CMD [ "bin/main" ]