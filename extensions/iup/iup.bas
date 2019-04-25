MODULE Iup

' System
DECLARE SUB ::Open ALIAS "PuiOpen" LIB "iup"
DECLARE SUB ::Close ALIAS "PuiClose" LIB "iup"
DECLARE SUB ::Version ALIAS "PuiVersion" LIB "iup"
DECLARE SUB ::Load ALIAS "PuiLoad" LIB "iup"
DECLARE SUB ::LoadBuffer ALIAS "PuiLoadBuffer" LIB "iup"
DECLARE SUB ::SetLanguage ALIAS "PuiSetLanguage" LIB "iup"
DECLARE SUB ::GetLanguage ALIAS "PuiGetLanguage" LIB "iup"
' Attribute
DECLARE SUB ::StoreAttribute ALIAS "PuiStoreAttribute" LIB "iup"
DECLARE SUB ::StoreGlobalAttribute ALIAS "PuiStorGlobaleAttribute" LIB "iup"
DECLARE SUB ::StoreAttributeId ALIAS "PuiStoreAttributeId" LIB "iup"
DECLARE SUB ::StoreGlobalAttributeId ALIAS "PuiStoreGlobalAttributeId" LIB "iup"
DECLARE SUB ::SetAttribute ALIAS "PuiSetAttribute" LIB "iup"
DECLARE SUB ::SetGlobalAttribute ALIAS "PuiSetGlobalAttribute" LIB "iup"
DECLARE SUB ::SetAttributeId ALIAS "PuiSetAttributeId" LIB "iup"
DECLARE SUB ::SetGlobalAttributeId ALIAS "PuiSetGlobalAttributeId" LIB "iup"
DECLARE SUB ::SetfAttribute ALIAS "PuiSetfAttribute" LIB "iup"
DECLARE SUB ::SetfAttributeId ALIAS "PuiSetfAttributeId" LIB "iup"
DECLARE SUB ::SetfAttributeId2 ALIAS "PuiSetfAttributeId2" LIB "iup"
DECLARE SUB ::SetAttributes ALIAS "PuiSetAttributes" LIB "iup"
DECLARE SUB ::ResetAttribute ALIAS "PuiResetAttribute" LIB "iup"
DECLARE SUB ::ResetGlobalAttribute ALIAS "PuiResetGlobalAttribute" LIB "iup"
DECLARE SUB ::SetAtt ALIAS "PuiSetAtt" LIB "iup"
DECLARE SUB ::SetAttributeHandle ALIAS "PuiSetAttributeHandle" LIB "iup"
DECLARE SUB ::GetAttributeHandle ALIAS "PuiGetAttributeHandle" LIB "iup"
DECLARE SUB ::GetAttribute ALIAS "PuiGetAttribute" LIB "iup"
DECLARE SUB ::GetGlobalAttribute ALIAS "PuiGetGlobalAttribute" LIB "iup"
DECLARE SUB ::GetAttributeId ALIAS "PuiGetAttributeId" LIB "iup"
DECLARE SUB ::GetGlobalAttributeId ALIAS "PuiGetGlobalAttributeId" LIB "iup"
DECLARE SUB ::GetAllAttributes ALIAS "PuiGetAllAttributes" LIB "iup"
DECLARE SUB ::GetAttributes ALIAS "PuiGetAttributes" LIB "iup"
DECLARE SUB ::GetFloat ALIAS "PuiGetFloat" LIB "iup"
DECLARE SUB ::GetFloatId ALIAS "PuiGetFloatId" LIB "iup"
DECLARE SUB ::GetFloatId2 ALIAS "PuiGetFloatId2" LIB "iup"
DECLARE SUB ::GetInt ALIAS "PuiGetInt" LIB "iup"
DECLARE SUB ::GetInt2 ALIAS "PuiGetInt2" LIB "iup"
DECLARE SUB ::GetIntInt ALIAS "PuiGetIntInt" LIB "iup"
DECLARE SUB ::GetIntId ALIAS "PuiGetIntId" LIB "iup"
DECLARE SUB ::GetIntId2 ALIAS "PuiGetIntId2" LIB "iup"
DECLARE SUB ::StoreGlobal ALIAS "PuiStoreGlobal" LIB "iup"
DECLARE SUB ::SetGlobal ALIAS "PuiSetGlobal" LIB "iup"
DECLARE SUB ::GetGlobal ALIAS "PuiGetGlobal" LIB "iup"
' Events
DECLARE SUB __GetEvent ALIAS "GetEvent" LIB "iup"
DECLARE SUB __MainLoop ALIAS "PuiMainLoop" LIB "iup"
DECLARE SUB ::MainLoopLevel ALIAS "PuiMainLoopLevel" LIB "iup"
DECLARE SUB __LoopStep ALIAS "PuiLoopStep" LIB "iup"
DECLARE SUB __LoopStepWait ALIAS "PuiLoopStepWait" LIB "iup"
DECLARE SUB __ExitLoop ALIAS "PuiExitLoop" LIB "iup"
DECLARE SUB ::Flush ALIAS "PuiFlush" LIB "iup"
DECLARE SUB ::GetCallback ALIAS "PuiGetCallback" LIB "iup"
DECLARE SUB __SetCallback ALIAS "PuiSetCallback" LIB "iup"
DECLARE SUB ::SetCallbacks ALIAS "PuiSetCallbacks" LIB "iup"
DECLARE SUB ::GetActionName ALIAS "PuiGetActionName" LIB "iup"
DECLARE SUB ::SetFunction ALIAS "PuiSetFunction" LIB "iup"
DECLARE SUB ::RecordInput ALIAS "PuiRecordInput" LIB "iup"
DECLARE SUB ::PlayInput ALIAS "PuiPlayInput" LIB "iup"
' Layout
DECLARE SUB ::Create ALIAS "PuiCreate" LIB "iup"
DECLARE SUB ::Createp ALIAS "PuiCreatep" LIB "iup"
DECLARE SUB ::Destroy ALIAS "PuiDestroy" LIB "iup"
DECLARE SUB ::Map ALIAS "PuiMap" LIB "iup"
DECLARE SUB ::Unmap ALIAS "PuiUnmap" LIB "iup"
DECLARE SUB ::GetAllClasses ALIAS "PuiGetAllClasses" LIB "iup"
DECLARE SUB ::GetClassName ALIAS "PuiGetClassName" LIB "iup"
DECLARE SUB ::GetClassType ALIAS "PuiGetClassType" LIB "iup"
DECLARE SUB ::ClassMatch ALIAS "PuiClassMatch" LIB "iup"
DECLARE SUB ::GetClassAttributes ALIAS "PuiGetClassAttributes" LIB "iup"
DECLARE SUB ::GetClassCallbacks ALIAS "PuiGetClassCallbacks" LIB "iup"
DECLARE SUB ::SaveClassAttributes ALIAS "PuiSaveClassAttributes" LIB "iup"
DECLARE SUB ::CopyClassAttributes ALIAS "PuiCopyClassAttributes" LIB "iup"
DECLARE SUB ::SetClassDefaultAttribute ALIAS "PuiSetClassDefaultAttribute" LIB "iup"
DECLARE SUB ::Fill ALIAS "PuiFill" LIB "iup"
DECLARE SUB ::Hbox ALIAS "PuiHbox" LIB "iup"
DECLARE SUB ::Vbox ALIAS "PuiVbox" LIB "iup"
DECLARE SUB ::Zbox ALIAS "PuiZbox" LIB "iup"
DECLARE SUB ::Radio ALIAS "PuiRadio" LIB "iup"
DECLARE SUB ::Normalizer ALIAS "PuiNormalizer" LIB "iup"
DECLARE SUB ::Cbox ALIAS "PuiCbox" LIB "iup"
DECLARE SUB ::Sbox ALIAS "PuiSbox" LIB "iup"
DECLARE SUB ::Split ALIAS "PuiSplit" LIB "iup"
DECLARE SUB ::Append ALIAS "PuiAppend" LIB "iup"
DECLARE SUB ::Detach ALIAS "PuiDetach" LIB "iup"
DECLARE SUB ::Insert ALIAS "PuiInsert" LIB "iup"
DECLARE SUB ::InsertTop ALIAS "PuiInsertTop" LIB "iup"
DECLARE SUB ::Reparent ALIAS "PuiReparent" LIB "iup"
DECLARE SUB ::GetParent ALIAS "PuiGetParent" LIB "iup"
DECLARE SUB ::GetChild ALIAS "PuiGetChild" LIB "iup"
DECLARE SUB ::GetChildPos ALIAS "PuiGetChildPos" LIB "iup"
DECLARE SUB ::GetChildCount ALIAS "PuiGetChildCount" LIB "iup"
DECLARE SUB ::GetNextChild ALIAS "PuiGetNextChild" LIB "iup"
DECLARE SUB ::GetBrother ALIAS "PuiGetBrother" LIB "iup"
DECLARE SUB ::GetDialog ALIAS "PuiGetDialog" LIB "iup"
DECLARE SUB ::GetDialogChild ALIAS "PuiGetDialogChild" LIB "iup"
DECLARE SUB ::Refresh ALIAS "PuiRefresh" LIB "iup"
DECLARE SUB ::RefreshChildren ALIAS "PuiRefreshChildren" LIB "iup"
DECLARE SUB ::Update ALIAS "PuiUpdate" LIB "iup"
DECLARE SUB ::UpdateChildren ALIAS "PuiUpdateChildren" LIB "iup"
DECLARE SUB ::Redraw ALIAS "PuiRedraw" LIB "iup"
DECLARE SUB ::ConvertXYToPos ALIAS "PuiConvertXYToPos" LIB "iup"
' Dialog
DECLARE SUB ::Dialog ALIAS "PuiDialog" LIB "iup"
DECLARE SUB ::Popup ALIAS "PuiPopup" LIB "iup"
DECLARE SUB ::Show ALIAS "PuiShow" LIB "iup"
DECLARE SUB ::ShowXY ALIAS "PuiShowXY" LIB "iup"
DECLARE SUB ::Hide ALIAS "PuiHide" LIB "iup"
DECLARE SUB ::FileDlg ALIAS "PuiFileDlg" LIB "iup"
DECLARE SUB ::MessageDlg ALIAS "PuiMessageDlg" LIB "iup"
DECLARE SUB ::ColorDlg ALIAS "PuiColorDlg" LIB "iup"
DECLARE SUB ::FontDlg ALIAS "PuiFontDlg" LIB "iup"
DECLARE SUB ::Alarm ALIAS "PuiAlarm" LIB "iup"
DECLARE SUB ::GetFile ALIAS "PuiGetFile" LIB "iup"
DECLARE SUB ::GetColor ALIAS "PuiGetColor" LIB "iup"
DECLARE SUB ::GetParam ALIAS "PuiGetParam" LIB "iup"
DECLARE SUB ::GetText ALIAS "PuiGetText" LIB "iup"
DECLARE SUB ::ListDialog ALIAS "PuiListDialog" LIB "iup"
DECLARE SUB ::Message ALIAS "PuiMessage" LIB "iup"
DECLARE SUB ::LayoutDialog ALIAS "PuiLayoutDialog" LIB "iup"
DECLARE SUB ::ElementPropertiesDialog ALIAS "PuiElementPropertiesDialog" LIB "iup"
' Controls
DECLARE SUB ::Button ALIAS "PuiButton" LIB "iup"
DECLARE SUB ::Canvas ALIAS "PuiCanvas" LIB "iup"
DECLARE SUB ::Frame ALIAS "PuiFrame" LIB "iup"
DECLARE SUB ::Label ALIAS "PuiLabel" LIB "iup"
DECLARE SUB ::List ALIAS "PuiList" LIB "iup"
DECLARE SUB ::MultiLine ALIAS "PuiMultiLine" LIB "iup"
DECLARE SUB ::ProgressBar ALIAS "PuiProgressBar" LIB "iup"
DECLARE SUB ::Spin ALIAS "PuiSpin" LIB "iup"
DECLARE SUB ::Tabs ALIAS "PuiTabs" LIB "iup"
DECLARE SUB ::Tabsv ALIAS "PuiTabsv" LIB "iup"
DECLARE SUB ::Text ALIAS "PuiText" LIB "iup"
DECLARE SUB ::Toggle ALIAS "PuiToggle" LIB "iup"
DECLARE SUB ::Tree ALIAS "PuiTree" LIB "iup"
DECLARE SUB ::Val ALIAS "PuiVal" LIB "iup"
DECLARE SUB ::Cells ALIAS "PuiCells" LIB "iup"
DECLARE SUB ::Colorbar ALIAS "PuiColorbar" LIB "iup"
DECLARE SUB ::ColorBrowser ALIAS "PuiColorBrowser" LIB "iup"
DECLARE SUB ::Dial ALIAS "PuiDial" LIB "iup"
DECLARE SUB ::Matrix ALIAS "PuiMatrix" LIB "iup"
DECLARE SUB ::GLCanvas ALIAS "PuiGLCanvas" LIB "iup"
DECLARE SUB ::PPlot ALIAS "PuiPPlot" LIB "iup"
DECLARE SUB ::WebBrowser ALIAS "PuiWebBrowser" LIB "iup"
DECLARE SUB ::ControlsOpen ALIAS "PuiControlsOpen" LIB "iup"
' Resources
DECLARE SUB ::Image ALIAS "PuiImage" LIB "iup"
DECLARE SUB ::ImageRGB ALIAS "PuiImageRGB" LIB "iup"
DECLARE SUB ::ImageRGBA ALIAS "PuiImageRGBA" LIB "iup"
DECLARE SUB ::ImageLibOpen ALIAS "PuiImageLibOpen" LIB "iup"
DECLARE SUB ::LoadImage ALIAS "PuiLoadImage" LIB "iup"
DECLARE SUB ::SaveImage ALIAS "PuiSaveImage" LIB "iup"
DECLARE SUB ::NextField ALIAS "PuiNextField" LIB "iup"
DECLARE SUB ::PreviousField ALIAS "PuiPreviousField" LIB "iup"
DECLARE SUB ::GetFocus ALIAS "PuiGetFocus" LIB "iup"
DECLARE SUB ::SetFocus ALIAS "PuiSetFocus" LIB "iup"
DECLARE SUB ::Item ALIAS "PuiItem" LIB "iup"
DECLARE SUB ::Menu ALIAS "PuiMenu" LIB "iup"
DECLARE SUB ::Menuv ALIAS "PuiMenuv" LIB "iup"
DECLARE SUB ::Separator ALIAS "PuiSeparator" LIB "iup"
DECLARE SUB ::Submenu ALIAS "PuiSubmenu" LIB "iup"
DECLARE SUB ::SetHandle ALIAS "PuiSetHandle" LIB "iup"
DECLARE SUB ::GetHandle ALIAS "PuiGetHandle" LIB "iup"
DECLARE SUB ::GetName ALIAS "PuiGetName" LIB "iup"
DECLARE SUB ::GetAllNames ALIAS "PuiGetAllNames" LIB "iup"
DECLARE SUB ::GetAllDialogs ALIAS "PuiGetAllDialogs" LIB "iup"
DECLARE SUB ::Clipboard ALIAS "PuiClipboard" LIB "iup"
DECLARE SUB ::Timer ALIAS "PuiTimer" LIB "iup"
DECLARE SUB ::User ALIAS "PuiUser" LIB "iup"
DECLARE SUB ::Help ALIAS "PuiHelp" LIB "iup"
' Helper Functions
DECLARE SUB ::GetListText ALIAS "PuiGetListText" LIB "iup"
DECLARE SUB ::GetBtnPressed ALIAS "PuiGetBtnPressed" LIB "iup"
DECLARE SUB ::GetBtnState ALIAS "PuiGetBtnState" LIB "iup"
DECLARE SUB ::ClearList ALIAS "PuiClearList" LIB "iup"
DECLARE SUB ::Info ALIAS "PuiInfo" LIB "iup"

SUB MainLoop
LOCAL this_event, aname 
ExitLoop = 0
REPEAT
  __LoopStepWait()
  this_event = __GetEvent()
  IF this_event <> undef THEN
    aname = ::GetActionName()
    IF this_event = event{this_event}{aname}[0] THEN
      ICALL(event{this_event}{aname}[1])
    END IF
  END IF
UNTIL ExitLoop
END SUB

FUNCTION SetCallback(ih, aname, faddr)
  event{ih}{aname}[0] = ih 
  event{ih}{aname}[1] = faddr
  SetCallback = __SetCallback(ih, aname)
END FUNCTION

FUNCTION CreateImg(text)
  LOCAL i, strip, array, binstr
  strip = " " & CHR(9) & CHR(10) & CHR(13)
  FOR i = 1 TO LEN(strip)
    text = REPLACE(text, MID(strip, i, 1), "")
  NEXT i
  SPLITA text BY "," TO array
  FOR i = 0 TO UBOUND(array)
    binstr &= CHR(array[i])
  NEXT i
  CreateImg = binstr
END FUNCTION

END MODULE
