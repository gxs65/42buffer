# use debian image as base image 
FROM debian:bullseye

# define "/home" (in the image) as the PWD
WORKDIR /home

# install PHP, PHP-FPM (CGI backend server), extension for PHP database connections, and MariaDB client
RUN apt-get update
RUN apt-get install -y php php7.4-fpm php-mysql mariadb-client curl wget netcat procps

# ADMINER SETUP

# install Adminer, which a unique big PHP file
RUN wget https://www.adminer.org/latest.php

# prepare directory for Adminer file
RUN mkdir -p /var/www/html/adminer
RUN mv /home/latest.php /var/www/html/adminer/index.php

# set correct ownership and permission on Adminer file
RUN chown -R www-data:www-data /var/www
RUN chmod -R 775 /var/www

# PHP-FPM SETUP

# make PHP-FPM listen on port 9000 (where NGINX will pass CGI requests) instead of UNIX socket
RUN sed -i -e "s/listen = \/run\/php\/php7\.4-fpm\.sock/listen = 9000/g" /etc/php/7.4/fpm/pool.d/www.conf

# create directory for PHP in 'run' (where it will store PID, and would store UNIX socket if it used one)
RUN mkdir -p /run/php

# make containers instantiated from this image run PHP-FPM in foreground when they start
ENTRYPOINT ["/usr/sbin/php-fpm7.4", "-F"]
