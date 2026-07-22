FROM ubuntu:25.10

RUN apt-get -y update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
      gcc g++ build-essential gdb \
      python3-full python3-pip

RUN mkdir -p /usr/code && \
    python3 -m venv /usr/code/venv && \
    /usr/code/venv/bin/pip install gcovr

## Cleanup cached apt data we don't need anymore
RUN apt-get autoremove -y && apt-get clean && \
    rm -rf /var/lib/apt/lists/*


VOLUME /usr/code/src

WORKDIR /usr/code/src

CMD ["/bin/bash"]
