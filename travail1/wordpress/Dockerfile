# use debian image as base image 
FROM debian:bullseye

# define "/home" (in the image) as the PWD
WORKDIR /home

# install PHP, PHP-FPM (CGI backend server), extension for PHP database connections, and MAriaDB client
RUN apt-get update
RUN apt-get install -y php php8.3-fpm php-mysql mariadb-client

# copy bash configuration script inside image, and make it executable
COPY cfg_wordpress_inception.sh cfg_wordpress_inception.sh
RUN chmod 744 cfg_wordpress_inception.sh

# make containers instantiated from this image run the bash configuration script when they start
# (bash script will start PHP-FPM backend server when configuration is complete)
ENTRYPOINT ["./cfg_wordpress_inception.sh"]