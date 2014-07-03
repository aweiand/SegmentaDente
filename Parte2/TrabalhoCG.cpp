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
#include <math.h>
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

#define LARGURA_JAN 600
#define ALTURA_JAN 600

int kernel = 5;
int limiar = 100;

int mco[255][255];

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
    cout << "'w' - Converter P&B" << endl;
    cout << "'i' - Intervalo" << endl;
    cout << "'m' - Mediana" << endl;
    cout << "'h' - Linearizacao Histograma" << endl;
    cout << "'b' - Aplica PassaBaixa" << endl;
    cout << "'o' - Cria Matriz de co-ocorrencia" << endl;
    cout << endl;
    cout << "'f' - Aplica TexturaFundo" << endl;
    cout << "'p' - Aplica TexturaPinos" << endl;
    cout << endl;
    cout << "'1' - Menu" << endl;
    cout << " - - - - - " << endl;
}

// **********************************************************************
//      Sequencia de algoritmos para tratar as imagens
// **********************************************************************
void algoritmoTratamento(){

}


// **********************************************************************
//      Trata as imagens da pasta dataIN, colocando o resultado
//      na pasta dataOUT e o relatorio na pasta dataREPORT
// **********************************************************************
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

                algoritmoTratamento();

                report(groundfile, filein.name, Image, NovaImagem);

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
    // Image->Load("default.png");
     Image->Load("desc_dentina.png");
    // Image->Load("desc_pino.png");
    // Image->Load("desc_preto.png");

    NovaImagem  = new ImageClass(Image->SizeX(), Image->SizeY(), Image->Channels());

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

// Ajusta o ZOOM da imagem para que apareca na metade da janela
    NovaImagem->SetZoomH(zoomH);
    NovaImagem->SetZoomV(zoomV);

// Coloca as imagens na tela
    Image->Display();
    NovaImagem->Display();

// Mostra a tela OpenGL
    glutSwapBuffers();
}

// **********************************************************************
//      Trata os eventos do teclado
// **********************************************************************
void keyboard ( unsigned char key, int x, int y ){
    switch ( key ){
        case 'w':
            ConvertBlackAndWhite(Image, NovaImagem, limiar);
        break;
        case 'i':
            Interval(Image, NovaImagem);
        break;
        case 'b':
            PassaBaixa(Image, NovaImagem);
        break;
        case 'm':
            AplicaMediana(Image, NovaImagem, kernel);
        break;
        case 'h':
            linearHisto(Image, NovaImagem);
        break;
        case '1':
            printaMenu();
        break;
        case 'f':
            texturaFundo(Image, NovaImagem, kernel);
        break;
        case 'p':
            texturaPinos(Image, NovaImagem, kernel);
        break;
        case 'o':
            _createMCO(2, 0, Image);
        break;

        case 27:
            NovaImagem->Delete();
            Image->Delete();
            exit(0);
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
        case GLUT_KEY_PAGE_UP:
            kernel += 1;
            cout << "Kernel: " << kernel << endl;
        break;
        case GLUT_KEY_PAGE_DOWN:
            kernel -= 1;
            cout << "Kernel: " << kernel << endl;
        break;
        case GLUT_KEY_UP:
            limiar += 10;
            cout << "Limiar: " << limiar << endl;
        break;
        case GLUT_KEY_DOWN:
            limiar -= 10;
            cout << "Limiar: " << limiar << endl;
        break;
        case GLUT_KEY_LEFT:
            NovaImagem->CopyTo(Image);
            cout << "NovaImagem copiada para Image" << endl;
        break;
        case GLUT_KEY_RIGHT:
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


