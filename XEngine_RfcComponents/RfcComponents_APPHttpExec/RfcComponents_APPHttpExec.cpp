#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//just ansi
//g++ -std=c++17 -Wall -g RfcComponents_APPHttpExec.cpp -o RfcComponents_APPHttpExec.exe 

int main(int argc, char** argv)
{
    char tszMsgBuffer[260];
    char tszPrintBuffer[512];

    memset(tszMsgBuffer, '\0', 260);
    memset(tszPrintBuffer, '\0', 512);

    int nRet = sprintf(tszMsgBuffer, "%s %s %s", argv[1], argv[2], argv[3]);
    sprintf(tszPrintBuffer, "200\r\nhtml\r\n%d\r\n%s", nRet, tszMsgBuffer);

    printf("%s", tszPrintBuffer);
    return 0;
}