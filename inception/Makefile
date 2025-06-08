all: up

# "docker-compose up" will do all image build + volume/network creation + container start
# 	("-d" option to put containers in background instead of 'attaching' to them)
# "docker-compose down" will remove containers and networks, but not images and volumes
up:
#	mkdir -p mariadb/data
#	mkdir -p wordpress/data
	docker-compose -f ./docker-compose.yml up -d
down:
	docker-compose -f ./docker-compose.yml down
pause:
	docker-compose -f ./docker-compose.yml stop
play:
	docker-compose -f ./docker-compose.yml start

# To remove images and volumes
rmi:
	docker image rm mariadb:user || true
	docker image rm wordpress:user || true
	docker image rm nginx:user || true
	docker image rm redis:user || true
	docker image rm vsftpd:user || true
	docker image rm speedtest:user || true
	docker image rm adminer:user || true
rmv:
	docker volume rm mariadb || true
	docker volume rm wordpress || true
	docker volume rm speedtest || true
#	docker run --rm -it -v ./mariadb/data:/home -w /home debian:bullseye sh -c "rm -rf *"
#	docker run --rm -it -v ./wordpress/data:/home -w /home debian:bullseye sh -c "rm -rf *"
#	rm -rf mariadb/data/*
#	rm -rf wordpress/data/*
clean: down rmi rmv
re: clean up

check_existing:
	docker ps -a
	@echo ""
	docker image ls
	@echo ""
	docker network ls
	@echo ""
	docker volume ls

# rules to run NGINX container alone ;
# requires to modify NGINX conf file
# 	- comment out CGI part
# 	- set root path as '/home'
# then test in browser on 'https://localhost:8080/hello.html'
# ---
# sample_run_nginx:
# 	docker image build nginx/ -t nginx
# 	docker run --name cont_nginx -p 8080:443 nginx &
# sample_stop_nginx:
# 	docker stop cont_nginx
# 	docker rm cont_nginx
# 	docker image rm nginx

# rules to run WordPress container alone ;
# error messages will happen since WordPress can't be configured
# so it cannot be tested
# ---
# sample_run_wp:
# 	docker image build wordpress/ -t wordpress
# 	docker run --name cont_wordpress wordpress &
# sample_stop_wp:
# 	docker stop cont_wordpress
# 	docker rm cont_wordpress
# 	docker image rm wordpress

# rules to run MariaDB container alone ;
# testing requires to
# 		- create a network and make cont_mariadb member
# 		- run another container on same network, 'apt-get update' + install mariadb-client
# 		- connect to mariaDB server using : 
# 		  mariadb db_inception -u user_wp_inception --password=safepwd -h cont_mariadb -P 3306
# ---
# sample_run_mariadb:
# 	docker image build mariadb/ -t mariadb
# 	docker run --name cont_mariadb mariadb &
# sample_stop_mariadb:
# 	docker stop cont_mariadb
# 	docker rm cont_mariadb
# 	docker image rm mariadb