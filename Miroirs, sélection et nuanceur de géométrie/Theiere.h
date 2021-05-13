#ifndef __THEIERE_H__
#define __THEIERE_H__

//
// une théière
//
class Theiere
{
    static FormeSphere *sphere; // une sphère centrée de rayon 1 pour les yeux
public:
    Theiere( float r = 1.0, float h = 0.0, float ang = 0.0, float vit = 1.0, float tai = 0.5 )
        : rayon(r), hauteur(h), angle(ang), vitesse(vit), taille(tai)
    {
        // créer une théière graphique
        initialiserGraphique();
    }
    ~Theiere()
    {
        conclureGraphique();
    }
    void initialiserGraphique()
    {
        // créer quelques formes
        if ( sphere == NULL ) sphere = new FormeSphere( 1.0, 8, 8, true );

        // allouer les objets OpenGL
        glGenVertexArrays( 1, &vao );

        // initialiser le VAO pour la théière
        glBindVertexArray( vao );

        // créer le VBO pour les sommets
        glGenBuffers( 1, &vboTheiereSommets );
        glBindBuffer( GL_ARRAY_BUFFER, vboTheiereSommets );
        glBufferData( GL_ARRAY_BUFFER, sizeof(gTheiereSommets), gTheiereSommets, GL_STATIC_DRAW );
        // faire le lien avec l'attribut du nuanceur de sommets
        glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
        glEnableVertexAttribArray(locVertex);

        // créer le VBO la connectivité
        glGenBuffers( 1, &vboTheiereConnec );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboTheiereConnec );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(gTheiereConnec), gTheiereConnec, GL_STATIC_DRAW );

        glBindVertexArray(0);
    }
    void conclureGraphique()
    {
        delete sphere;
    }

    void afficher()
    {
        matrModel.PushMatrix();{ // sauvegarder la tranformation courante

            // amener le repère à la position courante
            matrModel.Rotate( angle, 0, 0, 1 ); // tourner selon l'angle
            matrModel.Translate( rayon, 0.0, hauteur ); // se déplacer dans la direction de l'axe des X ainsi tourné

            // partie 2: modifs ici ...

            // afficher le corps : la théière
            glm::vec3 coulCorps( 1.0, 0.2, 0.0 ); // rouge légèrement orangé

            if (estSelectionnee || Etat::enSelection) {
                coulCorps = couleurSel;
            }
            // donner la couleur de sélection au besoin (partie 2)
            glVertexAttrib3fv( locColor, glm::value_ptr(coulCorps) );


            matrModel.PushMatrix();{
                matrModel.Scale( taille, taille, taille );
                matrModel.Rotate( 90, 0, 0, 1 ); // tourner la théière pour avoir son couvercle tangeant à la rotation
                matrModel.Rotate( 90, 1, 0, 0 ); // mettre la théière avec le couvercle vers le haut
                if ( vitesse < 0.0 ) matrModel.Rotate( 180.0, 0.0, 1.0, 0.0 );
                glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // ==> Avant de tracer, on doit informer la carte graphique des changements faits à la matrice de modélisation
                glBindVertexArray( vao );
                glDrawElements( GL_TRIANGLES, sizeof(gTheiereConnec)/sizeof(GLuint), GL_UNSIGNED_INT, 0 );
                glBindVertexArray(0);

                // donner la couleur des yeux
                glVertexAttrib3f( locColor, 0.8, 0.8, 0.2 ); // jaune

                // afficher les yeux
                const GLfloat facteur[2] = { -1.0, 1.0 };
                for (unsigned int i = 0; i < 2; ++i)
                {
                    matrModel.PushMatrix(); {
                        matrModel.Rotate( 30.0 * facteur[i], 0.0, 1.0, 0.0 );
                        matrModel.Translate( 2.6, 3.0, 0.0 );
                        matrModel.Scale( 0.4, 0.4, 0.4 );
                        glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                        sphere->afficher();
                    }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                }

            }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // informer ...
    }

    void avancerPhysique()
    {
        if (!estSelectionnee)
            angle += Etat::dt * vitesse;
    }

    // les variables de la théière
    GLuint vao = 0;
    GLuint vboTheiereSommets = 0;
    GLuint vboTheiereConnec = 0;
    float rayon;          // en unités
    float hauteur;        // en unités
    float angle;          // en degrés
    float vitesse;        // en degrés/seconde
    float taille;         // en unités
    bool estSelectionnee = false; // la théière est sélectionnée ?
    glm::vec3 couleurSel; // la couleur en mode sélection
};

FormeSphere* Theiere::sphere = NULL;

#endif
