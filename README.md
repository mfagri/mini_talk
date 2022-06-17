# Minitalk
communication program in the form of a client and a server
**PROJECT Instruction :**

- you have to turn in a Makefile which will compile your source files . it must not relink.
- you have to handle errors thoroughly in no way your program should quit unexpectedly (segmentation fault , bus error, double free, and so forth).
- your program mustn’t have memory leaks .
- you can have on global variable per program ( one for the client and one for the server ), but you will have to justify their use .
- in order to complete the mandatory part you are allowed to use the following function
- function’s
    
    ◦ write
    ◦ ft_printf and any equivalent YOU coded
    ◦ signal
    ◦ sigemptyset
    ◦ sigaddset
    ◦ sigaction
    ◦ kill
    ◦ getpid
    ◦ malloc
    ◦ free
    ◦ pause
    ◦ sleep
    ◦ usleep
    ◦ exit
    

### Mandatory part :

you must create a communication program in the form of a client and a server .

- the server must be started first . After its launch . it has to print it **PID**
- **the client take two parameters**
1. The server PID 
2. The string to send

- The client must send the string passed as a parameter to the server once the string has been received the server must print it .
- The server has to display the string pretty quickly , Quickly means that if you think it takes too long then it is probably too long

### tip:

1 s for displaying 100 characters is way too much !

- your server should be able to receive string from serval clients in row without needing to restart.
- The communication between your client and your server has to be done only using UNIX signals .
- You can only use these two signals : **SIGUSR1 SIGUSR2**
# code :

## 1st Step make server receive a signal from client :

in MINItalk there are two processes/programs : the client and the server . what the project consists of is the communication between both processes more precisely . more precisely the client process sends a message string to the process whose id was given as parameter -server

The first logical step i took while doing the project was to make the first connection between both programs so i made client send a signal to server sigusr1 by using the kill() command 

by using the function getpid() i managed to get the process ID of the server program i want to send a signal to. this is what happens when you execute server :

```c
-> Minitalk git :(main)  ~ ./server
PID: 12750 
```

the client will then receive the server’s pid and by using the kill() command will send a signal - SIGUSR1 to the server you exectue client like this :

 

```c
-> MiniTalk git : (main) ./client 12750
```

then just like that the server receives a signal and outputs  a message to the shell 

```c
-> Minitalk git : (main) ~ ./server
PID :12750
signal 10 received.
```

what happens is that client.c file use signal() and when it receives SIGUSR1 it sends it to a function called handler_sigusr1 that outputs a success message . The server program then continues running , waiting for more  SIGUSR1 signals until the user stops it - CTRl + c  .

if instead of an infinite while loop the program used the pause() function . After receiving and handling the signal the server would terminate on its own .
CODE : 

```c
//client.c
int main(int argc,char **argv)
{
	int pid;
	
	if (argc!= 2)
		{
			ft_printf("client : invalid arguments\n");
				exit(EXIT_FAILURE);
		}
	pid = atoi(argv[1]);
	kill(pid, SIGUSR1);
	return(0);
}
```

```c
//server.c
void handler_sigusr1(int signum)
{
	printf("signal %d received.\n", signum);
}
int main(void)
{
	pid_t pid;
	pid = getpid();
	ft_printf("PID:%d",pid);
	signal(SIGUSR1, handler_igusr1);
	while (1)
		pause();
} 
```

## 2nd step : convert ASCII character to Binary character

ASCII stands for American standard Code for information interchange and its a way to represent text in a computer . The ASCII table represents all the 127 ASCII characters and their values . That means that every character is represented by number ..

The way two programs can communicate via signals more precisely SIGUSR1 and SIGUSR2 - is to make them represent a binary value each  in this case we consider that SIGUSR represent 1 and SIGUSR2  represents 0. so when we send a SIGUSR1 signal we are sending 1 to server and by sending  SIGUSR2 signal we are sending 0 .

This second step is focused on getting all the bits from a character - 8 bits in ASCII - this means that server will need to receive 8 signals from client for every ASCII character .

so guys i did use bits shifting operation i do really want u to search about it before u read that it can be a little be complicated so , first of all  we have 3 operator **& | ^** : the and or and Xor 
soo every want of them has her own operations like for example & :

> 0 & 1 = 0;
1 & 1 = 1;
0 & 0 = 0;
> 

Or is way more different :

> 0 | 1  = 1 
0| 0  = 0
1 | 1  = 1
> 

Xor :

> 1 ^ 1 : 0
1 ^ 0 : 1
1 ^ 1 : 0
> 

so this the operation that u will see in this document let’s see so we want send character as a signal so first of all that character should be in binary we will use & , for example we have the **a** character  ****in binary which is 01000001 it will be compared with 128 which is 10000000 ,This operator will return a **0,** if the first bit of that character is zero, and it will return **128** if the first bit is one, then the program won’t use 128 instead he will use 64 that is (01000000) this will enable the program to get the second left character .

```c
//client.c 
#include "minitalk.h"

void	convert_to_binary(int pid, char *s)
{
	int	i;
	int	bit;

	i = 0;
	if (!s)
		exit(1);
	while (1)
	{
		bit = -1;
		while (++bit < 9)
		{
			if (s[i] & (128 >> bit))
			{
				if (kill(pid, SIGUSR1) == -1)
					exit(1);
			}
			else
				if (kill(pid, SIGUSR2) == -1)
					exit(1);
			usleep(300);
		}
		if (!s[i])
			break ;
		i++;
	}
}
```

## 3rd step how to convert from binary to ascii :

so first. of all client can send signals to server which he can receive it ,and now since the client is able to decide whose to send sigusr1 or sigusr2 it’s up to the server to convert those character back to an ascii character .

so now we’re going to work with this two bitwise operator **^ and |**  

so now instead of starting with the value 0 , we will initialize a variable with 0xFF 255 in decimal which is : 11111111 in binary max value  .

so if he found sigusr2 (1) we will use the or operator else ^ will be used 

and after that when the bit = 8 we will put the character and reinitialize the two variables .

our code will be like:

```c
//server.c 

void	binary_to_ascii(int signum, siginfo_t *info, void *context)
{
	static int	g_pid = 0;
	static char	c = 0xFF;
	static int	bit = 0;	

	(void) context;
	if (g_pid != info->si_pid)
	{
		c = 0xFF;
		bit = 0;
		g_pid = info->si_pid;
	}
	if (signum == SIGUSR1)
		c |= 0x80 >> bit;
	else if (signum == SIGUSR2)
		c ^= 0x80 >> bit;
	if (bit++ == 8)
	{
		if (!c)
			fmsg(g_pid, SIGUSR1);
		ft_putchar(c);
		bit = 0;
		c = 0xFF;
	}
}
```

## 4 step : sending string from client to server :

 so this the final step it’s not a big deal we just should mesure the len of the string that we want send it and in the send signals function we’re going to increase until it equal it it’s an easy thing.
 
 ##### Don't forget to report to me the Errors in the project and also your suggestions about it .... to make it better 😉
 <a href="https://github.com/mfagri/MiniShell" target="_blank"><img src="https://img.shields.io/badge/github-000000?style=flat-square&logo=Github&logoColor=white"/></a>
[![Gmail Badge](https://img.shields.io/badge/-Gmail-d14836?style=flat-square&logo=Gmail&logoColor=white&link=mailto:marouane.fagri1@gmail.com)](mailto:marouane.fagri1@gmail.com)
[![Twitter Badge](https://img.shields.io/badge/-Twitter-1c89f0?style=flat-square&logo=twitter&logoColor=white&link=https://twitter.com/FagriMarouane/)](https://twitter.com/FagriMarouane/) 
[![Twitter Badge](https://img.shields.io/badge/-Facebook-1c89f0?style=flat-square&logo=facebook&logoColor=white&link=https://www.facebook.com/marwan.fagri.7/)](https://www.facebook.com/marwan.fagri.7/)
