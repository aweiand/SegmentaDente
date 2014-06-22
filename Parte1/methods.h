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
//      Função que monta um array com o histograma da imagem
// **********************************************************************
void getHistograma(){
    int x,y;
    cout << "Iniciando calculo do histograma....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            histo[(int)Image->GetPointIntensity(x,y)] += 1;
        }
    }

    int pontoa = 0;
    int pontob = 0;
    for(x = 0; x <=255; x++){
        if (histo[x] >= pontoa){
            pontoa = x;
        } else if (histo[x] >= pontob){
            pontob = x;
        }
    }

    media = histo[(pontob - pontoa)];
    cout << media;
    cout << " - Calculo do histograma Finalizado.\n";
}

// **********************************************************************
//      Função que Lineariza o histograma da Imagem
//      deixando o linear no máximo a 55
// **********************************************************************
void linearHisto(){
    int x,y;
    int i;
    cout << "Iniciou Linear Histo....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            i = NovaImagem->GetPointIntensity(x,y);

            if (i > 55){
                Image3->DrawPixel(x, y, 255, 255, 255);
            } else {
                Image3->DrawPixel(x, y, i, i, i);
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
    cout << "Iniciou Aplica Mediana | kernel " << kernel_2 << endl;

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
int MediaDosVizinhos(int x, int y){
    int soma = 0;
    for(int i =-KERNEL_SIZE; i<=KERNEL_SIZE;i++){
        for(int j =-KERNEL_SIZE; j<=KERNEL_SIZE;j++){
            soma = soma + Image->GetPointIntensity(x+i,y+j);
        }
    }

    float larg = (KERNEL_SIZE*2)+1;
    return soma /(larg*larg);
}

// **********************************************************************
//      Função que aplica o filtro de passa baixa
// **********************************************************************
void PassaBaixa(){
	unsigned char r,g,b;
    int x,y;
    cout << "Iniciou Passa Baixa....";
    //NovaImagem->DrawPixel(20, 1,100,255,0,0 );

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
			Image->ReadPixel(x,y,r,g,b);

            // printf("Intens Fora: %5d\n",i);
            int m = MediaDosVizinhos(x,y);
            NovaImagem->DrawPixel(x, y,m,m,m);

        }
    }
    cout << "Concluiu Passa Baixa.\n";
}

// **********************************************************************
//      Função que limiariza o histograma da imagem de acordo
//      de acordo com os intervalor especificados
// **********************************************************************
void Interval(){
    int x,y;
    int i;
    cout << "Iniciou Interval....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            i = Image->GetPointIntensity(x,y);

            if (i < 50)
                NovaImagem->DrawPixel(x, y, 25, 25, 25);
            else if (i >= 50 && i < 100)
                NovaImagem->DrawPixel(x, y, 75, 75, 75);
            else if (i >= 100 && i < 200)
                NovaImagem->DrawPixel(x, y, 125, 125, 125);
            else if (i >= 200 && i <= 255)
                NovaImagem->DrawPixel(x, y, 200, 200, 200);
        }
    }
    cout << "Concluiu Interval.\n";

}

// **********************************************************************
//      Função que converte a imagem em preto e branco
// **********************************************************************
void ConvertBlackAndWhite(){
	unsigned char r,g,b;
    int x,y;
    int i;
    cout << "Iniciou Black & White....";
    //NovaImagem->DrawPixel(20, 1,100,255,0,0 );

    for(x=0;x<Image->SizeX();x++)
    {
        for(y=0;y<Image->SizeY();y++)
        {
            i = Image->GetPointIntensity(x,y);
			Image->ReadPixel(x,y,r,g,b);

            // printf("Intens Fora: %5d\n",i);
           if (i > 0 && i < 100)
            {
                NovaImagem->DrawPixel(x, y, 255,0,0);
            }
			else NovaImagem->DrawPixel(x, y, 0,0,0);
        }
    }
    cout << "Concluiu Black & White.\n";

}

// **********************************************************************
//      Função que encontra os pinos de acordo com a média
//      dos vizinhos dos pontos
// **********************************************************************
void AchaPinos(){
    int x,y;
    cout << "Iniciou AchaPinos....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            int m = MediaDosVizinhos(x,y);

            if (m > 100){
                Image3->DrawPixel(x, y,0,0,0);
            } else {
                Image3->DrawPixel(x, y, 255, 255, 255);
            }

        }
    }
    cout << "Concluiu AchaPinos.\n";
}

// **********************************************************************
//      Função que aplica a expansão nos pinos com máscara
//                                                  OXO
//                                                  OOO
// **********************************************************************
void ExpandePinos(){
    unsigned char r,g,b;
    int x,y;
    cout << "Iniciou Expande Pinos....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            NovaImagem->ReadPixel(x,y,r,g,b);

            if (b == 255){
                Image3->DrawPixel(x, y,0,0,255);

                for (int h=1; h<10;h++){
                    Image3->DrawPixel(x-h, y,0,0,255);
                    Image3->DrawPixel(x-h, y-h,0,0,255);

                    Image3->DrawPixel(x+h, y,0,0,255);
                    Image3->DrawPixel(x+h, y-h,0,0,255);
                    Image3->DrawPixel(x, y-h,0,0,255);
                }
            } else {
                Image3->DrawPixel(x, y,r,g,b);
            }
        }
    }
    Image3->CopyTo(Image);
    Image3->CopyTo(NovaImagem);
    cout << "Concluiu Expande Pinos.\n";
}

// **********************************************************************
//      Função que pinta os pinos de azul
// **********************************************************************
void ColorePinos(){
    int x,y;
    int i;
    cout << "Iniciou ColorePinos....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            unsigned char r,g,b;
            i = Image3->GetPointIntensity(x,y);

            if (i < 100){
                NovaImagem->DrawPixel(x, y, 0, 0, 255);
            } else {
                NovaImagem->ReadPixel(x,y,r,g,b);
                NovaImagem->DrawPixel(x, y, r, g, b);
            }
        }
    }
    cout << "Concluiu ColorePinos.\n";
}

// **********************************************************************
//      Função que pinta a dentina de verde
// **********************************************************************
void verDentina(int tam = 3){
    int x,y;
    cout << "Iniciou VerDentina....";

    NovaImagem->CopyTo(Image3);

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            float soma = 0;
            for(int i =0; i<=tam;i++){
                for(int j =0; j<=tam;j++){
                    if (i == 0 || i == 3){
                        soma = soma + NovaImagem->GetPointIntensity(x+i,y+j) * -2;
                    } else {
                        soma = soma + NovaImagem->GetPointIntensity(x+i,y+j) * 2;
                    }
                }
            }

            if (soma > NovaImagem->GetPointIntensity(x,y)){
                Image3->DrawPixel(x ,y , 255,0,0);
                Image3->DrawPixel(x+1 ,y , 255,0,0);
                Image3->DrawPixel(x+1 ,y+1 , 255,0,0);
                Image3->DrawPixel(x ,y-1 , 255,0,0);
                Image3->DrawPixel(x-1 ,y , 255,0,0);
                Image3->DrawPixel(x-1 ,y-1 , 255,0,0);
            }
        }
    }
    cout << "Concluiu VerDentina.\n";
}

// **********************************************************************
//      Função que remove os pontos "perdidos" do fundo
// **********************************************************************
void LimpaFundo(){
    unsigned char r,g,b;
    int x,y;
    cout << "Iniciou LimpaFundo....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            NovaImagem->ReadPixel(x,y,r,g,b);

            int m = MediaDosVizinhos(x,y);

            if (m > 10 && m < 45 && b != 255){
                Image3->DrawPixel(x, y,0,0,0);
            } else {
                Image3->DrawPixel(x, y, r, g, b);
            }
        }
    }

    Image3->CopyTo(NovaImagem);
    cout << "Concluiu LimpaFundo.\n";
}

// **********************************************************************
//      Função para encontrar a dentina
// **********************************************************************
void AchaDentina(){
    unsigned char r,g,b;
    int x,y;
    cout << "Iniciou AchaDentina....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++){
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++){
            NovaImagem->ReadPixel(x,y,r,g,b);

            int m = MediaDosVizinhos(x,y);

            if (m > 10 && b != 255){
                Image3->DrawPixel(x, y,0,255,0);
            } else {
                Image3->DrawPixel(x, y, r, g, b);
            }
        }
    }

    Image3->CopyTo(NovaImagem);
    cout << "Concluiu AchaDentina.\n";
}

// **********************************************************************
//      Função que calcula a magnitude da imagem
// **********************************************************************
int Magnitude(int x, int y, int masc){
    int soma = 0;
    for(int i =-KERNEL_SIZE; i<=KERNEL_SIZE;i++){
        for(int j =-KERNEL_SIZE; j<=KERNEL_SIZE;j++){
            soma = soma + (Image->GetPointIntensity(x+i,y+j) * masc);
        }
    }

    return soma;
}

// **********************************************************************
//      Função que limpa os erros da dentina
// **********************************************************************
void limpaErrosDentina(){
    unsigned char r,g,b;
    int x,y, quant = 0;
    cout << "Iniciou limpaErrosDentina...." << endl;

    for(x=0;x<NovaImagem->SizeX();x++){
        for(y=0;y<NovaImagem->SizeY();y++){
            NovaImagem->ReadPixel(x,y,r,g,b);

            if (g == 255 && b == 0 && r == 0){
                while(g == 255 && b == 0 && r == 0){
                    quant++;
                    y++;
                    NovaImagem->ReadPixel(x,y,r,g,b);
                }

                if (quant <= kernel_mediana){
                    for(int i=y-quant; i<=y; i++){
                        Image3->DrawPixel(x,i, 0,0,0);
                    }
                } else {
                    quant = 0;
                    y = y-quant;
                }
            } else {
                Image3->DrawPixel(x,y, r,g,b);
            }
        }
    }

    Image3->CopyTo(NovaImagem);
    cout << "Concluiu limpaErrosDentina.\n";
}

// **********************************************************************
//      Função que acha os canais e os pinta de vermelho
// **********************************************************************
void achaCanal(){
    unsigned char r,g,b;
    int x,y;
    int borders = 100;
    cout << "Iniciou Acha Canal...." << endl;

    for(x=borders;x<NovaImagem->SizeX()-borders;x++){
        for(y=borders;y<NovaImagem->SizeY()-borders;y++){
            NovaImagem->ReadPixel(x,y,r,g,b);

            if (g == 255 && b == 0 && r == 0){
                NovaImagem->ReadPixel(x,y+1,r,g,b);
                if (g == 0 && b == 0 && r == 0){
                    int conta = 0;
                    while(g == 0 && b == 0 && r == 0){
                        conta++;
                        y++;
                        NovaImagem->ReadPixel(x,y,r,g,b);
                    }

                    NovaImagem->ReadPixel(x,y++,r,g,b);
                    if (conta > 18 && conta < 190 && g == 255){
                        for(int i=y-conta; i<=y; i++){
                            Image3->DrawPixel(x,i, 255,0,0);
                        }
                    }
                }
            } else {
                Image3->DrawPixel(x,y, r,g,b);
            }
        }
    }

    Image3->CopyTo(NovaImagem);

    for(x=borders;x<NovaImagem->SizeX()-borders;x++){
        for(y=borders;y<NovaImagem->SizeY()-borders;y++){
            NovaImagem->ReadPixel(x,y,r,g,b);

            if (g == 0 && b == 0 && r == 255){
                int conta = 0;
                while(g == 0 && b == 0 && r == 255){
                    conta++;
                    x++;
                    NovaImagem->ReadPixel(x,y,r,g,b);
                }

                if (conta <= 3 || conta > 80){
                    for(int i=x-conta; i<=x; i++){
                        Image3->DrawPixel(i,y, 0,0,0);
                    }
                }
                x -= conta;
            } else {
                Image3->DrawPixel(x,y, r,g,b);
            }
        }
    }

    Image3->CopyTo(NovaImagem);
    cout << "Concluiu Acha Canal.\n";
}
