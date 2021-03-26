#include "envH.h"
// this is useless, but soapcpp2 won't give us soap_serve without some method to implement
int env_void(struct soap *,int &)
{
    return 0;
}
