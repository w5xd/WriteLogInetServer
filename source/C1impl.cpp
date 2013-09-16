// These are the implementations that get called from WL V10--the SOAP SDK V2 client

#include "ServerState.h"
#include "C1ContestQsos_USCOREServiceBindingObject.h"
#include "C1.nsmap"

void SetQsoParts(const std::vector<std::string> &other, C1::contest2__Qso *q)
{
    q->qsoparts->str_USCOREel = other;
}

std::vector<std::string> GetQsoParts(const C1::contest2__Qso &q)
{
    return q.qsoparts->str_USCOREel;
}

namespace C1
{
// creation functions

    contest2__RigFrequency *create_RigFrequency(soap *s)
    {
        return soap_new_contest2__RigFrequency(s, 1);
    }

    contest2__Qso *create_Qso(soap *s)
    {
        contest2__Qso *ret = soap_new_contest2__Qso(s, 1);
        ret->qsoparts = soap_new_contest2__ArrayOfstring(s, 1);
        return ret;
    }

    contest2__QsoIdVersion *create_QsoIdVersion(soap *s)
    {
        return soap_new_contest2__QsoIdVersion(s, 1);
    }

/// Web service operation 'GetSessionId' (returns error code or SOAP_OK)
int contest3__GetSessionId(soap *s, std::string &response)
{
    if (!appState(s)->useridAndPswdOK(s->userid, s->passwd))
        return 401;	// for GetSession--user must provide password
    response = appState(s)->GetSessionId();
    return SOAP_OK;
}

/// Web service operation 'AddAndGetLogSummary' (returns error code or SOAP_OK)
int contest3__AddAndGetLogSummary(soap *s, std::string _SessionId, 
                contest2__ArrayOfQso *_QsoAddArray, int OldState, int MaxRequested, 
                struct contest3__AddAndGetLogSummaryResponse &_param_1)
{
    if (!appState(s)->SessionOK(_SessionId))
        return soap_receiver_fault(s, "Invalid SessionId", "The Session ID must match the one sent at initialization time");
    if (!_QsoAddArray)
        return soap_receiver_fault(s, "AddAndGetLogSummary requres an Array of QSOs", "");

    _param_1._response = soap_new_contest2__LogSummary(s, 1);
    _param_1._response->logSummaryIds = soap_new_contest2__ArrayOfQsoIdVersion(s, 1);
    return appState(s)->AddAndGetLogSummary(s, _QsoAddArray->qso_USCOREel, OldState, MaxRequested, 
        &create_QsoIdVersion, 
        _param_1._response->logSummaryIds->id_USCOREel, 
        _param_1._response->logState);
}

/// Web service operation 'addAndGetQsos' (returns error code or SOAP_OK)
int contest3__addAndGetQsos(soap *s, std::string _SessionId, 
                                                           contest2__ArrayOfQso *_QsoAddArray, 
                                                           int _OldState, 
                                                           int _MaxRequested, 
                        struct contest3__addAndGetQsosResponse &_param_2)
{
    if (!appState(s)->SessionOK(_SessionId))
        return soap_receiver_fault(s, "Invalid SessionId", "The Session ID must match the one sent at initialization time");
    if (!_QsoAddArray) return SOAP_NO_DATA;
    _param_2._response = soap_new_contest2__QsoUpdate(s, 1);
    _param_2._response->qsoArray = soap_new_contest2__ArrayOfQso(s, 1);
    return appState(s)->addAndGetQsos(s, _QsoAddArray->qso_USCOREel, _OldState, _MaxRequested,
                    &create_Qso,
                    _param_2._response->qsoArray->qso_USCOREel,
                    _param_2._response->logState);
}

/// Web service operation 'getQsosByKeyArray' (returns error code or SOAP_OK)
int contest3__getQsosByKeyArray(soap *s, 
        std::string _SessionId, 
          contest2__ArrayOfstring *_QsoKeyArray, 
                struct contest3__getQsosByKeyArrayResponse &_param_3)
{
    if (!appState(s)->SessionOK(_SessionId))
        return soap_receiver_fault(s, "Invalid SessionId", "The Session ID must match the one sent at initialization time");
    if (!_QsoKeyArray)
        return soap_receiver_fault(s, "Invalid QsoKeyArray", "The QsoKeyArray must not be empty");
    _param_3._response = soap_new_contest2__QsoUpdate(s, 1);
    _param_3._response->qsoArray = soap_new_contest2__ArrayOfQso(s, 1);
    return appState(s)->getQsosByKeyArray(
        s, 
        &create_Qso,
        _QsoKeyArray->str_USCOREel,
        _param_3._response->qsoArray->qso_USCOREel,
        _param_3._response->logState);
}

/// Web service operation 'ColumnNamesToIndices' (returns error code or SOAP_OK)
int contest3__ColumnNamesToIndices(soap *s, 
    std::string _SessionId, 
    contest2__ArrayOfstring *_ColumnNames, 
    struct contest3__ColumnNamesToIndicesResponse &_param_4)
{
    if (!appState(s)->SessionOK(_SessionId))
        return soap_receiver_fault(s, "Invalid SessionId", "The Session ID must match the one sent at initialization time");
    if (_ColumnNames)
    {
        _param_4._response = soap_new_contest2__ArrayOfint(s, 1);	
        return appState(s)->ColumnNamesToIndices(_ColumnNames->str_USCOREel, _param_4._response->int_USCOREel);
    }
    else 
        return soap_receiver_fault(s, "Invalid ColumnNames", "ColumnNamesToIndices requires ColumnNames");
}

/// Web service operation 'ExchangeFrequencies' (returns error code or SOAP_OK)
int contest3__ExchangeFrequencies(soap *s, 
        contest2__ArrayOfRigFrequency *_IncomingFreqs, 
        struct contest3__ExchangeFrequenciesResponse &_param_5)
{
    if (!_IncomingFreqs) return SOAP_NO_DATA;
    _param_5._response = soap_new_contest2__ArrayOfRigFrequency(s, 1);
    return appState(s)->ExchangeFrequencies(s, &create_RigFrequency, 
            _IncomingFreqs->rf_USCOREel, _param_5._response->rf_USCOREel);
}
}