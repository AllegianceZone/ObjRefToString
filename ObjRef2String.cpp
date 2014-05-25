// ObjRef2String.cpp : Defines the entry point for the console application.
// Imago 8/10 #50

#include "stdafx.h"
#include "ObjRef2String.h"

// Global members
// This is intended to be setup and destroyed once per object (from some other program... such as a JSON-RPC/SQL Handler)
int			   m_id = 0;
DATE           m_time = 0;
CComBSTR       m_bstrComputerName = "(none)";
long           m_idSubject = 0;
CComBSTR       m_bstrSubjectName = "(none)";
CComBSTR       m_bstrContext = "(none)";
XProperties    m_Properties;
IPersistStreamPtr   m_spPersist;
CComPtr<IUnknown>	m_punkMBV;

// Similar to CreateVARIANTFromData but way uglier
bool CreateBSTRFromVARIANT(BSTR * pbstr, CComVariant * value) {
	char * szString = new char[c_iMaxRef];
    // Get the variant type and value
    VARTYPE vt = value->vt;
    switch (vt)
    {
		 case VT_BOOL: 
		{
			 VARIANT_BOOL var = value->boolVal;
			 if (var == VARIANT_TRUE) 
				 sprintf(szString,"true");
			 else
				 sprintf(szString,"false");
			 *pbstr = _com_util::ConvertStringToBSTR(szString);
			 return true;
		}
		 case VT_I1: { //char
			 CHAR var = value->cVal;
			 _itoa(var,szString,10);
			 *pbstr = _com_util::ConvertStringToBSTR(szString);
			 return true;
					 }
		 case VT_I2: { //short
			 short var = value->iVal;
			 _itoa(var,szString,10);
			 *pbstr = _com_util::ConvertStringToBSTR(szString);
			 return true;
					 }
		 case VT_I4: { //long
		 case VT_ERROR:
			 long var = value->lVal;
			 sprintf(szString,"%d",var);
			 *pbstr = _com_util::ConvertStringToBSTR(szString);
			 return true;
					 }
		 case VT_UI1: { //byte
			 BYTE var = value->bVal;
			 sprintf(szString,"%hu",var);
			 *pbstr = _com_util::ConvertStringToBSTR(szString);
			 return true;
					  }
		 case VT_UI2: { //unsigned short
			 unsigned short var = value->iVal;
			 sprintf(szString,"%hu",var);
			 *pbstr = _com_util::ConvertStringToBSTR(szString);
			 return true;
					  }
		 case VT_R4: //float
					{
			float var = value->fltVal;
			sprintf(szString,"%G",var);
			*pbstr = _com_util::ConvertStringToBSTR(szString);
			return true;
					}
		 case VT_DATE: {
			 SYSTEMTIME time;
			 ::VariantTimeToSystemTime(value->date, &time);
			 sprintf(szString,"%04d-%02d-%02d %02d:%02d:%02d.%03d",time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
			 *pbstr = _com_util::ConvertStringToBSTR(szString);
			 return true;
					   }
		 case VT_R8: { //double
			double var = value->dblVal;
			sprintf(szString,"%G",var);
			*pbstr = _com_util::ConvertStringToBSTR(szString);
			return true;
					   }
		 case VT_DECIMAL: { //a spicey signed 64bit
			 //NYI
			 break;
						  }
		 case VT_CY: { //32low & 32high
			 CY var =  value->cyVal;
			 sprintf(szString,"%d,%d",var.Hi,var.Lo);
			 *pbstr = _com_util::ConvertStringToBSTR(szString);
			 return true;
					 }
		 case VT_BSTR: { // Hey look, it's what we're after anyways, lucky us
			 *pbstr = value->bstrVal;
			 return true;
					   }
		 case VT_UNKNOWN: 
		 case VT_DISPATCH: { // value is output as a GUID and base64 encoded c data data seperated by a comma
			ITypeInfo* pTypeInfo;
			TYPEATTR* ppTypeAttr;
			HRESULT hr = V_DISPATCH(value)->GetTypeInfo(0,LOCALE_SYSTEM_DEFAULT,&pTypeInfo);
			if (hr != S_OK)
				break;
			pTypeInfo->GetTypeAttr(&ppTypeAttr);
			WCHAR* wzGuid;
			hr = StringFromCLSID( ppTypeAttr->guid, &wzGuid );
			if (hr != S_OK || ppTypeAttr->typekind != TKIND_DISPATCH)
				break;
			char * guid = _com_util::ConvertBSTRToString(wzGuid);
			IUnknown* punk = value->punkVal;
			IPersistStreamPtr sppstm(punk);
			if (sppstm == NULL)
				break;
			char * pszAnsi = (char*)::VirtualAlloc(NULL, c_iMaxRef / 2, MEM_COMMIT, PAGE_READWRITE); 
			char * psz64 = (char*)::VirtualAlloc(NULL, c_iMaxRef * 2, MEM_COMMIT, PAGE_READWRITE);
			TCSimpleStream stm;
			stm.Init(c_iMaxRef / 2, pszAnsi);
			sppstm->Save(&stm,TRUE);
			ULARGE_INTEGER cbSize; LARGE_INTEGER dlibMove = {0, 0};
			stm.Seek(dlibMove, STREAM_SEEK_CUR, &cbSize);
			if (cbSize.HighPart != 0 || cbSize.LowPart <= 0)
				break;
			size_t size = c_iMaxRef * 2;
			CBase64 * c64 = new CBase64;
			c64->Encode((BYTE*)pszAnsi,cbSize.LowPart,(BYTE*)psz64,size);
			*pbstr = _com_util::ConvertStringToBSTR(guid);
			wcscat(*pbstr,_T(","));
			wcscat(*pbstr,_com_util::ConvertStringToBSTR((char*)psz64));
			::VirtualFree(pszAnsi,c_iMaxRef / 2,MEM_RELEASE);
			::VirtualFree(psz64,c_iMaxRef * 2,MEM_RELEASE);
			return true;
		 }
		 case VT_VARIANT: {
			// Could recurse here to support VT_VARIANT - NOT!
			break;
						  }
		  case VT_LPSTR:
		  {
			LPCSTR psz = value->pcVal;
			*pbstr = _com_util::ConvertStringToBSTR(psz);
			return true;
		  }
		  case VT_LPWSTR:
		  {
			LPCWSTR psz = (LPCWCHAR)value->pcVal;
			wcscpy(*pbstr,psz);
			return true;
		  }
		  default: //BYREF - that would probably only working on the running server?
			  {
				  break;
			  }
    }
	return false; //fail
}

// Fixed...
HRESULT ReadStringFromStream(IStream* pStm, BSTR* pbstr, BYTE bIndicator)
{
  assert(!*pbstr);

  // Read the size indicator if zero was specified
  if (!bIndicator)
  {
    RETURN_FAILED(pStm->Read(&bIndicator, sizeof(bIndicator), NULL));
  }

  // Read the number of ANSI character to follow
  ULONG cchAnsi;
  switch (bIndicator)
  {
    case LengthZero:
      return S_OK;
    case Length1Byte:
    {
      BYTE cch;
      RETURN_FAILED(pStm->Read(&cch, sizeof(cch), NULL));
      cchAnsi = cch;
      break;
    }
    case Length2Bytes:
    {
      WORD cch;
      RETURN_FAILED(pStm->Read(&cch, sizeof(cch), NULL));
      cchAnsi = cch;
      break;
    }
    case Length4Bytes:
    {
      RETURN_FAILED(pStm->Read(&cchAnsi, sizeof(cchAnsi), NULL));
      break;
    }
    default:
		return -1;
  }

  if (cchAnsi)
  {
	// Now with no stack corruption
	LPSTR pszAnsi = (LPSTR)::VirtualAlloc(NULL, cchAnsi, MEM_COMMIT, PAGE_READWRITE);
    RETURN_FAILED(pStm->Read(pszAnsi, cchAnsi, NULL));
	*pbstr = _com_util::ConvertStringToBSTR(pszAnsi);
	::VirtualFree(pszAnsi,0,MEM_RELEASE);
  }
 
  // Indicate success
  return S_OK;
}

// More AGC/ATL copy/paste
HRESULT ReadVariantFromStream(IStream* pStm, CComVariant& var)
{
  // Clear the specified variant
  var.Clear();

  // Read the custom byte indicator
  BYTE bCustom;
  RETURN_FAILED(pStm->Read(&bCustom, sizeof(bCustom), NULL));
  if (bCustom)
  {
    // Delegate to ReadStringFromStream
    V_VT(&var) = VT_BSTR;
    V_BSTR(&var) = NULL;
    RETURN_FAILED(ReadStringFromStream(pStm, &V_BSTR(&var), bCustom));
  }
  else
  {
    // Delegate non-custom variant types to CComVariant's implementation
	HRESULT hr = var.ReadFromStream(pStm);
  }

  // Indicate success
  return S_OK;
}

HRESULT LoadFromStream(IStream* pStm)
{
  // Read the signature string (allows for future version checking)
  char szSignature[2];
  RETURN_FAILED(pStm->Read(szSignature, sizeof(szSignature), NULL));

  // Check the signature string
  int iSignature = -1;
  const static LPCSTR szSignatures[] = {"0:", "1:"};
  for (int i = 0; i < sizeofArray(szSignatures); ++i)
  {
    if (0 == strncmp(szSignature, szSignatures[i], strlen(szSignatures[i])))
    {
      iSignature = i;
      break;
    }
  }
  if (-1 == iSignature)
    return E_INVALIDARG;

  // Read the ID and time
  RETURN_FAILED(pStm->Read(&m_id, sizeof(m_id), NULL));
  RETURN_FAILED(pStm->Read(&m_time, sizeof(m_time), NULL));

  // Read the number of key/item pairs being read
  long cCount = 0;
  RETURN_FAILED(pStm->Read(&cCount, sizeof(cCount), NULL));
  
  // Read each pair from the stream
  while (cCount--)
  {
    // Read the Key/Item pair from the stream
    CComBSTR    bstrKey;
    CComVariant varItem;
    RETURN_FAILED(ReadStringFromStream(pStm, &bstrKey));
    RETURN_FAILED(ReadVariantFromStream(pStm, varItem));
    m_Properties.insert(XProperties::value_type(bstrKey, varItem));
  }
  
  // Read the computer name
  m_bstrComputerName.Empty();
  RETURN_FAILED(ReadStringFromStream(pStm, &m_bstrComputerName));

  // Read the subject ID and name
  RETURN_FAILED(pStm->Read(&m_idSubject, sizeof(m_idSubject), NULL));
  m_bstrSubjectName.Empty();
  RETURN_FAILED(ReadStringFromStream(pStm, &m_bstrSubjectName));

  // Read the context string
  m_bstrContext.Empty();
  if (iSignature >= 1)
  {
    RETURN_FAILED(ReadStringFromStream(pStm, &m_bstrContext));
  }

  // Indicate success
  return S_OK;
}

// Get Popcorn!
int _tmain(int argc, _TCHAR* argv[])
{
	// Use Debugging
	// what an 'encrypted' object reference may look like ;-)
	//ZString in = "tc00:1:&BE0F0000BBC6;|&1D;D&BCE3;@&13000000F107;Message&F113;This is just a test&F106;Source&000300FFFFFFFFF10B;Source Name&F107;Cheater&F106;Target&F106;AllSrv&F104;Type&F106;Cheat!&F107;VT_BOOL&000B00FFFFF107;VT_BSTR&F10A;I'm a BSTR&F105;VT_CY&000600;c&0000000C000000F107;VT_DATE&000700111CD2;r=&BCE3;@&F10B;VT_DISPATCH&0009001492;$&12B513D3118B;^&00C0;Oh&16;3&030000000000; A&0000A0;A&0000F0;A&F108;VT_ERROR&000A0001;@&0080F105;VT_I1&001000;i&F105;VT_I2&0002000DF0F109;VT_LPWSTR&F111;I'm a wide string&F105;VT_R4&00040014AEC7;?&F105;VT_R8&000500000000;`&FB;!&09;@&F106;VT_UI1&00110096F106;VT_UI2&0012000FD0F10A;VT_UNKNOWN&000D001492;$&12B513D3118B;^&00C0;Oh&16;3&030000000000C8;B&0000;HC&000096;C&F107;HEATER7&FFFFFFFFF113;TestAGCEvents cheat&F111;U0827220448000200";
	//BSTR bInput = zStr.MakeBSTR();

	// Use cmd-line 
	//_TCHAR* cmdline = ::GetCommandLine();
	//_TCHAR* token = wcstok(cmdline,_T(" "));
	//BSTR bInput =  wcstok(NULL,_T(" ")); //seems to play nicely!

	//Use Unicode input
	/*
	WCHAR * tmp = new WCHAR[BUFSIZ];
	OLECHAR * in = new WCHAR[c_iMaxRef];
	ZeroMemory(tmp,BUFSIZ);
	ZeroMemory(in,c_iMaxRef);
	_setmode( _fileno( stdin ), _O_BINARY );
	while (fgetws(tmp,BUFSIZ,stdin)) {
		wcscat(in,tmp);
		if (wcsstr(tmp,_T("\0")) != NULL)
			break;
	}
	*/
	
	//Use Ansi input
	
	char * tmp = new char[BUFSIZ];
	ZeroMemory(tmp,BUFSIZ);
	ZString in; in.SetEmpty();
	setvbuf ( stdout , NULL , _IOFBF , 8096 );
	while (fgets(tmp,BUFSIZ,stdin)) {
		in += ZString(tmp);
		if (in.ReverseFindAny("\n") != -1)
			break;
	}
	

	if (in.IsEmpty())
		return E_INVALIDARG;

	// Imago wipe the map, game time
	m_Properties.clear();

	// Get the number of bytes the encoded string will decode to
	DWORD cbData = TCBinString<OLECHAR>::GetDecodedDataSize(in.MakeBSTR());
	if (0xFFFFFFFF == cbData || cbData == 0)
		return E_INVALIDARG;

	// Allocate a buffer for the decoded binary data
	void* pvData = _alloca(cbData);

	// Decode the data, the jewel
	TCBinString<OLECHAR>::DecodeFromString(in.MakeBSTR(), pvData, NULL);

	// Create a simple stream on the data buffer
	TCSimpleStream stm;
	stm.Init(cbData, pvData);

	CoInitialize(NULL); // For VT_DISPATCH (to PersistStream)

	// Load the object from the stream, another jewel (but cracked)
	RETURN_FAILED(LoadFromStream(&stm));
  
	// Print the Key & Value pairs delimited /w a tab, one for each new line.  Seperates object properties within a value using a comma,
	// We're limited to c_iMaxRef bytes here (AGC) - Currently none of the AGC Event object references (as decoded strings) come anywhere near that.
	XPropertyIt it;
	for (it = m_Properties.begin(); m_Properties.end() != it; ++it) {
		CComBSTR&    bstrKey = const_cast<CComBSTR&>(it->first);
		CComVariant& varItem = it->second;
		BSTR bstrValue = _T("(unknown)");
		if (!CreateBSTRFromVARIANT(&bstrValue,&varItem))
			_tprintf(_T("%s\t(error),%s\n"),bstrKey,bstrValue);
		else 
			_tprintf(_T("%s\t%s\n"),bstrKey,bstrValue);
		varItem.Clear();
		bstrKey.Empty();
	}

	// Show's over
	_tprintf(_T("Done\tOK\n"));
	CoUninitialize();
	fflush(stdout);
	return 0;
}

//Imago 8/10
inline uchar CBase64::Decode(uchar c) const
{
	if (c == '/')
		return 63;
	if (c == '+')
		return 62;
	if (c >= '0' && c <= '9')
		return c - '0' + 52;
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 26;
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	return (uchar)-1;
}

size_t CBase64::Encode(const uchar* in, size_t length, uchar* outbuf, size_t size, UINT option /*=BASE64_STANDARD*/) const
{
	size_t cnt = 0;
	size_t rc  = 0;
	while ( rc < length )
	{
		if ( size < 4 )
		{
			// insufficient buffer
			cnt = (size_t)-1;
			break;
		}
		size_t left = length - rc;
		switch( left )
		{
		case 1:
			*outbuf++ = Encode((in[rc] >> 2));
			*outbuf++ = Encode((in[rc] << 4) & 0x30);
			*outbuf++ = to_uchar(BASE64_PAD);
			*outbuf++ = to_uchar(BASE64_PAD);
			break;
		case 2:
			*outbuf++ = Encode((in[rc] >> 2));
			*outbuf++ = Encode(((in[rc] << 4) & 0x30) | ((in[rc + 1] >> 4) & 0x0f));
			*outbuf++ = Encode((in[rc+1] << 2) & 0x3c);
			*outbuf++ = to_uchar(BASE64_PAD);
			break;
		default:
			*outbuf++ = Encode((in[rc] >> 2));
			*outbuf++ = Encode(((in[rc] << 4) & 0x30) | ((in[rc + 1] >> 4) & 0x0f));
			*outbuf++ = Encode(((in[rc + 1] << 2) & 0x3c) | ((in[rc + 2] >> 6)));
			*outbuf++ = Encode((in[rc + 2]) & 0x3f);
			break;
		}
		rc  += 3;
		cnt += 4;
		size -= 4;
		if ( (option&BASE64_STANDARD) && !(cnt%BASE64_NCHAR))
			*outbuf++ = to_uchar(BASE64_LF);
	}

	if ( cnt != -1 )
		*outbuf = 0;
	return cnt;
}

size_t CBase64::Decode(const uchar* in, size_t length, uchar* outbuf, size_t size, UINT /*option =BASE64_STANDARD*/) const
{
	size_t cnt = 0;
	size_t rc  = 0;
	while ( (*in) && (rc < length) )
	{
		if ( (in[rc] == BASE64_CR || in[rc] == BASE64_LF))
			rc++;
		else if ( size > 1 )
		{
			uchar b1 = ( Decode(in[rc])<<2 &0xfc ) | ( Decode(in[rc+1])>>4 &0x03 );
			*outbuf++ = b1;
			--size;
			++cnt;

			if ((size>1) && in[rc + 2] != BASE64_PAD)
			{
				uchar b2 = ( Decode(in[rc+1])<<4 &0xf0 ) | ( Decode(in[rc+2])>>2 &0x0f );
				*outbuf++ = b2;
				--size;
				++cnt;
			}
			if ((size>1) && in[rc + 3] != BASE64_PAD)
			{
				uchar b3 = ( Decode(in[rc+2])<<6 &0xc0 ) | ( Decode(in[rc+3] )& 0x3f );
				*outbuf++ = b3;
				--size;
				++cnt;
			}
			rc += 4;
		}
		else
		{
			// insufficient buffer
			cnt = (size_t)-1;
			break;
		}
	}
	if ( cnt != -1 )
		*outbuf = 0;
	return cnt;
}
//End Ernest Laurentin / Imago

