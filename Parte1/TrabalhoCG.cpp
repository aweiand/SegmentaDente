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
#include <string>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

#ifdef WIN32
    #include <windows.h>
    #include "gl\glut.h"
    #include "tinydir.h"
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
FILE *fp;
tinydir_dir dir;
// **********************************************************************

// **********************************************************************
// Biblioteca de métodos para a segmentação
#include "methods.h"
// Biblioteca para montar e gerar o relatorio
#include "report.h"
// **********************************************************************


// **********************************************************************
//      Mostra na tela o menu de opções
// **********************************************************************
void printaMenu(){
    cout << " - - - - - " << endl;
    cout << "Menu:" << endl;
    cout << "'z' - Limpa Fundo" << endl;
    cout << "'j' - Acha Dentina" << endl;
    cout << "'i' - Interval" << endl;
    cout << "'q' - Expandir Pinos" << endl;
    cout << "'e' - Acha Pinos" << endl;
    cout << "'p' - Colore Pinos" << endl;
    cout << "'t' - Ver Dentina" << endl;
    cout << "'l' - Limpa Erros Dentina" << endl;
    cout << "'2' - ConvertBlackAndWhite" << endl;
    cout << "'3' - Passa Baixa" << endl;
    cout << "'9' - Salva NovaImagem" << endl << endl;
    cout << "'m' - Reimprime Menu" << endl;
    cout << "Seta para Esquerda copia NovaImage para Image" << endl;
    cout << "Seta para Direita copia Image para NovaImage" << endl;
    cout << "..........." << endl;
    cout << "'7' - Tratar as Imagens em dataIN colocando o resultado em dataOUT com relatorio em dataREPORT." << endl;
    cout << " - - - - - " << endl;
}

void trataImagens(){
    tinydir_open(&dir, ".\\dataIN\\");

    while (dir.has_next){
        tinydir_file filein;
        tinydir_readfile(&dir, &filein);
        char groundfile[1024] = ".\\dataGROUND\\";
        char fromfile[1024] = ".\\dataIN\\";
        char tofile[1024] = ".\\dataOUT\\";

        if (strlen(filein.name) > 4){
            Image->Delete();
            NovaImagem->Delete();
            Image3->Delete();

            Image = new ImageClass();
            int r = Image->Load(strcat(fromfile, filein.name));
            if (r){
                // Instacia o objeto que irá exibir a nova imagem
                // Caso precise alterar o tamanho da nova imagem, mude os parâmetros
                // da construtura, na chamada abaixo
                NovaImagem  = new ImageClass(Image->SizeX(), Image->SizeY(), Image->Channels());
                Image3      = new ImageClass(Image->SizeX(), Image->SizeY(), Image->Channels());

                Interval(); // i
                AchaPinos(); // e
                ColorePinos(); // p
                ExpandePinos(); // q
                LimpaFundo(); // z
                ExpandePinos(); // q
                ExpandePinos(); // q
                AchaDentina(); // j

                report(groundfile, filein.name);

                NovaImagem->Save(strcat(tofile, filein.name));
            }
        }
        tinydir_next(&dir);
    }
    tinydir_close(&dir);

    cout << "Processo de Tratamento de Imagens Concluido!" << endl;
    printaMenu();
}

// **********************************************************************
//      Inicializa o sistema e o OpenGL
// **********************************************************************
void init(void){
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto

    //trataImagens();

    Image = new ImageClass();
    Image->Load("default.png");

    NovaImagem  = new ImageClass(Image->SizeX(), Image->SizeY(), Image->Channels());
    Image3      = new ImageClass(Image->SizeX(), Image->SizeY(), Image->Channels());

    printaMenu();
}

// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h ){
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
void display( void ){
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
void keyboard ( unsigned char key, int x, int y ){
    switch ( key ){
    case 'u':
        Image->Load("generate.png");
        break;
    case '7':
        trataImagens();
        break;
    case 'm':
        printaMenu();
        break;
    case 'z':
        LimpaFundo();
        break;
    case 'j':
        AchaDentina();
        break;
    case 'q':
        ExpandePinos();
        break;
    case 27: // Termina o programa qdo a tecla ESC for pressionada
        NovaImagem->Delete();
        Image->Delete();
        Image3->Delete();
        exit (0);
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
    case 'e':
        AchaPinos();
        break;
    case 'p':
        ColorePinos();
        break;
    case 't':
        verDentina();
        break;
    case 'l':
        limpaErrosDentina();
        break;
    case 'i':
        Interval();
        break;
    default:
        break;
    }

    glutPostRedisplay();    // obrigatório para redesenhar a tela
}

// **********************************************************************
//      Trata teclas especiais
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y ){
    switch ( a_keys ){
    case GLUT_KEY_LEFT:       // When Up Arrow Is Pressed...
        NovaImagem->CopyTo(Image);
        cout << "NovaImagem copiada para Image" << endl;
        break;
    case GLUT_KEY_RIGHT:     // When Down Arrow Is Pressed...
        Image->CopyTo(NovaImagem);
        cout << "Image copiada para NovaImagem" << endl;
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

// **********************************************************************
//      Função principal do sistema
// **********************************************************************
int main ( int argc, char** argv ){
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


