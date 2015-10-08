#include<cstdio>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>

using namespace std;

class tcp_client
{
  int socketClient;
  struct sockaddr_in server;
  //struct hostent *serv;

public:
  tcp_client(char *port, char *ip){
    //Initializing socket
    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    
    //Connecting socket
    memset(&server, 0, sizeof(server));
    //serv = gethostbyname(ip);//This pc;
    server.sin_family = AF_INET;
    inet_aton(ip,&server.sin_addr);
    //bcopy((char *)serv->h_addr,(char *)&server.sin_addr.s_addr,serv->h_length);//server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server.sin_port = htons(atoi(port));
    if(connect(socketClient, (struct sockaddr *)&server,sizeof(sockaddr)) < 0)
      printf("Erro\n");
  }
  int send(char buffer[]){
    write(socketClient,buffer,32);
  }
  int receive(char buffer[]){
    read(socketClient,buffer,32);
   // printf("%s\n",buffer);
  }
  ~tcp_client(){
    close(socketClient);
  }

};

int main(int argc, char *argv[])
{
  if(argc < 3)
  {
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    exit(0);
  }

  char buffer[33];
  int tag = 0;
  int tam_matrix[2][2];
  int element;
  tcp_client client(argv[2],argv[1]);
 
  while(tag != 5) //Enquanto operador não pede para sair
  {
    if(tag == 0) //Tag = 0 -> Menu
    {
      bzero(buffer,33);
      printf("Selecione 1 - para somar\n2 - para subtrair\n3 - para multiplicar\n4 - para transposta\n5 - para sair\n");
      scanf("%d",&tag);
      sprintf(buffer,"%d",tag);
      client.send(buffer);
      client.receive(buffer);
      tag = atoi(buffer);
    }
    else if(tag == 1 || tag == 2) //Tag = 1 -> Soma e Tag = 2 -> Subtração
    {
      printf("Insira o tamanho da primeira matriz no formato MxN: ");
      scanf("%dx%d",&tam_matrix[0][0],&tam_matrix[0][1]);
     
      printf("Insira o tamanho da segunda matriz no formato MxN: ");
      scanf("%dx%d",&tam_matrix[1][0],&tam_matrix[1][1]);
      
      //Teste de tamanho correto das matrizes
      if(tam_matrix[0][0] != tam_matrix[1][0] || tam_matrix[0][1] != tam_matrix[1][1])
        printf("Tamanhos de matrizes incompativeis!\n");
      else
      {
        //Passando tamanho pro servidor
        for(int i = 0; i < 2; i++)
        {
          for(int j = 0; j < 2; j++)
          {
            bzero(buffer,33);
            sprintf(buffer,"%d",tam_matrix[i][j]);

            client.send(buffer);
            client.receive(buffer);        
          }
        }
        //Passando elementos da primeira matriz pro servidor
        for(int i = 0; i < tam_matrix[0][0]; i++)
        {
          for(int j = 0; j < tam_matrix[0][1]; j++)
          {
            bzero(buffer,33);
            printf("Insira o elemento %d linha e %d coluna: ",i+1,j+1);
            scanf("%d",&element);
            
            sprintf(buffer,"%d",element);
            
            client.send(buffer);
            client.receive(buffer);
          }
        }
        
        //Passando elementos da segunda matriz pro servidor
        for(int i = 0; i < tam_matrix[0][0]; i++)
        {
          for(int j = 0; j < tam_matrix[0][1]; j++)
          {
            bzero(buffer,33);
            printf("Insira o elemento %d linha e %d coluna: ",i+1,j+1);
            scanf("%d",&element);
            
            sprintf(buffer,"%d",element);
            
            client.send(buffer);
            client.receive(buffer);
          }
        }

        //Recebendo Matriz Resultado do Servidor
        for(int i = 0; i < tam_matrix[0][0]; i++)
        { 
          bzero(buffer,33);
          sprintf(buffer,"%d",0);

          client.send(buffer);
          client.receive(buffer);

          printf("%d",atoi(buffer));

          for(int j = 1; j < tam_matrix[0][1]; j++)
          {
            printf(" ");
            bzero(buffer,33);
            sprintf(buffer,"%d",0);

            client.send(buffer);
            client.receive(buffer);

            printf("%d",atoi(buffer));
          }
          printf("\n");
        }        
      }
      tag = 0;
    }
    else if(tag == 3) //Tag = 3 -> Multiplicação
    {
      printf("Insira o tamanho da primeira matriz no formato MxN: ");
      scanf("%dx%d",&tam_matrix[0][0],&tam_matrix[0][1]);
     
      printf("Insira o tamanho da segunda matriz no formato MxN: ");
      scanf("%dx%d",&tam_matrix[1][0],&tam_matrix[1][1]);
      
      //Teste de tamanho correto das matrizes
      if(tam_matrix[0][1] != tam_matrix[1][0])
        printf("Tamanhos de matrizes incompativeis!\n");
      else
      {
        //Passando tamanho pro servidor
        for(int i = 0; i < 2; i++)
        {
          for(int j = 0; j < 2; j++)
          {
            bzero(buffer,33);
            sprintf(buffer,"%d",tam_matrix[i][j]);

            client.send(buffer);
            client.receive(buffer);        
          }
        }
        //Passando elementos da primeira matriz pro servidor
        for(int i = 0; i < tam_matrix[0][0]; i++)
        {
          for(int j = 0; j < tam_matrix[0][1]; j++)
          {
            bzero(buffer,33);
            printf("Insira o elemento %d linha e %d coluna: ",i+1,j+1);
            scanf("%d",&element);
            
            sprintf(buffer,"%d",element);
            
            client.send(buffer);
            client.receive(buffer);
          }
        }
        
        //Passando elementos da segunda matriz pro servidor
        for(int i = 0; i < tam_matrix[1][0]; i++)
        {
          for(int j = 0; j < tam_matrix[1][1]; j++)
          {
            bzero(buffer,33);
            printf("Insira o elemento %d linha e %d coluna: ",i+1,j+1);
            scanf("%d",&element);
            
            sprintf(buffer,"%d",element);
            
            client.send(buffer);
            client.receive(buffer);
          }
        }

        //Recebendo Matriz Resultado do Servidor
        for(int i = 0; i < tam_matrix[0][0]; i++)
        { 
          bzero(buffer,33);
          sprintf(buffer,"%d",0);

          client.send(buffer);
          client.receive(buffer);

          printf("%d",atoi(buffer));

          for(int j = 1; j < tam_matrix[1][1]; j++)
          {
            printf(" ");
            bzero(buffer,33);
            sprintf(buffer,"%d",0);

            client.send(buffer);
            client.receive(buffer);

            printf("%d",atoi(buffer));
          }
          printf("\n");
        }        
      }
      tag = 0;
      
    }
    else if(tag == 4) //Tag = 4 -> Transposta
    {
      printf("Insira o tamanho da matriz no formato MxN: ");
      scanf("%dx%d",&tam_matrix[0][0],&tam_matrix[0][1]);
     
        //Passando tamanho pro servidor
        for(int j = 0; j < 2; j++)
        {
          bzero(buffer,33);
          sprintf(buffer,"%d",tam_matrix[0][j]);

          client.send(buffer);
          client.receive(buffer);        
        }
        //Passando elementos da matriz pro servidor
        for(int i = 0; i < tam_matrix[0][0]; i++)
        {
          for(int j = 0; j < tam_matrix[0][1]; j++)
          {
            bzero(buffer,33);
            printf("Insira o elemento %d linha e %d coluna: ",i+1,j+1);
            scanf("%d",&element);
            
            sprintf(buffer,"%d",element);
            
            client.send(buffer);
            client.receive(buffer);
          }
        }
        
        //Recebendo Matriz Resultado do Servidor
        for(int i = 0; i < tam_matrix[0][1]; i++)
        { 
          bzero(buffer,33);
          sprintf(buffer,"%d",0);

          client.send(buffer);
          client.receive(buffer);

          printf("%d",atoi(buffer));

          for(int j = 1; j < tam_matrix[0][0]; j++)
          {
            printf(" ");
            bzero(buffer,33);
            sprintf(buffer,"%d",0);

            client.send(buffer);
            client.receive(buffer);

            printf("%d",atoi(buffer));
          }
          printf("\n");
        }        
      
      tag = 0;
    }   
  }
  return 0;
}
