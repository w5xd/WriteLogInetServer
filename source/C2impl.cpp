// These are the implementations that get called from WL V11 and up--the CLR-based client

#include "ServerState.h"
#include "C2.nsmap"

void SetQsoParts(const std::vector<std::string> &other, C2::contest25__Qso *q)
{
    q->qsoparts->string = other;
}

std::vector<std::string> GetQsoParts(const C2::contest25__Qso &q)
{
    return q.qsoparts->string;
}

namespace C2
{
    contest25__RigFrequency *createRigFrequency(soap *s)
    {
        return soap_new_contest25__RigFrequency(s, 1);
    }

    contest25__Qso *create_Qso(soap *s)
    {
        contest25__Qso *ret = soap_new_contest25__Qso(s, 1);
        ret->qsoparts = soap_new_contest26__ArrayOfstring(s, 1);
        return ret;
    }

     contest25__QsoIdVersion *create_QsoIdVersion(soap *s)
    {
        return soap_new_contest25__QsoIdVersion(s, 1);
    }

// generated for the WCF version of the client

int __contest23__GetSessionId(soap *s, _contest23__GetSessionId *p1, _contest23__GetSessionIdResponse &r)
{
    if (!appState(s)->useridAndPswdOK(s->userid, s->passwd))
        return 401;	// for GetSession--user must provide password
    r.GetSessionIdResult = appState(s)->GetSessionId();
    return SOAP_OK;
}

int __contest23__addAndGetQsos(soap *s, 
    _contest23__addAndGetQsos *p1, _contest23__addAndGetQsosResponse &r)
{
    if (!p1 ) return SOAP_NO_DATA;
    if (!appState(s)->SessionOK(p1->SessionId))
        return soap_receiver_fault(s, "Invalid SessionId", "The Session ID must match the one sent at initialization time");
    if (!p1->QsoAddArray)
        return soap_receiver_fault(s, "Invalid QsoAddArray", "The addAndGetQso method requires a QsoAddArray");
    r.addAndGetQsosResult = soap_new_contest25__QsoUpdate(s, 1);
    r.addAndGetQsosResult->qsoArray = soap_new_contest25__ArrayOfQso(s, 1);
    return appState(s)->addAndGetQsos(s, 
                    p1->QsoAddArray->Qso, p1->OldState, p1->MaxRequested,
                    &create_Qso,
                    r.addAndGetQsosResult->qsoArray->Qso,
                    r.addAndGetQsosResult->logState);
}

int __contest23__AddAndGetLogSummary(soap *s, _contest23__AddAndGetLogSummary *p1, _contest23__AddAndGetLogSummaryResponse &r)
{
    if (!p1) return SOAP_NO_DATA;
    if (!appState(s)->SessionOK(p1->SessionId))
        return soap_receiver_fault(s, "Invalid SessionId", "The Session ID must match the one sent at initialization time");
    if (!p1->QsoAddArray)
        return soap_receiver_fault(s, "AddAndGetLogSummary requres an Array of QSOs", "");

    r.AddAndGetLogSummaryResult = soap_new_contest25__LogSummary(s, 1);
    r.AddAndGetLogSummaryResult->logSummaryIds = soap_new_contest25__ArrayOfQsoIdVersion(s,1);
    return appState(s)->AddAndGetLogSummary(s, p1->QsoAddArray->Qso, p1->OldState, p1->MaxRequested, 
        &create_QsoIdVersion, 
        r.AddAndGetLogSummaryResult->logSummaryIds->QsoIdVersion, 
        r.AddAndGetLogSummaryResult->logState);
}

int __contest23__getQsosByKeyArray(soap *s, 
    _contest23__getQsosByKeyArray *p1, 
    _contest23__getQsosByKeyArrayResponse &r)
{
    if (!p1 ) return SOAP_NO_DATA;
    if (!p1->QsoKeyArray)
        return soap_receiver_fault(s, "getQsosByKeyArray requires a QsoKeyArray", "");
    r.getQsosByKeyArrayResult = soap_new_contest25__QsoUpdate(s, 1);
    r.getQsosByKeyArrayResult->qsoArray = soap_new_contest25__ArrayOfQso(s, 1);
    return appState(s)->getQsosByKeyArray(
        s, &create_Qso, 
        p1->QsoKeyArray->string,
        r.getQsosByKeyArrayResult->qsoArray->Qso,
        r.getQsosByKeyArrayResult->logState);
}

int __contest23__ColumnNamesToIndices(soap *s, 
    _contest23__ColumnNamesToIndices *p1, 
    _contest23__ColumnNamesToIndicesResponse &r)
{
    if (!p1 ) return SOAP_NO_DATA;
    if (!appState(s)->SessionOK(p1->SessionId))
        return soap_receiver_fault(s, "Invalid SessionId", "The Session ID must match the one sent at initialization time");
    if (!p1->ColumnNames)
        return soap_receiver_fault(s, "Invalid columns", "The ColumnNamesToIndices function requires ColumnNames");
    r.ColumnNamesToIndicesResult = soap_new_contest26__ArrayOfint(s, 1);
    return appState(s)->ColumnNamesToIndices(p1->ColumnNames->string, r.ColumnNamesToIndicesResult->int_);
}

int __contest23__ExchangeFrequencies(soap *s, 
    _contest23__ExchangeFrequencies *p1, 
    _contest23__ExchangeFrequenciesResponse &r)
{
    if (!p1 ) return SOAP_NO_DATA;
    r.ExchangeFrequenciesResult = soap_new_contest25__ArrayOfRigFrequency(s, 1);
    return appState(s)->ExchangeFrequencies(s, &createRigFrequency, 
                    p1->IncomingFreqs->RigFrequency,
                    r.ExchangeFrequenciesResult->RigFrequency);
}

}