#include "Etat.h"

SINGLETON_DECLARATION_CPP(Etat);

bool         Etat::enmouvement = true;
bool         Etat::impression = false;
bool         Etat::afficheAxes = true;
GLenum       Etat::modePolygone = GL_FILL;
unsigned int Etat::nparticules = 400;
float        Etat::tempsDeVieMax = 15.0;
float        Etat::temps = 0.0;
float        Etat::dt = float(1.0/60.0);
float        Etat::gravite = float(10.0);
int          Etat::texnumero = 1;
GLuint       Etat::textureDVD = 0;
GLuint       Etat::textureMARIO = 0;
bool         Etat::boucleTexture = false;
glm::ivec2   Etat::sourisPosPrec = glm::ivec2(0);
int          Etat::modele = 1;
glm::vec3    Etat::posPuits = posPuitsOrig;
glm::vec3    Etat::posPuitsOrig = glm::vec3( 50.0, 0.0, 10.0 );
glm::vec3    Etat::bDim = glm::vec3( 110., 75., 105. );
float        Etat::pointsize = 9.0;
