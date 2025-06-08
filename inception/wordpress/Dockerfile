# use debian image as base image 
FROM debian:bullseye

# define "/home" (in the image) as the PWD
WORKDIR /home

# install PHP, PHP-FPM (CGI backend server), extension for PHP database connections, and MariaDB client
# /!\ WordPress files are not downloaded yet, it will be done by container using WordPress CLI utility
RUN apt-get update
RUN apt-get install -y php php7.4-fpm php-mysql mariadb-client curl wget netcat procps

# [BONUS] install Adminer by downloading its unique big PHP file
RUN wget https://www.adminer.org/latest.php

# copy bash configuration script inside image, and make it executable
COPY cfg_wordpress_inception.sh cfg_wordpress_inception.sh
RUN chmod 744 cfg_wordpress_inception.sh

# [BONUS] for vsftpd container reading files in wordpress volumes
# define healthcheck = command run inside the container (at docker daemon initiative) to check responsiveness
# 	used by "depends_on" instruction of docker-compose to know when to start vsftpd setup
HEALTHCHECK --interval=2s --timeout=3s --retries=5 \
  CMD netcat -z 127.0.0.1 9000

# make containers instantiated from this image run the bash configuration script when they start
# (bash script will start PHP-FPM backend server when configuration is complete)
ENTRYPOINT ["./cfg_wordpress_inception.sh"]