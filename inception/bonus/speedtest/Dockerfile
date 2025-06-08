# use debian image as base image 
FROM debian:bullseye

# define "/home" (in the image) as the PWD
WORKDIR /home

# install PHP, PHP-FPM (CGI backend server)
RUN apt-get update
RUN apt-get install -y php php7.4-fpm netcat procps

# SPEEDTEST SETUP

# copy Speedtest files (not downloaded in image, because it would require to install Git)
# RUN mkdir -p /var/www/html/speedtest/backend
COPY speedtest_files/ /var/www/html/speedtest/
# COPY speedtest_files/favicon.ico /var/www/html/speedtest/
# COPY speedtest_files/index.html /var/www/html/speedtest/
# COPY speedtest_files/speedtest_worker.js /var/www/html/speedtest/
# COPY speedtest_files/speedtest.js /var/www/html/speedtest/
# COPY speedtest_files/backend /var/www/html/speedtest/

# set correct ownership and permission on Speedtest file
RUN chown -R www-data:www-data /var/www
RUN chmod -R 775 /var/www

# PHP-FPM SETUP

# make PHP-FPM listen on port 9000 (where NGINX will pass CGI requests) instead of UNIX socket
RUN sed -i -e "s/listen = \/run\/php\/php7\.4-fpm\.sock/listen = 9000/g" /etc/php/7.4/fpm/pool.d/www.conf

# create directory for PHP in 'run' (where it will store PID, and would store UNIX socket if it used one)
RUN mkdir -p /run/php

# make containers instantiated from this image run PHP-FPM in foreground when they start
ENTRYPOINT ["/usr/sbin/php-fpm7.4", "-F"]
