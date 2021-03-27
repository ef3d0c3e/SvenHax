#ifndef SDK_CONCOMMANDBASE_HPP
#define SDK_CONCOMMANDBASE_HPP

class IConCommandBaseAccessor;
typedef int CVarDLLIdentifier_t;

class ConCommandBase
{
	friend class CCvar;
	friend class ConVar;
	friend class ConCommand;
	friend void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor* pAccessor);
	friend void ConVar_PublishToVXConsole();

	// FIXME: Remove when ConVar changes are done
	friend class CDefaultCvar;

public:
	ConCommandBase(void);
	ConCommandBase(const char* pName, const char* pHelpString = 0, int flags = 0);

	virtual ~ConCommandBase(void);

	virtual bool IsCommand(void) const;

	// Check flag
	virtual bool IsFlagSet(int flag) const;
	// Set flag
	virtual void AddFlags(int flags);
	// Clear flag
	virtual void RemoveFlags(int flags);

	virtual int GetFlags() const;

	// Return name of cvar
	virtual const char* GetName(void) const;

	// Return help text for cvar
	virtual const char* GetHelpText(void) const;

	// Deal with next pointer
	const ConCommandBase* GetNext(void) const;
	ConCommandBase* GetNext(void);

	virtual bool IsRegistered(void) const;

	// Returns the DLL identifier
	virtual CVarDLLIdentifier_t GetDLLIdentifier() const;

protected:
	virtual void Create(const char* pName, const char* pHelpString = 0, int flags = 0);

	// Used internally by OneTimeInit to initialize/shutdown
	virtual void Init();
	void Shutdown();

	// Internal copy routine ( uses new operator from correct module )
	char* CopyString(const char* from);

private:
	// Next ConVar in chain
	// Prior to register, it points to the next convar in the DLL.
	// Once registered, though, m_pNext is reset to point to the next
	// convar in the global list
	ConCommandBase* m_pNext;

	// Has the cvar been added to the global list?
	bool m_bRegistered;

	// Static data
	const char* m_pszName;
	const char* m_pszHelpString;

	// ConVar flags
	int m_nFlags;

protected:
	// ConVars add themselves to this list for the executable.
	// Then ConVar_Register runs through  all the console variables
	// and registers them into a global list stored in vstdlib.dll
	static ConCommandBase* s_pConCommandBases;

	// ConVars in this executable use this 'global' to access values.
	static IConCommandBaseAccessor* s_pAccessor;
};

#endif // SDK_CONCOMMANDBASE_HPP
