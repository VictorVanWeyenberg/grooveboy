FROM devkitpro/devkitarm:latest AS build-stage
RUN apt update
RUN apt install ffmpeg -y
WORKDIR /grooveboy
ENTRYPOINT make
