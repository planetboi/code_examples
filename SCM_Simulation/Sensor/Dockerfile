#build stage
FROM alpine:3.14
RUN apk add --no-cache gcc g++ musl-dev
COPY . /usr/src/sensor
WORKDIR /usr/src/sensor
RUN g++ -o sensor main.cpp
CMD [ "./sensor"]