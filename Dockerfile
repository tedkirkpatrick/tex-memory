FROM ubuntu:25.10

RUN apt-get -y update && \
  apt-get -y install gcc g++

CMD bash
