#!/bin/bash

# WORDPRESS SETUP

# install WordPress CLI utility `wp`
cd /home
curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar
chmod +x wp-cli.phar
mv wp-cli.phar /usr/local/bin/wp

# prepare directory for WordPress files
wp_dir="/var/www/html/wordpress"
mkdir -p $wp_dir
cd $wp_dir

# extract docker secrets
export MDB_USER_WP_PASSWORD=$(cat /run/secrets/mdb_user_wp_password)
export WP_USER_ADMIN_PASSWORD=$(cat /run/secrets/wp_user_admin_password)
export WP_USER_SUBSID_PASSWORD=$(cat /run/secrets/wp_user_subsid_password)

# download WordPress files, setup database connection (using CLI utility)
wp core download --allow-root
wp config create --dbname=$MDB_INCEPTION_DB \
				 --dbuser=$MDB_USER_WP \
				 --dbpass=$MDB_USER_WP_PASSWORD \
				 --dbhost=mariadb:3306 \
				 --allow-root

# setup website title, root user, additional user (using CLI utility)
wp core install  --url=$BASE_URL \
				 --title="SAMPLE SITE" \
				 --admin_user=$WP_USER_ADMIN \
				 --admin_password=$WP_USER_ADMIN_PASSWORD \
				 --admin_email=some@email.com \
				 --allow-root
wp config set WP_HOME "$BASE_URL" --allow-root
wp config set WP_SITEURL "$BASE_URL" --allow-root
wp user create $WP_USER_SUBSID other@email.com \
				 --user_pass=$WP_USER_SUBSID_PASSWORD \
				 --allow-root
# (option "--allow-root" needed to make `wp` accept the command from the root user despite safety issues ;
#  since the script installs WordPress inside a container it cannot switch to a non-root user)

# [BONUS] make wordpress use Redis for caching
# 	- know how to cache data thanks to WordPress' Redis-Cache plugin
# 	- access Redis server running on another container
sed -i "2idefine('WP_CACHE', true);" $wp_dir/wp-config.php
sed -i "2idefine('WP_REDIS_PORT', 6379 );" $wp_dir/wp-config.php
sed -i "2idefine('WP_REDIS_HOST', 'redis');" $wp_dir/wp-config.php
wp plugin install redis-cache --allow-root
wp plugin activate redis-cache --allow-root
wp redis enable --allow-root

# [BONUS] move Adminer PHP file with other WordPress PHP scripts
mv /home/latest.php /var/www/html/wordpress/adminer.php

# set correct ownership and permission on WordPress files
chown -R www-data:www-data /var/www
chmod -R 775 /var/www
# [BONUS] for files uploaded through vsftpd server, make them get the same group owner as the rest (www-data)
chmod -R g+s /var/www

# PHP-FPM SETUP

# make PHP-FPM listen on port 9000 (where NGINX will pass CGI requests) instead of UNIX socket
sed -i -e "s/listen = \/run\/php\/php7\.4-fpm\.sock/listen = 9000/g" /etc/php/7.4/fpm/pool.d/www.conf

# create directory for PHP in 'run' (where it will store PID, and would store UNIX socket if it used one)
mkdir -p /run/php

# start PHP-FPM in foreground with option -F
echo "WORDPRESS CONTAINER - end of bash config script, now running php-fpm"
exec /usr/sbin/php-fpm7.4 -F