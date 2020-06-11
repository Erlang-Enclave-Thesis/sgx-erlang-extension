
# Dockered PCCS (Provisioning Certificate Caching Service)

## Description
In order to support DCAP attestation, a PCCS has to be set up. We created a
Dockefile to simplify the deployment of the Intel provided PCCS.

It is not made for a production environment, just for local testing.

## Build
Configure the `config.json` file to suit your needs (look at `config.json.sample`).

```
docker build --pull -t pccs .
```

## Run
```
./run.sh
```


