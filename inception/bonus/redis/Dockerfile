# /!\ here, redis only serves a "cache" function, not as a full data platform

# use debian image as base image 
FROM debian:bullseye

# define "/home" (in the image) as the PWD
WORKDIR /home

# install redis server
RUN apt-get update
RUN apt-get install -y redis-server procps
# [TEST] install ftp client to be able to test vsftpd container
RUN apt-get install -y ftp

# configure redis memory storage :
# limit RAM used and define that in case of lacking memory, oldest keys should be forgotten
RUN echo "maxmemory 256mb" >> /etc/redis/redis.conf
RUN echo "maxmemory-policy allkeys-lru" >> /etc/redis/redis.conf

# configure where redis listens : all interfaces instead of only localhost
# (another default parameter in configuration file defines port 6379, which can be kept)
RUN sed -i "s/bind 127.0.0.1/#bind 127.0.0.0/" /etc/redis/redis.conf

# avoid redis running as a daemon
RUN sed -i "s/daemonize yes/daemonize no/" /etc/redis/redis.conf

# define healthcheck = command run inside the container (at docker daemon initiative) to check responsiveness ;
# here command uses small redis client built in `redis-cli`,
# 	if redis runs correctly it should exit successfully with message "PONG"
HEALTHCHECK --interval=5s --timeout=3s --retries=5 \
  CMD redis-cli ping || exit 1

# launch redis without protected mode (so that it accepts connections outside localhost)
# (once inside container, redis cannot be stopped through `service`, but through `redis-cli shutdown`)
CMD ["redis-server", "/etc/redis/redis.conf", "--protected-mode", "no"]
