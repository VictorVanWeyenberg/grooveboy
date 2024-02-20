#!/bin/bash

# docker build --no-cache -t experimental .
docker run --rm --mount type=bind,source=./,target=/grooveboy experimental
