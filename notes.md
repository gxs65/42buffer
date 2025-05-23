########## Inception - Documentation notes

##### General descriprion of Docker

- Docker is :
	\ [concretely, for Inception] a program that creates containers where applications can run in a custom environment
		-> uses "containerization" (a partial virtualization) without the overhead of a whole virtual machine,
.			because it lets the containers access the host OS kernel instead of simulating a virtual one
	\ [more generally] a tool that creates/packages/ships applications in the form "images",
		from which the containers, standardized units able to work on any system, can be run
			-> many applications are available in Docker image form on public repositories like DockerHub
- usage of Docker on a host computer :
	\ once installed, it runs as a daemon just like "ssh" or "cron" (= 'server' side)
		-> can be seen with "service --status-all", can be stopped/started with "systemctl"
	\ instructions are given by the user to the daemon through command line with commands "docker ..." (= 'client' side)
		which affect only docker objects (images, containers...) owned by this user
- to achieve "isolation" of running containers without a full virtual machine,
  Docker uses low-level Linux kernel features developed over a long period of time to meet demands for isolated environments :
	\ for filesystem isolation <=> a container cannot access the host's filesystem (directly) :
		"chroot" feature redefining what is the root directory for a process
	\ for process isolation <=> a container cannot see other processes running on host OS :
		"namespaces" feature redefining the process tree for a specific process
			(+ also allows to isolate various less substantive elements like hostname, mount points, network routing)
	\ for resource isolation <=> a container can consume only a limited part of host's resources :
		"control groups (cgroups)" feature to assemble processes into groups and limiting group resources access
  -> creating a container requires using all these tools, which can be complex and require a lot of precautions,
	so docker automates it into a single containerization interface

- terminology for describing what is done with Docker :
	\ "images" = base state for containers ; "container" = an active instance of an image, running some application
	\ command "docker build ..." creates an image following directives from a dockerfile
	  command "docker run IMAGE ..." instantiates and executes a new container from the given docker image
		("run" has options to override any directive from the docker image, if the user wants to)
	\ "physical X" refers to the X of the host computer (on which Docker runs)
	  "virtual X" refers to the X inside a container (ie. inside the custom environment created by Docker)
	\ component [defined specifically for Inception] = one part of multi-container application : NGINX, MariaDB, WordPress
- useful general docker commands (see list with "docker help") :
	\ "docker ps -a" : see all existing containers (owned by user), including stopped containers
	\ "docker logs" : see docker logs for debug
	\ "docker container run/stop/start/rm" : manage a container
	\ "docker image pull IMAGE" : pull the given image from a public Docker respository (not allowed for Inception)
	\ "docker image ls/build/inspect/rm" : see all existing docker images, manage a docker image
	\ "docker volume ls/create/inspect/rm" : see all existing docker volumes, manage a docker volume
- each time "docker run" instantiates a container, it is a new container with a new empty virtual filesystem, so
	(1) data stored in the virtual filesystem of a container will disappear once the container is removed
	(2) data stored and managed by a container is unavailable to other containers (and to the host OS)
  -> for persistent and shared data, instead make containers store data on the (permanent) physical filesystem, with mounting :
	a SOURCE directory of physical (<=> host's) filesystem is mounted on a DEST directory of virtual filesystem
		<=> all data written in DEST by container application is in fact written in SOURCE on physical filesystem, so
	(1) the next time a container runs the application with the same mounting, it will access data from previous runs
	(2) all containers with a part of their filesystem mounted the same way can share data
  (to avoid the user having to manage and remember what is physical SOURCE directory, Docker can take care of it
   and lets user simply define "volumes" of data, and the place DEST where a volume is mounted on container's virtual filesystem)
- so the Inception application would typically comprise :
	\ for each component (NGINX, WordPress, MariaDB) :
		~ a dockerfile defining its docker image, (potentially) mentioning a volume for persistent storage
		~ (potentially) a configuration bash script to be executed inside the container when it starts
	\ one docker compose file defining and running the multi-container application, using docker networking and volumes
	\ one Makefile to create directories and launch "docker-compose" commands

##### Specific details of the components

### NGINX : HTTPS and TLS Certificates

- to run inside container, NGINX will need a config file defining the features of the server,
	the directives of this file are very similar to the directives handled in Webserv,
		except that NGINX will use HTTPS protocol instead of HTTP (thus listen on port 443 instead of 80)
- HTTPS = extension of HTTP to create between server and client "a secure connection over an insecure network"
	<=> even when adversaries can intercept packets and read/modify them on the network, protocol ensures that
		(a) adversaries won't be able to understand the data transported by the packets
		(b) adversaries can't 'impersonate' the server, ie. they can't interact with the client
		    while making the client believe he is interacting with the server
	+ if possible, (c) the process achieving should be fast, because it will be repeated for each new TCP connection
- protocol that respects (a)+(b)+(c) is TLS = Transport Layer Security
	\ (name is misleading :) located at layer 6 [presentation] of OSI model,
		between layers 5 TCP [session] and 7 HTTP [application]
			<=> layers TCP and below (IP frame, ethernet framed) are not affected by encryption
	\ sufficiently fast because it adds only 4 messages at the beginning of each TCP connection
		during the "TLS handshake" happening just after the TCP connection
  /!\ HTTPS is main user of TLS, but TLS can be used for other application protocols (like FTP, which becomes SFTP with TLS)

# Constraint (a) : encryption of HTTP data

- to respect (a), HTTP data sent in packets over the TLS-secured connection will be "encrypted" according to a session encryption
	-> only the client and server know the session encryption mechanism and can use it to encrypt/decrypt
- the specific encryption algorithm used for session encryption is not relevant,
  it will be chosen by server during TLS handshake among options offered by client,
	the only constraint is : it must be derived entirely from a single value called "premaster key"
		(eg. Vigenere, if it were less easy to break, could serve as session encryption,
		 since it only requires a key text for encryption/decryption -> the premaster key can be this key text)
- thus, to achieve secure connection with the server, client must simply send to the server a premaster key
	using a method where nobody can intercept this premaster key (and derive the session encryption from it)
		<=> the premaster key is a "shared secret" between client and server
- TLS establishes the premaster key during TLS handshake using the obvious way to generate a shared secret : RSA public key

# Constraint (b)

# Full TLS handshake










 


- remember at all point : dockerfile --(build)--> docker image --(run)--> docker container
- dockerfile = plan to create a docker image, with directives for :
	\ defining a base image from which this image will inherit
		~ usually base image is not present already on host but pulled (=downloaded) from a docker repository
		~ in Inception, use of any predefined application image is forbidden,
			so the pulled image is the most basic image possible : the OS (here, Debian) running inside our container
	\ defining environment variables that will exist inside the container
	\ installing application that will run in the container, and all its dependencies
		usually simply using "apt-get install ..."
	\ copying files from the host filesystem to the container's filesystem
	\ mounting some parts of the host filesystem to the container's filesystem
	\ defining commands that the container will run once it starts



##### Plan

- more documentation on specific aspects :
	\ basic knowledge of WordPress
	\ SQL language and basic commands to manipulate tables in MariaDB
	\ RSA key exchange, unrolling of TLS securisation, role of certificate
	\ docker networks, how the hostname inside a container is associated to another container in same network
	\ what happens when a process stops inside a container + can there be multiple processes running in foreground ?
- small example docker with only MariaDB to experience running docker commands
- setup the 3 services like in the subject, but not in a virtual machine
	\ experiment docker-compose file and commands, create the network
	\ experiment with .env file
	\ why does PHP need to have a wordpress directory ?
	\ create Makefile with docker-compose commands
- work on bonuses
- transfer to the virtual machine


##########
