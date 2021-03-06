/* C2Server.cpp
   Generated by gSOAP 2.8.112 for ContestQsos2.h

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
#include "C2H.h"

namespace C2 {

SOAP_SOURCE_STAMP("@(#) C2Server.cpp ver 2.8.112 2021-03-26 01:14:50 GMT")
SOAP_FMAC5 int SOAP_FMAC6 C2_serve(struct soap *soap)
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
		if ((C2::C2_serve_request(soap) || (soap->fserveloop && soap->fserveloop(soap))) && soap->error && soap->error < SOAP_STOP)
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
SOAP_FMAC5 int SOAP_FMAC6 C2_serve_request(struct soap *soap)
{
	(void)soap_peek_element(soap);
	if (!soap_match_tag(soap, soap->tag, "contest23:GetSessionId"))
		return soap_serve___contest23__GetSessionId(soap);
	if (!soap_match_tag(soap, soap->tag, "contest23:addAndGetQsos"))
		return soap_serve___contest23__addAndGetQsos(soap);
	if (!soap_match_tag(soap, soap->tag, "contest23:AddAndGetLogSummary"))
		return soap_serve___contest23__AddAndGetLogSummary(soap);
	if (!soap_match_tag(soap, soap->tag, "contest23:getQsosByKeyArray"))
		return soap_serve___contest23__getQsosByKeyArray(soap);
	if (!soap_match_tag(soap, soap->tag, "contest23:ColumnNamesToIndices"))
		return soap_serve___contest23__ColumnNamesToIndices(soap);
	if (!soap_match_tag(soap, soap->tag, "contest23:ExchangeFrequencies"))
		return soap_serve___contest23__ExchangeFrequencies(soap);
	return soap->error = SOAP_NO_METHOD;
}
#endif

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___contest23__GetSessionId(struct soap *soap)
{	struct __contest23__GetSessionId soap_tmp___contest23__GetSessionId;
	_contest23__GetSessionIdResponse contest23__GetSessionIdResponse;
	contest23__GetSessionIdResponse.soap_default(soap);
	soap_default___contest23__GetSessionId(soap, &soap_tmp___contest23__GetSessionId);
	if (!soap_get___contest23__GetSessionId(soap, &soap_tmp___contest23__GetSessionId, "-contest23:GetSessionId", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __contest23__GetSessionId(soap, soap_tmp___contest23__GetSessionId.contest23__GetSessionId, contest23__GetSessionIdResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	contest23__GetSessionIdResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || contest23__GetSessionIdResponse.soap_put(soap, "contest23:GetSessionIdResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || contest23__GetSessionIdResponse.soap_put(soap, "contest23:GetSessionIdResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___contest23__addAndGetQsos(struct soap *soap)
{	struct __contest23__addAndGetQsos soap_tmp___contest23__addAndGetQsos;
	_contest23__addAndGetQsosResponse contest23__addAndGetQsosResponse;
	contest23__addAndGetQsosResponse.soap_default(soap);
	soap_default___contest23__addAndGetQsos(soap, &soap_tmp___contest23__addAndGetQsos);
	if (!soap_get___contest23__addAndGetQsos(soap, &soap_tmp___contest23__addAndGetQsos, "-contest23:addAndGetQsos", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __contest23__addAndGetQsos(soap, soap_tmp___contest23__addAndGetQsos.contest23__addAndGetQsos, contest23__addAndGetQsosResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	contest23__addAndGetQsosResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || contest23__addAndGetQsosResponse.soap_put(soap, "contest23:addAndGetQsosResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || contest23__addAndGetQsosResponse.soap_put(soap, "contest23:addAndGetQsosResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___contest23__AddAndGetLogSummary(struct soap *soap)
{	struct __contest23__AddAndGetLogSummary soap_tmp___contest23__AddAndGetLogSummary;
	_contest23__AddAndGetLogSummaryResponse contest23__AddAndGetLogSummaryResponse;
	contest23__AddAndGetLogSummaryResponse.soap_default(soap);
	soap_default___contest23__AddAndGetLogSummary(soap, &soap_tmp___contest23__AddAndGetLogSummary);
	if (!soap_get___contest23__AddAndGetLogSummary(soap, &soap_tmp___contest23__AddAndGetLogSummary, "-contest23:AddAndGetLogSummary", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __contest23__AddAndGetLogSummary(soap, soap_tmp___contest23__AddAndGetLogSummary.contest23__AddAndGetLogSummary, contest23__AddAndGetLogSummaryResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	contest23__AddAndGetLogSummaryResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || contest23__AddAndGetLogSummaryResponse.soap_put(soap, "contest23:AddAndGetLogSummaryResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || contest23__AddAndGetLogSummaryResponse.soap_put(soap, "contest23:AddAndGetLogSummaryResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___contest23__getQsosByKeyArray(struct soap *soap)
{	struct __contest23__getQsosByKeyArray soap_tmp___contest23__getQsosByKeyArray;
	_contest23__getQsosByKeyArrayResponse contest23__getQsosByKeyArrayResponse;
	contest23__getQsosByKeyArrayResponse.soap_default(soap);
	soap_default___contest23__getQsosByKeyArray(soap, &soap_tmp___contest23__getQsosByKeyArray);
	if (!soap_get___contest23__getQsosByKeyArray(soap, &soap_tmp___contest23__getQsosByKeyArray, "-contest23:getQsosByKeyArray", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __contest23__getQsosByKeyArray(soap, soap_tmp___contest23__getQsosByKeyArray.contest23__getQsosByKeyArray, contest23__getQsosByKeyArrayResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	contest23__getQsosByKeyArrayResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || contest23__getQsosByKeyArrayResponse.soap_put(soap, "contest23:getQsosByKeyArrayResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || contest23__getQsosByKeyArrayResponse.soap_put(soap, "contest23:getQsosByKeyArrayResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___contest23__ColumnNamesToIndices(struct soap *soap)
{	struct __contest23__ColumnNamesToIndices soap_tmp___contest23__ColumnNamesToIndices;
	_contest23__ColumnNamesToIndicesResponse contest23__ColumnNamesToIndicesResponse;
	contest23__ColumnNamesToIndicesResponse.soap_default(soap);
	soap_default___contest23__ColumnNamesToIndices(soap, &soap_tmp___contest23__ColumnNamesToIndices);
	if (!soap_get___contest23__ColumnNamesToIndices(soap, &soap_tmp___contest23__ColumnNamesToIndices, "-contest23:ColumnNamesToIndices", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __contest23__ColumnNamesToIndices(soap, soap_tmp___contest23__ColumnNamesToIndices.contest23__ColumnNamesToIndices, contest23__ColumnNamesToIndicesResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	contest23__ColumnNamesToIndicesResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || contest23__ColumnNamesToIndicesResponse.soap_put(soap, "contest23:ColumnNamesToIndicesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || contest23__ColumnNamesToIndicesResponse.soap_put(soap, "contest23:ColumnNamesToIndicesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___contest23__ExchangeFrequencies(struct soap *soap)
{	struct __contest23__ExchangeFrequencies soap_tmp___contest23__ExchangeFrequencies;
	_contest23__ExchangeFrequenciesResponse contest23__ExchangeFrequenciesResponse;
	contest23__ExchangeFrequenciesResponse.soap_default(soap);
	soap_default___contest23__ExchangeFrequencies(soap, &soap_tmp___contest23__ExchangeFrequencies);
	if (!soap_get___contest23__ExchangeFrequencies(soap, &soap_tmp___contest23__ExchangeFrequencies, "-contest23:ExchangeFrequencies", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __contest23__ExchangeFrequencies(soap, soap_tmp___contest23__ExchangeFrequencies.contest23__ExchangeFrequencies, contest23__ExchangeFrequenciesResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	contest23__ExchangeFrequenciesResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || contest23__ExchangeFrequenciesResponse.soap_put(soap, "contest23:ExchangeFrequenciesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || contest23__ExchangeFrequenciesResponse.soap_put(soap, "contest23:ExchangeFrequenciesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

} // namespace C2


#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of C2Server.cpp */
