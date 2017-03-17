#include "layer005/RtCommandLineArgs.h"

#include "layer004/RtChar.h"


/**
 * <p>
 * Beware that an argument alone cannot be analyzed as it can be an option value.
 * </p>
 *
 * @param lpArg An argument from lpArgV.
 * @param lpArgType Returns the type of argument.
 * @param lpShortOptionsBuffer In case of short option receive all concatenated short options plus possible value.
 * @param lpLongOptionBuffer In case of long option receive the long option name.
 * @param lpValue In case of long option, point on the value if it was concatenated with option name and '='.
 */
RT_B RT_CALL RtAnalyzeCommandLineArg(RT_CHAR* lpArg, RT_UN* lpArgType,
                                     RT_CHAR* lpShortOptionsBuffer, RT_UN unShortOptionsBufferSize, RT_UN* lpShortOptionsWritten,
                                     RT_CHAR* lpLongOptionBuffer,   RT_UN unLongOptionsBufferSize,  RT_UN* lpLongOptionsWritten,
                                     RT_CHAR** lpValue)
{
  RT_UN unArgSize;
  RT_UN unI;
  RT_B bResult;

  unArgSize = RtGetStringSize(lpArg);
  if (unArgSize <= 1)
  {
    /* Empty string or only one character like 'a' or '-'. */
    *lpArgType = RT_COMMAND_LINE_ARG_TYPE_NON_OPTION;
    *lpValue = lpArg;
  }
  else
  {
    if (lpArg[0] == '-')
    {
      if (lpArg[1] == '-')
      {
        if (unArgSize == 2)
        {
          /* '--' -> end of options. */
          *lpArgType = RT_COMMAND_LINE_ARG_TYPE_END_OF_OPTIONS;
          *lpValue = RT_NULL;
        }
        else
        {
          /* '--XXXX', long option. */

          *lpArgType = RT_COMMAND_LINE_ARG_TYPE_LONG;

          /* Search for equals. */
          for (unI = 0; unI < unArgSize; unI++)
          {
            if (lpArg[unI] == _R('=')) break;
          }
          /* Is there an equals sign? */
          if (unI != unArgSize)
          {
            /* Copy until equals sign without '--'. */
            if (!RtCopyStringWithSize(&lpArg[2], unI - 2, lpLongOptionBuffer, unLongOptionsBufferSize, lpLongOptionsWritten)) goto handle_error;
            /* Value is after equals sign. */
            *lpValue = &lpArg[unI + 1];
          }
          else
          {
            /* Copy the whole arg without '--'. */
            if (!RtCopyString(&lpArg[2], lpLongOptionBuffer, unLongOptionsBufferSize, lpLongOptionsWritten)) goto handle_error;
            /* No value directly concatenated after the option and '='. */
            *lpValue = RT_NULL;
          }
        }
      }
      else
      {
        /* '-XXXX', short option. */
        /* Simply return the short option, maybe concatenated with some others short options and maybe concatenated with a value. */

        *lpArgType = RT_COMMAND_LINE_ARG_TYPE_SHORT;
        if (!RtCopyString(&lpArg[1], lpShortOptionsBuffer, unShortOptionsBufferSize, lpShortOptionsWritten)) goto handle_error;
        *lpValue = RT_NULL;
      }
    }
    else
    {
      /* No dash -> non-option. */
      *lpArgType = RT_COMMAND_LINE_ARG_TYPE_NON_OPTION;
      *lpValue = lpArg;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}

RT_B RT_API RtParseCommandLineArgs(RT_N32* lpArgC, RT_CHAR* lpArgV[], RT_COMMAND_LINE_ARGS_CALLBACK lpCommandLineArgsCallback, void* lpContext,
                                   RT_CHAR* lpShortOptionsWithoutArg,  RT_CHAR* lpShortOptionsWithOptionalArg,  RT_CHAR* lpShortOptionsWithArg,
                                   RT_CHAR* lpLongOptionsWithoutArg[], RT_CHAR* lpLongOptionsWithOptionalArg[], RT_CHAR* lpLongOptionsWithArg[],
                                   RT_N32* lpNonOptionsIndex)
{
  RT_UN unArgType;
  RT_CHAR lpShortOptionsBuffer[RT_CHAR_HALF_BIG_STRING_SIZE];
  RT_UN unShortOptionsWritten;
  RT_CHAR lpLongOptionBuffer[RT_CHAR_QUARTER_BIG_STRING_SIZE];
  RT_UN unLongOptionsWritten;
  RT_CHAR* lpValue;
  RT_CHAR nShortOption;
  RT_B bValid;
  RT_UN unValueCardinality;
  RT_N32 nNonOptions;
  RT_CHAR* lpNonOption;
  RT_N32 nLastOption;
  RT_CHAR* lpEndOfOptions;
  RT_N32 nI;
  RT_UN unJ;
  RT_N32 nK;
  RT_B bResult;

  *lpNonOptionsIndex = RT_TYPE_MAX_N32;
  nNonOptions = RT_TYPE_MAX_N32;

  for (nI = 1; nI < *lpArgC; nI++)
  {
    unShortOptionsWritten = 0;
    unLongOptionsWritten = 0;

    if (!RtAnalyzeCommandLineArg(lpArgV[nI], &unArgType,
                                 lpShortOptionsBuffer, RT_CHAR_BIG_STRING_SIZE,         &unShortOptionsWritten,
                                 lpLongOptionBuffer,   RT_CHAR_QUARTER_BIG_STRING_SIZE, &unLongOptionsWritten,
                                 &lpValue)) goto handle_error;

    if (unArgType == RT_COMMAND_LINE_ARG_TYPE_SHORT)
    {
      unJ = 0;
      while (unJ < unShortOptionsWritten)
      {
        nShortOption = lpShortOptionsBuffer[unJ];

        if (RtSearchChar(lpShortOptionsWithoutArg, nShortOption) != RT_TYPE_MAX_UN)
        {
          bValid = RT_TRUE;
          unValueCardinality = RT_COMMAND_LINE_ARG_VALUE_NONE;
          lpValue = RT_NULL;
          /* No value, we will switch to the next character. */
        }
        else if (RtSearchChar(lpShortOptionsWithOptionalArg, nShortOption) != RT_TYPE_MAX_UN)
        {
          bValid = RT_TRUE;
          unValueCardinality = RT_COMMAND_LINE_ARG_VALUE_OPTIONAL;
          /* Optional available values for short options are always concatenated to the option. */
          if (unJ == unShortOptionsWritten - 1)
          {
            lpValue = RT_NULL;
          }
          else
          {
            lpValue = &lpShortOptionsBuffer[unJ + 1];
            unJ = unShortOptionsWritten;
          }
        }
        else if (RtSearchChar(lpShortOptionsWithArg, nShortOption) != RT_TYPE_MAX_UN)
        {
          bValid = RT_TRUE;
          unValueCardinality = RT_COMMAND_LINE_ARG_VALUE_REQUIRED;

          /* The mandatory value can be either concatenated or not. */
          if (unJ == unShortOptionsWritten - 1)
          {
            /* Retrieve next argument as value if available. */
            if (nI + 1 < *lpArgC)
            {
              nI++;
              lpValue = lpArgV[nI];
            }
          }
          else
          {
            /* Concatenated value. */
            lpValue = &lpShortOptionsBuffer[unJ + 1];
            unJ = unShortOptionsWritten;
          }
        }
        else
        {
          bValid = RT_FALSE;
          unValueCardinality = RT_COMMAND_LINE_ARG_VALUE_NONE;
          lpValue = RT_NULL;
        }

        if (!lpCommandLineArgsCallback(unArgType, bValid, nShortOption, RT_NULL, unValueCardinality, lpValue, lpContext)) goto handle_error;
        unJ++;
      }
    }
    else if (unArgType == RT_COMMAND_LINE_ARG_TYPE_LONG)
    {
      if (RtSearchStringInStrings(lpLongOptionsWithoutArg, lpLongOptionBuffer) != RT_TYPE_MAX_UN)
      {
        bValid = RT_TRUE;
        unValueCardinality = RT_COMMAND_LINE_ARG_VALUE_NONE;
      }
      else if (RtSearchStringInStrings(lpLongOptionsWithOptionalArg, lpLongOptionBuffer) != RT_TYPE_MAX_UN)
      {
        bValid = RT_TRUE;
        unValueCardinality = RT_COMMAND_LINE_ARG_VALUE_OPTIONAL;
      }
      else if (RtSearchStringInStrings(lpLongOptionsWithArg, lpLongOptionBuffer) != RT_TYPE_MAX_UN)
      {
        bValid = RT_TRUE;
        unValueCardinality = RT_COMMAND_LINE_ARG_VALUE_REQUIRED;

        /* If the value has not been provided with equals while mandatory (Optional arguments needs '='). */
        if (!lpValue)
        {
          /* We do need a value, just peek it if available. */
          if (nI + 1 < *lpArgC)
          {
            nI++;
            lpValue = lpArgV[nI];
          }
        }
      }
      else
      {
        bValid = RT_FALSE;
        unValueCardinality = RT_COMMAND_LINE_ARG_VALUE_NONE;
      }

      if (!lpCommandLineArgsCallback(unArgType, RT_TRUE, 0, lpLongOptionBuffer, unValueCardinality, lpValue, lpContext)) goto handle_error;
    }
    else if (unArgType == RT_COMMAND_LINE_ARG_TYPE_NON_OPTION)
    {
      /* This is a non-option that can be after all options or that can be between options. */
      /* It must be pushed after all options and after all previously pushed non-options. */
      
      /* Remember non-option. */
      lpNonOption = lpArgV[nI];

      /* If no non-options have been pushed yet. */
      if (nNonOptions == RT_TYPE_MAX_N32)
      {
        /* We must find the last option. */

        nLastOption = -1;
        for (nK = nI + 1; nK < *lpArgC; nK++)
        {
          if (!RtAnalyzeCommandLineArg(lpArgV[nK], &unArgType,
                                       lpShortOptionsBuffer, RT_CHAR_BIG_STRING_SIZE,         &unShortOptionsWritten,
                                       lpLongOptionBuffer,   RT_CHAR_QUARTER_BIG_STRING_SIZE, &unLongOptionsWritten,
                                       &lpValue)) goto handle_error;

          if (unArgType == RT_COMMAND_LINE_ARG_TYPE_SHORT)
          {
            nLastOption = nK;

            unJ = 0;
            while (unJ < unShortOptionsWritten)
            {
              nShortOption = lpShortOptionsBuffer[unJ];

              if (RtSearchChar(lpShortOptionsWithOptionalArg, nShortOption) != RT_TYPE_MAX_UN)
              {
                /* Optional value should be concatenated, skip it. */
                break;
              }
              else if (RtSearchChar(lpShortOptionsWithArg, nShortOption) != RT_TYPE_MAX_UN)
              {
                /* If the mandatory value is not concatenated. */
                if (unJ == unShortOptionsWritten - 1)
                {
                  /* Skip the argument. */
                  nK++;
                  nLastOption = nK;
                }
                else
                {
                  /* The other letters are the mandatory argument. */
                  break;
                }
              }
              unJ++;
            }
          }
          else if (unArgType == RT_COMMAND_LINE_ARG_TYPE_LONG)
          {
            if (RtSearchStringInStrings(lpLongOptionsWithArg, lpLongOptionBuffer) != RT_TYPE_MAX_UN)
            {
              if (!lpValue)
              {
                /* Skip the argument if not provided yet using '='. */
                nK++;
              }
            }
            nLastOption = nK;
          }
          else if (unArgType == RT_COMMAND_LINE_ARG_TYPE_END_OF_OPTIONS)
          {
            /* There won't be options after '--'. */
            break; 
          }
        }

        if (nLastOption == -1)
        {
          /* There is no remaining options. */

          /* Adjust non-options index to current non-option if needed. */
          if (*lpNonOptionsIndex > nI)
          {
            *lpNonOptionsIndex = nI;
          }

          /* We do not break as there might be still an end of options to skip. */
        }
        else
        {
          /* Shift arguments and replace last option. */

          for (nK = nI; nK < nLastOption; nK++)
          {
            lpArgV[nK] = lpArgV[nK + 1];
          }

          /* Put the non-option at the end. */
          lpArgV[nK] = lpNonOption;

          /* Adjust non-options index to current non-option if needed. */
          if (*lpNonOptionsIndex > nK)
          {
            *lpNonOptionsIndex = nK;
          }

          /* Following non-options will be pushed after this one. */
          nNonOptions = nK;

          /* We replaced current argument by next argument. */
          nI--;
        }
      }
      else
      {
        /* We have already pushed a non-option at nNonOptions. */
        /* We must shift arguments until nNonOptions included and replace it. */

        /* If the non-option has not be handled already. */
        if (nI < *lpNonOptionsIndex)
        {
          /* Shift until last pushed non-option. */
          for (nK = nI; nK < nNonOptions; nK++)
          {
            lpArgV[nK] = lpArgV[nK + 1];
          }

          /* Put the non-option at the end. */
          lpArgV[nK] = lpNonOption;

          /* First non-option has been shifted. */
          (*lpNonOptionsIndex)--;

          /* We replaced current argument by next argument. */
          nI--;
        }
      }
    }
    else
    {
      /* RT_COMMAND_LINE_ARG_TYPE_END_OF_OPTIONS */
      /* All remaining arguments are non-options that must be shifted to the left. */
      /* The end of option '--' must be put at the end of argv and argc must be decremented. */

      /* Remember end of option. */
      lpEndOfOptions = lpArgV[nI];

      /* Shift remaining arguments to the left. */
      for (nK = nI; nK < *lpArgC - 1; nK++)
      {
        lpArgV[nK] = lpArgV[nK + 1];
      }

      /* Put end of options at the end. */
      lpArgV[nK] = lpEndOfOptions;

      /* Skip end of options. */
      (*lpArgC)--;

      /* Adjust non options index. */
      if (*lpNonOptionsIndex > nI)
      {
        *lpNonOptionsIndex = nI;
      }

      /* All arguments have been handled. */
      break;
    }
  }

  bResult = RT_SUCCESS;
free_resources:
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
