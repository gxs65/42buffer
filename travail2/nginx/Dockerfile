# use debian image as base image 
FROM debian:bullseye

# define "/home" (in the image) as the PWD
WORKDIR /home

# install NGINX, and openssl for TLS protocol
RUN apt-get update
RUN apt-get install -y nginx openssl

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

# create a sample HTML page for testing NGINX container alone (no WordPress)
RUN echo "<html>SAMPLE PAGE</html>" > /home/hello.html

# copy nginx configuration file inside image
COPY cfg_nginx_inception.conf cfg_nginx_inception.conf

# make containers instantiated from this image run nginx in foreground when they start
ENTRYPOINT ["nginx", "-c", "/home/cfg_nginx_inception.conf", "-g", "daemon off;"]
