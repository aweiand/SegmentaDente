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
//      Função para criar a matriz com a orientação e
//       direção fornecidas em dx e dy
// **********************************************************************
void _createMCOCanalTruth(int dx, int dy, ImageClass *from, ImageClass *ground){
    unsigned char r,g,b;
    int normal = 0;
    ImageClass *out;
    out = new ImageClass(TAM, TAM, Image->Channels());

    for(int i=0; i<TAM; i++)
        for(int j=0; j<TAM; j++)
            canal[i][j] = 0;

    // For para percorrer toda a imagem no X e Y
    for(int x=0;x<ground->SizeX();x++){
        for(int y=0;y<ground->SizeY();y++){
            if ((x+dx) > 0 && (x+dx < (from->SizeX())) &&
                (y-dy) > 0 && (y-dy < (from->SizeY()))){
                ground->ReadPixel(x,y,r,g,b);

                if (g == 0 && r == 255 && b == 0){
                    int p1 = from->GetPointIntensity(x, y);
                    int p2 = from->GetPointIntensity((x + dx), (y - dy));

                    if (p1 < TAM && p2 < TAM){
                        canal[(int)p1][(int)p2]++;
                        canal[(int)p2][(int)p1]++;

                        normal+=2;
                    }
                }
            }
        }
    }

    for(int i=0; i<TAM; i++)
        for(int j=0; j<TAM; j++){
            if (canal[i][j] > 0 ){
                out->DrawPixel(i,j, canal[i][j], canal[i][j], canal[i][j]);
                canal[i][j] = canal[i][j] / normal;
            } else {
                out->DrawPixel(i,j, 255,255,255);
            }
        }

    _infosCanal[0] = 0;
    _infosCanal[1] = 0;
    _infosCanal[2] = 0;
    _infosCanal[3] = 0;
    for(int i=0; i<TAM; i++){
        for(int j=0; j<TAM; j++){
            if (canal[i][j] > 0 ){
                // Homogeneidade
                _infosCanal[0] += canal[i][j] / (1 + (i - j) * (i - j));
                // Contraste
                _infosCanal[1] += (i - j) * (i - j) * canal[i][j];
                // Entropia
                _infosCanal[2] += canal[i][j] * log2(canal[i][j]);
                // Energia
                _infosCanal[3] += canal[i][j] * canal[i][j];
            }
        }
    }

    /* gerador de relatorio, caso necessario */
    out->Save(".\\dataREPORT\\mcoCanal.png");
    out->Delete();
    fp = fopen(".\\dataREPORT\\mco_canal.txt", "w+");
    for(int i=0; i<TAM; i++){
        for(int j=0; j<TAM; j++){
            fprintf(fp, "%f;", canal[i][j]);
        }
        fprintf(fp, "\n");
    }

    fprintf(fp, "\n\n -------------\n");
    fprintf(fp, "Homogeneidade: %f\n", _infosCanal[0]);
    fprintf(fp, "Contraste: %f\n", _infosCanal[1]);
    fprintf(fp, "Entropia: %f\n", _infosCanal[2]);
    fprintf(fp, "Energia: %f\n", _infosCanal[3]);
    fclose(fp);

    //cout << "Concluiu Create MCO.\n";
}

// **********************************************************************
//      Função para criar a matriz com a orientação e
//       direção fornecidas em dx e dy
// **********************************************************************
void _createMCODentinaTruth(int dx, int dy, ImageClass *from, ImageClass *ground){
    unsigned char r,g,b;
    int normal = 0;
    ImageClass *out;
    out = new ImageClass(TAM, TAM, Image->Channels());

    for(int i=0; i<TAM; i++)
        for(int j=0; j<TAM; j++)
            dentina[i][j] = 0;

    // For para percorrer toda a imagem no X e Y
    for(int x=0;x<ground->SizeX();x++){
        for(int y=0;y<ground->SizeY();y++){
            if ((x+dx) > 0 && (x+dx < (from->SizeX())) &&
                (y-dy) > 0 && (y-dy < (from->SizeY()))){
                ground->ReadPixel(x,y,r,g,b);

                if (g == 255 && r == 0 && b == 0){
                    int p1 = from->GetPointIntensity(x, y);
                    int p2 = from->GetPointIntensity((x + dx), (y - dy));

                    if (p1 < TAM && p2 < TAM){
                        dentina[(int)p1][(int)p2]++;
                        dentina[(int)p2][(int)p1]++;

                        normal+=2;
                    }
                }
            }
        }
    }

    _infos[0] = 0;
    _infos[1] = 0;
    _infos[2] = 0;
    _infos[3] = 0;
    for(int i=0; i<TAM; i++)
        for(int j=0; j<TAM; j++){
            if (dentina[i][j] > 0 ){
                out->DrawPixel(i,j, dentina[i][j], dentina[i][j], dentina[i][j]);
                dentina[i][j] = dentina[i][j] / normal;
            } else {
                out->DrawPixel(i,j, 255,255,255);
            }
        }

    for(int i=0; i<TAM; i++){
        for(int j=0; j<TAM; j++){
            if (dentina[i][j] > 0 ){
                // Homogeneidade
                _infos[0] += dentina[i][j] / (1 + (i - j) * (i - j));
                // Contraste
                _infos[1] += (i - j) * (i - j) * dentina[i][j];
                // Entropia
                _infos[2] += dentina[i][j] * log2(dentina[i][j]);
                // Energia
                _infos[3] += dentina[i][j] * dentina[i][j];
            }
        }
    }

    /* gerador de relatorio, caso necessario */
    out->Save(".\\dataREPORT\\mcoDentina.png");
    out->Delete();
    fp = fopen(".\\dataREPORT\\mco_dentina.txt", "w+");
    for(int i=0; i<TAM; i++){
        for(int j=0; j<TAM; j++){
            fprintf(fp, "%f;", dentina[i][j]);
        }
        fprintf(fp, "\n");
    }

    fprintf(fp, "\n\n -------------\n");
    fprintf(fp, "Homogeneidade: %f\n", _infos[0]);
    fprintf(fp, "Contraste: %f\n", _infos[1]);
    fprintf(fp, "Entropia: %f\n", _infos[2]);
    fprintf(fp, "Energia: %f\n", _infos[3]);
    fclose(fp);

    //cout << "Concluiu Create MCO.\n";
}


// **********************************************************************
//      Função para criar a matriz com a orientação e
//       direção fornecidas em dx e dy
// **********************************************************************
void _createtemporariMCO(int dx, int dy, int x, int y, int tamanho, ImageClass *from){
    int normal = 0;
    //cout << "Iniciou Create MCO... ";

    for(int i=0; i<TAM; i++)
        for(int j=0; j<TAM; j++)
            temporariMCO[i][j] = 0;

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

                if (p1 < TAM && p2 < TAM){
                    temporariMCO[(int)p1][(int)p2]++;
                    temporariMCO[(int)p2][(int)p1]++;

                    normal+=2;
                }
            }
        }
    }

    for(int i=0; i<TAM; i++)
        for(int j=0; j<TAM; j++){
            if (temporariMCO[i][j] > 0 ){
                temporariMCO[i][j] = temporariMCO[i][j] / normal;
            }
        }

    temporariosdata[0] = 0;
    temporariosdata[1] = 0;
    temporariosdata[2] = 0;
    temporariosdata[3] = 0;

    for(int i=0; i<TAM; i++){
        for(int j=0; j<TAM; j++){
            if (temporariMCO[i][j] > 0 ){
                // Homogeneidade
                temporariosdata[0] += temporariMCO[i][j] / (1 + (i - j) * (i - j));

                // Contraste
                temporariosdata[1] += (i - j) * (i - j) * temporariMCO[i][j];
                // Entropia
                temporariosdata[2] += temporariMCO[i][j] * log2(temporariMCO[i][j]);
                // Energia
                temporariosdata[3] += temporariMCO[i][j] * temporariMCO[i][j];
            }
        }
    }

    /* gerador de relatorio, caso necessario
    fp = fopen(".\\dataREPORT\\mco2.txt", "w+");
    for(int i=0; i<TAM; i++){
        for(int j=0; j<TAM; j++){
            fprintf(fp, "%f;", temporariMCO[i][j]);
        }
        fprintf(fp, "\n");
    }

    fprintf(fp, "\n\n -------------\n");
    fprintf(fp, "Homogeneidade: %f\n", temporariosdata[0]);
    fprintf(fp, "Contraste: %f\n", temporariosdata[1]);
    fprintf(fp, "Entropia: %f\n", temporariosdata[2]);
    fprintf(fp, "Energia: %f\n", temporariosdata[3]);
    fclose(fp);
*/
    //cout << "Concluiu Create MCO.\n";
}

void texturaDentina(ImageClass *from, ImageClass *to){
    unsigned char r,g,b;
    int x = 0,y = 0;
    float limiar = 0.7;
    float limiar2 = 1.4;
    // DX = 0 || DY = 1
    int dx = 0, dy = -1;

    // DX = 1 || DY = 0
    // int dx = 1, dy = 0;

    // DX = 1 || DY = 1
    // int dx = 1, dy = -1;

    cout << "Iniciou TexturaDentina....";

    _createMCODentinaTruth(dx, dy, from, tmp);
    _createMCOCanalTruth(dx, dy, from, tmp);

    for(x=0;x<from->SizeX()-1;x+=counter){
        for(y=0;y<from->SizeY()-1;y+=counter){
            _createtemporariMCO(dx, dy, x, y, 70, from);

            if (x == 190 && y == 180 ||
                x == 230 && y == 220 ||
                x == 1500 && y == 1500 ||
                x == 2000 && y == 1000){
                printf("\n------ X: %d && Y: %d------\n", x, y);
                printf("Desc: Dentina | Canal || Atual\n");
                printf("Homo: %f | %f || %f \n", _infos[0], _infosCanal[0], temporariosdata[0]);
                printf("Cont: %f | %f || %f \n", _infos[1], _infosCanal[1], temporariosdata[1]);
                printf("Entr: %f | %f || %f \n", _infos[2], _infosCanal[2], temporariosdata[2]);
                printf("Ener: %f | %f || %f \n", _infos[3], _infosCanal[3], temporariosdata[3]);
            }

            // #### Verificando se é Dentina ####
            if (
                (temporariosdata[0] >= (0.05) && temporariosdata[0] <= (0.1))
                // && (temporariosdata[1] >= (_infos[0] * limiar) && temporariosdata[1] <= (_infos[1] * limiar2))
                // && (-temporariosdata[2] >= (-_infos[2] * limiar) && -temporariosdata[2] <= (-_infos[2] * limiar2))
                 && (temporariosdata[3] >= (_infos[3] * limiar) && temporariosdata[3] <= (_infos[3] * limiar2))
                ){
                for(int x2=x;x2<x+counter;x2++){
                    if (x2 <= to->SizeX()){
                        for(int y2=y;y2<y+counter;y2++){
                            if (y2 <= to->SizeY())
                                to->DrawPixel(x2, y2, 0, 255, 0);
                        }
                    }
                }
            // #### Verificando se é Canal ####
            } else if (
                       (temporariosdata[0] >= (_infosCanal[0] * limiar) && temporariosdata[0] <= (_infosCanal[0] * limiar2))
                        && (temporariosdata[1] >= (150) && temporariosdata[1] <= (260))
                        // && (-temporariosdata[2] >= (-_infosCanal[2] * limiar) && -temporariosdata[2] <= (-_infosCanal[2] * limiar2))
                        // && (temporariosdata[3] >= (_infosCanal[3] * limiar) && temporariosdata[3] <= (_infosCanal[3] * limiar2))
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
                for(int x2=x;x2<x+counter;x2++){
                    if (x2 <= to->SizeX() && x2 <= from->SizeX()){
                        for(int y2=y;y2<y+counter;y2++){
                            if (y2 <= to->SizeY() && y2 <= from->SizeY()){
                                from->ReadPixel(x2,y2,r,g,b);
                                to->DrawPixel(x2, y2,r,g,b);
                            }
                        }
                    }
                }
            }
        }
    }

    cout << "Concluiu TexturaDentina.\n";
}



