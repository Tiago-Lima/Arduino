#include <EtherCard.h>
#include <stdio.h>
#include <stdlib.h>


static byte mymac[] = {0x00,0xE0,0x4C,0x6A,0x82,0x55}; // Mac Adress do módulo Ethernet
static byte myip[]= {192,168,1,220}; // definindo um ip para o módulo ethernet

char fila[10];
int fim=10;
char reset;
int i; //Variável de controle global, referência do índice da fila
boolean reinicia;

int s1=0, s2=0, s3=0;

int EstadSensorUm;
int EstAntSensorUm;
int EstadSensorDois;
int EstAntSensorDois;

int sensorUm= 24;
int releUm= 28;
int sensorDois= 33;
int releDois=32;
int sensorTres= 37;
int releTres=36;


byte Ethernet::buffer[500];


BufferFiller bfill; 


static word homePage()//essa função contém a página inicial em html
{
  bfill= ether.tcpOffset();
  bfill.emit_p(PSTR(
    
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<html>"
    "<head>"
    "<title> Com- Arduino </title>""</head>"
    "<body>"
    "<h1><center>WEB SERVER ARDUINO</center></h1>"
    "\r\n"
    "<h1> Fila da esteira REINICIADA!</h1>"
    "<h2> Pronta para receber valores de leitura...</h2>"

  "</body>"
  
   ));
return bfill.position();
}


static word pageA()//essa função contém auma segunda página chamada "/A" página em html
{
  bfill= ether.tcpOffset();
  bfill.emit_p(PSTR(
    
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<html>"
    "<head>"
    "<title> Com- Arduino </title>""</head>"
    "<body>"
    "<h1>Produto adicionado com sucesso na fila!</h1>"
  "</body>"
  
   ));
return bfill.position();
}

static word ErrorPage()//essa função contém uma terceira página para retornar uma mensagem de error caso receba algum parâmetro que não identifique um número de saída.
{
     
  bfill= ether.tcpOffset();
  bfill.emit_p(PSTR(
     
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<html>"
    "<head>"
    "<title> Com- Arduino </title>""</head>"
    "<body>"
    "<h1>ERRO de leitura!</h1>"
  "</body>"
  
   ));
return bfill.position();
}

void setup() {

//******************************************** CABECALHO DE IO *********************************************************

    pinMode (sensorUm, INPUT_PULLUP); // Pull Up seta a porta sempre em positivo (Na ausência do sinal negativo emitido pelo acionamento do sensor)
    pinMode (releUm,OUTPUT);
    pinMode (sensorDois, INPUT_PULLUP);
    pinMode (releDois, OUTPUT);
    pinMode (sensorTres, INPUT_PULLUP);
    pinMode (releTres, OUTPUT);


  //************************************ CONFIGURAÇÃO DO MÓDULO ETHERNET ************************************************
  
  Serial.begin(57600); // velocidade de comunicação da rede
  Serial.println("Teste");

  
  if (ether.begin( sizeof Ethernet::buffer,mymac)==0)
    Serial.println(" Failed to Acess Ethernet Controller");
  else
    Serial.println(" Ethernet Controller is Running!");

    ether.staticSetup (myip); // definindo o IP como estático

//****************************************************************************************************
     
}//Fecha o Void Setup




void loop() {

if(s1==fim)
s1=0;

word pos= ether.packetLoop (ether.packetReceive());// tratando esse pacote e armazenando na variavel pos
    

     if (pos){
      for( int c=pos;Ethernet::buffer[c];c++)
          Serial.print((char)Ethernet::buffer[c]);
          Serial.println("");

  word n; // Variável Que Recebe as Páginas em HTML
  
 //Armazenando valores de 1 a 9
      switch(Ethernet::buffer[pos+5]){
        case '1':  fila[i]='1';i=i+1; n= pageA();break;
        case '2':  fila[i]='2';i=i+1; n= pageA();break;
        case '3':  fila[i]='3';i=i+1; n= pageA();break;
        case '4':  fila[i]='4';i=i+1; n= pageA();break;
        case '5':  fila[i]='5';i=i+1; n= pageA();break;
        case '6':  fila[i]='6';i=i+1; n= pageA();break;
        case '7':  fila[i]='7';i=i+1; n= pageA();break;
        case '8':  fila[i]='8';i=i+1; n= pageA();break;
        case '9':  fila[i]='9';i=i+1; n= pageA();break;
        case 'r':  reinicia= true; n= homePage();break;
        default:n=ErrorPage();
      }
     
      
                          if(i==fim) //Fim da fila!! Seta o indíce para começa a gravar dados no início da fila denovo
                          i=0;
                          
                      
                                        if(i==0) 
                                        n=homePage();
                              
                              for(int j=0;j<10;j++)
                              {
                                 Serial.println(fila[j]);// Imprime a fila na saída serial (debug)
                              }    
                            
                  ether.httpServerReply(n);           
       
     
      
        
  }  //Fecha a condição de leitura da variável pos
      
if (reinicia==true)
{
reset= false;
asm volatile ("  jmp 0");
}
//****************************************** Lógica de Leitura e Acionamento dos Sensores *****************************************8

 EstadSensorUm= digitalRead (sensorUm); // Lê sensor Um e armazena na variável
      
      if (EstadSensorUm == LOW && EstadSensorUm != EstAntSensorUm)
      {                 
                 if( fila[s1]=='1')
                {
                        
                        digitalWrite(releUm, HIGH);
                          s1=s1+1; // Ao retirar profuto da fila, incrementa todas as variáveis de controle de fila dos sensores a frente.
                          s2++;
                          s3++;
                          delay(2000);
                                
                }
                     
              else
              s1=s1+1;
              
              Serial.println("");
              Serial.println("sensor UM");
              Serial.println(s1);
      }
              
              digitalWrite(releUm, LOW);
               
              EstAntSensorUm= EstadSensorUm;

//***************************************** SAIDA 2**********************************************************************//            

      if (s2==fim)
      s2=0;
      
                
                EstadSensorDois= digitalRead (sensorDois); // Lê sensor Um e armazena na variável
      
      if (EstadSensorDois == LOW && EstadSensorDois != EstAntSensorDois)
      {                 
                 if( fila[s2]=='2')
                {
                        
                        digitalWrite(releDois, HIGH);
                        s2=s2+1; // Ao retirar produto da fila, incrementa todas as variáveis de controle de fila dos sensores a frente
                        s3++;
                        delay (2000);
                }
                     
              else
              s2=s2+1;
              
              
              Serial.println("");
              Serial.println("sensor DOIS");
              Serial.println(s2);  
      }
          
              
              
              digitalWrite(releDois, LOW);
    
              EstAntSensorDois= EstadSensorDois;
  
              delay(10);//delay de 10 milisegundos por padrão, para os módulos não se confundirem.

  }//fecha o Void loop
