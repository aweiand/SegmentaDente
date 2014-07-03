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

// **********************************************************************
//      Função
// **********************************************************************
void _createMCO(int dx, int dy, ImageClass *from){
    int cont = 0;
    int x,y;
    cout << "Iniciou Create MCO... ";

    for(x=0;x<from->SizeX();x++){
        for(y=0;y<from->SizeY();y++){
            int p1 = from->GetPointIntensity(x,y);
            if (x+dx > 0 && y-dy > 0 && x+dx < from->SizeX() && y-dy < from->SizeY()){
                int p2 = from->GetPointIntensity((x + dx), (y - dy));

                mco[p1][p2]++;
                mco[p2][p1]++;
                cont++;
            }
        }
    }

    /* Esta etapa acaba com a matriz... :(
    if (cont > 0){
        for(int i=0; i<=255; i++){
            for(int j=0; j<=255; j++){
                if (mco[i][j] > 0 ){
                    mco[i][j] /= cont;
                }
            }
        }
    }
    */
    /* gerador de relatorio, caso necessario */
    fp = fopen(".\\dataREPORT\\mco.txt", "w+");
    for(int i=0; i<=255; i++){
        for(int j=0; j<=255; j++){
            //cout << mco[i][j] << " ";
            fprintf(fp, "%d ", mco[i][j]);
        }
        //cout << endl;
        fprintf(fp, "\n");
    }
    fclose(fp);

    cout << "Concluiu Create MCO.\n";
}

int getEnergy(){
    int y = 0;

    for(int i=0; i<=255; i++){
        for(int j=0; j<=255; j++){
            if (mco[i][j] > 0 ){
                y += mco[i][j] * mco[i][j];
            }
        }
    }

    return y;
}

int getEntropy(){
    float y = 0;

    for(int i=0; i<=255; i++){
        for(int j=0; j<=255; j++){
            if (mco[i][j] > 0 ){
                y += mco[i][j] * log2(mco[i][j]);
            }
        }
    }

    return -y;
}

int getContrast(){
    int y = 0;

    for(int i=0; i<=255; i++){
        for(int j=0; j<=255; j++){
            if (mco[i][j] > 0 ){
                y += (i - j) * (i - j) * mco[i][j];
            }
        }
    }

    return y;
}

int getHomogen(){
    int y = 0;

    for(int i=0; i<=255; i++){
        for(int j=0; j<=255; j++){
            if (mco[i][j] > 0 ){
                y += mco[i][j] / (1 + (i - j) * (i - j));
            }
        }
    }

    return y;
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
                to->DrawPixel(x, y, 255, 255, 255);
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

// **********************************************************************
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
            else if (i >= 200 && i <= 255)
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
                NovaImagem->DrawPixel(x, y, 255,255,255);
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
