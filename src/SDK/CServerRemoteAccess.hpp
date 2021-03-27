#ifndef SDK_CSERVERREMOTEACCESS_HPP
#define SDK_CSERVERREMOTEACCESS_HPP

class CServerRemoteAccess
{
public:
	virtual ~CServerRemoteAccess() = 0;
	virtual void WriteDataRequest(const void* buffer, int bufSiz) = 0;
};

#endif // SDK_CSERVERREMOTEACCESS_HPP
