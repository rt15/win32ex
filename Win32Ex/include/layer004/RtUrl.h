#ifndef RT_URL_H
#define RT_URL_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Urls management.
 *
 * <pre>
 * scheme:[//[user:password@]host[:port]][/]path[?query][#fragment]
 * </pre>
 *
 */

typedef struct _RT_URL_INFO
{
  RT_CHAR* lpScheme;     /* Mandatory. */
  RT_UN unSchemeSize;
  RT_CHAR* lpUser;       /* Optional. */
  RT_UN unUserSize;
  RT_CHAR* lpPassword;   /* Optional. */
  RT_UN unPasswordSize;
  RT_CHAR* lpHost;       /* Mandatory. */
  RT_UN unHostSize;
  RT_UN unPort;          /* Optional, RT_TYPE_MAX_UN if not set. */
  RT_CHAR* lpPath;       /* Mandatory but can be empty. */
  RT_UN unPathSize;
  RT_CHAR* lpQuery;      /* Optional. */
  RT_UN unQuerySize;
  RT_CHAR* lpFragment;   /* Optional. */
  RT_UN unFragmentSize;
}
RT_URL_INFO;

/**
 * Parse given <tt>lpUrl</tt> to fill <tt>lpUrlInfo</tt>.
 */
RT_B RT_API RtUrl_Parse(RT_CHAR* lpUrl, RT_URL_INFO* lpUrlInfo);

#endif /* RT_URL_H */
