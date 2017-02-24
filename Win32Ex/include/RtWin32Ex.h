#ifndef RT_WIN32_EX_H
#define RT_WIN32_EX_H

/**
 * @file
 * This special header must be included by win32ex API consumers.<br>
 * It includes all headers of the library except some special headers.
 *
 * <p>
 * It has no associated .c file.
 * </p>
 */

/**

#### TODO:

Better hash map implementation (Java like)? Use it in Properties?
http://javahungry.blogspot.com/2013/08/hashing-how-hash-map-works-in-java-or.html

Ensure that there cannot be memory leaks while setting array/list size.
The items must remain accessible and untouched.

Implement an index system for lists (b-tree?).

Check structure values and flag values against operating system (Like AF_INET).

Implement:
producingopensourcesoftware.pdf

Modify resource management.
Normal execution flow should not branch.
That will put return in the middle of the code and errors treatments at the end.
Remove the_end.

See http://corelibrary.sourceforge.net/.
The xml part and maybe others.

See what is available on gtk side.

Gcc provide built-in equivalent for most of C runtime:
https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
They are not activated when -ansi is used but they can be called directly using __builtin_ prefix.

Propose setsockopt usage. See available options in RtSocket.c.

Signals management under Linux.
"When SO_KEEPALIVE is enabled on the socket the protocol checks in a protocol-specific manner if the other end is still alive. A SIGPIPE signal is raised if a process sends or receives on a broken stream; this causes naive processes, which do not handle the signal, to exit."

Create an "Hello, world!" sample project.

Use more flags :
/D "WIN32" /D "_UNICODE" /D "UNICODE"
/D "NDEBUG" /D _DEBUG
/D "_WINDOWS" /D _CONSOLE
Console/GUI app with gcc?

Take care of alignement for atomic usage.
See __attribute__ ((aligned (16))) et  __declspec(align(#)), I might define a macro in RtWin32ExTypes.h.

Fix memory leaks in RtList.c (See TODOs in this file).

Complete ttmemorytest.

Optimize memory copy and other memory functions.

Faire des tests "manuels".
Vérification du chemin du fichier exécutable.
Récupération de variable d'environement.
Récupération du répertoires de configuration de l'application.
Messages correspondants aux erreurs supportées.
Affichage de la fréquence des compteurs (QueryPerformanceFrequency et équivalent sous Linux).


Fonction pour récupérer un nom de fichier temporaire ?

Support long paths in RtFile.c.

Pour les noms de fichiers long, ne pas ajouter le préfix si le nom est court...

Un programme qui vérifie l'architecture des headers (Les différents layers et que l'on n'inclut pas d'un layer courant ou supérieur).

TODO dans page heap : ne réallouer que si nécessaire.

lpHeap->free devrait prendre null ? Simplifierais le code des free_resources.

Renommer toutes les applications en zz.
zzmain.

Faire un essaie de plantage.

(Un bouton qui fait planter l'application).

Command.

Menu checkbox (Fonction activée désactivée).
Séparateur.
Bitmap (Activée, désactivée, permanente, grisée ?)
Grayed/Enabled.
ampersand.
Shortcut : &Close\tAlt+F4 (Keyboard Accelerators)
DWORD helpId.

Les commandes sont gérées dans :

Menu classique.
Menu contextuel.
Toolbar (Deux types : bitmaps et textes.

Un menu contient des items.
Un item peut être
  Un sous menu.
  Un séparateur.
  Une commande.

Help/tooltip message -> Au moins utilisé dans le cas de la toolbar.

Gérer le cas des radios boutons.

## Detect handles leaks

Find a solution.

## Menu icons

Manage messages on library side.

## Multi array

Suppose that we reach the capacity of an array of 1000 items.
We will allocate a place for 1500 items then copy the 1000 items into it.
It is not effective.
It would be better to several smaller arrays.
Then we are not able to go through the array by incrementing a pointer, but it should not be too bad.
We should be able to compute fastly from a given index the array in charge (divide the index by array size).
Then use a modulo to find the right item in the corresponding array.
This solution might be used for tables.
Obviously, it would imply to adapt a lot of code that access the values by indexes in the array.
Like sorting.
The remaining question is about indexes.
Do we really need to have multi arrays for indexes?
What is the cost of copying 1Go of memory? Regarding the cost of reading?
Remember that there can be insertion in an index...

For data create/remove, multi array should prevent all copies.
In case of create, either there is some room and we initialize the new object in place.
Either there is no more room and we allocate a new chunk.

The issue is with indexes.
In that case new items are added in the middle of the list.
And removed items are also in the middle of the list.
If the list is big then there will be many copies.
But if there are chunks, empty spaces and so on... There will be a big cost for maintenance.
But maybe sorting is not that much important.

## Enums

Use enums when possible.

## GdipPlus

Ajouter une fonction d'initialization et une fonction de clean de gdiplus.
(Attention à bien lire la doc avant de le faire)

Faire une application de conversion d'image en png ou en jpg.

## Climbing

Manage update with index impact.
Not an easy one as a lot of items are moved forward and backward.
RtIndexUpdatedTableItem
Plus modify the interface.

Relationships will be a tough part too.

Manage index activation? Used for ranking?

## Better implementation for RtComputeStringHash

## A test on sizeof

sizeof(RT_UN32) == sizeof(DWORD)
size_t...

Plus display of the set flags.

## For Properties file, I will need some kind of HashMap.

It is faster to search a String using a hash than searching it in a sorted list of Strings.
The comparison of an hash with a hash is easier and faster.
The drawback is that it is consuming more memory because the structure is fare more complicated.
Maybe it is an overkill.

The principle is to have an array of the hashs.
But there can be collisions.
Perhaps could I have more hashs with the same values if there are collisions.
I need a function to compute a hash from a String or from an array of bytes.

A question is: could I manage all values but not allocating any other buffer than the one used with the decoded file.

The array of hashes should not be only an array of hashes.
It should also contain a pointer on the key and a pointer on the value.

The sorted array is alimented with pointers on all the values.
Then it is sorted if required.

## Better implementation for RtConvertIntegerToString

## A code lines counter.

Taking a list of extensions as parameters.

## Build rtchar8.h/rtchar8.c

I must get ride of "win32exosdefines".
It remains necessary for RtConvertIntegerToString.

Then I must be able to produce a rtchar8 from RtChar.

RT_CHAR -> RT_CHAR8.
_R("") -> ""
_R('') -> ''
String -> String8

Remplacer par RtEncode.

## CreateFile and MAX_PATH.

I probably need to call LongPath.

## Update samples architecture.

There should be different source folder, and even different folder by project.

## Source check

No length or count (Use size).
No folder (Use directory).
No tabs.
No trailing spaces.
No char (Use RT_CHAR and RT_CHAR8).
No NULL (Use RT_NULL).
Que du ASCII.

## OpenGL.

## List possible ifdef values and document.

I need a complete reference of define values used like WIN32, is 64 bits and so on.

Perhaps should I build my own defines.

## Test case writing small file with two encodings, checking of content.

## Test case on encoding conversion with RT_TYPE_MAX_UN or not as input size.

## Test the various flags to open a file.

## Test case on size of types.

But how to determine if we are under 64 bits or not ?

## Ne pas utiliser les types de taille fixe dans les containers.

A la base j'y avais pensé pour la sérialization et pour la taille.
Mais l'alignement est sûrement presque aussi intéressant que la taille.
Et la sérialization ne sera pas comme prévue.

## Ouverture d'un fichier que s'il n'existait pas.

## Utilisation de fsync.

## Vérifier les retours de RtCopyString dans l'api.

## Ordre des arguments.

## Attention à MAX_PATH!! Vérifier les appels.

Certaines fonctions ne supportent pas d'aller au dela de MAX_PATH.
Utiliser LongPath dans ce cas.

## Chercher TODO

Il y a des TODO un peu partout dans le code.


## Se débarrasser des commentaires en français.

*/


#include "layer000/RtWin32ExTypes.h"

/* #include "layer001/RtWin32ExOsDefines.h", internal use only. */

#include "layer002/RtErrorCode.h"

#include "layer003/RtAtomic.h"
#include "layer003/RtChrono.h"
#include "layer003/RtComparisonCallback.h"
#include "layer003/RtCriticalsection.h"
#include "layer003/RtEnvironmentVariable.h"
#include "layer003/RtEvent.h"
#include "layer003/RtMemory.h"
#include "layer003/RtSleep.h"
#include "layer003/RtThread.h"
#include "layer003/RtVirtualMemory.h"

#include "layer004/RtBinarySearch.h"
#include "layer004/RtChar.h"
#include "layer004/RtChar8.h"
#include "layer004/RtFastInitialization.h"
#include "layer004/RtInitialization.h"
#include "layer004/RtQuickSort.h"
#include "layer004/RtSocket.h"

#include "layer005/RtErrorMessage.h"
#include "layer005/RtFileSystem.h"
#include "layer005/RtHeap.h"
#include "layer005/RtRandom.h"
#include "layer005/RtStaticHeap.h"

#include "layer006/RtConsole.h"
#include "layer006/RtEncoding.h"
#include "layer006/RtFile.h"
#include "layer006/RtPageHeap.h"
#include "layer006/RtRuntimeHeap.h"

#include "layer007/RtFileUtils.h"
#include "layer007/RtTableMetadata.h"

#include "layer008/RtArray.h"
#include "layer008/RtSmallFile.h"
#include "layer008/RtTable.h"

#include "layer009/RtLinkedList.h"
#include "layer009/RtList.h"
#include "layer009/RtSortableArray.h"

#include "layer010/RtProperties.h"

#endif /* RT_WIN32_EX_H */
