/*
This is the class that contains the socket to another machine, and passes data through itself, 
as well as maintaining a sense of state.

The ULConnectionManager tells the ULConnection what data can actually be sent.

This class is done this way so that it can be eventually be replaced with a ULSecureConnection,
if such a protocol is warranted, so that all data that passes through can be managed through a
secure connection.  For now, this class provides a simple pass-through mechanism to the socket itself.

So, for instance, a gdcm object will be passes to this object, and it will then get passed 
along the connection, if that connection is in the proper state to do so.

For right now, this class is not directly intended to be inherited from, but the potential
for future ULSecureConnection warrants the addition, rather than having everything be managed
from within the ULConnectionManager (or this class) without a wrapper.

name and date: 16 sept 2010 mmr

*/
#ifndef ULCONNECTION_H
#define ULCONNECTION_H
#include "gdcmNetworkStateID.h"
#include "gdcmARTIMTimer.h"
#include <socket++/echo.h>
#include "gdcmULConnectionInfo.h"
#include "gdcmPresentationContext.h"
#include "gdcmDataElement.h"
#include "gdcmPresentationContext.h"


namespace gdcm{
  namespace network{
    class ULConnection {

      ULConnectionInfo mInfo;
      echo* mSocket;//of the three protocols offered by socket++-- echo, smtp, and ftp--
      //echo most closely matches what the DICOM standard describes as a network connection
      ARTIMTimer mTimer;

      ULConnection(gdcm::network::ULConnection& inConnection){}; //no copy construction allowed

      EStateID mCurrentState;
  
      std::vector<PresentationContext> mPresentationContexts;
      uint32_t mMaxPDUSize;

    public:
  
      ULConnection(const ULConnectionInfo& inUserInformation);
      //destructors are virtual to prevent memory leaks by inherited classes
      virtual ~ULConnection();

      EStateID GetState() const;
      void SetState(const EStateID& inState);//must be able to update state...

      echo* GetProtocol();
      void SetProtocol(echo* inProtocol);

      ARTIMTimer& GetTimer();

      ULConnectionInfo GetConnectionInfo() const;

      //when the connection is first associated, the connection is told
      //the max packet/PDU size and the way in which to present data
      //(presentation contexts, etc).  Store that here.
      void SetMaxPDUSize(const uint32_t& inSize);
      uint32_t GetMaxPDUSize() const;

      std::vector<PresentationContext> GetPresentationContexts() const;
      void SetPresentationContexts(const std::vector<PresentationContext>& inContexts);
      //given a particular data element, presumably the SOP class, 
      //find the presentation context for that SOP
      //NOT YET IMPLEMENTED
      PresentationContext FindContext(const DataElement& de) const;



    };
  }
}

#endif // ULCONNECTION_H