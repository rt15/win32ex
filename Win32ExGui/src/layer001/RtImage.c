#include "layer001/RtImage.h"

#include "layer000/RtWin32ExGuiOsDefines.h"

rt_h RtCreateDcFromBitmap(rt_h hBitmap)
{
	rt_h result_handle;

	result_handle = CreateCompatibleDC(RT_NULL);
	if (result_handle) {
		if (!SelectObject(result_handle, hBitmap)) {
			/* Free the allocated resource and notify failure. */
			DeleteDC(result_handle);
			result_handle = RT_NULL;
		}
	}
	return result_handle;
}

rt_s RtFillDc(rt_h hDc, rt_n32 nWidth, rt_n32 nHeight, rt_un32 unColor)
{
	HBRUSH hBrush;
	RECT rect;
	rt_s ret;

	hBrush = CreateSolidBrush(unColor);
	if (!hBrush) goto error;

	rect.left = 0;
	rect.top = 0;
	rect.right = nWidth;
	rect.bottom = nHeight;
	if (!FillRect(hDc, &rect, hBrush)) goto error;

	ret = RT_OK;
	goto free;
error:
	ret = RT_FAILED;
free:
	if (!DeleteObject(hBrush)) {
		if (ret) {
			hBrush = RT_NULL;
			goto error;
		}
		hBrush = RT_NULL;
	}
	return ret;
}

rt_s RtStretchImage(rt_h hSourceDc, rt_n32 nSourceWidth, rt_n32 nSourceHeight, rt_h hDestinationDc, rt_n32 nDestinationWidth, rt_n32 nDestinationHeight)
{
	rt_s ret;

	if (!SetStretchBltMode(hDestinationDc, HALFTONE)) goto error;
	if (!SetBrushOrgEx(hDestinationDc, 0, 0, RT_NULL)) goto error;
	if (!StretchBlt(hDestinationDc, 0, 0, nDestinationWidth, nDestinationHeight, hSourceDc, 0, 0, nSourceWidth, nSourceHeight, SRCCOPY)) goto error;

	ret = RT_OK;
	goto free;
error:
	ret = RT_FAILED;
free:
	return ret;
}

rt_s RtDrawImageList(rt_h hImageList, rt_h hDc, rt_n32 nX, rt_n32 nY)
{
	rt_n32 nImagesCount;
	rt_n32 nWidth;
	rt_n32 nHeight;
	rt_n32 i;
	rt_s ret;

	nImagesCount = ImageList_GetImageCount(hImageList);
	ret = ImageList_GetIconSize(hImageList, &nWidth, &nHeight);
	if (ret) {
		for (i = 0; i < nImagesCount; i++) {
			ret = ImageList_Draw(hImageList, i, hDc, nX + i * nWidth, nY, ILD_TRANSPARENT);
			if (!ret) {
				break;
			}
		}
	}

	return ret;
}

rt_h RtConvertIconToBitmap(rt_h hIcon, rt_n32 nWidth, rt_n32 nHeight, rt_un32 unBackgroundColor)
{
	HDC hBitmapDc;
	rt_h result_handle;

	result_handle = RT_NULL;
	hBitmapDc = RT_NULL;

	/* Create the resulting bitmap. */
	result_handle = CreateBitmap(nWidth, nHeight, 1, 32, RT_NULL);
	if (!result_handle) goto error;

	/* Create the DC to draw icon to. */
	hBitmapDc = RtCreateDcFromBitmap(result_handle);
	if (!hBitmapDc) goto error;

	/* Clean the DC using the background color. */
	if (!RtFillDc(hBitmapDc, nWidth, nHeight, unBackgroundColor)) goto error;

	/* Draw the icon. */
	if (!DrawIcon(hBitmapDc, 0, 0, hIcon)) goto error;

	goto free;
error:
	if (result_handle) {
		DeleteObject(result_handle);
		result_handle = RT_NULL;
	}
free:
	if (hBitmapDc) {
		if (!DeleteDC(hBitmapDc) && result_handle) {
			hBitmapDc = RT_NULL;
			goto error;
		}
		hBitmapDc = RT_NULL;
	}
	return result_handle;
}

rt_s RtGetPixels(rt_h hBitmap, rt_n32 nWidth, rt_n32 nHeight, rt_un32 *buffer, rt_h hDc)
{
	BITMAPINFO bitmapInfo;

	RT_MEMORY_ZERO(&bitmapInfo, sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = nWidth;
	bitmapInfo.bmiHeader.biHeight = -nHeight; /* Top down DIB. */
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	return GetDIBits(hDc, hBitmap, 0, nHeight, buffer, &bitmapInfo, DIB_RGB_COLORS);
}

rt_h RtCreateBitmap(rt_n32 nWidth, rt_n32 nHeight, rt_un32 **lpPixels, rt_h hDc)
{
	BITMAPINFO bitmapInfo;

	RT_MEMORY_ZERO(&bitmapInfo, sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = nWidth;
	bitmapInfo.bmiHeader.biHeight = -nHeight; /* Top down DIB. */
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	return CreateDIBSection(hDc, &bitmapInfo, DIB_RGB_COLORS, (void*)lpPixels, RT_NULL, 0);
}

rt_un32 RtConvertArgbColorToPargbColor(rt_un32 unColor)
{
	rt_un32 unR;
	rt_un32 unG;
	rt_un32 unB;
	rt_un32 unA;
	rt_un32 result;

	unR = GetRValue(unColor);
	unG = GetGValue(unColor);
	unB = GetBValue(unColor);
	unA = unColor >> 24;

	unR = unR * unA / 255;
	unG = unG * unA / 255;
	unB = unB * unA / 255;

	result = RGB(unR, unG, unB);
	result = result | (((DWORD) (unA)) << 24);
	return result;
}

rt_h RtConvertIconToPargbBitmap(rt_h hIcon, rt_n32 nWidth, rt_n32 nHeight, rt_un32 *lpMaskBuffer, rt_un32 *lpColorBuffer, rt_h hDc)
{
	ICONINFO iconInfo;
	BITMAP maskBitmap;
	BITMAP colorBitmap;
	rt_un32 *lpPixels;
	rt_b bIsArgb;
	rt_n32 i;
	rt_h result_handle;

	iconInfo.hbmMask = RT_NULL;
	iconInfo.hbmColor = RT_NULL;
	result_handle = RT_NULL;

	/* Retrieve icon's bitmaps. */
	if (!GetIconInfo(hIcon, &iconInfo)) goto error;
	if (!GetObject(iconInfo.hbmMask, sizeof(BITMAP), &maskBitmap)) goto error;
	if (!GetObject(iconInfo.hbmColor, sizeof(BITMAP), &colorBitmap)) goto error;

	/* Check sizes.*/
	if ((maskBitmap.bmWidth != nWidth) ||
			(maskBitmap.bmHeight != nHeight) ||
			(colorBitmap.bmWidth != nWidth) ||
			(colorBitmap.bmHeight != nHeight))
	{
		rt_error_set_last(RT_ERROR_BAD_ARGUMENTS);
		goto error;
	}

	/* Retrieve pixels. */
	if (!RtGetPixels(iconInfo.hbmMask, nWidth, nHeight, lpMaskBuffer, hDc)) goto error;
	if (!RtGetPixels(iconInfo.hbmColor, nWidth, nHeight, lpColorBuffer, hDc)) goto error;

	/* Create resulting bitmap. */
	result_handle = RtCreateBitmap(nWidth, nHeight, &lpPixels, hDc);
	if (!result_handle) goto error;

	/* Assume the bitmap has a mask. */
	bIsArgb = RT_FALSE;
	for (i = 0; i < nWidth * nHeight; i++) {
		if (lpMaskBuffer[i]) {
			lpPixels[i] = 0;
		} else {
			/* This is ugly but there is no other known way to determine if an icon uses a mask or an alpha channel (Like ILIF_ALPHA). */
			if (lpColorBuffer[i] & 0xFF000000) {
				/* Seems that the icon is ARGB without mask... Will use another conversion algorithm. */
				bIsArgb = RT_TRUE;
				break;
			}
			lpPixels[i] = 0xFF000000 | lpColorBuffer[i];
		}
	}

	/* Convert the bitmap from ARGB to PARGB. */
	if (bIsArgb) {
		for (i = 0; i < nWidth * nHeight; i++) {
			lpPixels[i] = RtConvertArgbColorToPargbColor(lpColorBuffer[i]);
		}
	}

	goto free;
error:
	if (result_handle) {
		DeleteObject(result_handle);
		result_handle = RT_NULL;
	}
free:
	if (iconInfo.hbmMask) {
		if (!DeleteObject(iconInfo.hbmMask) && result_handle) {
			iconInfo.hbmMask = RT_NULL;
			goto error;
		}
		iconInfo.hbmMask = RT_NULL;
	}
	if (iconInfo.hbmColor) {
		if (!DeleteObject(iconInfo.hbmColor) && result_handle) {
			iconInfo.hbmColor = RT_NULL;
			goto error;
		}
		iconInfo.hbmColor = RT_NULL;
	}
	return result_handle;
}

rt_h RtLoadIcon(rt_un unResourceIndex, rt_h hInstance)
{
	return LoadImage(hInstance, MAKEINTRESOURCE(unResourceIndex), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
}

