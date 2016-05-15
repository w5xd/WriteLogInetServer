#ifndef SERVERSTATE_H
#define SERVERSTATE_H

#include <map>
#include <deque>
#include <vector>
#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include "stdsoap2.h"

// performance tuned paramters
static const int MAX_QSOS_IN_UPDATE = 200;
static const int MAX_KEY_LENGTH = 19; 
static const int RIG_FREQ_TIMEOUT = 5 * 60;

/*
** November, 2001--original JAVA version
** November, 2009--C++ port using gSOAP
**
** Wayne E. Wright, W5XD
** Houston, Texas
**
**
** This is the main class for the Contest QSO server. Its public methods are all
** "synchronized" such that it must finish a request for any given client before
** starting on the next client.
**
** This class maintains a QSO logbook of QSO objects. Once it has been given a QSO,
** it NEVER lets it go (so its very important that clients do not send garbage,
** because the only way to remove it is to stop this web service and restart it.)
**
** The logbook is in the instance variable HashMap "id2QsoTable". Clients are
** required to assign a unique key to each QSO, and id2QsoTable maintains a
** map from those keys to the QSO values themselves. The key string must have
** a specific format. This format is not used in this server, but WriteLog
** uses the key to display the "network letter", and the "station id" for
** each QSO.
**
*********************
** Required format for a QSO key:
**		character number
**			0					Uppercase letter from 'A' to 'Z'
**			1 through 8			Hex digits, 0-9 or A-F. No other restrictions
**
**			9 through 12 		Hex digits, but may NOT all be 0 (zero)
**
**			13 through 18		Call sign of the originating station.
**                              Can be 6 or fewer characters, but must be at 
**								least one character. That is, the length of
**								the key string MUST be at least 14 characters.
**
**			If any client sends a key longer than 19 characters, this server
**			truncates the key.
**
*********************
**	Staying in sync	
**
** A client can poll for changes in the logbook since his last poll. He does
** this by using the LogState member of LogSummary and QsoUpdate. (Both mean
** the same thing--some web functions return one structure, some the other.)
**
** The LogState is a counter of all the QSO transactions that have happened
** to the server since it started. Adding a new QSO counts as one, and sending
** an update to an existing QSO also counts as one. A client keeps track of
** where he is in LogState space so that he can download only the QSOs he
** needs.
**
**
** Startup.
**
** The client first calls AddAndGetLogSummary to gather up all the.
** the IDs of all the QSOs in this logbook, It limits the return to 1000
** at a time.
**
** The client should download all the QSOs in the LogSummary using 
** getQsosByKeyArray. That function comes with a new LogState, but the 
** client can't use that LogState for much, because it reflects QSOs added
** to the log since the LogSummary.
**
** Next, the client should call AddAndGetLogSummary again, but this time
** sending QSOs it has, but did not appear in the original LogSummary. And
** The client should also set OldState to match the received LogState from
** the original LogSummary, and so can receive a list of IDs of QSOs that
** the client has not yet seen. The client should send no more than 100 QSOs
** in any one batch to the server. One reason is that the length of the messages
** to the server should not be extremely large--the server may simply reject
** the entire message. Another reason happens when the server restarts. It
** doesn't keep a disk copy of QSOs, but instead starts empty. There is no
** need for all the clients to resend all the QSOs to the server--each client
** should send blocks of 100, check the IDs it gets back, and remove those
** IDs from the list it plans to send to the server.
**
** Polling
**
** Once the client has gotten its LogState in sync with the server, it need
** only periodically call addAndGetQsos. The client will recognize a server
** restart by noticing he receives a null back instead of a QsoUpdate--the
** server does this when a client asks for a LogState that appears to be
** in the future. When a client gets this null, it must go all the way 
** back to the beginning--it must assume the server has restarted, and
** ask for a LogSummary using a -1 OldState and continue from there.
** 
**
*********************
**  EDITING QSOs already in the log.
**
** The client sends the edited QSO to the server using the same ID the QSO was
** originally assigned. However, the server uses two additional fields to decide
** what to do with this newly received QSO:
**
** If the Version on the new one is less than the Version on the one already in
** the log, the logbook ignores the new one.
**
** If the Version on the new one is greater than the one in the log, the logbook
** replaces the old QSO in the log with the new one.
**
** If the Version on the two match, then the UpdatedBy field of the two is examined.
** If they match, the logbook ignores the new. But if they are different, the 
** logbook replaces the old with the new, AND it increments the version number.
** The incremented version number eventually causes EVERYONE to have to redownload
** this edited QSO. The reason is that matching versions, but different UpdatedBy
** fields means that the QSO was edited simultaneously in two places at once. The
** logbook adopts the last one it sees, but has to have a way to tell everyone
** about the change.
**
** Client handling of QSO Versions and UpdatedBy
**
** A client gets to set the initial Version and Updated by field for all new QSOs.
** And it should always set them to zero, and an empty string, respectively. When
** a client changes a QSO it has already sent, it increments Version, and sets
** UpdatedBy to its own unique ID (WriteLog uses the call of the originating station.)
**
** When a client receives a QSO whose ID matches one it already has in its log,
** it should check the Version (but can ignore the UpdatedBy). If the Version
** matches, the client can ignore it. If its higher, the client must adopt the
** new QSO values as its own, replacing its entries with those received.
**
**
***********************
** Variable exchange fields
**
** This server handles the possible variation in exchange fields in a simple, but
** flexible way. Before sending any QSO values, a client must make a single call
** to: ColumnNamesToIndices
**
** The names it must send are the ADIF names (see the ADIF standard at 
** http://www.hosenose.com) for the columns it sends. The client sends an
** array of ADIF names, and the server responds with a a corresponding
** array of integers. These integers are the indices into the Qsoparts array
** of strings that goes with every QSO. Note that the returned array of
** integers may require the client to deal with "holes" in the Qsoparts
** array of each QSO. For example, If a client sends column names "RST_SENT"
** and "RST_RCVD" to ColumnNamesToIndices, it might get back, the array 
** {3, 0}, for example. That means the client must pack the RST_RCVD as
** the first string in qsoparts (the "0" in {3, 0}), and it must send
** an empty string at index 1 and index 2 of qsoparts, and it must pack
** the RST_SENT at index 3.
** When it receives a qsopart, the client must ignore any strings it
** receives at qsoparts[1], qsoparts[2], and any more entries beyond
** the one at qsoparts[3]. 
** 
*/

/* forward references 
**
*/
namespace C1 {
     class contest2__Qso;
}
namespace C2 {
     class contest25__Qso;
}
void SetQsoParts(const std::vector<std::string> &other, C1::contest2__Qso *q);
std::vector<std::string> GetQsoParts(const C1::contest2__Qso &q);
void SetQsoParts(const std::vector<std::string> &other, C2::contest25__Qso *q);
std::vector<std::string> GetQsoParts(const C2::contest25__Qso &q);


/*
** The templated members of Qso and RigFrequency are made to copy to/from
** the gSOAP-generated classes and the classes defined here. All the
** classes have the same member names (and that is assumed by the template
** defintions) but because they are generated by tools, are layed out
** differently.
*/

// Data structure to use to store QSO state in this server
class Qso
{
public:
    // construct from SOAP serialized QSO
    template <class T>
    Qso(const T &soapQ) :
      time64H(soapQ.time64H),		  time64L(soapQ.time64L),
          xmitFreq(soapQ.xmitFreq),		  recvFreq(soapQ.recvFreq),
          band(soapQ.band),
          station(soapQ.station),
          mode(soapQ.mode),
          dupe(soapQ.dupe),
          serial(soapQ.serial),
          version(soapQ.version),
          idKey(soapQ.idKey),
          updatedBy(soapQ.updatedBy),
          qsoparts(GetQsoParts(soapQ))
      {}

      const int time64H;		const int time64L;	// Win32 FILETIME
      const double xmitFreq;	const double recvFreq;	// KHz
      const int band;	// as duped. 0 is first band, 1 is second and so forth
      const std::string station;	// aka CALLSIGN
      const int mode;	// 0x31 ('1') is LSB, '2' USB, '3' CW,
					    // '4' FM , '5' AM , '6' FSK
      const int dupe;	// 32 is not-a-dupe (ascii space). 'D' is 0x44 and is a dupe. 'B' is 0x42 and is not-in-any-band
      const int serial;	
      const std::vector<std::string> qsoparts;	
      /*
      ** Version, and UpdatedBy are not part of the QSO itself.
      ** Instead, they are book keeping devices used in
      ** the logbook.
      **
      ** A QSO is always created with a Version of zero.
      ** Any client can modify any QSO's fields, and if it does,
      ** it sends it back to the log book with an incremented
      ** version number, and an UpdatedBy set to his own unique ID.
      ** That version/updatedby can be used by other clients to 
      ** decide whether they want to re-read that QSO.
      **
      */
      const int version;	
      const std::string idKey;	// assigned by client. needs to be unique across the network
      const std::string updatedBy;	

      // copy contents to a SOAP version of a QSO
      template <class T>
      void copy(T *q, soap *s) const
      {
          q->band = band;
          q->dupe = dupe;
          q->idKey = idKey;
          q->mode = mode;
          q->recvFreq = recvFreq;
          q->serial = serial;
          q->station = station;
          q->time64H = time64H;
          q->time64L = time64L;
          q->updatedBy = updatedBy;
          q->version = version;
          q->xmitFreq = xmitFreq;
          SetQsoParts(qsoparts, q);
      }
};

// Server's representation of Rig Frequencies it stores
class RigFrequency
{
public:
    // construct from a SOAP version
    template <class T>
    RigFrequency(const T &r, bool limitLabel6) : 
      station(r.station), networkLetter(r.networkLetter), 
          label(r.label.substr(0,limitLabel6 ? 6 : r.label.npos)),
          rigNumber(r.rigNumber), xmitFreq(r.xmitFreq), recvFreq(r.recvFreq),
          mode(r.mode)
      {
          ::time(&m_time);
      }
      RigFrequency() : networkLetter(0), rigNumber(0), xmitFreq(0), recvFreq(0), mode(0), m_time(0){}
      const std::string station;	
      const int networkLetter;	
      const std::string label;	
      const int rigNumber;	
      const double xmitFreq;	
      const double recvFreq;	
      const int mode;	
      bool AmOlderThanLimit() const 
      {
          time_t now;
          ::time(&now);
          return (now - m_time) > RIG_FREQ_TIMEOUT;
      }
      std::string keyString() const
      {
          char buf[64];
          sprintf(buf,"%d%d", networkLetter, rigNumber);
          return station+buf;
      }
      // copy contents to a SOAP version
      template <class T>
      void copy(T &r) const
      {
          r.station = station;
          r.networkLetter = networkLetter;
          r.label = label;
          r.rigNumber = rigNumber;
          r.xmitFreq = xmitFreq;
          r.recvFreq = recvFreq;
          r.mode = mode;
      }
protected:
    time_t m_time;
};

/*
** ContestQsos class
**
** This class holds the state of the server between soap calls.
** It has a "log" of QSOs, a sessionId, and a set of rig frequencies.
** ... and all the rpc methods.
*/
class ContestQsos
{
    typedef std::shared_ptr<Qso> QsoPtr;
    typedef std::shared_ptr<QsoPtr> QsoPtrPtr;
    typedef std::lock_guard<std::mutex> lock_t;
public:
    ContestQsos() 
       : m_verbose(false)
       , m_limitLableTo6(false)
    {
        time_t now;
        ::time(&now);
        char buf[20];
        sprintf(buf, "%d", (int)now);
        m_sessionId = buf;
    }

    std::string GetSessionId()
    {	return m_sessionId;	} // no locks cuz session never changes

    bool SessionOK(const std::string &s)
    {   return s == m_sessionId;	}

    bool useridAndPswdOK(const char *userid, const char *password)
    {   
        lock_t l(m_pwdMutex); 
        if (usernameToPassword.size() == 0)
            return true;	// no password file--anything goes
        if (!userid) return false; // no user provided

        // lookup the password
        std::map<std::string, std::string>::iterator itor =
            usernameToPassword.find(userid);
        if (itor == usernameToPassword.end())
            return false;	// username not found
        if (!password)	// no password provided
            return itor->second.length() == 0;
        return itor->second == password;
    }

    /*
    ** AddAndGetLogSummary
    **
    ** returns a structure containing:
    **	(a) the value of the state variable right now.
    **	(b) an array of ID/Version pairs--Qsos in log
    */
    template <class T1, class T2>
    int AddAndGetLogSummary(
        soap *s,
        std::vector<T1 * >&_QsoAddArray, 
        int OldState, 
        int MaxRequested, 
        T2 *(*createF)(soap *),
        std::vector<T2 * > &pOut,
        int &logState)
    {
        lock_t l(m_mutex); 
        if (m_verbose)
            std::cout << "AddAnGetLogSummary" << std::endl;
        int CurSize = LogbookOrderedByTransaction.size();
        int QsosInUpdate = CurSize - OldState;
        if (QsosInUpdate < 0)
            QsosInUpdate = 0;
        if (QsosInUpdate > MaxRequested)
            QsosInUpdate = MaxRequested;

        for (int i = 0; i < QsosInUpdate; i += 1)
        {
            T2 *qso = (*createF)(s);
            pOut.push_back(qso);
            if (OldState >= (int)LogbookOrderedByTransaction.size())
                return soap_receiver_fault(s, "OldState out of range", "");
            QsoPtr q = *LogbookOrderedByTransaction[OldState];
            qso->id = q->idKey;
            qso->version = q->version;
            qso->updatedBy = q->updatedBy;
            OldState += 1;
        }
        bool SentUpToDate = (OldState == LogbookOrderedByTransaction.size());
        AddQsos(_QsoAddArray);
        if (SentUpToDate)
        {
            OldState = LogbookOrderedByTransaction.size();
        }
        logState = OldState;
        if (m_verbose)
            std::cout << "Sent summaries for " << QsosInUpdate << " QSOs" << std::endl;
        return SOAP_OK;
    }

    /* addAndGetQsos
    ** Clients add batchs of QSOs, and get QSOs by calling here.
    ** We add all the QSOs into the logbook, except those that are already
    ** in the log with an equal or higher version number.
    **
    ** The return value of logState tells the caller what his next state variable should be
    */
    template <class T>
    int addAndGetQsos(soap *s,
        std::vector<T * > &QsoAddArray, 
        int OldState, 
        int MaxRequested, 
            T *(*createF)(soap *),
            std::vector<T * > &pOut,
            int &logState
            )
    {
        lock_t l(m_mutex);  
        if (m_verbose)
            std::cout << "addAndGetQsos" << std::endl;
        getQsosSinceState(OldState, MaxRequested, pOut, createF, logState, s);
        bool SentUpToDate = (logState == LogbookOrderedByTransaction.size());
        AddQsos(QsoAddArray);
        if (SentUpToDate)
        {	// Let the caller skip his sent transactions if he 	
            // managed to receive everything up to what he sent
            logState = LogbookOrderedByTransaction.size();
        }
        return SOAP_OK;
    }

    template <class T>
    int getQsosByKeyArray(soap *s, 
        T *(*createF)(soap *),
        const std::vector<std::string > &QsoKeyArray, 
        std::vector<T * > &r,
        int &logState)
    {
        lock_t l(m_mutex); 
        if (m_verbose)
            std::cout << "getQsosByKeyArray" << std::endl;
        for (int i = 0; i < (int)QsoKeyArray.size(); i += 1)
        {
            std::map<std::string, QsoPtrPtr>::iterator itor = 
                id2QsoTable.find(QsoKeyArray[i]);
            if (itor != id2QsoTable.end())
            {
                r.push_back((*createF)(s));
                (*itor->second)->copy(r.back(), s);
            }
            else
                return soap_receiver_fault(s, "Invalid QSO key", "");

        }
        logState = LogbookOrderedByTransaction.size();
        if (m_verbose)
            std::cout << "Sent " << QsoKeyArray.size() << " QSOs" << std::endl;
        return SOAP_OK;
    }

    // Client tells us what column names he's going to use.
    // We tell him what index into QsoParts he should use.
    int ColumnNamesToIndices( 
        std::vector<std::string> &cols, 
        std::vector<int> &response)
    {
        lock_t l(m_mutex); 
        if (m_verbose)
            std::cout << "ColumnNamesToIndices" << std::endl;
        {
            response.resize(cols.size());
            for (int i =0; i < (int)cols.size(); i++)
            {
                std::map<std::string,int>::iterator itor = ExfNames.find(cols[i]);
                if (itor == ExfNames.end())
                {
                    int idx = ExfNames.size();
                    response[i] = idx;
                    ExfNames[cols[i]] = idx;

                }
                else
                    response[i] = itor->second;
            }
        }
        return SOAP_OK;
    }

    // a client that knows some rig frequencies calls here to set them.
    // In exchange, it receives all the rig frequencies the server knows.
    template <class T>
    int ExchangeFrequencies(soap *s,
            T *(*createF)(soap *),
            std::vector<T * > &IncomingFreqs, 
            std::vector<T * > &param_5)
    {
        lock_t l(m_mutex); 
        if (m_verbose)
            std::cout << "ExchangeFrequencies " << IncomingFreqs.size() << " frequencies received" << std::endl;
        for (int i = 0; i < (int)IncomingFreqs.size(); i += 1)
        {
            RigFrequency rf(*IncomingFreqs[i], m_limitLableTo6);
            std::map<std::string, RigFrequency>::iterator I = id2RigFrequencyTable.find(rf.keyString());
            if (I != id2RigFrequencyTable.end())
                id2RigFrequencyTable.erase(I);
            else if (m_verbose)
                std::cout << "Adding frequency: " << rf.station << " label: " << rf.label << " freq: " << rf.recvFreq << std::endl;
            id2RigFrequencyTable.insert(std::map<std::string, RigFrequency>::value_type(rf.keyString(), rf));
        }

        // -- go through the id2RigFrequencyTable and remove all 
        // entries older than X minutes
        std::map<std::string, RigFrequency>::iterator I = id2RigFrequencyTable.begin();
        while (I != id2RigFrequencyTable.end())	
        {
            std::map<std::string, RigFrequency>::iterator J = I++;
            if (J->second.AmOlderThanLimit())
            {
                if (m_verbose)
                    std::cout << "Removing frequency " << J->second.station << " label: " 
                    << J->second.label << " freq: " << J->second.recvFreq << std::endl;
                id2RigFrequencyTable.erase(J);
            }
        }

        I = id2RigFrequencyTable.begin();
        while (I != id2RigFrequencyTable.end())
        {
            param_5.push_back((*createF)(s));
            I->second.copy(*param_5.back());
            I++;
        }
        if (m_verbose)
            std::cout << "ExchangeFrequencies " << id2RigFrequencyTable.size() << " frequencies sent" << std::endl;
        return SOAP_OK;
    }

    // read a file and populate our username/password map
    bool readUserFile(const std::string &fname)
    {
        lock_t l(m_pwdMutex); 	
        std::ifstream pwdFile(fname.c_str());
        if (!pwdFile.is_open())
            return false;
        while (!pwdFile.eof())
        {
            std::string line;
            std::getline(pwdFile, line);
            std::string user = line;  std::string pass;
            int space = line.find_first_of(" \t");
            if (space != line.npos)
            {
                user = user.substr(0, space);
                pass = line.substr(space+1);
            }
            if (user.length())
                usernameToPassword[user] = pass;
        }
        return true;
    }

    void setVerboseFlag(bool f) { m_verbose = f;}
    bool verbose() const { return m_verbose;}
    void setL6Flag(bool f) { m_limitLableTo6 = f;}

protected:
    template <class T>
    void AddQsos(std::vector<T * > &QsoAddArray)
    {
        for (int j = 0; j < (int)QsoAddArray.size(); j += 1)
        {
            T *qNew = QsoAddArray[j];
            std::string &key = qNew->idKey;
            if (key.length() > MAX_KEY_LENGTH)
                key.resize(MAX_KEY_LENGTH);

            std::map<std::string, QsoPtrPtr>::iterator itor = id2QsoTable.find(key);

            bool qFound = (itor != id2QsoTable.end());
            if (!qFound)	// ensure there is an empty QsoPtr in the map
                itor = id2QsoTable.insert( 
                std::map<std::string, QsoPtrPtr>::value_type(key, QsoPtrPtr(new QsoPtr))).first;

            if (!qFound || VersionCheck(*itor->second, qNew))
            {
                *itor->second = QsoPtr(new Qso(*qNew));
                LogbookOrderedByTransaction.push_back(itor->second);
            }
        }
        if (m_verbose)
            std::cout << "Added " << QsoAddArray.size() << " QSO records for a total of "
            << id2QsoTable.size() << std::endl;
    }

    template <class T>
    static bool VersionCheck(QsoPtr qOrig, T *qNew)
    {
        if (qOrig->version == qNew->version)
        {
            // equal version numbers but different origins!
            if (qOrig->updatedBy != qNew->updatedBy)
            {
                // let the new one overide the old, and
                // artificially bump the version number so
                // both must deal with it again.

                qNew->version += 1;
                return true;
            }
        }
        return qOrig->version < qNew->version;
    }

    /*  getQsosSinceState
    **
    **  returns all the log entries/modifications since OldState, up to a maximum of
    **  of MAX_QSOS_IN_UPDATE. Always starts at OldState and works up.
    */
    template <class T>
    void getQsosSinceState(int OldState, int MaxRequested, std::vector<T * > &qsoVec,
        T *(*createF)(soap *),
        int &logState, soap *s)
    {
        int CurSize = LogbookOrderedByTransaction.size();
        if (OldState > CurSize)
            return;

        int QsosInUpdate = CurSize - OldState;

        // limit the number of QSOs we'll return to the server's maximum and to the
        // client's maximum.
        if (QsosInUpdate > MAX_QSOS_IN_UPDATE)
            QsosInUpdate = MAX_QSOS_IN_UPDATE;
        if (QsosInUpdate > MaxRequested)
            QsosInUpdate = MaxRequested;

        for (int i = 0; i < QsosInUpdate; i += 1)
        {
            qsoVec.push_back((*createF)(s));
            (*LogbookOrderedByTransaction[OldState])->copy(qsoVec.back(), s);
            OldState += 1;
        }
        logState = OldState;
        if (m_verbose)
            std::cout << "Sent " << QsosInUpdate << " QSOs" << std::endl;
    }

    std::string m_sessionId;
    std::map<std::string, int> ExfNames;

    /* The reason these are QsoPtrPtr instead of QsoPtr
    ** is so that old entries in the deque get replaced by
    ** any edited versions as we go along. Otherwise we
    ** would keep an extra copy of any edited QSOs in the deque */
    std::deque<QsoPtrPtr>  LogbookOrderedByTransaction;
    std::map<std::string, QsoPtrPtr> id2QsoTable;

    std::map<std::string, RigFrequency>	id2RigFrequencyTable;

    std::map<std::string, std::string> usernameToPassword;

    bool m_verbose;
    mutable std::mutex m_mutex;
    mutable std::mutex m_pwdMutex;
    bool m_limitLableTo6;
};

static inline ContestQsos * appState (soap *arg) 
{	return (ContestQsos *)arg->user;  }

#endif
