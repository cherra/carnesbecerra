/* 
 * File:   cfdi.h
 * Author: cherra
 *
 * Created on 1 de enero de 2014, 03:12 PM
 */

#ifndef CFDI_H
#define	CFDI_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int timbra_cfdi( char *folio ){
    int sockfd;
    int len, n = 0;
    struct sockaddr_in address;
    int result;
    char recvBuff[1024];

    char ip[50]="";
    char usuario[50]="";
    char contrasena[50]="";
    char bd[50]="";
    int puerto=3306;
    FILE *fconfiguracionbd;
    int i=0;
    int j=0;
    char tipo[50]="";
    char dato[50]="";
    char tmp;
    char *error ;
    /*
     * Se lee el archivo de configuración de la base de datos
     */
    if((fconfiguracionbd = fopen(file_db_config,"r")))
    {
            printf("Si hay archivo...  \n");
            while(!feof(fconfiguracionbd))
            {
                    tmp = fgetc(fconfiguracionbd);
                    //printf("%c", tmp);
                    if(tmp != ' ')
                    {
                            tipo[i] = tmp;
                            i++;
                    }else
                    {
                            while(tmp != '\n')
                            {
                                    tmp = fgetc(fconfiguracionbd);
                                    dato[j]=tmp;
                                    j++;
                            }
                                    dato[j-1] = '\0';
                            j=0;
                            if(strcmp(tipo,"ip") == 0)
                                    strcpy(ip, dato);
                            if(strcmp(tipo,"usuario") == 0)
                                    strcpy(usuario, dato);
                            if(strcmp(tipo,"contrasena") == 0)
                                    strcpy(contrasena, dato);
                            if(strcmp(tipo,"bd") == 0)
                                    strcpy(bd, dato);
                            if(strcmp(tipo,"puerto") == 0)
                                    puerto = atoi(dato);
                            strcpy(tipo,"");
                            for(i=0;i<30;i++)
                                    tipo[i]='\0';
                            i=0;
                    }
            }
            fclose(fconfiguracionbd);
            leerconf=false;
    }else
    {
            printf("No se pudo abrir el archivo....");
                    return -1;
    }
    
        
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
            printf("Ocurrió un error: No se puede crear el socket \n");
            return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(4444);
    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1){
            perror("oops: cliente");
            return 1;
    }
    //write(sockfd, &ch, 1);
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
            recvBuff[n] = '\0';
            printf("Mensaje recibido: %s\n", recvBuff);
            if(fputs(recvBuff, stdout) == EOF)
            {
                printf("\n Error : Fputs error\n");
            }

    }

    if(n < 0)
    {
            printf("\n Read error \n");
    }
    //read(sockfd, &ch, 1);

    //printf("Char desde el servidor= %c\n", ch);
    close(sockfd);

    return 0;
}

#endif	/* CFDI_H */

