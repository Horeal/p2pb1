# P2P Blochen Network System
Hello, this system provides a more secure communication by encrypting data over the p2p system completely network systems sha256 and tcp protocol transfers the txt file securely

update & upgrade

    sudo apt-get update -y & sudo apt-get full-upgrade -y

build

    sudo apt-get install build-essential

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
run code client:

linux

    gcc client.c -o client -lnsl -lm -lcrypto
windows

    gcc client.c -o client -lws2_32 -lm -lcrypto
run code server:

linux

    gcc server.c -o server -lnsl -lm -lcrypto
windows

    gcc server.c -o server -lws2_32 -lm -lcrypto

Coder By Horeal(Gökdeniz G.)
