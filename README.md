#Introduction

Pour ce projet, nous devons implémenter un serveur proxy cache en C supportant le protocole HTTP 1.1. Le client enverra une requête au serveur. Celui-ci vérifiera si la ressource demandée est disponible en cache et si elle n'a pas changé. Si ces deux conditions sont vérifiées, le serveur proxy renverra au client la ressource à partir du cache et non à partir du serveur distant. Ceci permet de réduire de manière considérable la bande passante descendante dans un réseau où plusieurs clients sont susceptibles de télécharger les mêmes ressources. Dans le cas où le serveur ne possède pas la ressource en cache ou si la version mise en cache est expirée, alors le serveur téléchargera la ressource depuis le serveur distant et la stockera dans son cache avant de la renvoyer au client.

#Fonctionnalités implémentées

###Cache
A chaque demande de ressource venant d'un client, le serveur vérifie s'il dispose de cette ressource en cache. Si c'est le cas, il ajoute à la requête du client l'entête `If-Modified-Since` comprenant la date à laquelle cette ressource a été enregistrée dans le cache. Le serveur proxy vérifie alors le code de réponse du serveur distant. Si le code est `304`, cela signifie que la ressource n'a pas été modifiée et qu'elle peut être lue depuis le cache. Si le code de retour est `200`, cela signifie que la ressource a été modifiée ; le serveur stocke alors la nouvelle ressource dans le cache avant de la renvoyer au client.

###Sauvegarde
Le cache est implémenté sous forme de fichiers stockés dans un sous-répertoire (paramétrable) du proxy. Ainsi, le serveur peut redémarrer autant de fois que souhaité, ses fichiers caches seront conservés.

###Pipelining
Le serveur lit les données au fur-et-à-mesure que le client lui envoie. Dès qu'une requête complète est trouvée, celle-ci est exécutée et les requêtes suivantes sont conservées dans un buffer. Cela permet notamment le pipelining : un client peut envoyer plusieurs requêtes sans attendre la réponse de la requête précédente.

###Persistance
Tant que le client n'a pas explicitement demandé au serveur proxy de fermer la connexion (via l'entête `Connection` comme spécifié par HTTP/1.1), celle-ci reste active. Ainsi, si le client souhaite effectuer d'avantages de requêtes ultérieurement, il pourra réutiliser la connexion déjà établie.

###Connexions concurrentes
A chaque nouvelle connexion d'un client, le serveur crée un nouveau thread. Ce dernier gérera seul la connexion du client. Cela permet de gérer plusieurs clients en même temps sans qu'un client ne puisse en bloquer un autre.

#Compilation

Un makefile a été crée pour compiler le serveur proxy et le client http avec la plus grande simplicité. La commande suivante compile le serveur et assure que le dossier `cache_files` où seront stockés les fichiers caches existe :
```bash
make server
```
La commande suivante compile le client HTTP :
```bash
make client
```

#Utilisation

La commande suivante lance le serveur proxy :
```bash
./server port
```
- *port* : port réseau compris entre 0 et 65535 sur lequel le serveur proxy écoutera ; notez que les ports inférieurs à 1024 nécessitent les droits root sur Linux

**Exemple** : pour que le serveur proxy écoute sur le port `8080`,  voici la commande :
```bash
./server 8080
```

La commande suivante lance le client HTTP :
```bash
./client server port persistance pipelining uri1 [uri2 [uriN]]
```
- *server* : le serveur http à partir duquel les ressources devront être récupérées
- *port* : le port d'écoute du serveur proxy local auquel le client enverra ses requêtes
- *persistance* : si 1 le client tentera d'utiliser la même connexion TCP pour télécharger toutes les ressources sinon, le client utilisera une connexion TCP par ressources
- *pipelining* : si 1 le client enverra toutes les requêtes à l'ouverture de la connexion TCP sans attendre la réponse du serveur sinon le client attendra la fin de la réponse du serveur pour envoyer la requête suivante ; nécessite une connexion persistante
- *uri1 [uri2 [uriN]]* : les ressources à récupérer sur le serveur distant (au moins une)

**Exemple** : pour se connecter au serveur proxy local écoutant sur le port `8080` et récupérer les ressources `/` et `/404` à partir du serveur `www.perdu.com`, en utilisant le `pipelining` sur une `connexion persistante`, voici la commande :
```bash
./client www.perdu.com 8080 1 1 / /404
```

#Conclusion

Il est clair que le projet fût plus long et fastidieux qu'initialement pensé. Nous tenons d'ailleurs à remercier M. Michel d'abord pour sa compréhension vis-à-vis du délai de remise mais également pour le dévouement et le soutien qu'il nous a fourni tout au long de ce projet.  Nous avons pu ainsi découvrir ce magnifique langage qu'est le C. Ce n'est certes pas le plus facile des langages mais il demande une rigourosité défiante qui nous permet de bien comprendre la façon dont fonctionne notre programme. Grâce à ce projet, nous avons pu mettre en pratique la théorie vue en cours avec M. Quoitin et notre connaissance de cette merveilleuse branche de l'informatique que sont les réseaux ne s'en trouve que grandie.

**PS** : Une des raisons principales pour lesquelles notre programme initial ne fonctionnait pas est l'utilisation de la bibliothèque `<regex.h>` qui fournit un résultat aléatoire lorsque compilée avec GCC 4.8. Le rapport de bug est accessible ici  https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53631
