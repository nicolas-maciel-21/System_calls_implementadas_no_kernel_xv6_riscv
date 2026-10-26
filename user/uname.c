#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

 struct utsname{
	char sysname[32];
	char nodename[32];
	char release[32];
	char version[32];
	char machine[32];
};

 int main(int argc, char *argv[]){
	struct utsname un;

	if (uname(&un) < 0) {
	  fprintf(2, "uname: falha ao obter informacoes do sistema\n");
	  exit(1);
	}

	printf("%s %s %s %s %s\n", un.sysname, un.nodename, un.release, un.version, un.machine);
	exit(0);
 } 
