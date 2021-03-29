#ifndef SDK_IBASEINTERFACE_HPP
#define SDK_IBASEINTERFACE_HPP


class IBaseInterface
{
public:
	virtual	~IBaseInterface() {}
};

typedef IBaseInterface *(*CreateInterfaceFn)(const char *pName, int *pReturnCode);

#endif // SDK_IBASEINTERFACE_HPP
