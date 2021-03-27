#ifndef SDK_CENGINESEVER_HPP
#define SDK_CENGINESEVER_HPP

class CEngineServer
{
public:
	virtual ~CEngineServer() = 0;

	virtual int NumberOfPrecachedModels() = 0; // or unsigned, idk
	virtual void* GetModelsPtrByIndex(int) = 0;
};

#endif // SDK_CENGINESEVER_HPP
