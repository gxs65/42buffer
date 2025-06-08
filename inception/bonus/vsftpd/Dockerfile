# use debian image as base image 
FROM debian:bullseye

# define "/home" (in the image) as the PWD
WORKDIR /home

# install vsftpd server (and `tini`, to solve the issue of vsftpd not handling SIGTERM)
RUN apt-get update
RUN apt-get install -y vsftpd ftp tini procps

# copy bash configuration script inside image, and make it executable
COPY cfg_vsftpd_inception.sh cfg_vsftpd_inception.sh
RUN chmod 744 cfg_vsftpd_inception.sh

# make containers instantiated from this image run the bash configuration script when they start
# (bash script will transform into vsftpd server with `exec` when configuration is complete)
# /!\ Special case for vsftpd : it does not exit gracefully when receiving SIGTERM,
# so when it runs as PID 1 the container cannot stop properly (it must wait 10s for SIGKILL)
# 		-> solution : make `tini` run as PID 1 instead, a minimal init process
# 		   that starts vsftpd as a child and will make it terminate when receiving SIGTERM
ENTRYPOINT ["/usr/bin/tini", "--"]
CMD ["/home/cfg_vsftpd_inception.sh"]
# ENTRYPOINT ["tail", "-f"]