// **********************************************************************
// PUCRS/FACIN
// COMPUTAÇÃO GRÁFICA
//
//	methods.h
//  Programa de segmentação de Imagens de Micro Tomografia
//  de dentes com algoritmos de segmentação por linear.
//
//	Augusto Weiand <guto.weiand@gmail.com>
// **********************************************************************
void printStatus(int i){
    if (i == 0 || i == 500 || i == 1000 || i == 1500 || i == 2000)
        cout << "\n------ I: " << i << " ------\n";
}

void normalizaMCO(float mco2[TAM][TAM]){
    float normal = 0;

    for(int i=0;i<TAM;i++)
        for(int j=0;j<TAM;j++){
            if (mco2[i][j] > 0 )
                normal += mco2[i][j];
        }

    for(int i=0; i<=TAM; i++)
        for(int j=0; j<=TAM; j++){
            if (mco2[i][j] > 0 ){
                mco2[i][j] = mco2[i][j] / normal;
            }
        }
}

float getEnergy(float mco2[TAM][TAM]){
    int y = 0;

    for(int i=0; i<=TAM; i++){
        for(int j=0; j<=TAM; j++){
            if (mco2[i][j] > 0 ){
                y += mco2[i][j] * mco2[i][j];
            }
        }
    }

    return y;
}

float getEntropy(float mco2[TAM][TAM]){
    float y = 0;

    for(int i=0; i<=TAM; i++){
        for(int j=0; j<=TAM; j++){
            if (mco2[i][j] > 0 ){
                y += mco2[i][j] * log2(mco2[i][j]);
            }
        }
    }

    return -y;
}

float getContrast(float mco2[TAM][TAM]){
    int y = 0;

    for(int i=0; i<=TAM; i++){
        for(int j=0; j<=TAM; j++){
            if (mco2[i][j] > 0 ){
                y += (i - j) * (i - j) * mco2[i][j];
            }
        }
    }

    return y;
}

float getHomogen(float mco2[TAM][TAM]){
    int y = 0;

    for(int i=0; i<=TAM; i++){
        for(int j=0; j<=TAM; j++){
            if (mco2[i][j] > 0 ){
                y += mco2[i][j] / (1 + (i - j) * (i - j));
            }
        }
    }

    return y;
}

int getSegundoMomentoAngular(int mco2[TAM][TAM]){
    float segundoMomentoAngular = 0;
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            if(mco2[i][j] > 0) segundoMomentoAngular += pow(mco2[j][i],2);

    return segundoMomentoAngular;
}

// **********************************************************************
//      Função para criar a matriz com a orientação e
//       direção fornecidas em dx e dy
// **********************************************************************
void _createMCO(int dx, int dy, int x, int y, int tamanho, ImageClass *from, float mco2[TAM][TAM]){
    //cout << "Iniciou Create MCO... ";

    // For para percorrer toda a imagem no X e Y
    for(int i=0;i<tamanho;i++){
        for(int j=0;j<tamanho;j++){
            int coordX = x+i;
            int coordY = y+j;

            // Verifico se não está fora da janela
            if ((coordX+dx) > 0 && (coordX+dx < (from->SizeX()-tamanho)) &&
                (coordY-dy) > 0 && (coordY-dy < (from->SizeY()-tamanho))){

                int p1 = from->GetPointIntensity(coordX, coordY);
                int p2 = from->GetPointIntensity((coordX + dx), (coordY - dy));

                if (p1 <= TAM && p2 <= TAM){
                    mco2[(int)p1][(int)p2]++;
                    mco2[(int)p2][(int)p1]++;
                }
            }
        }
    }

    /* gerador de relatorio, caso necessario
    fp = fopen(".\\dataREPORT\\mco.txt", "w+");
    for(int i=0; i<=TAM; i++){
        for(int j=0; j<=TAM; j++){
            //cout << mco[i][j] << " ";
            fprintf(fp, "%d ", mco2[i][j]);
        }
        //cout << endl;
        fprintf(fp, "\n");
    }

    fprintf(fp, "\n\n -------------\n");
    fprintf(fp, "Energia: %d\n", getEnergy(mco2));
    fprintf(fp, "Entropy: %d\n", getEntropy(mco2));
    fprintf(fp, "Contrast: %d\n", getContrast(mco2));
    fprintf(fp, "Homogen: %d\n", getHomogen(mco2));
    fclose(fp);
    */

    //cout << "Concluiu Create MCO.\n";
}

// **********************************************************************
//      Função que Lineariza o histograma da Imagem
//      deixando o linear no máximo a 55
// **********************************************************************
void linearHisto(ImageClass *from, ImageClass *to){
    int x,y;
    int i;
    cout << "Iniciou Linear Histo....";

    for(x=0;x<from->SizeX();x++){
        for(y=0;y<from->SizeY();y++){
            i = from->GetPointIntensity(x,y);

            if (i > 55){
                to->DrawPixel(x, y, TAM, TAM, TAM);
            } else {
                to->DrawPixel(x, y, i, i, i);
            }
        }
    }
    cout << "Concluiu Linear Histo.\n";
}

// **********************************************************************
//      Função de comparação para ordenar um array
// **********************************************************************
int cmpfunc(const void * a, const void * b){
   return ( *(int*)a - *(int*)b );
}

// **********************************************************************
//      Função de cálculo de Mediana
// **********************************************************************
int Mediana(int x, int y, int kernel, ImageClass *img){
    int h = 0;
    int ker = (kernel-1)/2;
    int arr[(kernel*kernel)];

    for(int i =-ker; i<=ker;i++){
        for(int j =-ker; j<=ker;j++){
            if (x+i > 0 && y+i > 0 && x+i < img->SizeX() && y+i < img->SizeY()){
                arr[h] = img->GetPointIntensity(x+i,y+j);
                h++;
            }
        }
    }

    qsort(arr, kernel*kernel, sizeof(int), cmpfunc);

    return arr[(ker+1)];
}

// **********************************************************************
//      Função que aplica a Mediana
// **********************************************************************
void AplicaMediana(ImageClass *from, ImageClass *to, int kernel_2 = 3){
    int x,y;
    cout << "Iniciou Aplica Mediana | Kernel: " << kernel_2 << "... ";

    for(x=kernel_2;x<from->SizeX()-kernel_2;x++){
        for(y=kernel_2;y<from->SizeY()-kernel_2;y++){
            int m = Mediana(x,y, kernel_2, from);
            to->DrawPixel(x, y,m,m,m);
        }
    }
    cout << "Concluiu Aplica Mediana.\n";
}

// **********************************************************************
//      Função que calcula a média dos vizinhos dos pontos
//      de acordo com o KERNEL_SIZE
// **********************************************************************
int MediaDosVizinhos(int x, int y, ImageClass *from, int kernel_2 = 2){
    int soma = 0;
    int tam = (kernel_2-1)/2;

    for(int i=-tam; i<=tam;i++){
        for(int j=-tam; j<=tam;j++){
            if ((x+i > 0 && x+i < from->SizeX()) &&
                (y+j > 0 && y+j < from->SizeY())){
                soma = soma + from->GetPointIntensity(x+i,y+j);
            }
        }
    }

    return soma / (kernel_2*kernel_2);
}

// **********************************************************************
//      Função que aplica o filtro de passa baixa
// **********************************************************************
void PassaBaixa(ImageClass *from, ImageClass *to){
	unsigned char r,g,b;
    int x,y;
    cout << "Iniciou Passa Baixa....";

    for(x=0;x<from->SizeX();x++){
        for(y=0;y<from->SizeY();y++){
			from->ReadPixel(x,y,r,g,b);

            // printf("Intens Fora: %5d\n",i);
            int m = MediaDosVizinhos(x,y, from, kernel);
            to->DrawPixel(x, y,m,m,m);

        }
    }
    cout << "Concluiu Passa Baixa.\n";
}

// *******************************************_createMCO(1, 1, 580, 660, 100, from, mco);***************************
//      Função que limiariza o histograma da imagem de acordo
//      de acordo com os intervalor especificados
// **********************************************************************
void Interval(ImageClass *from, ImageClass *to){
    int x,y;
    int i;
    cout << "Iniciou Interval....";

    for(x=0;x<from->SizeX();x++){
        for(y=0;y<from->SizeY();y++){
            i = from->GetPointIntensity(x,y);

            if (i < 50)
                to->DrawPixel(x, y, 25, 25, 25);
            else if (i >= 50 && i < 100)
                to->DrawPixel(x, y, 75, 75, 75);
            else if (i >= 100 && i < 200)
                to->DrawPixel(x, y, 125, 125, 125);
            else if (i > 200)
                to->DrawPixel(x, y, 200, 200, 200);
        }
    }

    cout << "Concluiu Interval.\n";
}

// **********************************************************************
//      Função que converte a imagem em preto e branco
// **********************************************************************
void ConvertBlackAndWhite(ImageClass *from, ImageClass *to, int limiar = 100){
    int x,y;
    int i;
    cout << "Iniciou Black & White....";

    for(x=0;x<from->SizeX();x++){
        for(y=0;y<from->SizeY();y++){
            i = from->GetPointIntensity(x,y);

            if (i > 0 && i < limiar){
                NovaImagem->DrawPixel(x, y, TAM,TAM,TAM);
            } else
                NovaImagem->DrawPixel(x, y, 0,0,0);
        }
    }

    cout << "Concluiu Black & White.\n";
}

void texturaFundo(ImageClass *from, ImageClass *to, int kernel_2 = 7){
    unsigned char r,g,b;
    int x,y;
    cout << "Iniciou TexturaFundo....";

    for(x=0;x<from->SizeX();x++){
        for(y=0;y<from->SizeY();y++){

            int i = MediaDosVizinhos(x,y, from, kernel_2);

            if (i >= 0 && i <= limiar){
                to->DrawPixel(x, y, 0, 0, 0);
            } else {
                from->ReadPixel(x,y,r,g,b);
                to->DrawPixel(x, y, r,g,b);
            }
        }
    }

    cout << "Concluiu TexturaFundo.\n";
}

// Kernel == 7 fica tri bom!!
void texturaPinos(ImageClass *from, ImageClass *to, int kernel_2 = 7){
    unsigned char r,g,b;
    int x,y;
    cout << "Iniciou TexturaPinos....";

    for(x=0;x<from->SizeX();x++){
        for(y=0;y<from->SizeY();y++){

            int i = MediaDosVizinhos(x,y, from, kernel_2);

            if (i >= 0 && i <= limiar){
                to->DrawPixel(x, y, 0, 0, 0);
            } else {
                from->ReadPixel(x,y,r,g,b);
                to->DrawPixel(x, y, r,g,b);
            }
        }
    }

    cout << "Concluiu TexturaPinos.\n";
}

void texturaDentina(ImageClass *from, ImageClass *to){
    unsigned char r,g,b;
    int x = 0,y = 0;
    float limiar = 0.8;
    float limiar2 = 1.2;
    cout << "Iniciou TexturaDentina....";

    _createMCO(10, 0, 380, 470, 15, from, mco);
    // normalizaMCO(mco);

    float infos[] = { getEnergy(mco), -getEntropy(mco), getContrast(mco), getHomogen(mco) };
    cout << endl << "MCO Criada...\n";
    cout << "Eneriga: " << infos[0] << endl;
    cout << "Entropia: " << infos[1] << endl;
    cout << "Constraste: " << infos[2] << endl;
    cout << "Homog: " << infos[3] << endl;

    for(x=0;x<from->SizeX()/2-1;x+=counter){
        printStatus(x);
        // cout << endl << "--- X: " << x << endl;
        for(y=0;y<from->SizeY()/4-1;y+=counter){
            float mco2[TAM][TAM];
            _createMCO(10, 0, x, y, 15, from, mco2);
            // normalizaMCO(mco2);
            // cout << "Y: " << y << endl;

            int infos2[] = { getEnergy(mco2), -getEntropy(mco2), getContrast(mco2), getHomogen(mco2) };

            if (
                (infos[0] >= (infos2[0] * limiar) && infos[0] <= (infos2[0] * limiar2))
                //(infos[1] >= (infos2[1] * limiar) && infos[1] <= (infos2[1] * limiar2))
                //(infos[2] >= (infos2[2] * limiar) && infos2[2] <= (infos2[2] * limiar2))
                //(infos[3] >= (infos2[3] * limiar) && infos2[3] <= (infos2[3] * limiar2))
                ){
                for(int x2=x;x2<x+counter;x2++){
                    if (x2 <= to->SizeX()){
                        for(int y2=y;y2<y+counter;y2++){
                            if (y2 <= to->SizeY())
                                to->DrawPixel(x2, y2, 255, 0, 0);
                        }
                    }
                }
            } else {
                // from->ReadPixel(x,y,r,g,b);
                // to->DrawPixel(x, y, r,g,b);
                for(int x2=x;x2<x+counter;x2++){
                    if (x2 <= to->SizeX() && x2 <= from->SizeX()){
                        for(int y2=y;y2<y+counter;y2++){
                            if (y2 <= to->SizeY() && y2 <= from->SizeY()){
                                //unsigned char r,g,b;
                                //from->ReadPixel(x2,y2,r,g,b);
                                to->DrawPixel(x2, y2, 0,0,0);
                            }
                        }
                    }
                }
            }
            free(mco2);
            free(infos2);
        }
    }

    cout << "Concluiu TexturaDentina.\n";
}



