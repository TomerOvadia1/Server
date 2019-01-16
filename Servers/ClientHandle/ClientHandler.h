//
// Created by sagy on 1/6/19.
//

#ifndef SERVER_CLIENTHANDLER_H
#define SERVER_CLIENTHANDLER_H

#include <iostream>

class ClientHandler {

public:

    virtual void handleClient(int port ) = 0;
    virtual ~ClientHandler() {}
};


#endif //SERVER_CLIENTHANDLER_H
