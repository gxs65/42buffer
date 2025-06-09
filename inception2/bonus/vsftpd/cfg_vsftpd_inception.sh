#!/bin/bash

# create user for FTP connections
# 	with same username and password as wordpress admin
export WP_USER_ADMIN_PASSWORD=$(cat /run/secrets/wp_user_admin_password)
adduser $WP_USER_ADMIN --disabled-password
cat <<EOF | passwd $WP_USER_ADMIN
$WP_USER_ADMIN_PASSWORD
$WP_USER_ADMIN_PASSWORD
EOF
# /!\ user is made member of the same group that owns the files in the wordpress volume,
# 	  so that it has write rights on the files files are in mode 775)
# 		(files must exist since in docker-compose container vsftpd
# 		 has been set as depending on container wordpress, with a healthcheck) 
export WWW_DATA_GID=$(stat -c '%g' /var/www/html/wordpress/index.php)
groupadd -g $WWW_DATA_GID www-data-wordpress
usermod -aG $WWW_DATA_GID $WP_USER_ADMIN

# change home of FTP user to the volume containing WordPress files
# (instead of the default directory created by `adduser` in '/home')
usermod -d /var/www/html $WP_USER_ADMIN

# configure vsftpd for our needs :
# 	- allow uploads with "write_enable"
#   - make uploaded files keep their original permissions with 'local_umask'
#       (default 'local_umask' is 700, which deletes all permissions to 'group' and 'others')
# 	- chroot local users (ie. only WP_USER_ADMIN) to their home directory
# 		<=> won't be able to see other parts of the filesystem
# 	- custom banner displayed after connection
sed -i "s/#write_enable=YES/write_enable=YES/1" /etc/vsftpd.conf
echo "local_umask=022" >> /etc/vsftpd.conf
sed -i "s/#chroot_local_user=YES/chroot_local_user=YES/1" /etc/vsftpd.conf
sed -i "s/#ftpd_banner=Welcome to blah FTP service/ftpd_banner=FTP access to WordPress files/1" /etc/vsftpd.conf

# make vsftpd work as a standalone server instead of a system service,
sed -i "s/listen_ipv6=YES/#listen_ipv6=YES/1" /etc/vsftpd.conf
sed -i "s/listen=NO/listen=YES/1" /etc/vsftpd.conf
echo "background=NO" >> /etc/vsftpd.conf

# make vsftpd use a specified range of ports for its passive mode
# (where it asks the client to connect on ports other than 20 and 21)
echo "pasv_enable=YES" >> /etc/vsftpd.conf
echo "pasv_min_port=10500" >> /etc/vsftpd.conf
echo "pasv_max_port=10502" >> /etc/vsftpd.conf

# create vsftpd empty directory
# (not used here because users are chrooted to their home, but without it vsftpd is confused)
mkdir -p /var/run/vsftpd/empty
chmod 755 /var/run/vsftpd/empty
chown root:root /var/run/vsftpd/empty

# run vsftpd as process with PID 1
exec vsftpd /etc/vsftpd.conf
