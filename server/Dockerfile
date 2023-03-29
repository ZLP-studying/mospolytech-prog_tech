FROM ubuntu

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update
RUN apt install cmake -y
RUN apt-get install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools -y
RUN apt-get install build-essential -y

WORKDIR /root/
RUN mkdir server
WORKDIR /root/server/
RUN mkdir src
COPY src/*.cpp /root/server/src
COPY src/*.h /root/server/src
COPY src/*.pro /root/server/src
COPY CMakeLists.txt /root/server/

RUN mkdir build
WORKDIR /root/server/build
RUN cmake ..
RUN make

ENTRYPOINT ["./TcpServer"]