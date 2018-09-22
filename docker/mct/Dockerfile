FROM manimaul/proxygen-builder:stretch
MAINTAINER manimaul@gmail.com

## Marine Chart TileServer Build
COPY CMakeLists.txt /build/CMakeLists.txt
COPY src/ /build/src/
COPY resources/ build/resources
RUN cd build && \
    mkdir _build && \
    cd _build && \
    cmake .. && \
    make -j $(nproc) && \
    make install

CMD "/opt/mct/bin/marine_chart_tileserver"
