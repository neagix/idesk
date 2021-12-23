FROM debian:stable

MAINTAINER neagix@users.noreply.github.com

ARG UUID=1000
ARG UGID=1000

RUN groupadd --gid $UGID buser && \
    useradd -m buser --uid $UUID --gid $UGID

RUN export TERM=xterm && \
    apt-get update && apt-get install -y -qq \
            gcc g++ make automake pkg-config \
            libxft-dev libxpm-dev libimlib2-dev libpng-dev librsvg2-dev libgdk-pixbuf2.0-dev libstartup-notification0-dev && \
    apt-get autoremove -y && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /home/buser/idesk

USER buser
