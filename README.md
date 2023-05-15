# 2023-JCO-Airtime
Un simple platformer 2D codé en C++ avec Qt.
Créé avec le GameFramework de JCO, lors de l'atelier de programmation de jeux vidéo

Le jeu consiste à sauter sur des plateformes tout en évitant des obstacles ainsi d'atteindre la fin d'un niveau et lancer le suivant.
Différents obstacles se trouvant dans le niveau réinitialise le niveau lorsque le joueur entre en contact avec.

Les niveaux sont complétments généré depuis des fichiers JSON.
Il est donc possible de modifier ou même d'ajouté de divers niveaux et ainsi personnalisé complétement son experience de jeu.
Un wiki décrivant ce processus peut être trouvé [ici](https://github.com/divtec-cejef/2023-JCO-Airtime/wiki).

## Contrôles
- Les touches *A* et *D* peuvent être utilisé pour se déplacer à gauche et à droite respéctivement.
- La touche *Espace* permet au joueur de sauter.
- Le joueur peut effectuer un dash avec la touche *Shift*
  - Le dash est efféctué dans la direction actuelle du mouvement du joueur. De plus les touches *W* et *S* peuvent être utilisé pour plus de directions.
