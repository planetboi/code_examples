#build stage
FROM planetboi/ubuntu_thrift:2.6
COPY . /usr/src/lager
COPY thrift.sh .
ENTRYPOINT ["./thrift.sh"]