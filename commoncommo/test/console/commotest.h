#ifndef COMMOTEST_H_
#define COMMOTEST_H_

#include "commo.h"
#include "contactuid.h"
#include "commologger.h"
#include "cotmessageio.h"
#include "missionpackage.h"
#include <set>
#include <map>
#include <string>
#include <deque>
#include <vector>
#include <stdio.h>
#include <Mutex.h>

namespace atakmap {
namespace commoncommo {
namespace test {

class NetIfaceInfo
{
public:
    typedef enum {
        IT_STREAM,
        IT_BCAST,
        IT_INBOUND,
        IT_TCPIN
    } Type;
    NetIfaceInfo(Type type, NetInterface *iface) : type(type), iface(iface) {}
    
    Type type;
    NetInterface *iface;
};

class CommoTest : public CoTMessageListener, public GenericDataListener,
                public ContactPresenceListener,
                public CommoLogger, public InterfaceStatusListener,
                public MissionPackageIO, public CoTSendFailureListener,
                public SimpleFileIO, public CloudIO
{
public:
    CommoTest();
    virtual ~CommoTest();

    void run(int argc, char *argv[]);

    virtual void sendCoTFailure(const char *host, int port, const char *errorReason);
    virtual void cotMessageReceived(const char *msg, const char *rxIfaceEndpointId);
    virtual void genericDataReceived(const uint8_t *data, size_t length, const char *rxIfaceEndpointId);
    virtual void contactAdded(const ContactUID *c);
    virtual void contactRemoved(const ContactUID *c);

    virtual void log(Level level, const char *message);

    virtual void interfaceDown(NetInterface *iface);
    virtual void interfaceUp(NetInterface *iface);
    virtual void interfaceError(NetInterface *iface, netinterfaceenums::NetInterfaceErrorCode errCode);
    
    
    virtual MissionPackageTransferStatus missionPackageReceiveInit(
                char *destFile, size_t destFileSize,
                const char *transferName, const char *sha256hash,
                uint64_t expectedByteSize,
                const char *senderCallsign);
    virtual void missionPackageReceiveStatusUpdate(const MissionPackageReceiveStatusUpdate *update);
    virtual void missionPackageSendStatusUpdate(const MissionPackageSendStatusUpdate *update);
    virtual CoTPointData getCurrentPoint();
    virtual void createUUID(char *uuidString);
    

    virtual void fileTransferUpdate(const SimpleFileIOUpdate *update);

    virtual void cloudOperationUpdate(const CloudIOUpdate *up);

private:
    std::string myUID;
    std::string myCallsign;
    std::string baseDir;
    time_t nextCmdLineTime;
    int sendFreqMillis;
    int millisToNextSend;

    std::map<NetInterface *, std::string> ifaceDescs;
    std::vector<std::vector<NetIfaceInfo> > ifaceList;
    std::vector<CloudClient *> cloudClients;
    std::set<std::string> contactList;
    PGSC::Thread::Mutex contactMutex;
    bool contactsMenuDirty;

    PGSC::Thread::Mutex loggerMutex;
    Commo *commo;
    FILE *logFile;
    FILE *messageLog;

    std::deque<std::string> logQueue;
    std::deque<std::string> msgQueue;
    PGSC::Thread::Mutex logMutex;
    PGSC::Thread::Mutex msgMutex;
    bool msgDirty;
    bool logDirty;

    int contactH;
    int contactW;
    int textH;
    int curW, curH;



    void sendSA(bool mcast, const char *uid = NULL);
    void sendMarker(double lat, double lon);
    std::string makeChat(std::string &destUid); 
    void do_help(const char *name);

};


}
}
}


#endif /* COMMOTEST_H_ */
