#ifndef __AQUARIUM_H__
#define __AQUARIUM_H__

#include <cstdio>
#include <glm/fwd.hpp>
#include <vector>
#include <iterator>

//
// l'aquarium
//
class Aquarium
{
    static FormeCube *cubeFil;
    static FormeQuad *quad;
public:
    Aquarium( )
        : locillumination(-1)
    {
        // les hauteurs variées des théières
        float hauteur[] = { -6.0, 4.0, 3.0, -2.0, -5.0, -3.0, 8.0, 7.0, -1.0, -4.0, -9.9, -7.0, -8.0, 1.0, 5.0, -9.0, 6.0, 2.0 };

        // créer un aquarium graphique
        glUseProgram( prog );
        initialiserGraphique();

        // initialiser la génération de valeurs aléatoires pour la création de théières
        srand( time(NULL) );

        // remplir l'aquarium
        for ( unsigned int i = 0 ; i < sizeof(hauteur)/sizeof(hauteur[0]) ; ++i )
        {
            // donner distance aléatoire
            float dist = glm::mix( 2.0, 9.0, rand()/((double)RAND_MAX) );
            // donner un angle aléatoire en degrés
            float angle = glm::mix( 0, 360, rand()/((double)RAND_MAX) );
            // donner vitesse aléatoire de rotation
            float vit = glm::mix( -8.0, 8.0, rand()/((double)RAND_MAX) );
            vit += 4.0 * glm::sign(vit); // ajouter ou soustraire 4.0 selon le signe de vx afin d'avoir : 4.0 <= abs(vx) <= 12.0
            // donner taille aléatoire
            float taille = glm::mix( 0.1, 0.3, rand()/((double)RAND_MAX) );

            // créer une nouvelle théière
            Theiere *p = new Theiere( dist, hauteur[i], angle, vit, taille );

            // assigner une couleur de sélection
            // partie 2: modifs ici ...
            p->couleurSel = glm::vec3(1.0, 0.0, 1.0 - i / 255.0);

            // ajouter cette théière dans la liste
            theieres.push_back( p );
        }

        // créer quelques autres formes
        glUseProgram( progBase );
        if ( cubeFil == NULL ) cubeFil = new FormeCube( 2.0, false );
        if ( quad == NULL ) quad = new FormeQuad( 1.0, true );
    }
    ~Aquarium()
    {
        conclureGraphique();
        // vider l'aquarium
        while ( !theieres.empty() ) theieres.pop_back( );
    }
    void initialiserGraphique()
    {
        GLint prog = 0; glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
        if ( prog <= 0 )
        {
            std::cerr << "Pas de programme actif!" << std::endl;
            locVertex = locColor = -1;
            return;
        }
        if ( ( locVertex = glGetAttribLocation( prog, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
        if ( ( locColor = glGetAttribLocation( prog, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;
        if ( ( locillumination = glGetUniformLocation( prog, "illumination" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de illumination" << std::endl;
    }
    void conclureGraphique()
    {
        delete cubeFil;
        delete quad;
    }

    void afficherParoisXpos()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( 0.5, 0.0, 0.0 );
            matrModel.Rotate( -90.0, 0.0, 1.0, 0.0 );
            matrModel.Translate( -0.5, -0.5, 0.0 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherParoisXneg()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( -0.5, 0.0, 0.0 );
            matrModel.Rotate( 90.0, 0.0, 1.0, 0.0 );
            matrModel.Translate( -0.5, -0.5, 0.0 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherParoisYpos()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( 0.0, 0.5, 0.0 );
            matrModel.Rotate( 90.0, 1.0, 0.0, 0.0 );
            matrModel.Translate( -0.5, -0.5, 0.0 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherParoisYneg()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( 0.0, -0.5, 0.0 );
            matrModel.Rotate( -90.0, 1.0, 0.0, 0.0 );
            matrModel.Translate( -0.5, -0.5, 0.0 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherSolZneg()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( 0.0, 0.0, -0.5 );
            matrModel.Translate( -0.5, -0.5, 0.0 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherCoins()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( Etat::bDim.x, Etat::bDim.y, Etat::bDim.z );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            cubeFil->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherParois()
    {
        // tracer les coins de l'aquarium avec le programme de base
        glVertexAttrib3f( locColorBase, 1.0, 1.0, 1.0 ); // blanc
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        afficherCoins();

        // tracer les parois de verre de l'aquarium
        glEnable( GL_BLEND );
        glEnable( GL_CULL_FACE ); glCullFace( GL_BACK ); // ne pas afficher les faces arrière
        afficherCouleurParois(0.5, 0.2, 0.2, 0.4); // rougeâtre
        afficherParoisXpos(); // paroi en +X
        afficherCouleurParois(0.2, 0.5, 0.2, 0.4); // verdâtre
        afficherParoisYpos(); // paroi en +Y
        afficherCouleurParois(0.2, 0.2, 0.5, 0.4); // bleuté
        afficherParoisXneg(); // paroi en -X
        afficherCouleurParois(0.5, 0.5, 0.2, 0.4); // jauneâtre
        afficherParoisYneg(); // paroi en -Y

        glDisable( GL_CULL_FACE );

        // tracer le sol presque complètement opaque
        glVertexAttrib4f( locColorBase, 0.5, 0.5, 0.5, 0.95 ); // gris
        afficherSolZneg(); // sol en -Z
        glDisable( GL_BLEND );
    }

    void afficherCouleurParois(float rouge, float vert, float bleu, float alpha) {
        if (Etat::enSelection)
            glVertexAttrib4f(locColorBase, 0.0, 0.0, 0.0, 0.0);
        else
            glVertexAttrib4f(locColorBase, rouge, vert, bleu, alpha);
    }

    void afficherToutesLesTheieres()
    {
        glVertexAttrib4f( locColor, 1.0, 1.0, 1.0, 1.0 );

        for ( std::vector<Theiere*>::iterator it = theieres.begin() ; it != theieres.end() ; it++ )
        {
            (*it)->afficher();
        }
    }

    // afficher le contenu de l'aquarium
    void afficherContenu( GLenum ordre = GL_CCW )
    {
        glUseProgram( prog );

        // afficher des triangles en plein ou en fil de fer ?
        glPolygonMode( GL_FRONT_AND_BACK, Etat::modePolygone );
        // ne pas afficher les triangles dont on voit la face arrière ? (touche 'c')
        if ( Etat::culling ) glEnable( GL_CULL_FACE );
        if ( Etat::culling ) glCullFace( ordre == GL_CCW ? GL_BACK : GL_FRONT );

        // afficher les théières
        afficherToutesLesTheieres();

        if ( Etat::culling ) glDisable( GL_CULL_FACE );
    }

    // afficher l'aquarium
    void afficher()
    {
        // tracer l'aquarium avec le programme de nuanceur de base
        glUseProgram( progBase );
        glUniformMatrix4fv( locmatrProjBase, 1, GL_FALSE, matrProj );
        glUniformMatrix4fv( locmatrVisuBase, 1, GL_FALSE, matrVisu );
        glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );

        // 1) Remplir le stencil avec le miroir en affichant les parois de
        // façon similaire à ce qui est fait dans afficherParois().
        // On veut des 1,2,4 ou 8 pour tous les pixels des miroirs et
        // des 0 partout ailleurs, mais on ne veut rien tracer à l'écran
        // pour le moment: on s'organise pour que le test du stencil ne
        // passe jamais tout en remplissant le stencil de valeurs.
        glEnable( GL_CULL_FACE ); glCullFace( GL_BACK ); // ne pas afficher les faces arrière
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        // partie 1: modifs ici ...
        // ...
        glEnable( GL_STENCIL_TEST);

        glStencilOp( GL_REPLACE, GL_KEEP, GL_KEEP );
        glStencilFunc( GL_NEVER, 1, 1 );
        afficherParoisYpos(); // paroi en +Y

        glStencilFunc( GL_NEVER, 2, 2 );
        afficherParoisXpos(); // paroi en +X

        glStencilFunc( GL_NEVER, 4, 4 );
        afficherParoisYneg(); // paroi en -Y

        glStencilFunc( GL_NEVER, 8, 8 );
        afficherParoisXneg(); // paroi en -X

        glDisable( GL_CULL_FACE );

        // 2) Maintenant que le stencil est rempli de valeurs aux positions des miroirs,
        // on trace les scènes réfléchies.
        // On fait en sorte que seulement la région du stencil avec des 1,2,4,8 soit tracée
        // (c'est-à-dire seulement la région du miroir)
        // tracer les théières avec le programme de nuanceur de ce TP
        glUseProgram( prog );
        glUniformMatrix4fv( locmatrProj, 1, GL_FALSE, matrProj );
        glUniformMatrix4fv( locmatrVisu, 1, GL_FALSE, matrVisu );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        int illuminationEtat = Etat::illumination;
        glUniform1i( locillumination, ((Etat::enSelection) ? (Etat::illumination = 0) : (Etat::illumination = illuminationEtat)));
        glEnable(GL_STENCIL_TEST);
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
        Etat::illumination = illuminationEtat;
        // et on trace le contenu inversé 4 fois
        // +Y
        glStencilFunc( GL_EQUAL, 1, 1 );
        matrModel.PushMatrix();{
            matrModel.Translate( 0*Etat::bDim.x, 2*Etat::bDim.y, 0*Etat::bDim.z );
            matrModel.Scale( 1, -1, 1 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            afficherContenu();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    
        // +X
        glStencilFunc( GL_EQUAL, 2, 2 );
        matrModel.PushMatrix();{
            matrModel.Translate( 2*Etat::bDim.x, 0*Etat::bDim.y, 0*Etat::bDim.z );
            matrModel.Scale( -1, 1, 1 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            afficherContenu();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    
        // -Y
        glStencilFunc( GL_EQUAL, 4, 4 );
        matrModel.PushMatrix();{
            matrModel.Translate( 0*Etat::bDim.x, -2*Etat::bDim.y, 0*Etat::bDim.z );
            matrModel.Scale( 1, -1, 1 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            afficherContenu();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    
        // -X
        glStencilFunc( GL_EQUAL, 8, 8 );
        matrModel.PushMatrix();{
            matrModel.Translate( -2*Etat::bDim.x, 0*Etat::bDim.y, 0*Etat::bDim.z );
            matrModel.Scale( -1, 1, 1 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            afficherContenu();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );

        // afficher la réflexion d'abord
        glDisable( GL_STENCIL_TEST );
        
        // avant d'enfin tracer le contenu intérieur
        afficherContenu( );

        // afficher les parois de l'aquarium
        glUseProgram( progBase );

        afficherParois( );
    }

    // sélectionner une théière
    void selectionnerTheiere()
    {
        // s'assurer que toutes les opérations sont terminées
        glFinish();

        // obtenir la clôture et calculer la position demandée
        GLint cloture[4];
        glGetIntegerv( GL_VIEWPORT, cloture );
        GLint posX = Etat::sourisPosPrec.x, posY = cloture[3] - Etat::sourisPosPrec.y;

        // lire le tampon à dessiner
        glReadBuffer( GL_BACK );

        // obtenir les couleurs
        GLubyte couleur[3];
        glReadPixels( posX, posY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, couleur );

        // std::cout << "couleur = " << (int) couleur[0] << " " << (int) couleur[1] << " " << (int) couleur[2] << std::endl;

        // vérifier si l'objet est rouge
        if ( couleur[0] == 255) {
            
            // trouver la bonne théière
            for ( std::vector<Theiere*>::iterator it = theieres.begin() ; it != theieres.end() ; it++ )
            {
                if ( (*it)->couleurSel == glm::vec3(couleur[0] / 255.0, couleur[1] / 255.0, couleur[2] / 255.0) ) {
                    basculerSelection( **it );
                    break;
                }
            }
        }
    }

    void basculerSelection( Theiere &theiere ) {
        if ( theiere.estSelectionnee )
            theiere.estSelectionnee = false;
        else
            theiere.estSelectionnee = true;
    }

    void calculerPhysique( )
    {
        if ( Etat::enmouvement )
        {
            if ( getenv("DEMO") != NULL )
            {
                // faire varier la taille de la boite automatiquement pour la démo
                static int sensX = 1;
                Etat::bDim.x += sensX * 0.1 * Etat::dt;
                if ( Etat::bDim.x < 8.0 ) sensX = +1;
                else if ( Etat::bDim.x > 12.0 ) sensX = -1;

                static int sensY = -1;
                Etat::bDim.y += sensY * 0.07 * Etat::dt;
                if ( Etat::bDim.y < 8.0 ) sensY = +1;
                else if ( Etat::bDim.y > 12.0 ) sensY = -1;
            }

            for ( std::vector<Theiere*>::iterator it = theieres.begin() ; it != theieres.end() ; it++ )
            {
                (*it)->avancerPhysique();
            }
        }
    }

    GLint locillumination;

    // la liste des théières
    std::vector<Theiere*> theieres;
};

FormeCube* Aquarium::cubeFil = NULL;
FormeQuad* Aquarium::quad = NULL;

#endif
