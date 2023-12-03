FROM devkitpro/devkitarm:latest AS build-stage
RUN apt update
RUN apt install build-essential -y
RUN git clone https://github.com/gwilymk/bin2c.git 
RUN gcc -o /opt/devkitpro/tools/bin/bin2c ./bin2c/bin2c.c
WORKDIR /grooveboy
ENTRYPOINT make
