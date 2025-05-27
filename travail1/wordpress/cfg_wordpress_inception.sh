#!/bin/bash

cd /home

# WORDPRESS SETUP

# install WordPress CLI utility `wp`
curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar
chmod +x wp-cli.phar
mv wp-cli.phar /usr/local/bin/wp

# prepare directory for WordPress files in '/tmp/www'
wp_dir="/var/www/html/wordpress"
mkdir -p $wp_dir
chown -R www-data:www-data $wp_dir
chmod -R 755 /var/www
cd $wp_dir

# download WordPress files, setup database connection (using CLI utility)
wp core download --allow-root
wp config create --dbname=db_inception \
				 --dbuser=user_wp_inception \
				 --prompt=safepwd \
				 --dbhost=mariadb:3306 \
				 --allow-root

# setup website title, root user, additional user (using CLI utility)
wp core install  --url=custom.com \
				 --title="CUSTOM" \
				 --admin_user=wp_root_inception \
				 --admin_password=safepwd \
				 --admin_email=a@b.com \
				 --allow-root
wp user create wp_user1_inception a2@b.com \
				 --user_pass=safepwd \
				 --allow-root
# (option "--allow-root" needed to make `wp` accept the command from the root user despite safety issues ;
#  since the script installs WordPress inside a container it cannot switch to a non-root user)

# PHP-FPM SETUP

# make PHP-FPM listen on port 9000 (where NGINX will pass CGI requests) instead of UNIX socket
sed -i -e "s/listen = \/run\/php\/php7\.0-fpm\.sock/listen = 9000/g" /etc/php/8.3/fpm/pool.d/www.conf

# start PHP-FPM in foreground
/usr/sbin/php-fpm8.3 -F