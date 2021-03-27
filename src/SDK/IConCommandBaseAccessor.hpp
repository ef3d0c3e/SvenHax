#ifndef SDK_ICONCOMMANDBASEACCESSOR_HPP
#define SDK_ICONCOMMANDBASEACCESSOR_HPP

class ConCommandBase;
class IConCommandBaseAccessor
{
public:
	virtual bool RegisterConCommandBase( ConCommandBase *pVar ) = 0;
};

#endif // SDK_ICONCOMMANDBASEACCESSOR_HPP
