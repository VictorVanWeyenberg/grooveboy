FROM devkitpro/devkitarm:latest AS build-stage
RUN apt update
RUN apt install build-essential -y
RUN git clone https://github.com/gwilymk/bin2c.git 
RUN gcc -o /opt/devkitpro/tools/bin/bin2c ./bin2c/bin2c.c
WORKDIR /grooveboy
COPY . .
RUN ls .
RUN make bin
RUN make
RUN cp grooveboy.gba /

FROM scratch AS export-stage
COPY --from=build-stage /grooveboy.gba .
