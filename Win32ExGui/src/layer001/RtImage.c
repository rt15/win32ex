#include "layer001/RtImage.h"

#include "layer000/RtWin32ExGuiOsDefines.h"

RT_H RT_API RtCreateDcFromBitmap(RT_H hBitmap)
{
  RT_H hResult;

  hResult = CreateCompatibleDC(RT_NULL);
  if (hResult)
  {
    if (!SelectObject(hResult, hBitmap))
    {
      /* Free the allocated resource and notify failure. */
      DeleteDC(hResult);
      hResult = RT_NULL;
    }
  }
  return hResult;
}

RT_B RT_API RtFillDc(RT_H hDc, RT_N32 nWidth, RT_N32 nHeight, RT_UN32 unColor)
{
  HBRUSH hBrush;
  RECT rect;
  RT_B bResult;

  hBrush = CreateSolidBrush(unColor);
  if (!hBrush) goto handle_error;

  rect.left = 0;
  rect.top = 0;
  rect.right = nWidth;
  rect.bottom = nHeight;
  if (!FillRect(hDc, &rect, hBrush)) goto handle_error;

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  if (!DeleteObject(hBrush))
  {
    if (bResult)
    {
      hBrush = RT_NULL;
      goto handle_error;
    }
    hBrush = RT_NULL;
  }
  return bResult;
}

RT_B RT_API RtStretchImage(RT_H hSourceDc, RT_N32 nSourceWidth, RT_N32 nSourceHeight, RT_H hDestinationDc, RT_N32 nDestinationWidth, RT_N32 nDestinationHeight)
{
  RT_B bResult;

  if (!SetStretchBltMode(hDestinationDc, HALFTONE)) goto handle_error;
  if (!SetBrushOrgEx(hDestinationDc, 0, 0, RT_NULL)) goto handle_error;
  if (!StretchBlt(hDestinationDc, 0, 0, nDestinationWidth, nDestinationHeight, hSourceDc, 0, 0, nSourceWidth, nSourceHeight, SRCCOPY)) goto handle_error;

  bResult = RT_TRUE;
  goto free_resources;
handle_error:
  bResult = RT_FALSE;
free_resources:
  return bResult;
}

RT_B RT_API RtDrawImageList(RT_H hImageList, RT_H hDc, RT_N32 nX, RT_N32 nY)
{
  RT_N32 nImagesCount;
  RT_N32 nWidth;
  RT_N32 nHeight;
  RT_N32 nI;
  RT_B bResult;

  nImagesCount = ImageList_GetImageCount(hImageList);
  bResult = ImageList_GetIconSize(hImageList, &nWidth, &nHeight);
  if (bResult)
  {
    for (nI = 0; nI < nImagesCount; nI++)
    {
      bResult = ImageList_Draw(hImageList, nI, hDc, nX + nI * nWidth, nY, ILD_TRANSPARENT);
      if (!bResult)
      {
        break;
      }
    }
  }

  return bResult;
}

RT_H RT_API RtConvertIconToBitmap(RT_H hIcon, RT_N32 nWidth, RT_N32 nHeight, RT_UN32 unBackgroundColor)
{
  HDC hBitmapDc;
  RT_H hResult;

  hResult = RT_NULL;
  hBitmapDc = RT_NULL;

  /* Create the resulting bitmap. */
  hResult = CreateBitmap(nWidth, nHeight, 1, 32, RT_NULL);
  if (!hResult) goto handle_error;

  /* Create the DC to draw icon to. */
  hBitmapDc = RtCreateDcFromBitmap(hResult);
  if (!hBitmapDc) goto handle_error;

  /* Clean the DC using the background color. */
  if (!RtFillDc(hBitmapDc, nWidth, nHeight, unBackgroundColor)) goto handle_error;

  /* Draw the icon. */
  if (!DrawIcon(hBitmapDc, 0, 0, hIcon)) goto handle_error;

  goto free_resources;
handle_error:
  if (hResult)
  {
    DeleteObject(hResult);
    hResult = RT_NULL;
  }
free_resources:
  if (hBitmapDc)
  {
    if (!DeleteDC(hBitmapDc) && hResult)
    {
      hBitmapDc = RT_NULL;
      goto handle_error;
    }
    hBitmapDc = RT_NULL;
  }
  return hResult;
}

RT_B RT_API RtGetPixels(RT_H hBitmap, RT_N32 nWidth, RT_N32 nHeight, RT_UN32* lpBuffer, RT_H hDc)
{
  BITMAPINFO bitmapInfo;

  RtZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));
  bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bitmapInfo.bmiHeader.biWidth = nWidth;
  bitmapInfo.bmiHeader.biHeight = -nHeight; /* Top down DIB. */
  bitmapInfo.bmiHeader.biPlanes = 1;
  bitmapInfo.bmiHeader.biBitCount = 32;
  bitmapInfo.bmiHeader.biCompression = BI_RGB;

  return GetDIBits(hDc, hBitmap, 0, nHeight, lpBuffer, &bitmapInfo, DIB_RGB_COLORS);
}

RT_H RT_API RtCreateBitmap(RT_N32 nWidth, RT_N32 nHeight, RT_UN32** lpPixels, RT_H hDc)
{
  BITMAPINFO bitmapInfo;

  RtZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));
  bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bitmapInfo.bmiHeader.biWidth = nWidth;
  bitmapInfo.bmiHeader.biHeight = -nHeight; /* Top down DIB. */
  bitmapInfo.bmiHeader.biPlanes = 1;
  bitmapInfo.bmiHeader.biBitCount = 32;
  bitmapInfo.bmiHeader.biCompression = BI_RGB;

  return CreateDIBSection(hDc, &bitmapInfo, DIB_RGB_COLORS, (void*)lpPixels, RT_NULL, 0);
}

RT_UN32 RT_API RtConvertArgbColorToPargbColor(RT_UN32 unColor)
{
  RT_UN32 unR;
  RT_UN32 unG;
  RT_UN32 unB;
  RT_UN32 unA;
  RT_UN32 unResult;

  unR = GetRValue(unColor);
  unG = GetGValue(unColor);
  unB = GetBValue(unColor);
  unA = unColor >> 24;

  unR = unR * unA / 255;
  unG = unG * unA / 255;
  unB = unB * unA / 255;

  unResult = RGB(unR, unG, unB);
  unResult = unResult | (((DWORD) (unA)) << 24);
  return unResult;
}

RT_H RT_API RtConvertIconToPargbBitmap(RT_H hIcon, RT_N32 nWidth, RT_N32 nHeight, RT_UN32* lpMaskBuffer, RT_UN32* lpColorBuffer, RT_H hDc)
{
  ICONINFO iconInfo;
  BITMAP maskBitmap;
  BITMAP colorBitmap;
  RT_UN32* lpPixels;
  RT_B bIsArgb;
  RT_N32 nI;
  RT_H hResult;

  iconInfo.hbmMask = RT_NULL;
  iconInfo.hbmColor = RT_NULL;
  hResult = RT_NULL;

  /* Retrieve icon's bitmaps. */
  if (!GetIconInfo(hIcon, &iconInfo)) goto handle_error;
  if (!GetObject(iconInfo.hbmMask, sizeof(BITMAP), &maskBitmap)) goto handle_error;
  if (!GetObject(iconInfo.hbmColor, sizeof(BITMAP), &colorBitmap)) goto handle_error;

  /* Check sizes.*/
  if ((maskBitmap.bmWidth != nWidth) ||
      (maskBitmap.bmHeight != nHeight) ||
      (colorBitmap.bmWidth != nWidth) ||
      (colorBitmap.bmHeight != nHeight))
  {
    RtSetLastError(RT_ERROR_BAD_ARGUMENTS);
    goto handle_error;
  }

  /* Retrieve pixels. */
  if (!RtGetPixels(iconInfo.hbmMask, nWidth, nHeight, lpMaskBuffer, hDc)) goto handle_error;
  if (!RtGetPixels(iconInfo.hbmColor, nWidth, nHeight, lpColorBuffer, hDc)) goto handle_error;

  /* Create resulting bitmap. */
  hResult = RtCreateBitmap(nWidth, nHeight, &lpPixels, hDc);
  if (!hResult) goto handle_error;

  /* Assume the bitmap has a mask. */
  bIsArgb = RT_FALSE;
  for (nI = 0; nI < nWidth * nHeight; nI++)
  {
    if (lpMaskBuffer[nI])
    {
      lpPixels[nI] = 0;
    }
    else
    {
      /* This is ugly but there is no other known way to determine if an icon uses a mask or an alpha channel (Like ILIF_ALPHA). */
      if (lpColorBuffer[nI] & 0xFF000000)
      {
        /* Seems that the icon is ARGB without mask... Will use another conversion algorithm. */
        bIsArgb = RT_TRUE;
        break;
      }
      lpPixels[nI] = 0xFF000000 | lpColorBuffer[nI];
    }
  }

  /* Convert the bitmap from ARGB to PARGB. */
  if (bIsArgb)
  {
    for (nI = 0; nI < nWidth * nHeight; nI++)
    {
      lpPixels[nI] = RtConvertArgbColorToPargbColor(lpColorBuffer[nI]);
    }
  }

  goto free_resources;
handle_error:
  if (hResult)
  {
    DeleteObject(hResult);
    hResult = RT_NULL;
  }
free_resources:
  if (iconInfo.hbmMask)
  {
    if (!DeleteObject(iconInfo.hbmMask) && hResult)
    {
      iconInfo.hbmMask = RT_NULL;
      goto handle_error;
    }
    iconInfo.hbmMask = RT_NULL;
  }
  if (iconInfo.hbmColor)
  {
    if (!DeleteObject(iconInfo.hbmColor) && hResult)
    {
      iconInfo.hbmColor = RT_NULL;
      goto handle_error;
    }
    iconInfo.hbmColor = RT_NULL;
  }
  return hResult;
}

RT_H RT_API RtLoadIcon(RT_UN unResourceIndex, RT_H hInstance)
{
  return LoadImage(hInstance, MAKEINTRESOURCE(unResourceIndex), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
}

