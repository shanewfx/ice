#ifndef __ICE_REACTOR_H__
#define __ICE_REACTOR_H__

#include <sys/select.h>
#include <map>

namespace ICE {

class Reactor;
class EventHandler {
public:
    virtual ~EventHandler() {};
    virtual bool handleInput(int fd, Reactor &reactor) = 0;
    virtual bool handleOutput(int fd, Reactor &reactor) = 0;
};

class Reactor {

/*    
    enum {
        MASK_READ = 0x1,
        MASK_WRITE = 0x2
    };
       struct eventItem {
        int fd;
        EventHandler *handler;
        int mask;
    };
*/

    typedef std::map<int, EventHandler*> HandlerMap;
    typedef HandlerMap::iterator HandlerMapIter;
    typedef std::pair<int, EventHandler*> HandlerPair;

    HandlerMap _handlerMap;
    fd_set _readFdSet, _writeFdSet;
    int _maxFd;
    void UnregisterHandler(int fd);
public:
    Reactor();
    ~Reactor();
    bool RegisterHandler(int fd, EventHandler *handler, bool toRead, bool toWrite);
    void forbitRead(int fd);
    void forbitWrite(int fd);
    void permitRead(int fd);
    void permitWrite(int fd);
    void run();
};
};

#endif //__ICE_REACTOR_H__
