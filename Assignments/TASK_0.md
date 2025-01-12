# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
Sur quelle touche faut-il appuyer pour ajouter un avion ?

- c

Comment faire pour quitter le programme ?

- q ou x

A quoi sert la touche 'F' ?

- passer en fullscreen

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?

- il fait des tours, il attéri, se garre puis repart.

Quelles informations s'affichent dans la console ?

```bash
KL6197 is now landing...
now servicing KL6197...
done servicing KL6197
KL6197 lift off
```

Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ?

- les 3 premiers avions font ce que j'ai décrit avant. Le 4ème lui fait des tours jusqu'à ce que le premier avion reparte. Le dernier avion peut donc attérrir et faire comme les autres. Ce paterne se relance en boucle.

## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.

```c
struct AircraftType -> structure qui génère les types d'avions
class Aircraft -> classe qui génère les avions
class AirportType -> classe qui génère les types d'aéroports
class Airport -> classe qui génère les aéroports
struct Point2D -> structure qui génère un point en 2D
struct Point3D -> structure qui génère un point en 3D
struct Runway -> structure qui génère une piste d'attérisage
class Terminal -> classe qui génère un terminal d'aéroport
class Tower -> classe qui génère une tour de contrôle
enum WaypointType -> enum qui génère les types de destinations
class Waypoint -> classe qui génère une destination
```

Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.

```c
class Tower
    get_instructions ->
    arrived_at_terminal ->

class Aircraft
    get_flight_num -> renvoie le numéro de vol "flight_numer"
    distance_to -> renvoie la distance entre le point "p" et le point "pos"
    display -> affiche le dessin de l'avion dans la fenêtre
    move -> déplace le point pos de l'avion

class Airport
    get_tower -> renvoi un l'objet "tower" dont Airport est owner
    display -> affiche le dessin de l'aéroport dans la fenêtre
    move -> applique la méthode move sur chaque terminal dans "terminals" dont Airport est owner

class Terminal
    in_use -> renvoie un booleen qui vérifie que le "current_aircraft" s'il n'es pas nul.
    is_servicing -> renvoie true tant que "service_progress" est inférieur à "SERVICE_CYCLES"
    assign_craft -> assigne une référence sur "current_aircraft".
    start_service -> affiche un message pour savoir quel avion est en train d'être servit.
    finish_service -> affiche un message pour dire que le "current_aircraft" a fini de faire ses trucs. On change sa valeur par nullptr.
    move -> incrémente le compteur "service_progress".
```

Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.

Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
Expliquez les intérêts de ce choix.

## C- Bidouillons !

1. Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.

- aircraft_types.hpp, dans init_aircraft_types.

2. Identifiez quelle variable contrôle le framerate de la simulation.\
   Le framerate correspond au temps de rafraichissement du programme, c'est-à-dire le nombre de fois où les éléments du programme seront mis à jour (ajout de nouvel avion à la simulation, déplacement, etc) en une seconde.\
   Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
   Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\
   Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.

- ticks_per_sec

Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.

Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\

- Le jeu crash.

  Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.

3. Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.

- SERVICING_CYCLES

4. Lorsqu'un avion a décollé, il réattérit peu de temps après.
   Assurez-vous qu'à la place, il soit supprimé de la `move_queue`.\
   Pour tester, il suffit de dézoomer et de vérifier que les avions suffisament éloignés ne bougent plus.
   Indices :\
   A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?\
   Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ?
   A quel endroit de la callstack pourriez-vous le faire à la place ?\
   Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?

- Lorsqu'il n'y a plus de waypoint.

  Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ?

- Car il est référencé ailleur.

  A quel endroit de la callstack pourriez-vous le faire à la place ?\

- opengl interface.

  Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?

- move.

5. Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
   Il faut également penser à le supprimer de cette liste avant de le détruire.
   Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.\
   Essayez maintenant de supprimer complètement l'avion du programme lorsque vous le retirez de la `move_queue`.\
   En dézoomant, vous devriez maintenant constater que les avions disparaissent maintenant de l'écran.

- Comme la supression est effectuer dans timer(), qui itère sur la move_queue. Si l'on supprime l'item de cette move_queue dans le destructeur des `dynamic_object`, alors il y aura une invalidation de l'iterator utilisé par timer(). On a donc une segfault.

6. La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.

## D- Théorie

1. Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?

- reserved_terminals est un champs privé de la classe Tower. Donc il est initialisé et est manipulé uniquement dans celle-ci. De plus, la classe n'a aucun setter public.

2. En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference constante ?
   Pourquoi n'est-il pas possible d'éviter la copie du `Point3D` passé en paramètre ?

- Parce que la méthode cap_length() renvoie déjà une **référence sur un Point3D**.

  Pensez-vous qu'il soit possible d'éviter la copie du `Point3D` passé en paramètre ?

- Non car dans la méthode cap_length() le Point3D est modifié.

## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.

//TODO
