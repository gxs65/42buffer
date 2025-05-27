#!/bin/bash

# start mariadb server
# (need sleep line, otherwise next lines with mariadb client start to execute while mariadb server is not up)
service mariadb start
sleep 5

# use mariadb client for configuration (all SQL instructions run as mariadb root user)
# 	- create database
# 	- create user for WordPress
# 	- give privileges on database to new user
mariadb <<EOF
CREATE DATABASE IF NOT EXISTS db_inception;
CREATE USER IF NOT EXISTS user_wp_inception@'%' IDENTIFIED BY 'safepwd';
GRANT ALL PRIVILEGES ON db_inception.* TO user_wp_inception@'%';
FLUSH PRIVILEGES;
EOF

# restart mariadb to apply changes
# 	- in the foreground (so that container does not stop)
# 	- with "port" and "bind-address" options to make certain it listens on port 3306
# 	- with "datadir" option to make certain it stores its data in the volume
mysqladmin -u root shutdown
mysqld_safe --port=3306 --bind-address=0.0.0.0 --datadir='/var/lib/mysql'