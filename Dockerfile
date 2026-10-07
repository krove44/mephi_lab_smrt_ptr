FROM ubuntu:22.04
RUN apt-get update -qq && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential cmake git valgrind gdb