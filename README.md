# 🐚 Minishell

## Présentation
**Minishell** est un projet de l’école 42 qui consiste à recréer un **mini interpréteur de commandes Unix**.  
L’objectif est de comprendre en profondeur le fonctionnement d’un shell en implémentant les principales fonctionnalités nécessaires pour exécuter des commandes et gérer les processus.  

Ce projet a été réalisé en **binôme avec [Antoine Jourdan Astruc ](https://github.com/Ajap75)**.


Ce projet permet de se familiariser avec :  
- La gestion des processus (`fork`, `execve`, `waitpid`).  
- La gestion des signaux (`ctrl-C`, `ctrl-D`, `ctrl-\`).  
- La manipulation des descripteurs de fichiers (redirections, pipes).  
- Le parsing et l’interprétation des commandes.  


## Objectifs et Contraintes
- Implémenter une boucle de lecture de commandes (prompt).  
- Exécuter les commandes saisies par l’utilisateur avec leurs arguments.  
- Gérer les **redirections** (`>`, `>>`, `<`, `<<`) et les **pipes** (`|`).  
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

### Gestion des signaux
- `ctrl-C` : affiche un retour à la ligne et réaffiche le prompt sans quitter le shell.  
- `ctrl-D` : ferme le shell proprement (équivalent de la commande `exit`).  
- `ctrl-\` : ne fait rien (n’interrompt pas le shell). 
  

## Utilisation

### Compilation
```bash
make  
```

### Lancement du shell
```bash
./minishell  
```

Vous arrivez sur un prompt interactif :
```bash
Fanny & Antoine SHELL :$  
```



