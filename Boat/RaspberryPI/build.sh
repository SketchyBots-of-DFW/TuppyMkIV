#!/usr/bin/env bash

docker build -t sketchybots/roboboat-2024:base -f Dockerfile .
docker push sketchybots/roboboat-2024:base

docker build -t sketchybots/roboboat-2024:nvidia -f Dockerfile.nvidia .
docker push sketchybots/roboboat-2024:nvidia
