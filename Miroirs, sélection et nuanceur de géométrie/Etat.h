#ifndef __ETAT_H__
#define __ETAT_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "inf2705-Singleton.h"

//
// variables d'état
//
class Etat : public Singleton<Etat>
{
    SINGLETON_DECLARATION_CLASSE(Etat);
public:
    static bool enSelection;       // on est en mode sélection?
    static bool enmouvement;         // le modèle est en mouvement/rotation automatique ou non
    static bool afficheAxes;         // indique si on affiche les axes
    static bool culling;             // indique si on veut ne pas afficher les faces arrières
    static GLenum modePolygone;      // comment afficher les polygones (GL_LINE ou GL_FILL)
    static bool illumination;         // indique si on veut atténuer selon l'éloignement
    static glm::vec4 bDim;           // les dimensions de l'aquarium: une boite [-x,+x][-y,+y][-z,+z]
    static glm::ivec2 sourisPosPrec;
    static float dt;                 // intervalle entre chaque affichage (en secondes)
};

#endif
