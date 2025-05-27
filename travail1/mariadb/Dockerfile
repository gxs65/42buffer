# use debian image as base image 
FROM debian:bullseye

# define "/home" (in the image) as the PWD
WORKDIR /home

# install mariadb
RUN apt-get update
RUN apt-get install -y mariadb-server

# copy bash configuration script inside image, and make it executable
COPY cfg_mariadb_inception.sh cfg_mariadb_inception.sh
RUN chmod 744 cfg_mariadb_inception.sh

# make containers instantiated from this image run the bash configuration script when they start
# (bash script will start mariadb server when configuration is complete) 
ENTRYPOINT ["./cfg_mariadb_inception.sh"]