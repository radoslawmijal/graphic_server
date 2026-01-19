#include "ProgramInterpreter.hh"
#include "Preprocessor.hh"
#include "AbstractInterp4Command.hh" 
#include "Configuration.hh"
#include <sstream>
#include <memory>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

bool ProgramInterpreter::ConnectToServer(const char* addr, int port) 
{
    // if socket error
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0) {
        cerr << "Blad tworzenia gniazda!" << endl;
        return false;
    }

    // setup server address structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // wrong ip address
    if (inet_pton(AF_INET, addr, &server_addr.sin_addr) <= 0) {
        cerr << "Niepoprawny adres IP!" << endl;
        close(_socket);
        return false;
    }

    // connect to server error
    if (connect(_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Blad polaczenia z serwerem!" << endl;
        close(_socket);
        return false;
    }

    cout << "Polaczono z serwerem: " << addr << " na porcie " << port << endl;
    return true;
}

bool SendToSocket(int Sk2Server, const char* Msg)
{
    // send message to server
    size_t MsgLen = strlen(Msg);
    ssize_t NumberOfSentBytes;

    // send all bytes
    while((NumberOfSentBytes = write(Sk2Server, Msg, MsgLen)) > 0)
    {
        // check for send error
        if(NumberOfSentBytes < 0)
        {
            cerr << "Blad wysylania danych do serwera graficznego!" << endl;
            return false;
        }
        // update remaining message length and pointer
        MsgLen -= NumberOfSentBytes;
        Msg += NumberOfSentBytes;
    }
    return true;
}

bool ProgramInterpreter::ExecProgram(const char* fileName) 
{
    Preprocessor preproc;
    string processedCode = preproc.ProcessFile(fileName);
    
    // check for preprocessor errors
    if (processedCode.empty()) {
        cerr << "Blad preprocesora lub pusty plik!" << endl;
        return false;
    }

    istringstream IStrm(processedCode);
    string cmdName;

    // read commands from the stream
    while (IStrm >> cmdName) {
        
        cout << "Wczytano polecenie: " << cmdName << endl;

        std::shared_ptr<LibInterface> plugin = _libManager.FindPlugin(cmdName);
        
        // check if plugin exists
        if (!plugin) {
            cerr << "Nieznana komenda: " << cmdName << endl;
            string unknown_command; getline(IStrm, unknown_command); 
            continue; 
        }

        // create command instance
        AbstractInterp4Command *cmd = plugin->CreateInterp();
        
        // check if command instance was created successfully
        if (!cmd) {
            cerr << "Blad tworzenia instancji polecenia!" << endl;
            continue;
        }

        // read command parameters
        if (!cmd->ReadParams(IStrm)) {
            cerr << "Blad wczytywania parametrow!" << endl;
            delete cmd; 
            return false;
        }

        // execute command
        cmd->PrintCmd();

        // execute the command
        if (!cmd->ExecCmd(_scn, this)) {
            cerr << "Blad wykonania polecenia: " << cmdName << endl;
            delete cmd;
            return false;
        }
        
        // cleanup
        delete cmd;
    }

    // send close message to server
    SendToSocket(_socket, "Close\n");
    close(_socket);
    _socket = -1;

    return true;
}

