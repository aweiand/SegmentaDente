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

// **********************************************************************
//      Função que imprime um char no arquivo do relatorio
// **********************************************************************
void writeReport(char data[1024]){
    fprintf(fp, data);
    fprintf(fp, "\n");
}

// **********************************************************************
//      Função que fecha o arquivo do relatorio
// **********************************************************************
void endFile(){
    fclose(fp);
}

// **********************************************************************
//      Função que cria o relatorio de acordo com a imagem
//      original e o ground truth, devidamente localizados
//      nas pastas do sistema, preenche o arquivo do relatorio
//      com os dados necessarios.
// **********************************************************************
void report(char groundfile[1024], char filename[1024], ImageClass *fromimg, ImageClass *groundimg){
    openFile();
    unsigned char r,g,b;
    int x,y;
    int rcont = 0,gcont = 0,bcont = 0, pcont = 0;
    int grcont = 0,ggcont = 0, gbcont = 0, gpcont = 0;
    cout << "Iniciou Report...." << endl;

    for(x=0;x<fromimg->SizeX();x++){
        for(y=0;y<fromimg->SizeY();y++){
            fromimg->ReadPixel(x,y,r,g,b);

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

    groundimg->Delete();
    groundimg->Load(strcat(groundfile, filename));

    for(x=0;x<groundimg->SizeX();x++){
        for(y=0;y<groundimg->SizeY();y++){
            groundimg->ReadPixel(x,y,r,g,b);

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

    fprintf(fp, "%s;", filename);
    fprintf(fp, "%d%% (%d);", (rcont * 100) / grcont);
    fprintf(fp, "%d%% (%d);", (gcont * 100) / ggcont);
    fprintf(fp, "%d%% (%d);", (bcont * 100) / gbcont);
    fprintf(fp, "%d%% (%d)\n",(pcont * 100) / gpcont);

    cout << "Finalizou Report...." << endl;
    endFile();
}
