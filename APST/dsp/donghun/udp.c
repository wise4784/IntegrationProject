#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define  BUFF_SIZE   32

int   main( void)
{
   int   sock;
   int   client_addr_size;

   struct sockaddr_in   server_addr;
   struct sockaddr_in   client_addr;

   char   buff_rcv[BUFF_SIZE+5];
   char   buff_snd[BUFF_SIZE+5];

   int setCNT = 7200;
   int degree = 30;


   sock  = socket( PF_INET, SOCK_DGRAM, 0);
   
   if( -1 == sock)
   {
      printf("socket 생성 실패");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons( 7777);
   server_addr.sin_addr.s_addr= htonl( INADDR_ANY);

   if( -1 == bind( sock, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "bind() 실행 에러");
      exit( 1);
   }

   while( 1)
   {
      client_addr_size  = sizeof( client_addr);

      recvfrom(sock, buff_rcv, BUFF_SIZE, 0 ,
                     ( struct sockaddr*)&client_addr, &client_addr_size);

      printf( "receive: %s\n", buff_rcv);
     
#if 0
      sprintf( buff_snd, "%s%s", buff_rcv, buff_rcv);
      sendto( sock, buff_snd, strlen( buff_snd)+1, 0,  // +1: NULL까지 포함해서 전송
                     ( struct sockaddr*)&client_addr, sizeof( client_addr)); 
#else
      usleep(10000);
//      sprintf(buff_snd,"a%d",setCNT);

      buff_snd[0] = 's';
      buff_snd[1] = (setCNT & 0xFF000000) >> 24;
      buff_snd[2] = (setCNT & 0x00FF0000) >> 16;
      buff_snd[3] = (setCNT & 0x0000FF00) >> 8;
      buff_snd[4] = setCNT & 0x000000FF;
      buff_snd[5] = (degree & 0xFF000000) >> 24;
      buff_snd[6] = (degree & 0x00FF0000) >> 16;
      buff_snd[7] = (degree & 0x0000FF00) >> 8;
      buff_snd[8] = degree & 0x000000FF;

      sendto(sock, buff_snd, 10, 0,
             (struct sockaddr*)&client_addr, sizeof(client_addr));
#endif
   }
}
