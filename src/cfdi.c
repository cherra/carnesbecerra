#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int hostname_to_ip(char *hostname , char *ip)
{
    int sockfd; 
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in *h;
    int rv;
 
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
 
    if ( (rv = getaddrinfo( hostname , "http" , &hints , &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
 
    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        h = (struct sockaddr_in *) p->ai_addr;
        strcpy(ip , inet_ntoa( h->sin_addr ) );
    }
     
    freeaddrinfo(servinfo); // all done with this structure
    return 0;
}

int timbra_cfdi( char *folio ){
    int sockfd;
    int len, n = 0;
    struct sockaddr_in address;
    int result;
    char json[1024];
    char respuesta[1024];

    char ip[50]="";
    char hostname[100]="";
    char usuario[50]="";
    char contrasena[50]="";
    char bd[50]="";
    int puerto=4444;
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
                                    strcpy(hostname, dato);
                            if(strcmp(tipo,"usuario") == 0)
                                    strcpy(usuario, dato);
                            if(strcmp(tipo,"contrasena") == 0)
                                    strcpy(contrasena, dato);
                            if(strcmp(tipo,"bd") == 0)
                                    strcpy(bd, dato);
                            if(strcmp(tipo,"puerto_cfdi") == 0)
                                    puerto = atoi(dato);
                            strcpy(tipo,"");
                            for(i=0;i<30;i++)
                                    tipo[i]='\0';
                            i=0;
                    }
            }
            fclose(fconfiguracionbd);
    }else
    {
            printf("No se pudo abrir el archivo de configuración....");
                    return -1;
    }
    
        
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
            printf("Ocurrió un error: No se puede crear el socket \n");
            return 1;
    }
    
    hostname_to_ip(hostname , ip);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(puerto);
    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1){
            perror("oops: cliente");
            return 1;
    }
    
    sprintf(json, "{'bd':'%s','username':'%s','password':'%s','folio':'%s'}\0", bd, usuario, contrasena, folio);
    write(sockfd, &json, strlen(json));
    while ( (n = read(sockfd, &respuesta, sizeof(respuesta)-1)) > 0)
    {
            respuesta[n] = '\0';
            //printf("Mensaje recibido: %s\n", respuesta);
            if(fputs(respuesta, stdout) == EOF)
            {
                printf("\n Error : Fputs error\n");
            }

    }
//    if(n = read(sockfd, &respuesta, sizeof(respuesta)-1) > 0){
//        printf("Tamaño del buffer: %d\n", sizeof(respuesta));
//        //respuesta[n] = '\0';
//        //printf("Respuesta desde el servidor= %s\n", respuesta);
//    }

    close(sockfd);

    if(strcmp(respuesta,"OK") == 0)
        return 1;
    else
        return 0;
}
