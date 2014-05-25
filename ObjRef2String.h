#ifndef obj2ref_h
#define obj2ref_h

const int c_iMaxRef = 4000;

// Some AGC Events copy/paste
struct less_BSTR : public std::binary_function<CComBSTR, CComBSTR, bool> 
{
		result_type operator()(first_argument_type a1, second_argument_type a2) const
		{
			return _wcsicmp((BSTR)a1, (BSTR)a2) < 0;
		}
};
typedef std::map<CComBSTR, CComVariant, less_BSTR> XProperties;
typedef XProperties::iterator                      XPropertyIt;

// Value (variant) length headers (custom)
enum
{
	LengthZero   = 0xF0,
	Length1Byte  = 0xF1,
	Length2Bytes = 0xF2,
	Length4Bytes = 0xF4
};

// Keep this in sync with the actual DLL/Allsrv
enum AGCEventID
{	
	EventID_Unknown	= 0,
	EventID_AGC_LowerBound	= 1,
	EventID_EventLogging_LowerBound	= 100,
	EventID_DBEventLogStarted	= 101,
	EventID_DBEventLogStopped	= 102,
	EventID_NTEventLogStarted	= 103,
	EventID_NTEventLogStopped	= 104,
	EventID_AGCNotInitialized	= 105,
	EventID_AGCAssert	= 106,
	EventID_EventLogging_UpperBound	= 199,
	EventID_Station_LowerBound	= 200,
	EventID_StationCreated	= 201,
	EventID_StationDeleted	= 202,
	EventID_StationChangesSides	= 203,
	EventID_StationCreatedEscort	= 211,
	EventID_StationDeletedEscort	= 212,
	EventID_StationChangesSidesEscort	= 213,
	EventID_Station_UpperBound	= 299,
	EventID_Ship_LowerBound	= 300,
	EventID_ShipChangesSectors	= 301,
	EventID_ShipKilled	= 302,
	EventID_ShipUsesRipcord	= 303,
	EventID_Ship_UpperBound	= 399,
	EventID_AGC_UpperBound	= 1999,
	AllsrvEventID_Allsrv_LowerBound	= 2000,
	AllsrvEventID_Initialize	= 2001,
	AllsrvEventID_Terminate	= 2002,
	AllsrvEventID_BadFtpPath	= 2003,
	AllsrvEventID_PlayerDropped	= 2004,
	AllsrvEventID_SessionLost	= 2005,
	AllsrvEventID_MsgFromUnknownPlayer	= 2006,
	AllsrvEventID_IncreaseTokensMax	= 2007,
	AllsrvEventID_InvalidZoneTicket	= 2008,
	AllsrvEventID_DecryptTicketFailed	= 2009,
	AllsrvEventID_NoTicket	= 2010,
	AllsrvEventID_LoginFailed	= 2011,
	AllsrvEventID_CreateCharFailed	= 2012,
	AllsrvEventID_BadClientVersion	= 2013,
	AllsrvEventID_MessageBox	= 2014,
	AllsrvEventID_MoneyError	= 2015,
	AllsrvEventID_Connecting	= 2016,
	AllsrvEventID_ConnectError	= 2017,
	AllsrvEventID_NoLobby	= 2018,
	AllsrvEventID_HostSession	= 2019,
	AllsrvEventID_CantHostSession	= 2020,
	AllsrvEventID_CantWatchArt	= 2021,
	AllsrvEventID_StartingService	= 2022,
	AllsrvEventID_ZoneAuthServer	= 2023,
	AllsrvEventID_Assert	= 2024,
	AllsrvEventID_ArtChangedInInit	= 2025,
	AllsrvEventID_SystemError	= 2027,
	AllsrvEventID_LostLobby	= 2028,
	AllsrvEventID_GameStarted	= 2029,
	AllsrvEventID_GameEnded	= 2030,
	AllsrvEventID_GameOver	= 2031,
	AllsrvEventID_LatencyExceeded	= 2032,
	AllsrvEventID_ConnectedLobby	= 2033,
	AllsrvEventID_DisconnectingLobby	= 2034,
	AllsrvEventID_DisconnectedLobby	= 2035,
	AllsrvEventID_DatabaseErrorSQL	= 2036,
	AllsrvEventID_Pause	= 2037,
	AllsrvEventID_Continue	= 2038,
	AllsrvEventID_DatabaseErrorOLEDB	= 2039,
	AllsrvEventID_Allsrv_UpperBound	= 3999,
	EventID_Admin_LowerBound	= 4000,
	EventID_ServerShutdown	= 4001,
	EventID_NewSession	= 4010,
	EventID_LoginServer	= 4020,
	EventID_LogoutServer	= 4021,
	EventID_ChatMessage	= 4030,
	EventID_AdminPage	= 4031,
	EventID_AdminChat	= 4032,
	EventID_GameCreated	= 4040,
	EventID_GameDestroyed	= 4041,
	EventID_GameStateChange	= 4042,
	EventID_LoginGame	= 4050,
	EventID_LogoutGame	= 4051,
	EventID_JoinTeam	= 4060,
	EventID_LeaveTeam	= 4061,
	EventID_TeamInfoChange	= 4062,
	EventID_HackLog	= 4070,
	EventID_HackBoot	= 4071,
	EventID_HackBootNoPlayer	= 4072,
	EventID_Admin_UpperBound	= 5999,
	PigEventID_Pigs_LowerBound	= 6000,
	PigEventID_PigObject_LowerBound	= 6020,
	PigEventID_ConnectFailed	= 6021,
	PigEventID_LogonDenied	= 6022,
	PigEventID_ZoneAuthFailed	= 6023,
	PigEventID_Performance_LowerBound	= 6060,
	PigEventID_ShipAngleThreshold	= 6061,
	PigEventID_ShipDistanceThreshold	= 6062,
	PigEventID_ShipsUpdateLatencyThreshold	= 6063,
	PigEventID_Performance_UpperBound	= 6079,
	PigEventID_AutoUpdate_LowerBound	= 6080,
	PigEventID_AutoUpdateSucceeded	= 6081,
	PigEventID_AutoUpdateRestart	= 6082,
	PigEventID_AutoUpdateError	= 6083,
	PigEventID_AutoUpdate_UpperBound	= 6099,
	PigEventID_StateChange_LowerBound	= 6100,
	PigEventID_StateChange_NonExistant	= 6101,
	PigEventID_StateChange_LoggingOn	= 6102,
	PigEventID_StateChange_LoggingOff	= 6103,
	PigEventID_StateChange_MissionList	= 6104,
	PigEventID_StateChange_CreatingMission	= 6105,
	PigEventID_StateChange_JoiningMission	= 6106,
	PigEventID_StateChange_QuittingMission	= 6107,
	PigEventID_StateChange_TeamList	= 6108,
	PigEventID_StateChange_JoiningTeam	= 6109,
	PigEventID_StateChange_WaitingForMission	= 6110,
	PigEventID_StateChange_Docked	= 6111,
	PigEventID_StateChange_Launching	= 6112,
	PigEventID_StateChange_Flying	= 6113,
	PigEventID_StateChange_Terminated	= 6114,
	PigEventID_StateChange_UpperBound	= 6129,
	PigEventID_PigObject_UpperBound	= 6199,
	PigEventID_PigScript_LowerBound	= 6200,
	PigEventID_ScriptError	= 6201,
	PigEventID_Trace	= 6202,
	PigEventID_ScriptsLoaded	= 6203,
	PigEventID_ScriptBaseRecursive	= 6204,
	PigEventID_ScriptBaseError	= 6205,
	PigEventID_ScriptBaseNonExistant	= 6206,
	PigEventID_PigScript_UpperBound	= 6399,
	PigEventID_Pigs_UpperBound	= 7999
} AGCEventID;

HRESULT ReadStringFromStream(IStream* pStm, BSTR* pbstr, BYTE bIndicator = 0x00); //Just for default

// Ernest Laurentin - http://www.ernzo.com/base64enc.aspx 8/10 by Imago
typedef unsigned char uchar;
class CBase64;
class CBase64
{
public:
	CBase64() { }
	static inline uchar to_uchar (char c) { return c; }
	size_t CalcBufferSize(size_t length) const { return ((length+2)/3)*4+(length/BASE64_NCHAR)+1; };
	bool IsBase64Char(uchar c) const { return ( Decode( c ) != (uchar)-1 ); }
	uchar Encode(uchar c) const { return ( c < 64 ? base64_encode[ c ] : (uchar)-1 ); }
	size_t Encode(const uchar* in, size_t length, uchar* outbuf, size_t size, UINT option = BASE64_STANDARD) const;
	uchar Decode(uchar c) const;
	size_t Decode(const uchar* in, size_t length, uchar* outbuf, size_t size, UINT option = BASE64_STANDARD) const;

private:
	enum {
		BASE64_PAD      = '=',	///< Pad character
		BASE64_CR       = '\r',	///< Carriage return
		BASE64_LF       = '\n',	///< New line separator
		BASE64_NCHAR    = 76,	///< Number of characters per line when encoding
		BASE64_STANDARD = 0x01	///< Multi line support
	};
	static uchar base64_encode[];	///< Base64 Alphabet encoder-string
};
__declspec(selectany) uchar CBase64::base64_encode[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//End Imago

HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA)
{

    ULONG cbAnsi, cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (pszW == NULL)
    {
        *ppszA = NULL;
        return NOERROR;
    }

    cCharacters = wcslen(pszW)+1;
    // Determine number of bytes to be allocated for ANSI string. An
    // ANSI string can have at most 2 bytes per character (for Double
    // Byte Character Strings.)
    cbAnsi = cCharacters*2;

    // Use of the OLE allocator is not required because the resultant
    // ANSI  string will never be passed to another COM component. You
    // can use your own allocator.
    *ppszA = (LPSTR) CoTaskMemAlloc(cbAnsi);
    if (NULL == *ppszA)
        return E_OUTOFMEMORY;

    // Convert to ANSI.
    if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,
                  cbAnsi, NULL, NULL))
    {
        dwError = GetLastError();
        CoTaskMemFree(*ppszA);
        *ppszA = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }
    return NOERROR;

}

#endif