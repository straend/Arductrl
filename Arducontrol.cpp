#include <stdlib.h>
#include <string.h>

#include <Arducontrol.h>

Arducontrol::Arducontrol(Stream &seriali){
    serial = &seriali;
    ccmds = 0;
    cPos=0;
    recievingData = false;
}

int Arducontrol::GetInt(){
    if (curData==NULL)
        return NULL;
    return atoi(strtok(NULL, ","));
}

long Arducontrol::GetLong(){
    if (curData==NULL)
        return NULL;
    return atol(strtok(NULL, ","));
}

double Arducontrol::GetDouble(){
    if (curData==NULL)
        return NULL;
    return atof(strtok(NULL, ","));
}

char* Arducontrol::GetString(){
    if (curData==NULL)
        return NULL;
    return strtok(NULL, ",");
}

bool Arducontrol::GetBoolean(){
    if (curData==NULL)
        return NULL;
    char *a =strtok(NULL, ",");
    return (*a=='1');
}


bool Arducontrol::AddCmd(char *cmd, void(*funcp)(char*)){
    allCmds = (struct command*)realloc(allCmds, (ccmds+1) *sizeof(cmd));
    if (allCmds==NULL){
        printf("Failed allocating ram\n");
        return false;
    }
    struct command c;
    memcpy(c.cmd, cmd, CMDLEN);
    c.function = funcp;
    allCmds[ccmds]=c;
    ccmds++;
    return true;
}

void Arducontrol::StartCmd(char *cmd){
    inSendBuffer=0;
    sendBuffer[inSendBuffer++]='$';
    memcpy(&sendBuffer[inSendBuffer],cmd, BUFFERSIZE);
    inSendBuffer = strlen(sendBuffer);
}

void Arducontrol::SendInt(int value){
    sendBuffer[inSendBuffer++]=',';
    itoa(value, &sendBuffer[inSendBuffer], 10);
    inSendBuffer = strlen(sendBuffer);
}

void Arducontrol::SendLong(long value){
    sendBuffer[inSendBuffer++]=',';
    ltoa(value, &sendBuffer[inSendBuffer], 10);
    inSendBuffer = strlen(sendBuffer);
}

void Arducontrol::SendDouble(double value){
    sendBuffer[inSendBuffer++]=',';
    dtostrf(value, 4, 6, &sendBuffer[inSendBuffer]);
    inSendBuffer = strlen(sendBuffer);
}

void Arducontrol::SendString(char *value){
    sendBuffer[inSendBuffer++]=',';
    memcpy(&sendBuffer[inSendBuffer],value, BUFFERSIZE - inSendBuffer);

    inSendBuffer = strlen(sendBuffer);
}

void Arducontrol::SendBoolean(bool value){
    sendBuffer[inSendBuffer++]=',';
    if (value){
        memcpy(&sendBuffer[inSendBuffer], "1", BUFFERSIZE - inSendBuffer);
    }
    else {
        memcpy(&sendBuffer[inSendBuffer], "0", BUFFERSIZE - inSendBuffer);
    }
    inSendBuffer++;
}

// todo: add sending of checksum
void Arducontrol::EndCmd(){
    serial->println(sendBuffer);
    inSendBuffer = '\0';
}

// todo: add timeout functions
void Arducontrol::ReadSerial(){
    while(serial->available()){
        if (cPos >= BUFFERSIZE){
            cPos = 0;
            return;
        }
        char tmp = serial->read();
        if (tmp == '$'){
            cPos=0;
            buffer[cPos++] = tmp;
            recievingData = true;

        } else if (tmp == '\r'){
            buffer[cPos]='\0';
            cPos=0;
            recievingData = false;
            parseMsg();
        } else if (recievingData){
            buffer[cPos++]=tmp;
        }


    }
}

// todo: add checksumchecks
void Arducontrol::parseMsg(){
    char *cmd;
    char *data;
    data = (char*) strchr(buffer, ',') + 1;
    cmd  = (char*) strtok(buffer, ",") + 1;
    curData=data;
    executeCommand(cmd, data);
    curData=NULL;
}

void Arducontrol::executeCommand(char *command, char *data){
    unsigned char i;
    for(i=0;i<ccmds;i++){
        if(strcmp(allCmds[i].cmd, command) == 0){
            allCmds[i].function(data);
            return;
        }
    }
}

void Arducontrol::timeout(){
    cPos = 0;
    recievingData=false;

}


