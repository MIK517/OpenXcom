FROM alpine:latest

RUN apk add \
        autoconf \
        automake \
        bash \
        binutils \
        bison \
        bzip2 \
        flex \
        g++ \
        gdk-pixbuf \
        gettext \
        git \
        gperf \
        intltool \
        libtool \
        linux-headers \
        lzip \
        make \
        openssl \
        openssl-dev \
        p7zip \
        patch \
        perl \
        python3 \
        py3-mako \
        ruby \
        unzip \
        wget \
        xz \
        zip \
        zlib

RUN cd /opt && git clone https://github.com/mxe/mxe.git
RUN adduser -D builder
RUN chown -R builder: /opt/mxe
RUN ln -s /usr/bin/7z /usr/bin/7za
USER builder
RUN cd /opt/mxe && make MXE_PLUGIN_DIRS=plugins/gcc13 MXE_TARGETS=x86_64-w64-mingw32.static JOBS=8 gcc cmake sdl sdl_gfx sdl_mixer sdl_image yaml-cpp zlib
ENV PATH="/opt/mxe/usr/bin:$PATH"
