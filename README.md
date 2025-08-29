# 🐚 Minishell

## 📖 Présentation
**Minishell** est un projet de l’école 42 qui consiste à recréer un **mini interpréteur de commandes Unix**.  
L’objectif est de comprendre en profondeur le fonctionnement d’un shell en implémentant les principales fonctionnalités nécessaires pour exécuter des commandes et gérer les processus.  

Ce projet permet de se familiariser avec :  
- La gestion des processus (`fork`, `execve`, `waitpid`).  
- La gestion des signaux (`ctrl-C`, `ctrl-D`, `ctrl-\`).  
- La manipulation des descripteurs de fichiers (redirections, pipes).  
- Le parsing et l’interprétation des commandes.  

---

## 🎯 Objectifs et Contraintes
- Implémenter une boucle de lecture de commandes (prompt).  
- Exécuter les commandes saisies par l’utilisateur avec leurs arguments.  
- Gérer les **redirections** (`>`, `>>`, `<`) et les **pipes** (`|`).  
- Implémenter les variables d’environnement (`$VAR`).  
- Gérer les guillemets simples et doubles.  
- Implémenter les **builtins** suivants :  
  - `echo` (avec option `-n`)  
  - `cd`  
  - `pwd`  
  - `export`  
  - `unset`  
  - `env`  
  - `exit`  

- Le shell doit :  
  - Afficher un prompt interactif.  
  - Ne pas crasher en cas d’erreur.  
  - Gérer les erreurs comme un vrai shell (codes de retour).  
  - Respecter les normes de l’école 42.  

### ⚡ Gestion des signaux
- `ctrl-C` : affiche un retour à la ligne et réaffiche le prompt sans quitter le shell.  
- `ctrl-D` : ferme le shell proprement (équivalent de la commande `exit`).  
- `ctrl-\` : ne fait rien (n’interrompt pas le shell).  

---

## 🚀 Utilisation

### 1️⃣ Compilation
Dans le répertoire du projet, lancer :  
make  

Cela génère l’exécutable :  
minishell  

### 2️⃣ Lancement du shell
./minishell  

Vous arrivez sur un prompt interactif :  
minishell$  
### 3️⃣ Exemple d’utilisation

#### Commandes simples
minishell$ echo Hello World  
Hello World  

minishell$ pwd  
/home/user/minishell  

#### Variables d’environnement
minishell$ export NAME=42  
minishell$ echo $NAME  
42  

minishell$ echo "Hello $NAME Students"  
Hello 42 Students  

#### Redirections
minishell$ echo "Ceci est un test" > file.txt  
minishell$ cat < file.txt  
Ceci est un test  

minishell$ echo "ajout" >> file.txt  
minishell$ cat file.txt  
Ceci est un test  
ajout  

#### Pipes
minishell$ ls -l | grep minishell | wc -l  
1  

minishell$ cat /etc/passwd | grep root | cut -d: -f1  
root  

#### Combinaisons redirections + pipes
minishell$ cat /etc/passwd | grep "bin" | cut -d: -f1 > users.txt  
minishell$ cat users.txt  
daemon  
bin  
sys  

#### Gestion des erreurs
minishell$ cd dossier_inexistant  
minishell: cd: dossier_inexistant: No such file or directory  

minishell$ ls | grep "fichier" > /root/test.txt  
minishell: permission denied: /root/test.txt  

#### Signaux
minishell$ sleep 100  
^C   (ctrl-C) → interrompt correctement la commande et retourne au prompt.  

minishell$ cat  
^D   (ctrl-D) → quitte proprement le shell.  

