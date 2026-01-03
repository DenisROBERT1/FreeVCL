//-----------------------------------------------------------------------------
//! @file WebCam.h
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
#ifndef _WEBCAM_H
#define _WEBCAM_H
#if __GNUC__ >= 3
#pragma GCC system_header
#endif

#include <vfw.h>

//---------------------------------------------------------------------------
// Prototypes et macros manquants dans certaines versions de vfw.h
// entre autres capDriverConnect (donc on teste sur cette variable)
//---------------------------------------------------------------------------

#ifndef capDriverConnect

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RC_INVOKE

HWND VFWAPI capCreateCaptureWindowA(LPCSTR,DWORD,int,int,int,int,HWND,int);
HWND VFWAPI capCreateCaptureWindowW(LPCWSTR,DWORD,int,int,int,int,HWND,int);
BOOL VFWAPI capGetDriverDescriptionA(WORD,LPSTR,INT,LPSTR,INT);
BOOL VFWAPI capGetDriverDescriptionW(WORD,LPWSTR,INT,LPWSTR,INT);


// ------------------------------------------------------------------
//  Macros
// ------------------------------------------------------------------

#define capCaptureStop(hwnd) ((BOOL) SendMessage((hwnd), WM_CAP_STOP, (WPARAM) 0, 0L))
#define capDlgVideoSource(hwnd) ((BOOL) SendMessage((hwnd), WM_CAP_DLG_VIDEOSOURCE, (WPARAM) 0, 0L))
#define capCaptureSequenceNoFile(hwnd) ((BOOL) SendMessage((hwnd), WM_CAP_SEQUENCE_NOFILE, (WPARAM) 0, 0L))
#define capDlgVideoCompression(hwnd) ((BOOL) SendMessage((hwnd), WM_CAP_DLG_VIDEOCOMPRESSION, (WPARAM) 0, 0L))
#define capDlgVideoDisplay(hwnd) ((BOOL) SendMessage((hwnd), WM_CAP_DLG_VIDEODISPLAY, (WPARAM) 0, 0L))
#define capDlgVideoFormat(hwnd) ((BOOL) SendMessage((hwnd), WM_CAP_DLG_VIDEOFORMAT, (WPARAM) 0, 0L))
#define capDriverConnect(hwnd, iIndex) ((BOOL) SendMessage((hwnd), WM_CAP_DRIVER_CONNECT, (WPARAM) (iIndex), 0L))
#define capDriverDisconnect(hwnd) ((BOOL) SendMessage((hwnd), WM_CAP_DRIVER_CONNECT, 0, 0L))
#define capDriverGetName(hwnd, szName, wSize) ((BOOL) SendMessage((hwnd), WM_CAP_DRIVER_GET_NAME, (WPARAM) (wSize), (LPARAM) (LPVOID) (LPSTR) (szName)))
#define capGetVideoFormat(hwnd, psVideoFormat, wSize) ((DWORD) SendMessage((hwnd), WM_CAP_GET_VIDEOFORMAT, (WPARAM) (wSize), (LPARAM) (LPVOID) (psVideoFormat)))
#define capGrabFrameNoStop(hwnd) ((BOOL) SendMessage((hwnd), WM_CAP_GRAB_FRAME_NOSTOP, 0, 0L))
#define capPreview(hwnd, f) ((BOOL) SendMessage((hwnd), WM_CAP_SET_PREVIEW, (WPARAM) (BOOL) (f), 0L))
#define capPreviewRate(hwnd, wMS) ((BOOL) SendMessage((hwnd), WM_CAP_SET_PREVIEWRATE, (WPARAM) (wMS), 0L))
#define capPreviewScale(hwnd, f) ((BOOL) SendMessage((hwnd), WM_CAP_SET_SCALE, (WPARAM) (BOOL) (f), 0L))
#define capSetCallbackOnFrame(hwnd, fpProc) ((BOOL) SendMessage((hwnd), WM_CAP_SET_CALLBACK_FRAME, (WPARAM) 0, (LPARAM) (LPVOID) (fpProc)))

// ------------------------------------------------------------------
//  Structures
// ------------------------------------------------------------------

#pragma pack(push, 1)

typedef struct videohdr_tag {
    LPBYTE      lpData;
    DWORD       dwBufferLength;
    DWORD       dwBytesUsed;
    DWORD       dwTimeCaptured;
    DWORD       dwUser;
    DWORD       dwFlags;
    DWORD       dwReserved[4];
} VIDEOHDR, *PVIDEOHDR, *LPVIDEOHDR;

#pragma pack(pop)

#define VHDR_DONE       0x00000001
#define VHDR_PREPARED   0x00000002
#define VHDR_INQUEUE    0x00000004
#define VHDR_KEYFRAME   0x00000008


// ------------------------------------------------------------------
//  Windows message
// ------------------------------------------------------------------

// Defines start of the message range
#define WM_CAP_START                    WM_USER

// start of unicode messages
#define WM_CAP_UNICODE_START            WM_USER + 100

#define WM_CAP_GET_CAPSTREAMPTR         (WM_CAP_START + 1)

#define WM_CAP_SET_CALLBACK_ERRORW      (WM_CAP_UNICODE_START + 2)
#define WM_CAP_SET_CALLBACK_STATUSW     (WM_CAP_UNICODE_START + 3)
#define WM_CAP_SET_CALLBACK_ERRORA      (WM_CAP_START + 2)
#define WM_CAP_SET_CALLBACK_STATUSA     (WM_CAP_START + 3)
#ifdef UNICODE
#define WM_CAP_SET_CALLBACK_ERROR        WM_CAP_SET_CALLBACK_ERRORW
#define WM_CAP_SET_CALLBACK_STATUS       WM_CAP_SET_CALLBACK_STATUSW
#else
#define WM_CAP_SET_CALLBACK_ERROR        WM_CAP_SET_CALLBACK_ERRORA
#define WM_CAP_SET_CALLBACK_STATUS       WM_CAP_SET_CALLBACK_STATUSA
#endif

#define WM_CAP_SET_CALLBACK_YIELD        (WM_CAP_START + 4)
#define WM_CAP_SET_CALLBACK_FRAME        (WM_CAP_START + 5)
#define WM_CAP_SET_CALLBACK_VIDEOSTREAM  (WM_CAP_START + 6)
#define WM_CAP_SET_CALLBACK_WAVESTREAM   (WM_CAP_START + 7)
#define WM_CAP_GET_USER_DATA             (WM_CAP_START + 8)
#define WM_CAP_SET_USER_DATA             (WM_CAP_START + 9)

#define WM_CAP_DRIVER_CONNECT            (WM_CAP_START + 10)
#define WM_CAP_DRIVER_DISCONNECT         (WM_CAP_START + 11)

#define WM_CAP_DRIVER_GET_NAMEA          (WM_CAP_START + 12)
#define WM_CAP_DRIVER_GET_VERSIONA       (WM_CAP_START + 13)
#define WM_CAP_DRIVER_GET_NAMEW          (WM_CAP_UNICODE_START + 12)
#define WM_CAP_DRIVER_GET_VERSIONW       (WM_CAP_UNICODE_START + 13)
#ifdef UNICODE
#define WM_CAP_DRIVER_GET_NAME           WM_CAP_DRIVER_GET_NAMEW
#define WM_CAP_DRIVER_GET_VERSION        WM_CAP_DRIVER_GET_VERSIONW
#else
#define WM_CAP_DRIVER_GET_NAME           WM_CAP_DRIVER_GET_NAMEA
#define WM_CAP_DRIVER_GET_VERSION        WM_CAP_DRIVER_GET_VERSIONA
#endif

#define WM_CAP_DRIVER_GET_CAPS           (WM_CAP_START + 14)

#define WM_CAP_FILE_SET_CAPTURE_FILEA    (WM_CAP_START + 20)
#define WM_CAP_FILE_GET_CAPTURE_FILEA    (WM_CAP_START + 21)
#define WM_CAP_FILE_SAVEASA              (WM_CAP_START + 23)
#define WM_CAP_FILE_SAVEDIBA             (WM_CAP_START + 25)
#define WM_CAP_FILE_SET_CAPTURE_FILEW    (WM_CAP_UNICODE_START + 20)
#define WM_CAP_FILE_GET_CAPTURE_FILEW    (WM_CAP_UNICODE_START + 21)
#define WM_CAP_FILE_SAVEASW              (WM_CAP_UNICODE_START + 23)
#define WM_CAP_FILE_SAVEDIBW             (WM_CAP_UNICODE_START + 25)
#ifdef UNICODE
#define WM_CAP_FILE_SET_CAPTURE_FILE     WM_CAP_FILE_SET_CAPTURE_FILEW
#define WM_CAP_FILE_GET_CAPTURE_FILE     WM_CAP_FILE_GET_CAPTURE_FILEW
#define WM_CAP_FILE_SAVEAS               WM_CAP_FILE_SAVEASW
#define WM_CAP_FILE_SAVEDIB              WM_CAP_FILE_SAVEDIBW
#else
#define WM_CAP_FILE_SET_CAPTURE_FILE     WM_CAP_FILE_SET_CAPTURE_FILEA
#define WM_CAP_FILE_GET_CAPTURE_FILE     WM_CAP_FILE_GET_CAPTURE_FILEA
#define WM_CAP_FILE_SAVEAS               WM_CAP_FILE_SAVEASA
#define WM_CAP_FILE_SAVEDIB              WM_CAP_FILE_SAVEDIBA
#endif

// out of order to save on ifdefs
#define WM_CAP_FILE_ALLOCATE             (WM_CAP_START + 22)
#define WM_CAP_FILE_SET_INFOCHUNK        (WM_CAP_START + 24)

#define WM_CAP_EDIT_COPY                 (WM_CAP_START + 30)

#define WM_CAP_SET_AUDIOFORMAT           (WM_CAP_START + 35)
#define WM_CAP_GET_AUDIOFORMAT           (WM_CAP_START + 36)

#define WM_CAP_DLG_VIDEOFORMAT           (WM_CAP_START + 41)
#define WM_CAP_DLG_VIDEOSOURCE           (WM_CAP_START + 42)
#define WM_CAP_DLG_VIDEODISPLAY          (WM_CAP_START + 43)
#define WM_CAP_GET_VIDEOFORMAT           (WM_CAP_START + 44)
#define WM_CAP_SET_VIDEOFORMAT           (WM_CAP_START + 45)
#define WM_CAP_DLG_VIDEOCOMPRESSION      (WM_CAP_START + 46)

#define WM_CAP_SET_PREVIEW               (WM_CAP_START + 50)
#define WM_CAP_SET_OVERLAY               (WM_CAP_START + 51)
#define WM_CAP_SET_PREVIEWRATE           (WM_CAP_START + 52)
#define WM_CAP_SET_SCALE                 (WM_CAP_START + 53)
#define WM_CAP_GET_STATUS                (WM_CAP_START + 54)
#define WM_CAP_SET_SCROLL                (WM_CAP_START + 55)

#define WM_CAP_GRAB_FRAME                (WM_CAP_START + 60)
#define WM_CAP_GRAB_FRAME_NOSTOP         (WM_CAP_START + 61)

#define WM_CAP_SEQUENCE                  (WM_CAP_START + 62)
#define WM_CAP_SEQUENCE_NOFILE           (WM_CAP_START + 63)
#define WM_CAP_SET_SEQUENCE_SETUP        (WM_CAP_START + 64)
#define WM_CAP_GET_SEQUENCE_SETUP        (WM_CAP_START + 65)

#define WM_CAP_SET_MCI_DEVICEA           (WM_CAP_START + 66)
#define WM_CAP_GET_MCI_DEVICEA           (WM_CAP_START + 67)
#define WM_CAP_SET_MCI_DEVICEW           (WM_CAP_UNICODE_START + 66)
#define WM_CAP_GET_MCI_DEVICEW           (WM_CAP_UNICODE_START + 67)
#ifdef UNICODE
#define WM_CAP_SET_MCI_DEVICE            WM_CAP_SET_MCI_DEVICEW
#define WM_CAP_GET_MCI_DEVICE            WM_CAP_GET_MCI_DEVICEW
#else
#define WM_CAP_SET_MCI_DEVICE            WM_CAP_SET_MCI_DEVICEA
#define WM_CAP_GET_MCI_DEVICE            WM_CAP_GET_MCI_DEVICEA
#endif

#define WM_CAP_STOP                      (WM_CAP_START + 68)
#define WM_CAP_ABORT                     (WM_CAP_START + 69)

#define WM_CAP_SINGLE_FRAME_OPEN         (WM_CAP_START + 70)
#define WM_CAP_SINGLE_FRAME_CLOSE        (WM_CAP_START + 71)
#define WM_CAP_SINGLE_FRAME              (WM_CAP_START + 72)

#define WM_CAP_PAL_OPENA                 (WM_CAP_START + 80)
#define WM_CAP_PAL_SAVEA                 (WM_CAP_START + 81)
#define WM_CAP_PAL_OPENW                 (WM_CAP_UNICODE_START + 80)
#define WM_CAP_PAL_SAVEW                 (WM_CAP_UNICODE_START + 81)
#ifdef UNICODE
#define WM_CAP_PAL_OPEN                   WM_CAP_PAL_OPENW
#define WM_CAP_PAL_SAVE                   WM_CAP_PAL_SAVEW
#else
#define WM_CAP_PAL_OPEN                   WM_CAP_PAL_OPENA
#define WM_CAP_PAL_SAVE                   WM_CAP_PAL_SAVEA
#endif

#define WM_CAP_PAL_PASTE                  (WM_CAP_START + 82)
#define WM_CAP_PAL_AUTOCREATE             (WM_CAP_START + 83)
#define WM_CAP_PAL_MANUALCREATE           (WM_CAP_START + 84)

// Following added post VFW 1.1
#define WM_CAP_SET_CALLBACK_CAPCONTROL    (WM_CAP_START + 85)

// Defines end of the message range
#define WM_CAP_UNICODE_END                WM_CAP_PAL_SAVEW
#define WM_CAP_END                        WM_CAP_UNICODE_END


// ------------------------------------------------------------------
//  UNICODE
// ------------------------------------------------------------------

#ifdef UNICODE
#define capCreateCaptureWindow capCreateCaptureWindowW
#define capGetDriverDescription capGetDriverDescriptionW
#else
#define capCreateCaptureWindow capCreateCaptureWindowA
#define capGetDriverDescription capGetDriverDescriptionA
#endif

#endif /* RC_INVOKED */

#ifdef __cplusplus
}
#endif

#endif /* capDriverConnect */

#endif /* _WINDNS_H */
