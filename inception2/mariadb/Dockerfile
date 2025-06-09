# use debian image as base image 
FROM debian:bullseye

# define "/home" (in the image) as the PWD
WORKDIR /home

# install mariadb
RUN apt-get update
RUN apt-get install -y mariadb-server procps

# copy bash configuration script inside image, and make it executable
COPY cfg_mariadb_inception.sh cfg_mariadb_inception.sh
RUN chmod 744 cfg_mariadb_inception.sh

# define healthcheck = command run inside the container (at docker daemon initiative) to check responsiveness
# 	used by "depends_on" instruction of docker-compose to know when to start WordPress container setup
# /!\ since command not run as OS root, cannot connect as MariaDB root user, therefore connects as dummy user
HEALTHCHECK --interval=5s --timeout=3s --retries=5 \
  CMD mysqladmin ping -u user_healthcheck --password=safepwd -h 127.0.0.1 || exit 1

# make containers instantiated from this image run the bash configuration script when they start
# (bash script will start mariadb server when configuration is complete) 
ENTRYPOINT ["./cfg_mariadb_inception.sh"]
