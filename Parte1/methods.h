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
    cout << media;
    cout << " - Calculo do histograma Finalizado.\n";
}

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

int MediaDosVizinhos(int x, int y)
{
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
//  void PassaBaixa()
//
//
// **********************************************************************
void PassaBaixa()
{
    // Tarefa 1:
    //  Mude o valor do LIMIAR para números pequenos como 5 ou 10

	unsigned char r,g,b;
    int x,y;
    int i;
    cout << "Iniciou Passa Baixa....";
    //NovaImagem->DrawPixel(20, 1,100,255,0,0 );

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            i = Image->GetPointIntensity(x,y);
			Image->ReadPixel(x,y,r,g,b);

            // printf("Intens Fora: %5d\n",i);
            int m = MediaDosVizinhos(x,y);
            NovaImagem->DrawPixel(x, y,m,m,m);

        }
    }
    cout << "Concluiu Passa Baixa.\n";

}

void Interval()
{
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
//  void ConvertBlackAndWhite()
//
//
// **********************************************************************
void ConvertBlackAndWhite()
{
    // Tarefa 1:
    //  Mude o valor do LIMIAR para números pequenos como 5 ou 10

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
// void DetectImageBorders()
//
//
// **********************************************************************
void DetectImageBorders()
{

    // varredura vertical

    // varredura horizontal

}

void AchaPinos(){
    int x,y;
    int i;
    cout << "Iniciou AchaPinos....";

    for(x=KERNEL_SIZE;x<Image->SizeX()-KERNEL_SIZE;x++)
    {
        for(y=KERNEL_SIZE;y<Image->SizeY()-KERNEL_SIZE;y++)
        {
            i = Image->GetPointIntensity(x,y);

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

void ExpandePinos(){
    unsigned char r,g,b;
    int x,y;
    int i;
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

int Magnitude(int x, int y, int masc){
    int soma = 0;
    for(int i =-KERNEL_SIZE; i<=KERNEL_SIZE;i++){
        for(int j =-KERNEL_SIZE; j<=KERNEL_SIZE;j++){
            soma = soma + (Image->GetPointIntensity(x+i,y+j) * masc);
        }
    }

    return soma;
}

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

            if (Magnitude(x,y,-2) < -6776){
                Image3->DrawPixel(x,y, 0,0,0);
            } else {
                Image3->DrawPixel(x,y, r,g,b);
            }
        }
    }

    Image3->CopyTo(NovaImagem);
    cout << "Concluiu limpaErrosDentina.\n";
}

