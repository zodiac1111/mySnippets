/**
 * 标准linux下的c语言编写的应用层uart测试程序,
 * 调用文件的open close read write 系统调用
 * UART_DEV 定义UART设备文件
 */
#include     <stdio.h>		/*标准输入输出定义 */
#include     <stdlib.h>		/*标准函数库定义 */
#include     <unistd.h>		/*Unix 标准函数定义 */
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>		/*文件控制定义 */
#include     <termios.h>	/*PPSIX 终端控制定义 */
#include     <errno.h>		/*错误号定义 */
#include <string.h>
#define FALSE 0
#define TRUE (!0)

#define UART_DEV "/dev/ttySAC1" //默认设备文件
#define BUAD 115200 //默认波特率
#define ONLY_SEND_TEST 1 //仅输出测试
int set_Parity(int fd, int databits, int stopbits, int parity);
void set_speed(int fd, int speed);
int main(int argc, const char *argv[])
{
	int fd;
	char uart_dev[128];
	//fd = open("/dev/ttyUSB1", O_RDWR|O_NOCTTY | O_NDELAY);
	if(argc==2){
		fd = open(argv[1], O_RDWR);
		printf("Use the uart_dev:%s\n",argv[1]);
	}else{
		fd = open(UART_DEV, O_RDWR);
	}
	if (-1 == fd) {
		perror("[ERR]open device "UART_DEV);
		return -1;
	} else {
		printf("[OK]open device %s.\n",UART_DEV);
	}
	printf("setting speed...\n");
	set_speed(fd, BUAD);
	printf("setting Parity...\n");
	set_Parity(fd, 8, 1, 'N');
	char buffer[1024] = "ls\n";
	//int Length = 10;
	int nByte;
	printf("Ready to write(send) to uart,please INPUT string and ENTER\n");
	scanf("%s",buffer);
	printf("write data...\n");
	//nByte = write(fd, buffer, Length);
	nByte = write(fd, buffer, strlen(buffer)+1);
	printf("\nwrite end,send length %d byte(s).\n",nByte);
#if ONLY_SEND_TEST
	printf("开始发送测试,一直发送字符串abcd,CTRL-C终止\n");
	int a_writetest[1]={0b10101010};
	while(1){
		write(fd,a_writetest,1);
	}
#endif
	int Len = 1024;
	char buf[1024];
	int readByte;
	int i;
	while (1) {
		//for(i=0;i<7000000;i++);
		printf("reading data...\n");
		readByte = read(fd, buf, 1);
		//tcflush(fd,TCIOFLUSH); 
		if (readByte <= 0){
			//printf("len=%d,read=%s\n",readByte, buf);
			printf("read over.len=%d\n",readByte);
			tcflush(fd,TCIOFLUSH); 
			break;
		}
		printf("len=%d,返回:%s\n", readByte,buf);
		//sleep(1);
	}
	int status=0;
	status= close(fd);
	printf("close status return: %d\n",status);

	return 0;
}

/**
 * *@brief  设置串口通信速率
 * *@param  fd     类型 int  打开串口的文件句柄
 * *@param  speed  类型 int  串口速度
 * *@return  void
 * */

int speed_arr[] = {
	B38400, B19200, B9600, B4800, B2400, B1200, B300
};

int name_arr[] = { 38400, 19200, 9600, 4800, 2400, 1200, 300
};

void set_speed(int fd, int speed)
{
	int i;
	int status;
	struct termios Opt;
	tcgetattr(fd, &Opt);
	for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
		if (speed == name_arr[i]) {
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd, TCSANOW, &Opt);
			if (status != 0) {
				perror("tcsetattr fd1");
				return;
			}
			tcflush(fd, TCIOFLUSH);
		}
	}
}/**

  *@brief   设置串口数据位，停止位和效验位
  *@param  fd     类型  int  打开的串口文件句柄
  *@param  databits 类型  int 数据位   取值 为 7 或者8
  *@param  stopbits 类型  int 停止位   取值为 1 或者2
  *@param  parity  类型  int  效验类型 取值为N,E,O,S
  */
int set_Parity(int fd, int databits, int stopbits, int parity)
{
	struct termios options;
	if (tcgetattr(fd, &options) != 0) {
		perror("SetupSerial 1");
		return (FALSE);
	}
	options.c_cflag &= ~CSIZE;
	switch (databits) {	/*设置数据位数 */
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;
			break;
		default:
			fprintf(stderr, "Unsupported data size\n");
			return (FALSE);
	}
	switch (parity) {
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;	/* Clear parity enable */
			options.c_iflag &= ~INPCK;	/* Enable parity checking */
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB);	/* 设置为奇效验 */
			options.c_iflag |= INPCK;	/* Disnable parity checking */
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;	/* Enable parity */
			options.c_cflag &= ~PARODD;	/* 转换为偶效验 */
			options.c_iflag |= INPCK;	/* Disnable parity checking */
			break;
		case 'S':
		case 's':		/*as no parity */
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:
			fprintf(stderr, "Unsupported parity\n");
			return (FALSE);
	}
	/* 设置停止位 */
	switch (stopbits) {
		case 1:
			options.c_cflag &= ~CSTOPB;
			break;
		case 2:
			options.c_cflag |= CSTOPB;
			break;
		default:
			fprintf(stderr, "Unsupported stop bits\n");
			return (FALSE);
	}
	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;
	tcflush(fd, TCIFLUSH);
	options.c_cc[VTIME] = 15;	/* 设置超时15 seconds */
	options.c_cc[VMIN] = 0;	/* Update the options and do it NOW */
	if (tcsetattr(fd, TCSANOW, &options) != 0) {
		perror("SetupSerial 3");
		return (FALSE);
	}
	return (TRUE);
}
