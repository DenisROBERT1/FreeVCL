#Présentation

##En quelques lignes

FreeVCL est un logiciel qui a pour but de reproduire l'EDI C++ Builder de Borland, en étant gratuit et en gardant ce qui a fait son succès dans les années 2000 : la simplicité et la rapidité de développement. Ceci en s'adaptant à l'évolution des ordinateurs et des systèmes (compilation 64 bits, nouvelles versions de Windows). Pour simplifier, FreeVCL se veut être à C++ Builder ce que Lazarus est à Delphi Pascal.

FreeVCL permet de créer rapidement des IHM (interfaces homme-machine), autrement dit des fenêtres et des boîtes de dialogue. Vos programmes seront compatibles avec toutes les versions de Windows (depuis Windows XP jusqu'à Windows 10). La licence vous permet d'utiliser ou de vendre les programmes que vous aurez créé sans limitations.

L'outil comprend une bibliothèque de composants reproduisant aussi fidèlement que possible la [VCL de C++ Builder](https://fr.wikipedia.org/wiki/Visual_Component_Library) (Visual Components Library). Elle se présente sous la forme de fichiers .h et .lib (ou .a) à inclure dans vos projets.

Les programmes créés avec FreeVCL sont du C++ pur, ce qui signifie que vous pouvez les compiler avec le compilateur de votre choix, utiliser toutes les fonctionnalités du C++ (librairie standard, librairies externes, dérivation multiple, templates, ...).

Ce logiciel et son auteur sont totalement indépendants de Borland et de ses successeurs (Code Gear, Embarcadero). Cependant, une bonne connaissance de C++ Builder et de ses composants est essentielle pour comprendre et utiliser FreeVCL. En particulier, de nombreux élément de l'aide en ligne de C++ Builder ne sont pas reproduits dans l'aide de ce site. Veuillez vous reporter à l'aide de ce logiciel si vous désirez des précisions supplémentaires.

##Fonctionnalités

Le logiciel FreeVCL comprend un EDI avec de nombreuses fonctionnalités, notamment :

    - Fonctions classiques d'un traitement de texte : UNDO/REDO, Copier/coller, formats des fichiers texte, majuscules, tabulations, espaces.
    - Coloration syntaxique pour les principaux langages, C, C++, Pascal, Python, COBOL, et bien sûr [GOTO++](https://www.gotopp.org/presentation.fr.html). Les couleurs sont paramétrables pour s'adapter à vos habitudes de programmation.
    - Fonctions de recherche et de remplacement dans un ou plusieurs fichiers, avec de nombreuses options.
    - Affichage et modification des fichiers binaires (en mode hexadécimal).
    - Marque-pages.
    - Module de construction et de modification de boîtes de dialogue.
    - Module de construction et de modification d'icônes et de curseurs.
    - Macros avec possibilité de répétition automatique.
    - Modèles de code paramétrables.
    - Outil de statistiques (nombre de fichiers, de lignes d'un projet, avec ou sans commentaires).
    - Formatage automatique du code.
    - Indentation automatique en cours de saisie.
    - Utilitaire de conversion de nombres.
    - Possibilité d'ajouter vos propres outils.
    - Raccourcis clavier paramétrables.
    - Police de caractères paramétrable.
    - Nombreuses options du logiciel lui-même.
    - Aide contextuelle.

##Compléments

L'installation contient également des exemples de programmes. Ils vous permettront de vous familiariser avec le fonctionnement de cette librairie.

Pour fonctionner, cette librairie a besoin d'un compilateur C++, non fourni dans l'installation. Les compilateurs testés sont Visual Studio et Code::Block.

© Copyright 2010-2025 Denis ROBERT 