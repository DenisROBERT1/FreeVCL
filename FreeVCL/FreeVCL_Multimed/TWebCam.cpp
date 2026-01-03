//-----------------------------------------------------------------------------
//! @file TWebCam.cpp
//!
//! @author Denis ROBERT (http://denis.beru.free.fr)
//!
//! Copyright 2008-2025
//! @n
//! This file is part of FreeVCL.
//! @n
//!     FreeVCL is free software: you can redistribute it and/or modify
//!     it under the terms of the GNU Lesser General Public License as
//!     published by the Free Software Foundation, either version 3 of
//!     the License, or (at your option) any later version.
//! @n
//!     FreeVCL is distributed in the hope that it will be useful,
//!     but WITHOUT ANY WARRANTY; without even the implied warranty of
//!     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//!     GNU Lesser General Public License for more details.
//! @n
//!     You should have received a copy of the GNU Lesser General Public
//!     License along with FreeVCL.
//!     If not, see <http://www.gnu.org/licenses/>.
//!
//-----------------------------------------------------------------------------

#include <FreeVcl.h>
#include <FreeVcl_Multimed.h>


//---------------------------------------------------------------------------
// Variables globales
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructeur
//---------------------------------------------------------------------------

TWebCam::TWebCam(TComponent* AOwner): TWinControl(AOwner) {
  HWND hWndParent = NULL;


  // Initialisations
  FClassName = _T("TWebCam");
	FConnected = false;
  Preview = false;
	FPreviewRate = 0;
  PreviewScale = false;
	biCompression = BI_RGB;
  if (AOwner) hWndParent = ((TControl *) AOwner)->Handle;

  FHandleWebCam = capCreateCaptureWindow(_T("TWebCam"), WS_CHILD | WS_VISIBLE,
                                   0, 0, 300, 200,
                                   hWndParent, FCommand);
	FHandle = FHandleWebCam;

  FHandleCache = CreateWindowEx(0,
																_T("Static"), FCaption,
																WS_CHILD,
																0, 0, 300, 200,
																hWndParent,
																(HMENU) (UINT_PTR) FCommand, ::HInstance, this);

}


//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------

TWebCam::~TWebCam() {
  if (FHandleWebCam) {
		if (FConnected) DisconnectWebCam();
		DestroyWindow(FHandleCache);
		FHandle = FHandleWebCam;
  }
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Connected
//---------------------------------------------------------------------------

bool TWebCam::Get_Connected(void) {
  return FConnected;
}

bool TWebCam::Set_Connected(bool NewConnected) {

  if (FConnected != NewConnected) {
    FConnected = NewConnected;
		if (FConnected) ConnectWebCam(0);
		else DisconnectWebCam();
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PreviewRate
//---------------------------------------------------------------------------

int TWebCam::Get_PreviewRate(void) {
  return FPreviewRate;
}

bool TWebCam::Set_PreviewRate(int NewPreviewRate) {

  if (FPreviewRate != NewPreviewRate) {
    FPreviewRate = NewPreviewRate;
		capPreviewRate(FHandleWebCam, FPreviewRate);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété Preview
//---------------------------------------------------------------------------

bool TWebCam::Get_Preview(void) {
  return FPreview;
}

bool TWebCam::Set_Preview(bool NewPreview) {

  if (FPreview != NewPreview) {
    FPreview = NewPreview;
    capPreview(FHandleWebCam, (BOOL) FPreview);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété PreviewScale
//---------------------------------------------------------------------------

bool TWebCam::Get_PreviewScale(void) {
  return FPreviewScale;
}

bool TWebCam::Set_PreviewScale(bool NewPreviewScale) {

  if (FPreviewScale != NewPreviewScale) {
    FPreviewScale = NewPreviewScale;
    capPreviewScale(FHandleWebCam, (BOOL) FPreviewScale);
  }
  return true;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DriverName
//---------------------------------------------------------------------------

AnsiString TWebCam::GetDriverName(int i) {
  TCHAR szDriverName[80];
  TCHAR szDriverVersion[128];


  szDriverName[0] = '\0';
  szDriverVersion[0] = '\0';
  capGetDriverDescription(i, szDriverName, sizeof(szDriverName) * sizeof(TCHAR),
                          szDriverVersion, sizeof(szDriverVersion) * sizeof(TCHAR));

  return szDriverName;
}

//---------------------------------------------------------------------------
// Accesseurs de la propriété DriverName
//---------------------------------------------------------------------------

AnsiString TWebCam::Get_DriverName(void) {
  TCHAR szDriverName[80];


  szDriverName[0] = '\0';
  capDriverGetName(FHandleWebCam, szDriverName, sizeof(szDriverName));

  return szDriverName;
}

//---------------------------------------------------------------------------
bool TWebCam::ConnectWebCam(int NumWebCam) {
  if (FHandleWebCam) {
    if (capDriverConnect(FHandleWebCam, NumWebCam)) {
      // capCaptureSequenceNoFile(FHandleWebCam);
      if (!ChangeFormatVideo()) return false;
			capSetUserData(FHandleWebCam, this);
	    if (capSetCallbackOnFrame(FHandleWebCam, CallbackOnFrame) == TRUE) {
				FConnected = true;
				return true;
			}
    }
    else {
			OnError(this, _TT("Aucune webcam trouvée"), _TT("Erreur connexion"));
			FConnected = false;
			return false;
    }
  }

	return false;
}


//---------------------------------------------------------------------------
bool TWebCam::DisconnectWebCam(void) {
  if (FHandleWebCam) {
		if (FConnected) {
			capPreview(FHandleWebCam, FALSE);
			capCaptureStop(FHandleWebCam);
			if (capDriverDisconnect(FHandleWebCam)) {
				FConnected = false;
				return true;
			}
			else {
				// Mis en commentaire car toujours une erreur ???
				// OnError(this, _TT("Déconnexion impossible"), _TT("Erreur déconnexion"));
				FConnected = false;
				return false;
			}
		}
		else {
			OnError(this, _TT("Web-cam non connectée"), _TT("Erreur déconnexion"));
			return false;
		}
  }

	return false;
}


//---------------------------------------------------------------------------
bool TWebCam::GetFrame() {
  return (capGrabFrameNoStop(FHandleWebCam) == TRUE);
}

//---------------------------------------------------------------------------
bool TWebCam::ProcessFrame(LPVIDEOHDR lpVHdr) {
  BYTE *lpBits1;
  BYTE *lpBits2;
  TBitmap *Bitmap;
  int SizeLine;
	int y;


  if (!FComponentState.Contains(csLoading)) {
		if (OnFrame) {
			Bitmap = new TBitmap();

			if (biCompression == BI_RGB) {

				// Format RGB

				Bitmap->Width = BitmapWidth;
				Bitmap->Height = BitmapHeight;
				Bitmap->PixelFormat = BitmapPixelFormat;
				DWORD dwBufferLength = lpVHdr->dwBufferLength;
				lpBits1 = (BYTE *) lpVHdr->lpData;
				lpBits2 = new BYTE[lpVHdr->dwBufferLength];
				SizeLine = BitmapWidth * (BitmapBitCount / 8);
				for (y = 0; y < BitmapHeight; y++) {
					memcpy(&lpBits2[(BitmapHeight - y - 1) * SizeLine],
								 &lpBits1[y * SizeLine],
								 SizeLine);
				}
				SetBitmapBits(Bitmap->Handle, dwBufferLength, lpBits2);
				delete[] lpBits2;
				Invalidate();

			}
			else if (biCompression == 0x32595559 ||  // Format YUY2
							 biCompression == 0x30323449) {  // Format I420

				Bitmap->Width = BitmapWidth;
				Bitmap->Height = BitmapHeight;
				Bitmap->PixelFormat = pf32bit;
				DWORD dwBufferLength = lpVHdr->dwBufferLength;
				lpBits1 = (BYTE *) lpVHdr->lpData;
				lpBits2 = new BYTE[BitmapWidth * BitmapHeight * 4];
				ConvertFormatVideo(biCompression, BitmapPixelFormat, BitmapWidth, BitmapHeight, lpBits1, lpBits2);
				SetBitmapBits(Bitmap->Handle, BitmapWidth * BitmapHeight * 4, lpBits2);

				// Affichage
				int PreviewLeft, PreviewTop, PreviewWidth, PreviewHeight;
				int DisplayLeft, DisplayTop, DisplayWidth, DisplayHeight;
				if (FPreviewScale) {
					PreviewLeft = 0;
					PreviewTop = 0;
					PreviewWidth = BitmapWidth;
					PreviewHeight = BitmapHeight;
					if (FHeight != 0 && FWidth != 0 && BitmapWidth != 0 && BitmapHeight != 0) {
						if (FWidth * BitmapHeight > FHeight * BitmapWidth) {
							DisplayWidth = FHeight * BitmapWidth / BitmapHeight;
							DisplayHeight = FHeight;
							DisplayLeft = (FWidth - DisplayWidth) / 2;
							DisplayTop = 0;
						}
						else {
							DisplayWidth = FWidth;
							DisplayHeight = FWidth * BitmapHeight / BitmapWidth;
							DisplayLeft = 0;
							DisplayTop = (FHeight - DisplayHeight) / 2;
						}
					}
					else {
							DisplayWidth = BitmapWidth;
							DisplayHeight = BitmapHeight;
							DisplayLeft = 0;
							DisplayTop = 0;
					}
				}
				else {
					PreviewLeft = 0;
					PreviewTop = 0;
					DisplayLeft = 0;
					DisplayTop = 0;
					if (BitmapWidth < FWidth) {
						PreviewWidth = BitmapWidth;
						DisplayWidth = BitmapWidth;
					}
					else {
						PreviewWidth = FWidth;
						DisplayWidth = FWidth;
					}
					if (BitmapHeight < FHeight) {
						PreviewHeight = BitmapHeight;
						DisplayHeight = BitmapHeight;
					}
					else {
						PreviewHeight = FHeight;
						DisplayHeight = FHeight;
					}
				}

				HDC hdc = GetDC(FHandleCache);
				BITMAPINFO bmi;
				memset(&bmi, 0, sizeof(bmi));
				bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biClrUsed = 0;
				bmi.bmiHeader.biWidth = BitmapWidth;
				bmi.bmiHeader.biHeight = -BitmapHeight;
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 32;
				bmi.bmiHeader.biSizeImage = BitmapWidth * BitmapHeight * 4;
				SetStretchBltMode(hdc, HALFTONE);
				StretchDIBits(hdc,
											DisplayLeft, DisplayTop, DisplayWidth, DisplayHeight,
											PreviewLeft, PreviewTop, PreviewWidth, PreviewHeight,
											lpBits2, &bmi, DIB_RGB_COLORS, SRCCOPY);
				ReleaseDC(Handle, hdc);

				delete[] lpBits2;

			}
			OnFrame(this, Bitmap);
			delete Bitmap;
		}
  }

  return true;
}

//---------------------------------------------------------------------------
bool TWebCam::ConvertFormatVideo(DWORD biCompression, TPixelFormat PixelFormat, int Width, int Height, BYTE* lpBits1, BYTE* lpBits2) {
  int SizeLine;
  int x, y;


	if (biCompression == BI_RGB) {
		// Format RGB
		int BytePerPixel;
		unsigned int Add1, Add2;
		COLORREF cr;

    switch (PixelFormat) {
      case pf15bit: BytePerPixel = 2; break;
      case pf16bit: BytePerPixel = 2; break;
      case pf24bit: BytePerPixel = 3; break;
      case pf32bit: BytePerPixel = 4; break;
      default: BytePerPixel = 0; break;
    }
		if (BytePerPixel == 0) return false;

		SizeLine = Width * BytePerPixel;
		for (y = 0; y < Height; y++) {
			Add1 = y * SizeLine;
			Add2 = (Height - y) * Width * 4;
			for (x = 0; x < Width; x++) {

				cr = * ((COLORREF *) &lpBits1[Add1]);
				Add1 += BytePerPixel;

				* ((COLORREF *) &lpBits2[Add2]) = cr;
				Add2 += 4;

			}
		}
	}
	else if (biCompression == 0x32595559) {
		// Format YUY2
		unsigned int Add1, Add2;
		int Y0, Y1, V, U;

		SizeLine = Width * 2;
		for (y = 0; y < Height; y++) {
			Add1 = y * SizeLine;
			Add2 = y * Width * 4;
			for (x = 0; x < Width; x += 2) {

				Y0 = lpBits1[Add1++] - 16;
				V = lpBits1[Add1++] - 128;
				Y1 = lpBits1[Add1++] - 16;
				U = lpBits1[Add1++] - 128;

				* ((COLORREF *) &lpBits2[Add2]) = YVUtoRGB(Y0, V, U);
				Add2 += 4;

				* ((COLORREF *) &lpBits2[Add2]) = YVUtoRGB(Y1, V, U);
				Add2 += 4;

			}
		}
	}
	else if (biCompression == 0x30323449) {
		// Format I420
		unsigned int Add1, Add2, Add3, Add4;
		int Y0, Y1, V, U;

		SizeLine = Width;
		for (y = 0; y < Height; y++) {
			Add1 = y * SizeLine;
			Add2 = Width * Height + y / 4 * SizeLine;
			Add3 = Width * Height + (Width * Height) / 4 + y / 4 * SizeLine;
			Add4 = y * Width * 4;
			for (x = 0; x < Width; x += 2) {

				Y0 = lpBits1[Add1++] - 16;
				Y1 = lpBits1[Add1++] - 16;

				V = lpBits1[Add2++] - 128;
				U = lpBits1[Add3++] - 128;

				* ((COLORREF *) &lpBits2[Add4]) = YVUtoRGB(Y0, V, U);
				Add4 += 4;

				* ((COLORREF *) &lpBits2[Add4]) = YVUtoRGB(Y1, V, U);
				Add4 += 4;

			}
		}
	}

	return true;
}

//---------------------------------------------------------------------------
COLORREF TWebCam::YVUtoRGB(int Y, int V, int U) {
	int R, G, B;


	R = Y + (113983 * V) / 100000;
	G = Y - (39465 * U + 58060 * V) / 100000;
	B = Y + (203211 * U) / 100000;
	// R = (1164 * Y0 + 1596 * V) / 1000;
	// G = (1164 * Y0 - 813 * V - 391 * U) / 1000;
	// B = (1164 * Y0 + 2018 * U) / 1000;
	// R = Y0 + (1403 * V) / 1000;
	// G = Y0 - (344 * U + 714 * V) / 1000;
	// B = Y0 + (1770 * U) / 1000;
	if (R < 0) R = 0;
	if (R > 255) R = 255;
	if (G < 0) G = 0;
	if (G > 255) G = 255;
	if (B < 0) B = 0;
	if (B > 255) B = 255;

	return RGB(R, G, B);
}

//---------------------------------------------------------------------------
bool TWebCam::DlgVideoCompression(void) {
  return (capDlgVideoCompression(FHandleWebCam) == TRUE);
}

//---------------------------------------------------------------------------
bool TWebCam::DlgVideoDisplay(void) {
  return (capDlgVideoDisplay(FHandleWebCam) == TRUE);
}

//---------------------------------------------------------------------------
bool TWebCam::DlgVideoFormat(void) {
  bool Ok;


  Ok = (capDlgVideoFormat(FHandleWebCam) == TRUE);
  if (Ok) ChangeFormatVideo();

  return Ok;
}

//---------------------------------------------------------------------------
bool TWebCam::ChangeFormatVideo() {
  BITMAPINFO VideoFormat;
  bool Ok;


	capGetVideoFormat(FHandleWebCam, &VideoFormat, sizeof(BITMAPINFO));

  BitmapWidth = VideoFormat.bmiHeader.biWidth;
  BitmapHeight = VideoFormat.bmiHeader.biHeight;
	BitmapBitCount = VideoFormat.bmiHeader.biBitCount;
  switch (BitmapBitCount) {
    case 1: BitmapPixelFormat = pf1bit; break;
    case 4: BitmapPixelFormat = pf4bit; break;
    case 8: BitmapPixelFormat = pf8bit; break;
    case 16: BitmapPixelFormat = pf16bit; break;
    case 24: BitmapPixelFormat = pf24bit; break;
    case 32: BitmapPixelFormat = pf32bit; break;
  }

	biCompression = VideoFormat.bmiHeader.biCompression;
  if (biCompression == BI_RGB) {
		MoveWindow(FHandleWebCam, FLeft, FTop, FWidth, FHeight, FALSE);
		ShowWindow(FHandleWebCam, FVisible? SW_SHOW: SW_HIDE);
		ShowWindow(FHandleCache, SW_HIDE);
		FHandle = FHandleWebCam;
    Ok = true;
  }
  else if (biCompression == 0x32595559 ||  // Format YUY2
					 biCompression == 0x30323449) {  // Format I420

		MoveWindow(FHandleCache, FLeft, FTop, FWidth, FHeight, FALSE);
		MoveWindow(FHandleWebCam, FLeft, FTop, 1, 1, FALSE);  // Un hide window bloque complètement les captures
		ShowWindow(FHandleCache, FVisible? SW_SHOW: SW_HIDE);
		FHandle = FHandleCache;
    Ok = true;
  }
  else {
		OnError(this, _TT("Formats compressés non supportés (choisissez RGB 24 ou YUY2)"), _TT("Erreur format vidéo"));
		MoveWindow(FHandleWebCam, FLeft, FTop, FWidth, FHeight, FALSE);
		ShowWindow(FHandleWebCam, FVisible? SW_SHOW: SW_HIDE);
		ShowWindow(FHandleCache, SW_HIDE);
		FHandle = FHandleWebCam;
    Ok = false;
  }

  return Ok;
}

//---------------------------------------------------------------------------
LRESULT CALLBACK TWebCam::CallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr) {

	TWebCam * _this = reinterpret_cast<TWebCam *>(capGetUserData(hWnd));
  if (_this) _this->ProcessFrame(lpVHdr);

  return 0;
}

//---------------------------------------------------------------------------
// Liste des propriétés publiées
//---------------------------------------------------------------------------
void TWebCam::GetListProperties(TStrings *ListProperties) {

  TWinControl::GetListProperties(ListProperties);

  ListProperties->Add(_T("Connected"));
  ListProperties->Add(_T("Preview"));
  ListProperties->Add(_T("PreviewScale"));
  ListProperties->Add(_T("PreviewRate"));
  ListProperties->Add(_T("OnFrame"));
  ListProperties->Add(_T("OnError"));
}

//---------------------------------------------------------------------------
// Type des propriétés publiées
//---------------------------------------------------------------------------

TYPEPROPERTY TWebCam::GetTypeProperty(AnsiString asProperty, AnsiString *asInfos) {
  if (asProperty == _T("Connected")) {
    return tpBool;
  }
  if (asProperty == _T("Preview")) {
    return tpBool;
  }
  if (asProperty == _T("PreviewScale")) {
    return tpBool;
  }
  if (asProperty == _T("PreviewRate")) {
    return tpNumber;
  }
  if (asProperty == _T("OnFrame")) {
    *asInfos = _T("(TObject *Sender, TBitmap *Frame)");
    return tpEvent;
  }
  if (asProperty == _T("OnError")) {
    *asInfos = _T("(TObject *Sender, const TCHAR *Message, const TCHAR *Title)");
    return tpEvent;
  }
  return TWinControl::GetTypeProperty(asProperty, asInfos);
}

//---------------------------------------------------------------------------
// Propriété par défaut
//---------------------------------------------------------------------------

AnsiString TWebCam::GetDefaultProperty(AnsiString asProperty) {
  if (asProperty == _T("Connected")) {
    return _T("False");
  }
  if (asProperty == _T("Preview")) {
    return _T("False");
  }
  if (asProperty == _T("PreviewScale")) {
    return _T("False");
  }
  if (asProperty == _T("PreviewRate")) {
    return _T("0");
  }
  if (asProperty == _T("OnFrame")) {
    return _T("");
  }
  if (asProperty == _T("OnError")) {
    return _T("");
  }
  return TWinControl::GetDefaultProperty(asProperty);
}

//---------------------------------------------------------------------------
// Lecture des propriétés publiées
//---------------------------------------------------------------------------

AnsiString TWebCam::GetProperty(AnsiString asProperty) {
  if (asProperty == _T("Connected")) {
    if (Get_Connected()) return _T("False");
    else return _T("False");
  }
  if (asProperty == _T("Preview")) {
    if (Get_Preview()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("PreviewScale")) {
    if (Get_PreviewScale()) return _T("True");
    else return _T("False");
  }
  if (asProperty == _T("PreviewRate")) {
    return IntToStr(Get_PreviewRate());
  }
  if (asProperty == _T("OnFrame")) {
    return OnFrame.AsString();
  }
  if (asProperty == _T("OnError")) {
    return OnError.AsString();
  }
  return TWinControl::GetProperty(asProperty);
}

//---------------------------------------------------------------------------
// Affectation des propriétés publiées
//---------------------------------------------------------------------------

bool TWebCam::SetProperty(AnsiString asProperty, AnsiString asValue,
                              TPersistent *Sender) {
  if (asProperty == _T("Connected")) {
    Set_Connected(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("Preview")) {
    Set_Preview(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("PreviewScale")) {
    Set_PreviewScale(asValue == _T("True"));
    return true;
  }
  if (asProperty == _T("PreviewRate")) {
    Set_PreviewRate(StrToInt(asValue));
    return true;
  }
  if (asProperty == _T("OnFrame")) {
		OnFrame.ObjCall = Sender;
    OnFrame = asValue.c_str();
    return true;
  }
  if (asProperty == _T("OnError")) {
		OnError.ObjCall = Sender;
    OnError = asValue.c_str();
    return true;
  }
  return TWinControl::SetProperty(asProperty, asValue, Sender);
}

//---------------------------------------------------------------------------
