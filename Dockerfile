FROM alpine:latest

RUN apk update

RUN apk add --no-cache build-base && apk add --no-cache cmake

COPY files /usr/src/webserver/files
COPY srcs /usr/src/webserver/srcs
COPY includes /usr/src/webserver/includes
COPY Makefile /usr/src/webserver

WORKDIR /usr/src/webserver
RUN ls srcs/
RUN make re

CMD ["./server"]
