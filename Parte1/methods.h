// **********************************************************************
//      Função que monta um array com o histograma da imagem
// **********************************************************************
void getHistograma(){
    int x,y;
    int i;
    cout << "Iniciando calculo do histograma....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            histo[(int)Image->GetPointIntensity(x,y)] += 1;
        }
    }

    int pontoa = 0, valora = 0;
    int pontob = 0, valorb = 0;
    for(x = 0; x <=255; x++){
        if (histo[x] >= pontoa){
            pontoa = x;
            valora = histo[x];
        } else if (histo[x] >= pontob){
            pontob = x;
            valorb = histo[x];
        }
    }

    media = histo[(pontob - pontoa)];
    cout << "Calculo do histograma Finalizado.\n";
}

// **********************************************************************
//      Função que Lineariza o histograma da Imagem
//      deixando o linear no máximo a 55
// **********************************************************************
void linearHisto(){
	unsigned char r,g,b;
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
    int soma = 0, i = 0;
    int ker = (kernel-1)/2;
    int arr[(kernel*kernel)];

    for(int i =-ker; i<=ker;i++){
        for(int j =-ker; j<=ker;j++){
            if (x+i > 0 && y+i > 0 && x+i < img->SizeX() && y+i < img->SizeY()){
                arr[i] = img->GetPointIntensity(x+i,y+j);
                i++;
            }
        }
    }

    qsort(arr, kernel*kernel, sizeof(int), cmpfunc);

    return arr[(ker+1)];
}

// **********************************************************************
//      Função que aplica a Mediana
// **********************************************************************
void AplicaMediana(ImageClass *from, ImageClass *to){
	unsigned char r,g,b;
    int x,y;
    int i;
    cout << "Iniciou Aplica Mediana....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
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
int MediaDosVizinhos(int x, int y, ImageClass *img){
    int soma = 0;
    for(int i =-KERNEL_SIZE; i<=KERNEL_SIZE;i++){
        for(int j =-KERNEL_SIZE; j<=KERNEL_SIZE;j++){
            soma = soma + img->GetPointIntensity(x+i,y+j);
        }
    }

    float larg = (KERNEL_SIZE*2)+1;
    return soma /(larg*larg);
}

// **********************************************************************
//      Função que aplica o filtro de passa baixa
// **********************************************************************
void PassaBaixa(ImageClass *from, ImageClass *to){
    int x,y;
    cout << "Iniciou Passa Baixa....";

    for(x=KERNEL_SIZE;x<from->SizeX()-KERNEL_SIZE;x++){
        for(y=KERNEL_SIZE;y<from->SizeY()-KERNEL_SIZE;y++){
            int m = MediaDosVizinhos(x,y, from);
            to->DrawPixel(x, y, m,m,m);
        }
    }
    cout << "Concluiu Passa Baixa.\n";
}

// **********************************************************************
//      Função que limiariza o histograma da imagem de acordo
//      de acordo com os intervalor especificados
// **********************************************************************
void Interval(ImageClass *from, ImageClass *to)
{
    int x,y;
    int i;
    cout << "Iniciou Interval....";

    for(x=KERNEL_SIZE;x<from->SizeX()-KERNEL_SIZE;x++){
        for(y=KERNEL_SIZE;y<from->SizeY()-KERNEL_SIZE;y++){
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
void ConvertBlackAndWhite(ImageClass *from, ImageClass *to){
    int x,y,i;
    cout << "Iniciou Black & White....";

    for(x=0;x<from->SizeX();x++){
        for(y=0;y<from->SizeY();y++){
            i = from->GetPointIntensity(x,y);

           if (i > 0 && i < 100)
                to->DrawPixel(x, y, 255,0,0);
			else
                to->DrawPixel(x, y, 0,0,0);
        }
    }
    cout << "Concluiu Black & White.\n";
}

// **********************************************************************
//      Função que encontra os pinos de acordo com a média
//      dos vizinhos dos pontos
// **********************************************************************
void AchaPinos(ImageClass *from, ImageClass *to){
    int x,y;
    int i;
    cout << "Iniciou AchaPinos....";

    for(x=KERNEL_SIZE;x<from->SizeX()-KERNEL_SIZE;x++){
        for(y=KERNEL_SIZE;y<from->SizeY()-KERNEL_SIZE;y++){
            i = from->GetPointIntensity(x,y);

            int m = MediaDosVizinhos(x,y, from);

            if (m > 100)
                to->DrawPixel(x, y,0,0,0);
            else
                to->DrawPixel(x, y, 255, 255, 255);
        }
    }
    cout << "Concluiu AchaPinos.\n";
}

// **********************************************************************
//      Função que aplica a expansão nos pinos com máscara
//                                                  OXO
//                                                  OOO
// **********************************************************************
void ExpandePinos(ImageClass *from, ImageClass *to){
    unsigned char r,g,b;
    int x,y;
    int i;
    cout << "Iniciou Expande Pinos....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++){
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++){
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
            i = Image3->GetPointIntensity(x,y);

            if (i < 100){
                NovaImagem->DrawPixel(x, y, 0, 0, 255);
            }
        }
    }
    cout << "Concluiu ColorePinos.\n";
}

// **********************************************************************
//      Função que pinta a dentina de verde
// **********************************************************************
void verDentina(int tam = 3){
    unsigned char r,g,b;
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
    int i;
    cout << "Iniciou LimpaFundo....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            NovaImagem->ReadPixel(x,y,r,g,b);
            i = Image->GetPointIntensity(x,y);

            int m = MediaDosVizinhos(x,y, NovaImagem);

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
    int i;
    cout << "Iniciou AchaDentina....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            NovaImagem->ReadPixel(x,y,r,g,b);
            i = Image->GetPointIntensity(x,y);

            int m = MediaDosVizinhos(x,y, NovaImagem);

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
int Magnitude(int x, int y, int masc, ImageClass *img){
    int soma = 0;
    for(int i =-KERNEL_SIZE; i<=KERNEL_SIZE;i++){
        for(int j =-KERNEL_SIZE; j<=KERNEL_SIZE;j++){
            soma = soma + (img->GetPointIntensity(x+i,y+j) * masc);
        }
    }

    return soma;
}

// **********************************************************************
//      Função que limpa os erros da dentina
// **********************************************************************
void limpaErrosDentina(){
    unsigned char r,g,b;
    int x,y;
    int i;
    cout << "Iniciou limpaErrosDentina....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            Image3->ReadPixel(x,y,r,g,b);
            i = Image3->GetPointIntensity(x,y);

            if (Magnitude(x,y,-2, Image) < -6776){
                Image3->DrawPixel(x,y, 0,0,0);
            } else {
                Image3->DrawPixel(x,y, r,g,b);
            }
        }
    }

    Image3->CopyTo(NovaImagem);
    cout << "Concluiu limpaErrosDentina.\n";
}

