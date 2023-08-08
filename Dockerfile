FROM devkitpro/devkitarm:latest
WORKDIR /grooveboy
COPY . .
RUN make gba
