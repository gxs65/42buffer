#!/bin/bash

# /!\ reminder : this bash script is run as root, inside the container,
#				 so <mariadb> and <mysqladmin> commands can access the MariaDB root user
#				 without authenticating with a password
# 					(other non-root process, eg. the healthcheck process, could not do this)

# start mariadb server to be able to setup the databases through the client
#   - need sleep line, otherwise next lines with mariadb client start to execute while mariadb server is not up)
#   - not listening on ports (only on UNIX socket) so that healthcheck fails while configuring
echo "[mysqld]" >> /etc/mysql/my.cnf
echo "skip-networking = 1" >> /etc/mysql/my.cnf
service mariadb start
sleep 5

# use mariadb client for configuration (all SQL instructions run as mariadb root user)
# 	- create database
# 	- create user for WordPress
# 	- give privileges on database to WordPress user
# 	- create dummy user with 0 privileges for healthcheck tests
# /!\ Values for usernames and passwords :
# 	- username of user for WordPress is retrieved from env
# 	- password of user for WordPress is retrieved from docker secrets
# 	- password and username of dummy user are hardcoded
export MDB_USER_WP_PASSWORD=$(cat /run/secrets/mdb_user_wp_password)
mariadb -u root <<EOF
CREATE DATABASE IF NOT EXISTS \`$MDB_INCEPTION_DB\`;
CREATE USER IF NOT EXISTS '$MDB_USER_WP'@'%' IDENTIFIED BY '$MDB_USER_WP_PASSWORD';
GRANT ALL PRIVILEGES ON \`$MDB_INCEPTION_DB\`.* TO '$MDB_USER_WP'@'%';
CREATE USER IF NOT EXISTS user_healthcheck@'%' IDENTIFIED BY 'safepwd';
FLUSH PRIVILEGES;
EOF

# restart mariadb to apply changes
#   - restoring config file to listen on ports
# 	- in the foreground (so that container does not stop)
# 	- with "port" and "bind-address" options to make certain it listens on port 3306
# 	- with "datadir" option to make certain it stores its data in the volume
mysqladmin -u root shutdown
sed -i "s/skip-networking = 1/#skip-networking = 1/1" /etc/mysql/my.cnf
echo "MARIADB CONTAINER - end of bash config script, now running mysqld"
exec mysqld --port=3306 --bind-address=0.0.0.0 --datadir='/var/lib/mysql'