#ifndef __CAMERA_H__
#define __CAMERA_H__

//
// variables pour définir le point de vue
//
class Camera
{
public:
    Camera() { theta = 270.0; phi = 80.0; dist = 300.0; }
    void definir()
    {
        glm::vec3 ptVise = glm::vec3( 0.0, 0.0, 0.5*Etat::bDim.z ); // un point au milieu du modèle
        matrVisu.LookAt( ptVise.x + dist*cos(glm::radians(theta))*sin(glm::radians(phi)),
                         ptVise.y + dist*sin(glm::radians(theta))*sin(glm::radians(phi)),
                         ptVise.z + dist*cos(glm::radians(phi)),
                         ptVise.x, ptVise.y, ptVise.z,
                         0.0, 0.0, 1.0 );
    }
    void verifierAngles() // vérifier que les angles ne débordent pas les valeurs permises
    {
        if ( theta > 360.0 ) theta -= 360.0; else if ( theta < 0.0 ) theta += 360.0;
        const GLdouble MINPHI = 0.01, MAXPHI = 180.0 - 0.01;
        phi = glm::clamp( phi, MINPHI, MAXPHI );
    }
    double theta;         // angle de rotation de la caméra (coord. sphériques)
    double phi;           // angle de rotation de la caméra (coord. sphériques)
    double dist;          // distance (coord. sphériques)
} camera;

#endif
