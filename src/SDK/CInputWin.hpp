#ifndef SDK_CINPUTWIN_HPP
#define SDK_CINPUTWIN_HPP

#include "IBaseInterface.hpp"
#include "CEngineVGui.hpp" // VPANEL
#include "CSurface.hpp" // HCursor
struct KeyCode;
struct MouseCode;
typedef int HInputContext;


class CInputWin : public IBaseInterface
{
public:
	virtual ~CInputWin() = 0;

	virtual void SetMouseFocus(VPANEL newMouseFocus) = 0;
	virtual void SetMouseCapture(VPANEL panel) = 0;

	// returns the string name of a scan code
	virtual void GetKeyCodeText(KeyCode code, char *buf, int buflen) = 0;

	// focus
	virtual VPANEL GetFocus() = 0;
	virtual VPANEL GetMouseOver() = 0;		// returns the panel the mouse is currently over, ignoring mouse capture

	// mouse state
	virtual void SetCursorPos(int x, int y) = 0;
	virtual void GetCursorPos(int &x, int &y) = 0;
	virtual bool WasMousePressed(MouseCode code) = 0;
	virtual bool WasMouseDoublePressed(MouseCode code) = 0;
	virtual bool IsMouseDown(MouseCode code) = 0;

	// cursor override
	virtual void SetCursorOveride(HCursor cursor) = 0;
	virtual HCursor GetCursorOveride() = 0;

	// key state
	virtual bool WasMouseReleased(MouseCode code) = 0;
	virtual bool WasKeyPressed(KeyCode code) = 0;
	virtual bool IsKeyDown(KeyCode code) = 0;
	virtual bool WasKeyTyped(KeyCode code) = 0;
	virtual bool WasKeyReleased(KeyCode code) = 0;

	virtual VPANEL GetAppModalSurface() = 0;

	// set the modal dialog panel.
	// all events will go only to this panel and its children.
	virtual void SetAppModalSurface(VPANEL panel) = 0;

	// release the modal dialog panel
	// do this when your modal dialog finishes.
	virtual void ReleaseAppModalSurface() = 0;

	virtual void GetCursorPosition(int &x, int &y) = 0;

	virtual void RunFrame() = 0;
	virtual void UpdateMouseFocus(int x, int y) = 0;

	// called when a panel becomes invalid
	virtual void PanelDeleted(VPANEL panel) = 0;

	// inputs into vgui2 input handling
	virtual bool InternalCursorMoved(int x,int y) = 0; //expects input in surface space
	virtual bool InternalMousePressed(MouseCode code) = 0;
	virtual bool InternalMouseDoublePressed(MouseCode code) = 0;
	virtual bool InternalMouseReleased(MouseCode code) = 0;
	virtual bool InternalMouseWheeled(int delta) = 0;
	virtual bool InternalKeyCodePressed(KeyCode code) = 0;
	virtual void InternalKeyCodeTyped(KeyCode code) = 0;
	virtual void InternalKeyTyped(wchar_t unichar) = 0;
	virtual bool InternalKeyCodeReleased(KeyCode code) = 0;

	// Creates/ destroys "input" contexts, which contains information
	// about which controls have mouse + key focus, for example.
	virtual HInputContext CreateInputContext() = 0;
	virtual void DestroyInputContext(HInputContext context) = 0;

	// Associates a particular panel with an input context
	// Associating NULL is valid; it disconnects the panel from the context
	virtual void AssociatePanelWithInputContext(HInputContext context, VPANEL pRoot) = 0;

	// Activates a particular input context, use DEFAULT_INPUT_CONTEXT
	// to get the one normally used by VGUI
	virtual void ActivateInputContext(HInputContext context) = 0;

	virtual VPANEL GetMouseCapture() = 0;
	virtual bool IsChildOfModalPanel(VPANEL panel) = 0;
	virtual void ResetInputContext(HInputContext context) = 0;
};

#endif // SDK_CINPUTWIN_HPP
