/* C1Server.cpp
   Generated by gSOAP 2.8.112 for ContestQsos.h

gSOAP XML Web services tools
Copyright (C) 2000-2021, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "C1H.h"

namespace C1 {

SOAP_SOURCE_STAMP("@(#) C1Server.cpp ver 2.8.112 2021-03-26 01:15:01 GMT")
SOAP_FMAC5 int SOAP_FMAC6 C1_serve(struct soap *soap)
{
#ifndef WITH_FASTCGI
	soap->keep_alive = soap->max_keep_alive + 1;
#endif
	do
	{
#ifndef WITH_FASTCGI
		if (soap->keep_alive > 0 && soap->max_keep_alive > 0)
			soap->keep_alive--;
#endif
		if (soap_begin_serve(soap))
		{	if (soap->error >= SOAP_STOP)
				continue;
			return soap->error;
		}
		if ((C1::C1_serve_request(soap) || (soap->fserveloop && soap->fserveloop(soap))) && soap->error && soap->error < SOAP_STOP)
		{
#ifdef WITH_FASTCGI
			soap_send_fault(soap);
#else
			return soap_send_fault(soap);
#endif
		}
#ifdef WITH_FASTCGI
		soap_destroy(soap);
		soap_end(soap);
	} while (1);
#else
	} while (soap->keep_alive);
#endif
	return SOAP_OK;
}

#ifndef WITH_NOSERVEREQUEST
SOAP_FMAC5 int SOAP_FMAC6 C1_serve_request(struct soap *soap)
{
	(void)soap_peek_element(soap);
	if (!soap_match_tag(soap, soap->tag, "contest1:GetSessionId"))
		return soap_serve_contest1__GetSessionId(soap);
	if (!soap_match_tag(soap, soap->tag, "contest1:AddAndGetLogSummary"))
		return soap_serve_contest1__AddAndGetLogSummary(soap);
	if (!soap_match_tag(soap, soap->tag, "contest1:addAndGetQsos"))
		return soap_serve_contest1__addAndGetQsos(soap);
	if (!soap_match_tag(soap, soap->tag, "contest1:getQsosByKeyArray"))
		return soap_serve_contest1__getQsosByKeyArray(soap);
	if (!soap_match_tag(soap, soap->tag, "contest1:ColumnNamesToIndices"))
		return soap_serve_contest1__ColumnNamesToIndices(soap);
	if (!soap_match_tag(soap, soap->tag, "contest1:ExchangeFrequencies"))
		return soap_serve_contest1__ExchangeFrequencies(soap);
	return soap->error = SOAP_NO_METHOD;
}
#endif

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_contest1__GetSessionId(struct soap *soap)
{	struct contest1__GetSessionId soap_tmp_contest1__GetSessionId;
	struct contest1__GetSessionIdResponse soap_tmp_contest1__GetSessionIdResponse;
	soap_default_contest1__GetSessionIdResponse(soap, &soap_tmp_contest1__GetSessionIdResponse);
	soap_default_contest1__GetSessionId(soap, &soap_tmp_contest1__GetSessionId);
	if (!soap_get_contest1__GetSessionId(soap, &soap_tmp_contest1__GetSessionId, "contest1:GetSessionId", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = contest1__GetSessionId(soap, soap_tmp_contest1__GetSessionIdResponse.response);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/"; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_contest1__GetSessionIdResponse(soap, &soap_tmp_contest1__GetSessionIdResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_contest1__GetSessionIdResponse(soap, &soap_tmp_contest1__GetSessionIdResponse, "contest1:GetSessionIdResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_contest1__GetSessionIdResponse(soap, &soap_tmp_contest1__GetSessionIdResponse, "contest1:GetSessionIdResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_contest1__AddAndGetLogSummary(struct soap *soap)
{	struct contest1__AddAndGetLogSummary soap_tmp_contest1__AddAndGetLogSummary;
	struct contest1__AddAndGetLogSummaryResponse _param_1;
	soap_default_contest1__AddAndGetLogSummaryResponse(soap, &_param_1);
	soap_default_contest1__AddAndGetLogSummary(soap, &soap_tmp_contest1__AddAndGetLogSummary);
	if (!soap_get_contest1__AddAndGetLogSummary(soap, &soap_tmp_contest1__AddAndGetLogSummary, "contest1:AddAndGetLogSummary", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = contest1__AddAndGetLogSummary(soap, soap_tmp_contest1__AddAndGetLogSummary._SessionId, soap_tmp_contest1__AddAndGetLogSummary._QsoAddArray, soap_tmp_contest1__AddAndGetLogSummary._OldState, soap_tmp_contest1__AddAndGetLogSummary._MaxRequested, _param_1);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/"; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_contest1__AddAndGetLogSummaryResponse(soap, &_param_1);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_contest1__AddAndGetLogSummaryResponse(soap, &_param_1, "contest1:AddAndGetLogSummaryResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_contest1__AddAndGetLogSummaryResponse(soap, &_param_1, "contest1:AddAndGetLogSummaryResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_contest1__addAndGetQsos(struct soap *soap)
{	struct contest1__addAndGetQsos soap_tmp_contest1__addAndGetQsos;
	struct contest1__addAndGetQsosResponse _param_1;
	soap_default_contest1__addAndGetQsosResponse(soap, &_param_1);
	soap_default_contest1__addAndGetQsos(soap, &soap_tmp_contest1__addAndGetQsos);
	if (!soap_get_contest1__addAndGetQsos(soap, &soap_tmp_contest1__addAndGetQsos, "contest1:addAndGetQsos", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = contest1__addAndGetQsos(soap, soap_tmp_contest1__addAndGetQsos._SessionId, soap_tmp_contest1__addAndGetQsos._QsoAddArray, soap_tmp_contest1__addAndGetQsos._OldState, soap_tmp_contest1__addAndGetQsos._MaxRequested, _param_1);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/"; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_contest1__addAndGetQsosResponse(soap, &_param_1);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_contest1__addAndGetQsosResponse(soap, &_param_1, "contest1:addAndGetQsosResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_contest1__addAndGetQsosResponse(soap, &_param_1, "contest1:addAndGetQsosResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_contest1__getQsosByKeyArray(struct soap *soap)
{	struct contest1__getQsosByKeyArray soap_tmp_contest1__getQsosByKeyArray;
	struct contest1__getQsosByKeyArrayResponse _param_1;
	soap_default_contest1__getQsosByKeyArrayResponse(soap, &_param_1);
	soap_default_contest1__getQsosByKeyArray(soap, &soap_tmp_contest1__getQsosByKeyArray);
	if (!soap_get_contest1__getQsosByKeyArray(soap, &soap_tmp_contest1__getQsosByKeyArray, "contest1:getQsosByKeyArray", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = contest1__getQsosByKeyArray(soap, soap_tmp_contest1__getQsosByKeyArray._SessionId, soap_tmp_contest1__getQsosByKeyArray._QsoKeyArray, _param_1);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/"; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_contest1__getQsosByKeyArrayResponse(soap, &_param_1);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_contest1__getQsosByKeyArrayResponse(soap, &_param_1, "contest1:getQsosByKeyArrayResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_contest1__getQsosByKeyArrayResponse(soap, &_param_1, "contest1:getQsosByKeyArrayResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_contest1__ColumnNamesToIndices(struct soap *soap)
{	struct contest1__ColumnNamesToIndices soap_tmp_contest1__ColumnNamesToIndices;
	struct contest1__ColumnNamesToIndicesResponse _param_1;
	soap_default_contest1__ColumnNamesToIndicesResponse(soap, &_param_1);
	soap_default_contest1__ColumnNamesToIndices(soap, &soap_tmp_contest1__ColumnNamesToIndices);
	if (!soap_get_contest1__ColumnNamesToIndices(soap, &soap_tmp_contest1__ColumnNamesToIndices, "contest1:ColumnNamesToIndices", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = contest1__ColumnNamesToIndices(soap, soap_tmp_contest1__ColumnNamesToIndices._SessionId, soap_tmp_contest1__ColumnNamesToIndices._ColumnNames, _param_1);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/"; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_contest1__ColumnNamesToIndicesResponse(soap, &_param_1);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_contest1__ColumnNamesToIndicesResponse(soap, &_param_1, "contest1:ColumnNamesToIndicesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_contest1__ColumnNamesToIndicesResponse(soap, &_param_1, "contest1:ColumnNamesToIndicesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_contest1__ExchangeFrequencies(struct soap *soap)
{	struct contest1__ExchangeFrequencies soap_tmp_contest1__ExchangeFrequencies;
	struct contest1__ExchangeFrequenciesResponse _param_1;
	soap_default_contest1__ExchangeFrequenciesResponse(soap, &_param_1);
	soap_default_contest1__ExchangeFrequencies(soap, &soap_tmp_contest1__ExchangeFrequencies);
	if (!soap_get_contest1__ExchangeFrequencies(soap, &soap_tmp_contest1__ExchangeFrequencies, "contest1:ExchangeFrequencies", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = contest1__ExchangeFrequencies(soap, soap_tmp_contest1__ExchangeFrequencies._IncomingFreqs, _param_1);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/"; /* use SOAP encoding style */
	soap_serializeheader(soap);
	soap_serialize_contest1__ExchangeFrequenciesResponse(soap, &_param_1);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_contest1__ExchangeFrequenciesResponse(soap, &_param_1, "contest1:ExchangeFrequenciesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_contest1__ExchangeFrequenciesResponse(soap, &_param_1, "contest1:ExchangeFrequenciesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

} // namespace C1


#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of C1Server.cpp */
