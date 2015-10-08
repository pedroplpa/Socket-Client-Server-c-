#include<cstdio>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<vector>
#include<string>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netdb.h>

using namespace std;

vector <int> soma(int L, int C, vector<int> m1, vector<int> m2);
vector <int> transposta(int L, int C, vector<int> m0);
vector <int> subtracao(int L, int C, vector<int> m1, vector<int> m2);
vector <int> produto(int L1, int C1, vector<int> m1, int L2, int C2, vector<int> m2);

class tcp_Server
{
private:
  int serverSocket, msgSocket;
  socklen_t clientLen;
  struct sockaddr_in addr, client;
public:
  tcp_Server(char *port){
   //Creating socket
   serverSocket = socket(AF_INET, SOCK_STREAM, 0);

   //Binding the socket
   memset(&addr, 0, sizeof(struct sockaddr_in));
   addr.sin_family = AF_INET;
   addr.sin_port = htons(atoi(port));
   addr.sin_addr.s_addr = INADDR_ANY;
   bind(serverSocket,(struct sockaddr *) &addr, sizeof(addr));

   //Start listening
   listen(serverSocket,20);
   
   //Accept connection
   clientLen = sizeof(client);
   msgSocket = accept(serverSocket,(struct sockaddr *)&client,&clientLen);
   printf("Client IP: %s\n",inet_ntoa(client.sin_addr));
   printf("Client Port: %hu\n", ntohs(client.sin_port));
  }

  int receive(char buffer[]){
    read(msgSocket,buffer,32);
  }
  int send(char buffer[]){
    write(msgSocket,buffer,32);
  }
  ~tcp_Server(){
    close(msgSocket);
    close(serverSocket);
  }
};

int main(int argc, char *argv[])
{

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  int tag = 0;
  int element;
  int tam_matrix[2][2];
  char buffer[33];
  char response[33];
  tcp_Server serv(argv[1]);

  while(tag != 5)
  {
    if(tag == 0)
    {
      bzero(buffer,33);
      serv.receive(buffer);
      tag = atoi(buffer);
      sprintf(buffer,"%d",tag);
      serv.send(buffer);
    }
    else if(tag == 1 || tag == 2) //Soma ou Subtração
    {
      bzero(buffer,33);
      //Recebe o tamanho das Matrizes
      for(int i = 0; i < 2; i++ )
      {
        for(int j = 0; j < 2; j++)
        {  
          bzero(buffer,33);
          serv.receive(buffer);
          tam_matrix[i][j] = atoi(buffer);
          serv.send(buffer);
        }
      }

      //Cria vector com tamanho das Matrizes
      vector<int> m1(tam_matrix[0][0] * tam_matrix[0][1]);
      vector<int> m2(tam_matrix[1][0] * tam_matrix[1][1]);

      //Preenche matriz 1 com elementos
      for(int i = 0; i < tam_matrix[0][0]*tam_matrix[0][1]; i++)
      {
        bzero(buffer,33);
        serv.receive(buffer);
        m1[i] = atoi(buffer);
        serv.send(buffer);
      }

      //Preenche matriz 2 com elementos
      for(int i = 0; i < tam_matrix[0][0]*tam_matrix[0][1]; i++)
      {
        bzero(buffer,33);
        serv.receive(buffer);
        m2[i] = atoi(buffer);
        serv.send(buffer);
      }
      if(tag == 1) //Calcula a soma
        m1 = soma(tam_matrix[0][0],tam_matrix[0][1],m1,m2);
      else
        m1 = subtracao(tam_matrix[0][0],tam_matrix[0][1],m1,m2);

      //Passa cada elemento do resultado para o cliente
      for(int i = 0; i < tam_matrix[0][0]*tam_matrix[0][1]; i++)
      {
        bzero(buffer,33);
        serv.receive(buffer);
        sprintf(buffer,"%d",m1[i]);
        serv.send(buffer);
      }
      tag = 0;
    }
    else if(tag == 3) //Multiplicação
    {
      bzero(buffer,33);
      //Recebe o tamanho das Matrizes
      for(int i = 0; i < 2; i++ )
      {
        for(int j = 0; j < 2; j++)
        {  
          bzero(buffer,33);
          serv.receive(buffer);
          tam_matrix[i][j] = atoi(buffer);
          serv.send(buffer);
        }
      }

      //Cria vector com tamanho das Matrizes
      vector<int> m1(tam_matrix[0][0] * tam_matrix[0][1]);
      vector<int> m2(tam_matrix[1][0] * tam_matrix[1][1]);

      //Preenche matriz 1 com elementos
      for(int i = 0; i < tam_matrix[0][0]*tam_matrix[0][1]; i++)
      {
        bzero(buffer,33);
        serv.receive(buffer);
        m1[i] = atoi(buffer);
        serv.send(buffer);
      }

      //Preenche matriz 2 com elementos
      for(int i = 0; i < tam_matrix[1][0]*tam_matrix[1][1]; i++)
      {
        bzero(buffer,33);
        serv.receive(buffer);
        m2[i] = atoi(buffer);
        serv.send(buffer);
      }
      //Calcula a multiplicação
      m1 = produto(tam_matrix[0][0],tam_matrix[0][1],m1,tam_matrix[1][0],tam_matrix[1][1],m2);

      //Passa cada elemento do resultado para o cliente
      for(int i = 0; i < tam_matrix[0][0]*tam_matrix[1][1]; i++)
      {
        bzero(buffer,33);
        serv.receive(buffer);
        sprintf(buffer,"%d",m1[i]);
        serv.send(buffer);
      }
      tag = 0;
    }
    else if(tag == 4) //Transposta
    {
      bzero(buffer,33);
      //Recebe o tamanho da Matrize
        for(int j = 0; j < 2; j++)
        {  
          bzero(buffer,33);
          serv.receive(buffer);
          tam_matrix[0][j] = atoi(buffer);
          serv.send(buffer);
        }

      //Cria vector com tamanho das Matrizes
      vector<int> m1(tam_matrix[0][0] * tam_matrix[0][1]);

      //Preenche matriz com elementos
      for(int i = 0; i < tam_matrix[0][0]*tam_matrix[0][1]; i++)
      {
        bzero(buffer,33);
        serv.receive(buffer);
        m1[i] = atoi(buffer);
        serv.send(buffer);
      }

      //Calcula a transposta
      m1 = transposta(tam_matrix[0][0],tam_matrix[0][1],m1);

      //Passa cada elemento do resultado para o cliente
      for(int i = 0; i < tam_matrix[0][0]*tam_matrix[0][1]; i++)
      {
        bzero(buffer,33);
        serv.receive(buffer);
        sprintf(buffer,"%d",m1[i]);
        serv.send(buffer);
      }
      tag = 0;

    }
    else if(tag == 5) //Sair
    {
      serv.receive(buffer);
      serv.send(buffer);    
    }
  }
  return 0;
}

vector <int> transposta(int L, int C,vector<int> m0)
{
  int i, j, aux, cont = 0;
  vector <int> mt(L*C);
  for(i=0;i<L;i++)
  {
    for(j=0;j<C;j++)
      mt[i + j*L ] = m0[i*C + j];
  }
  return mt;
}


vector <int> soma(int L, int C, vector<int> m1, vector<int> m2)
{
  int i;
  vector <int> ms(L*C);
  for(i=0;i<L*C;i++)
    ms[i] = m1[i] + m2[i];
  return ms;
}

vector <int> subtracao(int L, int C, vector<int> m1, vector<int> m2)
{
  int i;
  vector <int> ms(L*C);
  for(i=0;i<L*C;i++)
    ms[i] = m1[i] - m2[i];
  return ms;
}

vector <int> produto(int L1, int C1, vector<int> m1, int L2, int C2, vector<int> m2)
{
  int i,j,l,k=0;
  vector <int> m(L1*C2);
  int mr = 0;

  for(l=0;l<L1;l++)
  {
    for(j=0;j<C2;j++)
    {
      for(i=0;i<C1;i++)
      {
        mr=mr+m1[i+l*C1]*m2[i*C2+j];
      }
      m[k]=mr;
      k++;
      mr = 0;
    }
  }	
  return m;
}
