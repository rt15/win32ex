#include "layer001/RtGdiP.h"

#include "layer000/RtWin32ExGuiOsDefines.h"

RT_FAST_INITIALIZATION rt_gdipInitialization = RT_FAST_INITIALIZATION_STATIC_INIT;

RT_UN rt_nGdipToken;

RT_B RT_API RtGdipInitialize()
{
  GdiplusStartupInput gdipStartupInput;
  GpStatus nStatus;
  RT_B bResult;

  if (RtFastInitializationRequired(&rt_gdipInitialization))
  {
    gdipStartupInput.GdiplusVersion = 1;
    gdipStartupInput.DebugEventCallback  = RT_NULL;
    gdipStartupInput.SuppressBackgroundThread = RT_FALSE;
    gdipStartupInput.SuppressExternalCodecs = RT_FALSE;
    nStatus = GdiplusStartup(&rt_nGdipToken, &gdipStartupInput, RT_NULL);
    if (nStatus != Ok)
    {
      RtGdipSetLastErrorFromGpStatus(nStatus);
      bResult = RT_FAILURE;
    }
    else
    {
      bResult = RT_SUCCESS;
    }

    RtNotifyFastInitializationDone(&rt_gdipInitialization);
  }
  else
  {
    bResult = RT_SUCCESS;
  }
  return bResult;
}

RT_B RT_API RtGdipCleanUp()
{
  if (RtIsFastInitializationDone(&rt_gdipInitialization))
  {
    /* Returns void. */
    GdiplusShutdown(rt_nGdipToken);
  }
  return RT_TRUE;
}

void RT_API RtGdipSetLastErrorFromGpStatus(RT_UN unStatus)
{
  switch (unStatus)
  {
    case Ok:
      SetLastError(ERROR_SUCCESS);
      break;
    case InvalidParameter:
      RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
      break;
    case OutOfMemory:
      SetLastError(ERROR_OUTOFMEMORY);
      break;
    case ObjectBusy:
      SetLastError(ERROR_BUSY);
      break;
    case InsufficientBuffer:
      RtSetLastError(RT_ERROR_INSUFFICIENT_BUFFER);
      break;
    case FileNotFound:
      SetLastError(ERROR_FILE_NOT_FOUND);
      break;
    case ValueOverflow:
      RtSetLastError(RT_ERROR_ARITHMETIC_OVERFLOW);
      break;
    case AccessDenied:
      SetLastError(ERROR_ACCESS_DENIED);
      break;
    case Win32Error:
    case NotImplemented:
    case WrongState:
    case Aborted:
    case UnknownImageFormat:
    case FontFamilyNotFound:
    case FontStyleNotFound:
    case GdiplusNotInitialized:
    case PropertyNotFound:
    case PropertyNotSupported:
    case GenericError:
    case NotTrueTypeFont:
    case UnsupportedGdiplusVersion:
    default:
       RtSetLastError(RT_ERROR_FUNCTION_FAILED);
  }
}

RT_B RT_API RtGdipStretchBitmap(RT_H hBitmap, RT_H hDc, RT_N32 nWidth, RT_N32 nHeight, RT_GDIP_INTERPOLATION_MODE nInterpolationMode)
{
  GpGraphics* lpGraphics;
  GpBitmap* lpBitmap;
  GpBrush* lpBrush;
  GpStatus nStatus;
  RT_B bResult;

  lpGraphics = RT_NULL;
  lpBitmap = RT_NULL;
  lpBrush = RT_NULL;

  /* Create destination graphics hDc. */
  nStatus = GdipCreateFromHDC(hDc, &lpGraphics);
  if (nStatus)
  {
    RtGdipSetLastErrorFromGpStatus(nStatus);
    goto handle_error;
  }

  /* Adjust quality. */
  nStatus = GdipSetInterpolationMode(lpGraphics, (InterpolationMode)nInterpolationMode);
  if (nStatus)
  {
    RtGdipSetLastErrorFromGpStatus(nStatus);
    goto handle_error;
  }

  /* Create a brush to fill the background. */
  nStatus = GdipCreateSolidFill(0xFFFFFFFF, &lpBrush);
  if (nStatus)
  {
    RtGdipSetLastErrorFromGpStatus(nStatus);
    goto handle_error;
  }

  /* Fill the destination DC with a background. Necessary to avoid edges artifacts. */
  nStatus = GdipFillRectangleI(lpGraphics, lpBrush, 0, 0, nWidth, nHeight);
  if (nStatus)
  {
    RtGdipSetLastErrorFromGpStatus(nStatus);
    goto handle_error;
  }

  /* Create source image/bitmap from hBitmap. */
  nStatus = GdipCreateBitmapFromHBITMAP(hBitmap, RT_NULL, &lpBitmap);
  if (nStatus)
  {
    RtGdipSetLastErrorFromGpStatus(nStatus);
    goto handle_error;
  }

  /* Stretch. */
  nStatus = GdipDrawImageRectI(lpGraphics, lpBitmap, 0, 0, nWidth, nHeight);
  if (nStatus)
  {
    RtGdipSetLastErrorFromGpStatus(nStatus);
    goto handle_error;
  }

  bResult = RT_SUCCESS;
  goto free_resources;
handle_error:
  bResult = RT_FAILURE;
free_resources:
  if (lpGraphics)
  {
    nStatus = GdipDeleteGraphics(lpGraphics);
    lpGraphics = RT_NULL;
    if (nStatus && bResult)
    {
      RtGdipSetLastErrorFromGpStatus(nStatus);
      goto handle_error;
    }
  }
  if (lpBitmap)
  {
    nStatus = GdipDisposeImage(lpBitmap);
    lpBitmap = RT_NULL;
    if (nStatus && bResult)
    {
      RtGdipSetLastErrorFromGpStatus(nStatus);
      goto handle_error;
    }
  }
  if (lpBrush)
  {
    nStatus = GdipDeleteBrush(lpBrush);
    lpBrush = RT_NULL;
    if (nStatus && bResult)
    {
      RtGdipSetLastErrorFromGpStatus(nStatus);
      goto handle_error;
    }
  }
  return bResult;
}
