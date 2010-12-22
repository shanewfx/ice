#include "ice_reactor.h"
#include <signal.h>


namespace ICE {

Reactor::Reactor() {
    FD_ZERO(&_readFdSet);
    FD_ZERO(&_writeFdSet);
    _maxFd = 0;
};

Reactor::~Reactor() {
    HandlerMapIter it = _handlerMap.begin();
    HandlerMapIter end = _handlerMap.end();
    for(;it != end; ++it) delete it->second;
};

bool Reactor::RegisterHandler(int fd, EventHandler *handler, bool toRead, bool toWrite) {
    HandlerMapIter it = _handlerMap.lower_bound(fd);
    if(it != _handlerMap.end() && it->first == fd) {
        _handlerMap.insert(HandlerPair(fd, handler));
        if(toRead) FD_SET(fd, &_readFdSet);
        if(toWrite) FD_SET(fd, &_writeFdSet);
        if(fd > _maxFd) _maxFd = fd;
        return true;
    };
    return false;
};

void Reactor::UnregisterHandler(int fd) {
    HandlerMapIter it = _handlerMap.find(fd);
    if(it != _handlerMap.end()) {
        delete it->second;
        _handlerMap.erase(it);
        FD_CLR(fd, &_readFdSet);
        FD_CLR(fd, &_writeFdSet);
        if(fd == _maxFd) {
            if(_handlerMap.empty()) {
                _maxFd = 0;
            } else {
                _maxFd = _handlerMap.rbegin()->first;
            };
        };
    };
};

void Reactor::forbitRead(int fd) {
    FD_CLR(fd, &_readFdSet);
};

void Reactor::forbitWrite(int fd) {
    FD_CLR(fd, &_writeFdSet);
};

void Reactor::permitRead(int fd) {
    FD_SET(fd, &_readFdSet);
};

void Reactor::permitWrite(int fd) {
    FD_SET(fd, &_writeFdSet);
};

void Reactor::run() {
    struct sigaction act;
    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if(sigaction(SIGPIPE, &act, NULL) < 0) return;

    while(true) {
        if(_handlerMap.empty()) return;

        fd_set readSet = _readFdSet;
        fd_set writeSet = _writeFdSet;

        select(_maxFd + 1, &readSet, &writeSet, NULL, NULL);

        HandlerMapIter it = _handlerMap.begin();
        HandlerMapIter end = _handlerMap.end();
        for(;it != end; ++it) {
            int fd = it->first;
            EventHandler *handler = it->second;
            if(FD_SET(fd, &readSet)) {
                if(!handler->handleInput(fd, *this)) UnregisterHandler(fd);
            };
            if(FD_SET(fd, &writeSet)) {
                if(!handler->handleOutput(fd, *this)) UnregisterHandler(fd);
            };
        };
    };
};

};

