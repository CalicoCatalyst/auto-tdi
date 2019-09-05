#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

char *between(char *str, char from, char to){
while(*str && *str != from)
    ++str;//skip
if(*str == '\0')
    return NULL;
else
    ++str;

char *ret = malloc(strlen(str)+1);
char *p = ret;
while(*str && *str != to){
    *p++ = *str++;//To the end if `to` do not exist
}
*p = 0;
return ret;
}

char* concat(int count, ...)
{
    va_list ap;
    int i;

    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}
/*
Function : main
Arguments : 
*/
int main() {
    FILE *scan;
    char ipscanner[200];

    char arpLocation[] = "/usr/sbin/arp -a";
    char macAddress[] = "40:98:ad:1c:c7:92";
    

    char *target;

    printf("Hunting for device with MAC address %s\n", macAddress);

    scan = popen(arpLocation, "r");

    while (fgets(ipscanner, sizeof(ipscanner)-1, scan) != NULL) {
        if (strstr(ipscanner, macAddress) != NULL) {
            target = between(ipscanner, '(', ')');
            printf("Device found with ip: %s\n\n", target);
            break;
        }
    }
    char *command = concat(2, "export THEOS_DEVICE_IP=", target);
    printf("Creating Script with content: %s\n", command);
    printf("\nBash Script Generated. Running...\n");

    pclose(scan);
    FILE *scriptOutput = fopen("tdi.sh", "w+");
    fputs(command, scriptOutput);

    fclose(scriptOutput);
    
    return 0;
}