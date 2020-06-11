
# Prerequisites to using SCONE

Useful resource: <https://sconedocs.github.io>

## Setup Docker


```
sudo apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg-agent \
    software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io
sudo usermod -aG docker $USER
```

## Get Docker Scone repo access

Go to Scone's [website](https://scontain.com/) and request community/evaluation access for the private Docker repositories.
