run_nginx:
	docker image build . -t nginx
	docker run --name cont_nginx -p 8080:443 nginx &

stop_nginx:
	docker stop cont_nginx
	docker rm cont_nginx
	docker image rm nginx

check_existing:
	docker ps -a
	docker image ls
	docker network ls