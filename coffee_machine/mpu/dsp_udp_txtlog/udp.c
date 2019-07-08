#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <unistd.h>
#include <fcntl.h>

#define  BUFF_SIZE   400

int   main( void)
{
   int   sock, i=0;
   unsigned int   client_addr_size;
	int fd=0;
	off_t new_pos=0;
	int length=0;

   struct sockaddr_in   server_addr;
   struct sockaddr_in   client_addr;

   char   buff_rcv[BUFF_SIZE+5];
   char   buff_snd[BUFF_SIZE+5];
	char buff_txt[BUFF_SIZE+8];
	char initial_txt[1]="\n";

   sock  = socket( PF_INET, SOCK_DGRAM, 0);
   
   if( -1 == sock)
   {
      printf("socket 생성 실패");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons( 7767);
   server_addr.sin_addr.s_addr= htonl( INADDR_ANY);

   if( -1 == bind( sock, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "bind() 실행 에러");
      exit( 1);
   }
	fd= open("adc_data.txt", O_CREAT, 0644);
	//new_pos=lseek(fd, (off_t)0, SEEK_END);
	//write(fd, initial_txt, 1);
	write(fd, initial_txt, 1);
	close(fd);

   while( i<10000)
   {
      client_addr_size  = sizeof( client_addr);
      recvfrom( sock, buff_rcv, BUFF_SIZE, 0 , 
                     ( struct sockaddr*)&client_addr, &client_addr_size);
      printf( "receive: %s", buff_rcv);
      fd = open("udp_data.txt", O_RDWR);
      new_pos = lseek(fd, (off_t)0, SEEK_END);
	  write(fd, buff_rcv, strlen(buff_rcv));

	  i++;

        //write(fd, buff_rcv,strlen(buff_rcv));	//sizeof → strlen 해야 txt refresh중에 열때에러안난다.
	  close(fd);
      sprintf( buff_snd, "%s%s", buff_rcv, buff_rcv);
      sendto( sock, buff_snd, strlen( buff_snd)+1, 0,  // +1: NULL까지 포함해서 전송
                     ( struct sockaddr*)&client_addr, sizeof( client_addr)); 
   }
}
