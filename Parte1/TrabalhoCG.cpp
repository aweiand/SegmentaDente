// **********************************************************************
// PUCRS/FACIN
// COMPUTAÇÃO GRÁFICA
//
//	TrabalhoCG.cpp
//  Programa de segmentação de Imagens de Micro Tomografia
//  de dentes com algoritmos de segmentação por linear.
//
//	Augusto Weiand <guto.weiand@gmail.com>
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

// **********************************************************************
// Variáveis das imagens e outras globais utilizadas no sistema
ImageClass *Image, *NovaImagem, *Image3;

#define LIMIAR 100
#define LARGURA_JAN 600
#define ALTURA_JAN 600
#define KERNEL_SIZE 5

int histo[255];
int media = 0;
int **pinos;
int kernel_2 = 3;
// **********************************************************************

// **********************************************************************
// Biblioteca de métodos para a segmentação
#include "methods.h"
// **********************************************************************

// **********************************************************************
//      Mostra na tela o menu de opções
// **********************************************************************
void printaMenu(){
    cout << " - - - - - " << endl;
    cout << "Menu:" << endl;
    cout << "'y' - Aplica Mediana" << endl;
    cout << "'i' - Aplica Interval" << endl;
    cout << "'z' - Limpa Fundo" << endl;
    cout << "'j' - Acha Dentina" << endl;
    cout << "'q' - Expandir Pinos" << endl;
    cout << "'e' - Acha Pinos" << endl;
    cout << "'p' - Colore Pinos" << endl;
    cout << "'t' - Ver Dentina" << endl;
    cout << "'l' - Limpa Erros Dentina" << endl;
    cout << "'2' - ConvertBlackAndWhite" << endl;
    cout << "'3' - Passa Baixa" << endl;
    cout << "'9' - Salva NovaImagem" << endl << endl;
    cout << "'m' - Reimprime Menu" << endl;
    cout << "Seta para Cima aumenta Kernel_2" << endl;
    cout << "Seta para Baixo diminui Kernel_2" << endl;
    cout << "Seta para Esquerda copia NovaImage para Image" << endl;
    cout << "Seta para Direita copia Image para NovaImage" << endl;
    cout << " - - - - - " << endl;
}

// **********************************************************************
//      Inicializa o sistema e o OpenGL
// **********************************************************************
void init(void)
{
    Image = new ImageClass();
    int r;

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto

    //r = Image->Load("775.png");
    r = Image->Load("1014.png");

    if (!r)
        exit(1);
    else
        cout << ("Imagem carregada!\n");

    // Instacia o objeto que irá exibir a nova imagem
    // Caso precise alterar o tamanho da nova imagem, mude os parâmetros
    // da construtura, na chamada abaixo
    NovaImagem  = new ImageClass(Image->SizeX(), Image->SizeY(), Image->Channels());
    Image3      = new ImageClass(Image->SizeX(), Image->SizeY(), Image->Channels());

    ConvertBlackAndWhite(Image, NovaImagem);
    NovaImagem->CopyTo(Image);
    PassaBaixa(Image, NovaImagem);
    NovaImagem->CopyTo(Image);
    ColorePinos(Image, NovaImagem);
    NovaImagem->CopyTo(Image);

    for(int i=0; i<=4;i++){
        ExpandePinos(Image, NovaImagem);
        NovaImagem->CopyTo(Image);
    }

    LimpaFundo(Image, NovaImagem);
    NovaImagem->CopyTo(Image);

    //verDentina(Image, NovaImagem);

//    Interval(Image, NovaImagem);
//    AchaPinos(Image, NovaImagem);
//    ColorePinos(NovaImagem, Image);
//    ExpandePinos(Image, NovaImagem);
//    NovaImagem->CopyTo(Image);

//    LimpaFundo(Image, NovaImagem);
//    AchaDentina();

    printaMenu();
}

// **********************************************************************
//      Trata o redimensionamento da janela OpenGL
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
//      Display
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
//      Trata os eventos do teclado
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{

    switch ( key )
    {
    case 'z':
        LimpaFundo(Image, NovaImagem);
        break;
    case 'j':
        AchaDentina();
        break;
    case 'q':
        ExpandePinos(Image, NovaImagem);
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
        ConvertBlackAndWhite(Image, NovaImagem);
        break;
    case '3':
        PassaBaixa(Image, NovaImagem);
        break;
    case 'e':
        AchaPinos(Image, Image3);
        break;
    case 'p':
        ColorePinos(Image, NovaImagem);
        break;
    case 't':
        verDentina(Image, NovaImagem);
        break;
    case 'l':
        limpaErrosDentina();
        break;
    case 'y':
        AplicaMediana(Image, NovaImagem);
        break;
    case 'm':
        printaMenu();
        break;
    case 'i':
        Interval(Image, NovaImagem);
        break;
    default:
        break;
    }
    glutPostRedisplay();    // obrigatório para redesenhar a tela
}

// **********************************************************************
//      Trata teclas especiais
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    switch ( a_keys )
    {
    case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
        kernel_2 += 2;
        cout << "Kernel_2: " << kernel_2 << endl;
        break;
    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
        kernel_2 -= 2;
        cout << "Kernel_2: " << kernel_2 << endl;
        break;
    case GLUT_KEY_LEFT:     // When Down Arrow Is Pressed...
        NovaImagem->CopyTo(Image);
        cout << "NovaImagem Copiada para Image" << endl;
        break;
    case GLUT_KEY_RIGHT:     // When Down Arrow Is Pressed...
        Image->CopyTo(NovaImagem);
        cout << "Image Copiada para NovaImagem" << endl;
        break;
    default:
        break;
    }
    glutPostRedisplay();    // obrigatório para redesenhar a tela
}

// **********************************************************************
//      Função principal do sistema
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
