## Ping une adresse IP valide :

```
sudo ./ft_ping 127.0.0.1 
sudo ./ft_ping 8.8.8.8
```

## Ping un nom de domaine valide :

```
sudo ./ft_ping google.com
sudo ./ft_ping example.com
```

## Ping avec une adresse IP invalide :

```
sudo ./ft_ping 999.999.999.999
```

## Ping avec un nom de domaine invalide :

```
sudo ./ft_ping invalid-domain.com
```

## Ping avec une adresse IP non joignable :

```
sudo ./ft_ping 10.255.255.1
```

## Ping avec l'option -v (verbose mode) :

```
sudo ./ft_ping 127.0.0.1 -v
sudo ./ft_ping google.com -v
sudo ./ft_ping 8.8.8.8 -v
```

## Ping avec l'option -h (help) :

```
sudo ./ft_ping -h
sudo ./ft_ping -?
```

## Ping sans spécifier de destination :

```
sudo ./ft_ping
```

## Ping avec une option invalide :

```
sudo ./ft_ping -z google.com
sudo ./ft_ping -9 127.0.0.1
```

## Ping avec plusieurs options valides :

```
sudo ./ft_ping 8.8.8.8 -v -h
sudo ./ft_ping google.com -v
```

## Ping avec une mauvaise syntaxe (mauvais format d'adresse) :

sudo ./ft_ping 1234:5678:9abc:def:ghij::1

## Ping en essayant de résoudre une adresse FQDN non valide :

sudo ./ft_ping nonexistent.domain

## Ping en mode verbose avec une adresse non joignable :

sudo ./ft_ping 10.255.255.1 -v

## Ping une adresse sans privilèges root :

./ft_ping 127.0.0.1

## Ping avec une tentative de se connecter à une adresse multicast :

sudo ./ft_ping 224.0.0.1
