# P2P Blochen Network System
Hello, this system provides a more secure communication by encrypting data over the p2p system completely network systems sha256 and tcp protocol transfers the txt file securely

update & upgrade

    sudo apt-get update -y & sudo apt-get full-upgrade -y
eroor code (Command 'sudo' not found, but can be installed with)

    login to your root account
    apt-get install sudo

linuxOS open port

    netstat -na | grep :8080
or

    ss -na | grep :8080
CentOS 

    firewall-cmd --add-port=8080/tcp
For Other Linux Distributions

    iptables -A INPUT -p tcp --dport 8080 -j ACCEPT
ubuntu , debian and kali linux allow port

    sudo ufw allow 8080
run code client

    gcc client.c
run code server

    gcc server.c

Coder By Horeal(Gökdeniz G.)
