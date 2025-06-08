# use debian image as base image 
FROM debian:bullseye

# define "/home" (in the image) as the PWD
WORKDIR /home

# install NGINX, and openssl for TLS protocol
RUN apt-get update
RUN apt-get install -y nginx openssl netcat procps

# generate the public-private key pair and self-signed certificate
# /!\ this is a quick solution for tests :
# 	- all containers instantiated from this image will have the same key pair
# 	- for real use, key pair and certificate would exist beforehand,
# 	  and be copied from outside the image
RUN mkdir -p /home/ssl
RUN	openssl req -x509 -newkey rsa:4096 \
	-keyout /home/ssl/privkey_inception.key \
	-out /home/ssl/certif_inception.crt \
	-sha256 -days 3650 -nodes \
	-subj "/C=FR/ST=PA/L=PA/O=FT/OU=Student/CN=abedin.42.fr"

# create the directory for WordPress files
# (WordPress files installed by WordPress container,
#  but accessible to this container thanks to shared volume 'wordpress', cf. docker-compose)
RUN mkdir -p /var/www/html/wordpress

# [TEST] create a sample HTML page for testing NGINX container alone (served without PHP)
RUN echo "<html>SAMPLE PAGE</html>" > /home/hello.html
# [BONUS] copy static website files into the image (served without PHP)
# (at image build, context of Dockerfile is sent to docker daemon for building,
#  so the daemon sees only the files from Dockerfile directory
# 	-> rrclone files had to be located in Dockerfile directory, instead of directory '../bonus/')
COPY bonus_rrclone /home/website_rrclone
# [BONUS] create makeshift Adminer directory and file so that NGINX is not confused by their absence
RUN mkdir -p /var/www/html/adminer
RUN touch /var/www/html/adminer/index.php

# copy nginx configuration file inside image
COPY cfg_nginx_inception.conf /etc/nginx/cfg_nginx_inception.conf

# make containers instantiated from this image run nginx in foreground when they start
ENTRYPOINT ["nginx", "-c", "/etc/nginx/cfg_nginx_inception.conf", "-g", "daemon off;"]
# ENTRYPOINT ["tail", "-f"]