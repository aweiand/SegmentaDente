// **********************************************************************
// PUCRS/FACIN
// COMPUTAÇÃO GRÁFICA
// **********************************************************************

#include <iostream>
#include <stdlib.h>
using namespace std;

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
#endif

#ifdef __APPLE__
#include <GL/glut.h>
#endif

#include "SOIL/SOIL.h"
#include "ImageClass.h"

ImageClass *Image, *NovaImagem, *Image3;

#define LIMIAR 100
#define LARGURA_JAN 600
#define ALTURA_JAN 600
#define KERNEL_SIZE 5

bool limiar = false;
int histo[255];
int media = 0;
int **pinos;

#include "methods.h"

// **********************************************************************
//  void init(void)
//  Inicializa os parâmetros globais de OpenGL
//
// **********************************************************************
void init(void)
{
    Image = new ImageClass();
    int r;

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto

    r = Image->Load("775.png");
    //r = Image->Load("647.png");

    if (!r)
        exit(1);
    else
        cout << ("Imagem carregada!\n");

    // Instacia o objeto que irá exibir a nova imagem
    // Caso precise alterar o tamanho da nova imagem, mude os parâmetros
    // da construtura, na chamada abaixo
    NovaImagem  = new ImageClass(Image->SizeX(), Image->SizeY(), Image->Channels());
    Image3      = new ImageClass(Image->SizeX(), Image->SizeY(), Image->Channels());

    Interval();
    AchaPinos();
    ColorePinos();
    ExpandePinos();


//    NovaImagem->CopyTo(Image);
//    linearHisto();
//    PassaBaixa();
//    NovaImagem->Save("teste.bmp");
//    verDentina(3);
//    linearHisto();
}

// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    gluOrtho2D(0,w,0,h);

    // Set the clipping volume
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


}
// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
void display( void )
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
// Ajusta o ZOOM da imagem para que apareca na metade da janela
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH)/2.0)/Image->SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT)/2.0)/Image->SizeY();
    Image->SetZoomH(zoomH);
    Image->SetZoomV(zoomV);

// posiciona a imagem nova na metada da direita da janela
    Image->SetPos(0, glutGet(GLUT_WINDOW_HEIGHT)/2.0);
    NovaImagem->SetPos(glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_HEIGHT)/2.0);
    Image3->SetPos(0, 0);

// Ajusta o ZOOM da imagem para que apareca na metade da janela
    NovaImagem->SetZoomH(zoomH);
    NovaImagem->SetZoomV(zoomV);
    Image3->SetZoomH(zoomH);
    Image3->SetZoomV(zoomV);

// Desenha uma Linha Vermelha
    Image->DrawLine(0,0,1000,1000, 255,0,0);
// Coloca as imagens na tela
    Image->Display();
    NovaImagem->Display();
    Image3->Display();

// Mostra a tela OpenGL
    glutSwapBuffers();
}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{

    switch ( key )
    {
    case 'z':
        LimpaFundo();
        break;
    case 'q':
        ExpandePinos();
        break;
    case 27: // Termina o programa qdo
        NovaImagem->Delete();
        Image->Delete();
        Image3->Delete();
        exit ( 0 );   // a tecla ESC for pressionada
        break;
    case '9':
        NovaImagem->Save("generate.png");
        cout << "Imagem Salva generate.png" << endl;
        break;
    case '2':
        ConvertBlackAndWhite();
        break;
    case '3':
        PassaBaixa();
        break;
    case 'c':
        NovaImagem->CopyTo(Image);
        cout << "NovaImagem copiara para Image" << endl;
        break;
    case 'a':
        limiar = true;
        cout << "Limiar = true" << endl;
        break;
    case 'e':
        AchaPinos();
        break;
    case 'p':
        ColorePinos();
        break;
    case 't':
        verDentina();
        break;
    default:
        break;
    }
    glutPostRedisplay();    // obrigatório para redesenhar a tela
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    switch ( a_keys )
    {
    case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
        break;
    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...

        break;
    default:
        break;
    }
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (500,70);

    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize  ( LARGURA_JAN, ALTURA_JAN);
    glutCreateWindow    ( "Image Loader" );

    init ();

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );
    //glutIdleFunc ( display );

    glutMainLoop ( );

    cout << "FIM" << endl;

    return 0;
}


