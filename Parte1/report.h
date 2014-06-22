// **********************************************************************
// PUCRS/FACIN
// COMPUTAÇÃO GRÁFICA
//
//	report.h
//  Programa de segmentação de Imagens de Micro Tomografia
//  de dentes com algoritmos de segmentação por linear.
//
//	Augusto Weiand <guto.weiand@gmail.com>
// **********************************************************************

// **********************************************************************
//      Função que abre o arquivo para o relatorio
// **********************************************************************
void openFile(){
    fp = fopen(".\\dataREPORT\\report.txt", "a+");
    cout << "Arquivo report.txt Criado/Aberto..." << endl;
}

void writeReport(char data[1024]){
    fprintf(fp, data);
    fprintf(fp, "\n");
}

void endFile(){
    fclose(fp);
}

void report(char groundfile[1024], char filename[1024]){
    openFile();
    unsigned char r,g,b;
    int x,y;
    int rcont = 0,gcont = 0,bcont = 0, pcont = 0;
    int grcont = 0,ggcont = 0, gbcont = 0, gpcont = 0;
    cout << "Iniciou Report...." << endl;

    for(x=0;x<NovaImagem->SizeX();x++){
        for(y=0;y<NovaImagem->SizeY();y++){
            NovaImagem->ReadPixel(x,y,r,g,b);

            if (r == 0 && g == 0 && b == 255)
                bcont++;
            else if (r == 0 && g == 255 && b == 0)
                gcont++;
            else if (r == 255 && g == 0 && b == 0)
                rcont++;
            else if (r == 0 && g == 0 && b == 0)
                pcont++;
        }
    }

    Image3->Delete();
    Image3->Load(strcat(groundfile, filename));

    for(x=0;x<Image3->SizeX();x++){
        for(y=0;y<Image3->SizeY();y++){
            Image3->ReadPixel(x,y,r,g,b);

            if (r == 0 && g == 0 && b == 255)
                gbcont++;
            else if (r == 0 && g == 255 && b == 0)
                ggcont++;
            else if (r == 255 && g == 0 && b == 0)
                grcont++;
            else if (r == 0 && g == 0 && b == 0)
                gpcont++;
        }
    }

    int tot = Image3->SizeY()*Image3->SizeX();

    fprintf(fp, "Imagem Tratada: %s\n", filename);
    fprintf(fp, "Imagem Tratada | Imagem Ground\n");
    fprintf(fp, "Cor: Porcentagem inteira (total de pixels) | Porcentagem inteira (total de pixels)\n");
    fprintf(fp, "R: %d%% (%d) | %d%% (%d)\n", ((rcont * 100) / tot), rcont, ((grcont * 100) / tot), grcont);
    fprintf(fp, "G: %d%% (%d) | %d%% (%d)\n", ((gcont * 100) / tot), gcont, ((ggcont * 100) / tot), ggcont);
    fprintf(fp, "B: %d%% (%d) | %d%% (%d)\n", ((bcont * 100) / tot), bcont, ((gbcont * 100) / tot), gbcont);
    fprintf(fp, "Preto: %d%% (%d) | %d%% (%d)\n\n", ((pcont * 100) / tot), pcont, ((gpcont * 100) / tot), gpcont);
    fprintf(fp, "-------------------\n");

    cout << "Finalizou Report...." << endl;
    endFile();
}
