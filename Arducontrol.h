#include <Arduino.h>
#define BUFFERSIZE  50
#define CMDLEN      10

class Arducontrol
{
struct command{
    char cmd[CMDLEN];
    void (*function)(char *);
};

private:
    Stream          *serial;
    struct command  *allCmds;
    unsigned char   ccmds;
    char buffer[BUFFERSIZE];
    unsigned char cPos;
    char sendBuffer[BUFFERSIZE];
    unsigned char inSendBuffer;
    char *curData;
    bool recievingData;
    void parseMsg();
    void executeCommand(char *command, char *data);
    void timeout();


public:
    Arducontrol(Stream &seriali);
    int     GetInt();
    long    GetLong();
    double  GetDouble();
    char*   GetString();
    bool    GetBoolean();


    bool AddCmd(char *cmd, void (*function)(char *));

    void StartCmd(char *cmd);
    void SendInt(int value);
    void SendLong(long value);
    void SendDouble(double value);
    void SendString(char *value);
    void SendBoolean(bool value);
    void EndCmd();

    void ReadSerial(void);
};
