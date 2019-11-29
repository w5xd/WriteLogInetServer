/* This program is distributed in the hope that it will be useful, but WITHOUT ANY
** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
** PARTICULAR PURPOSE. */


#ifdef WIN32
#define WINVER 0x0501
#ifdef _DEBUG
#include <afx.h>	// this turns on memory tracking on the Win32 DEBUG build
#endif
#endif

#include <iostream>
#include <set>
#include <thread>
#include <functional>
#include <condition_variable>
#include "ServerState.h"
#include "envH.h"
#include "C1Stub.h"
#include "C2Stub.h"


// These are the only source files 
// NOT generated by, or copied from, the gSOAP tools:
//    WritelogInetServer.cpp (this file)
//    C1impl.cpp
//    C2impl.cpp
//    ContestQsos.wsdl and ContestQsos2.wsdl
//  The others are generated this way:
//
// pushd C1
//   gsoap-2.8\gsoap\bin\wsdl2h -o ../ContestQsos.h -ncontest -Ic:\dev\gsoap-2.8\gsoap\import ContestQsos.wsdl
//   cd ..\C2
//   gsoap-2.8\gsoap\bin\wsdl2h -o ../ContestQsos2.h -ncontest2 -Ic:\dev\gsoap-2.8\gsoap\import ContestQsos2.wsdl
//   popd
// soapcpp2 -L -S -IC:\dev\gsoap-2.8\gsoap\import -x -w -n -qC1 ContestQsos.h
// soapcpp2 -L -S -IC:\dev\gsoap-2.8\gsoap\import -x -w -n -qC2 ContestQsos2.h
// soapcpp2 -L -S env.h -penv
//

/* What is this C1 with ContestQsos.wsdl and C2 with ContestQsos2.wsdl all about?
** WriteLog V10 used an old Microsoft SOAP technology where ContestQsos.wsdl originated.
** WriteLog V11 supports that, but alternatively has a .net equivalent, but I was unable
** to figure out how to make a .NET client serve that original ContestQsos.wsdl so
** ContestQsos2.wsdl was born. It has exactly the same features, but different SOAP
** namespace goop. 
**
** So we need a server that can talk both dialects. Hense, C1 and C2.
**
*/

static const int MAX_CLIENT_BACKLOG = 20;

static bool keepRunning = true;

// static functions
static int http_get(struct soap *soap);

extern SOAP_NMAC struct Namespace C1_namespaces[];
extern SOAP_NMAC struct Namespace C2_namespaces[];

class CombinedNamespaces
{
    public:
        std::vector<struct Namespace> combinedNamespaces;

    public:
        void AddNamespaces(struct Namespace *pNamespace)
        {
            while (pNamespace->id != 0)
            {
                std::set<std::string>::iterator exists = ids.find(pNamespace->id);
                if (exists == ids.end())
                {
                    combinedNamespaces.push_back(*pNamespace);
                    ids.insert(pNamespace->id);
                }
                pNamespace++;
            }
        }
        void finish()
        {
            static struct Namespace end = {0,0,0,0};
            combinedNamespaces.push_back(end);
            ids.clear();
        }

    private:
        std::set<std::string> ids;
};

/* ProcessingQueue
** pool a fixed number of threads to process incoming soap requests.
*/
class ProcessingQueue
{
public:
    ProcessingQueue(unsigned numThreads) 
            :  m_shutdown(false)
            , m_threadsReady(0)
    {   // start the threads up front.
        while (numThreads-- > 0)
            m_threads.push_back(std::thread(std::bind(&ProcessingQueue::process, this)));
    }
    ~ProcessingQueue() {
        shutdown();
    }
    void addItem(std::shared_ptr<soap> p)
    {
        lock_t l(m_mutex);
        m_queue.push_back(p);
        m_condIn.notify_one();
    }
    void waitToAccepMore() const
    {
       lock_t l(m_mutex);
       while (m_threadsReady == 0)
           m_condOut.wait(l);
    }
    void shutdown()
    {
        {
            lock_t l(m_mutex);
            m_shutdown = true;
            m_condIn.notify_all();
        }
        for (auto &t : m_threads)
            t.join();
        m_threads.clear();
    }
protected:
    typedef std::unique_lock<std::mutex> lock_t;
    void process()
    {   
        for (;;)
        {
            std::shared_ptr<soap> p;
            {
                lock_t l(m_mutex);
                m_condOut.notify_one();
                m_threadsReady += 1;
                while (!m_shutdown && m_queue.empty())
                    m_condIn.wait(l);
                m_threadsReady -= 1;
                if (m_shutdown)
                    return;
                p = m_queue.front();
                m_queue.pop_front();
            }
            if (soap_serve(p.get()) != SOAP_OK) // process RPC request
                soap_print_fault(p.get(), stdout); // print error
        }
    }

    mutable std::mutex m_mutex;
    mutable std::condition_variable m_condIn;
    mutable std::condition_variable m_condOut;
    std::deque<std::shared_ptr<soap>> m_queue;
    std::vector<std::thread> m_threads;
    int m_threadsReady;
    bool m_shutdown;
};

int main(int argc, char **argv)
{
    std::string userFile = "NONE";
    int portNumber = 8001;
    ContestQsos state;
    bool l6Flag = false;
    unsigned numThreads = 1;
    bool bindOnlyLocalhost = true;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-h")
        {
            std::cout << std::endl
                << "usage: WritelogInetServer [-h] [-u <userFile>] [-p <portNumber>] [-v]" << std::endl << std::endl
                << "-v " << std::endl
                << "   turns on the verbose flag that prints sent/received QSO counts." << std::endl << std::endl
                << "-h " << std::endl
                << "   prints this help message" << std::endl << std::endl
                << "-u <userFile>" << std::endl
                << "   <userFile> is the path to a username/password file." << std::endl
                << "   The file is formatted with one username/password pair per line." << std::endl
                << "   Separate the username from the password with one space character." << std::endl
                << "   These become the legal entries from WriteLog users in their " << std::endl
                << "   Connect to Internet Logbook dialog." << std::endl << std::endl
                << "-p <portNumber>" << std::endl
                << "   This program requires a TCP port. The default without -p is 8001." << std::endl
                << "   The number must be in the range of 1 through 65534. Different operating" << std::endl
                << "   systems, internet providers, and routers will allow or not allow various" << std::endl
                << "   port numbers through." << std::endl
                << "-t <number of threads>" << std::endl
                << "   number of threads to use to serve. Limited to 10 max. Default is 1." << std::endl
                << "-L6 " << std::endl
                << "   turns on the limit-labels-to-6-characters flag, which then rejects invalid input" << std::endl
                << "   from WriteLog versions earlier than 10.76." << std::endl
                << "-bindAll" << std::endl
                << "   enables internet connections on all IP addresses for this machine."
                << std::endl;
            return 1;
        }
        else if (arg == "-p")
        {
            if (++i >= argc)
            {
                std::cout << "-p must be followed by a port number." << std::endl;
                return 1;
            }
            portNumber = atoi(argv[i]);
            if (portNumber == 0)
            {
                std::cout << "portNumber " << argv[i] << " is not legal." << std::endl;
                return 1;
            }
        }
        else if (arg == "-u")
        {
            if (++i >= argc)
            {
                std::cout << "-u must be followed by a file name." << std::endl;
                return 1;
            }
            userFile = argv[i];
            if (!state.readUserFile(userFile))
            {
                std::cout << "Could not open " << userFile << std::endl;
                return 1;
            }
        }
        else if (arg == "-v")
        {
            state.setVerboseFlag(true);
        }
        else if (arg == "-L6")
        {
            state.setL6Flag(true);
            l6Flag = true;
        }
        else if (arg == "-t")
        {
            if ((++i >= argc) || !isdigit(argv[i][0]))
            {
                std::cout << "-t must be followed by number of threads." << std::endl;
                return 1;
            }
            numThreads = atoi(argv[i]);
            if (numThreads > 10)
                numThreads = 10;
            if (numThreads == 0)
                numThreads = 1; // we run really, really slowly otherwise.
        }
        else if (arg == "-bindAll")
        {
            bindOnlyLocalhost = false;
        }
        else
        {
            std::cout << "Unrecognized argument: " << arg << std::endl;
            return 1;
        }
    }
    std::cout << "WritelogInetServer -h for help" << std::endl;
    std::cout << "Using port number " << portNumber << " and user file: " << userFile << std::endl;
    if (l6Flag)
        std::cout << "Limiting Network frequency labels to 6 characters for WriteLog version 10.75 and earlier." << std::endl;

    CombinedNamespaces combinedNamespaces;
    combinedNamespaces.AddNamespaces(C1_namespaces);
    combinedNamespaces.AddNamespaces(C2_namespaces);
    combinedNamespaces.finish();

    soap service;
    soap_init(&service);
    service.user = (void *)&state;
    service.accept_timeout =
        service.recv_timeout =
        service.send_timeout = 10;
    service.max_keep_alive = 100;
    service.fget = &http_get; 
    service.namespaces = &combinedNamespaces.combinedNamespaces[0];
    const char *localHost = "localhost";
    if (!bindOnlyLocalhost)
        std::cerr << "Binding to the IP address of this host" << std::endl;
    SOAP_SOCKET m; // master 
    m = soap_bind(&service, 
        bindOnlyLocalhost ? localHost : 0, 
        portNumber, MAX_CLIENT_BACKLOG);
    if (!soap_valid_socket(m))
        soap_print_fault(&service, stdout);
    else
    {
        std::unique_ptr<ProcessingQueue> pq(new ProcessingQueue(numThreads));
        for (int i = 1; ; i++)
        {
            if (!keepRunning)
                break;
            pq->waitToAccepMore();
            SOAP_SOCKET s = soap_accept(&service);
            if (!soap_valid_socket(s))  continue; // keep running until we get shutdown
            if (state.verbose())
            {
                std::cout << "Accepted connection from IP=" <<
                    (service.ip>>24 & 0xFF) << "." <<
                    (service.ip>>16 & 0xFF) << "." <<
                    (service.ip>>8 & 0xFF) << "." <<
                    (service.ip & 0xFF) << std::endl << std::flush;
            }
            // copy service and assign ownership
            std::shared_ptr<soap> cpy(
                soap_copy(&service), 
                []/*deleter*/(soap *t)
                    {   
                        soap_destroy(t);
                        soap_end(t);
                        soap_done(t);
                        free(t);
                    });
            if (!cpy)
            {
                std::cerr << "Failed to copy soap for thread!" << std::endl;
                break;
            }
            pq->addItem(cpy);
        }
        pq->shutdown();
    }
    soap_done(&service); // close master socket and detach environment
    return 0;
}

// envServer.cpp must be compiled WITH_NOSERVEREQUEST
// so that THIS one is the only one at link time.
SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap *soap)
{
    // We are really two different soap servers. Try
    // one server and if it doesn't have the method,
    // try the other. C1 is WL v10, and C2 is WL v11
    int ret = C1::C1_serve_request(soap);
    if (ret == SOAP_NO_METHOD)
        return C2::C2_serve_request(soap);
    return ret;
}

// http_get***********************************************************************************************

// http_get is called by gSOAP on HTTP GET message
// We will return any file that happens to match, but filter out ".." from the URI
int http_get(struct soap *soap)
{
    if (!appState(soap)->useridAndPswdOK(soap->userid, soap->passwd))
        return 401; // not authorized

    std::string filePath = ".";
    filePath += soap->path;
    if (filePath.find("..") != filePath.npos) // .. chars not allowed
        return 404;

    FILE *fd = fopen(filePath.c_str(), "rb"); // open WSDL file to copy
    if (!fd) return 404; // return HTTP not found error

    if (filePath.find("SHUTDOWN") != filePath.npos)
    {
        std::cout << "User requested file " << filePath << 
            " and we are shutting down" << std::endl;
        keepRunning = false;	// show down the main server loop
    }

    soap->http_content = "text/xml"; // HTTP header with text/xml content
    soap_response(soap, SOAP_FILE);
    for (;;)
    {
        size_t r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd);
        if (!r)
            break;
        if (soap_send_raw(soap, soap->tmpbuf, r))
            break; // can't send, but little we can do about that
    }
    fclose(fd);
    soap_end_send(soap);
    return SOAP_OK;
} 
