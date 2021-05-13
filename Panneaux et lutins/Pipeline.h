#ifndef __PIPELINE_H__
#define __PIPELINE_H__

// variables pour l'utilisation des nuanceurs
GLuint prog;      // votre programme de nuanceurs
GLint locVertex = -1;
GLint locColor = -1;
GLint locvitesse = -1;
GLint loctempsDeVieRestant = -1;
GLint locmatrModel = -1;
GLint locmatrVisu = -1;
GLint locmatrProj = -1;
GLint locpointsize = -1;
GLint locleLutin = -1;
GLint loctexnumero = -1;
GLuint progRetroaction;  // votre programme de nuanceurs pour la rétroaction
GLint locVertexRetroaction = -1;
GLint locColorRetroaction = -1;
GLint locvitesseRetroaction = -1;
GLint loctempsDeVieRestantRetroaction = -1;
GLint loctempsRetroaction = -1;
GLint locdtRetroaction = -1;
GLint locgraviteRetroaction = -1;
GLint loctempsDeVieMaxRetroaction = -1;
GLint locposPuitsRetroaction = -1;
GLint locbDimRetroaction = -1;
GLuint progBase;  // le programme de nuanceurs de base
GLint locColorBase = -1;
GLint locmatrModelBase = -1;
GLint locmatrVisuBase = -1;
GLint locmatrProjBase = -1;

GLuint vao[2];
GLuint vbo[2];
GLuint tfo[1];
GLuint requete;

// matrices du pipeline graphique
MatricePipeline matrModel, matrVisu, matrProj;

#endif
