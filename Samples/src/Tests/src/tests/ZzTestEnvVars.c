#include "ZzTests.h"

/**
 * Check the array against the block.
 */
RT_B RT_CALL ZzCheckEnvVars(RT_ENV_VARS *lpEnvVars)
{
  RT_CHAR* lpEnvVarsBlock;
  RT_CHAR** lpEnvVarsArray;
  RT_UN unI;
  RT_B bResult;

  if (!RtEnvVars_GetBlock(lpEnvVars, &lpEnvVarsBlock)) goto handle_error;
  if (!RtEnvVars_GetArray(lpEnvVars, &lpEnvVarsArray)) goto handle_error;

  unI = 0;
  while (RT_TRUE)
  {
    if (RtChar_CompareStrings(lpEnvVarsBlock, lpEnvVarsArray[unI])) goto handle_error;

    while (*lpEnvVarsBlock)
    {
      lpEnvVarsBlock++;
    }
    lpEnvVarsBlock++;
    unI++;

    /* End of array? */
    if (!lpEnvVarsArray[unI])
    {
      /* Should be also the end of the block. */
      if (*lpEnvVarsBlock) goto handle_error;
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

RT_B RT_CALL ZzTestEnvVars()
{
  RT_ENV_VARS zzEnvVars1;
  RT_ENV_VARS zzEnvVars2;
  RT_ENV_VARS zzEnvVars3;
  RT_B bEnvVars1Created;
  RT_B bEnvVars2Created;
  RT_B bEnvVars3Created;
  RT_B bContains;
  RT_CHAR lpValue[RT_CHAR_THIRD_BIG_STRING_SIZE];
  RT_UN unWritten;
  RT_B bResult;

  bEnvVars1Created = RT_FALSE;

  if (!RtCreateEnvVars(&zzEnvVars1)) goto handle_error;
  bEnvVars1Created = RT_TRUE;
  if (!ZzCheckEnvVars(&zzEnvVars1)) goto handle_error;

  if (!RtEnvVarsContains(&zzEnvVars1, _R("PATH"), &bContains)) goto handle_error;
  if (!bContains) goto handle_error;

  if (!RtEnvVarsContains(&zzEnvVars1, _R("pATH"), &bContains)) goto handle_error;
#ifdef RT_DEFINE_WINDOWS
  if (!bContains) goto handle_error;
#else
  if (bContains) goto handle_error;
#endif

  if (!RtEnvVarsContains(&zzEnvVars1, _R("RT_VAR_NAME"), &bContains)) goto handle_error;
  if (bContains) goto handle_error;

  /* Add a variable to process environment. */
  if (!RtEnvVar_Set(_R("RT_VAR_NAME"), _R("VALUE"))) goto handle_error;

  /* Check process environment */
  unWritten = 0;
  if (!RtEnvVar_Get(_R("RT_VAR_NAME"), lpValue, RT_CHAR_THIRD_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (RtChar_CompareStrings(lpValue, _R("VALUE"))) goto handle_error;

  /* Check that is not added to zzEnvVars1. */
  if (!RtEnvVarsContains(&zzEnvVars1, _R("RT_VAR_NAME"), &bContains)) goto handle_error;
  if (bContains) goto handle_error;

  /* Create zzEnvVars2 with the variable. */
  if (!RtCreateEnvVars(&zzEnvVars2)) goto handle_error;
  bEnvVars2Created = RT_TRUE;
  if (!ZzCheckEnvVars(&zzEnvVars2)) goto handle_error;

  /* Check that the new variable is in zzEnvVars2. */
  if (!RtEnvVarsContains(&zzEnvVars2, _R("RT_VAR_NAME"), &bContains)) goto handle_error;
  if (!bContains) goto handle_error;
  unWritten = 0;
  if (!RtEnvVars_GetEnvVar(&zzEnvVars2, _R("RT_VAR_NAME"), lpValue, RT_CHAR_THIRD_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (RtChar_CompareStrings(lpValue, _R("VALUE"))) goto handle_error;

  /* Remove the variable. */
  if (!RtEnvVar_Delete(_R("RT_VAR_NAME"))) goto handle_error;

  /* Check process environment */
  unWritten = 0;
  if (RtEnvVar_Get(_R("RT_VAR_NAME"), lpValue, RT_CHAR_THIRD_BIG_STRING_SIZE, &unWritten)) goto handle_error;

  /* Check that the variable is still in zzEnvVars2. */
  if (!RtEnvVarsContains(&zzEnvVars2, _R("RT_VAR_NAME"), &bContains)) goto handle_error;
  if (!bContains) goto handle_error;

  /* Create zzEnvVars3 with the variable. */
  if (!RtCreateEnvVars(&zzEnvVars3)) goto handle_error;
  bEnvVars3Created = RT_TRUE;
  if (!ZzCheckEnvVars(&zzEnvVars3)) goto handle_error;

  /* Check that the variable is not in zzEnvVars3. */
  if (!RtEnvVarsContains(&zzEnvVars3, _R("RT_VAR_NAME"), &bContains)) goto handle_error;
  if (bContains) goto handle_error;

  /* Add RT_VAR_NAME1 to zzEnvVars3. */
  if (!RtAddEnvVarIntoEnvVars(&zzEnvVars3, _R("RT_VAR_NAME1"), _R("value1"))) goto handle_error;
  if (!ZzCheckEnvVars(&zzEnvVars3)) goto handle_error;
  unWritten = 0;
  if (!RtEnvVars_GetEnvVar(&zzEnvVars3, _R("RT_VAR_NAME1"), lpValue, RT_CHAR_THIRD_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (RtChar_CompareStrings(lpValue, _R("value1"))) goto handle_error;

  /* Add RT_VAR_NAME2 to zzEnvVars3. */
  if (!RtAddEnvVarIntoEnvVars(&zzEnvVars3, _R("RT_VAR_NAME2"), _R(""))) goto handle_error;
  if (!ZzCheckEnvVars(&zzEnvVars3)) goto handle_error;
  unWritten = 0;
  if (!RtEnvVars_GetEnvVar(&zzEnvVars3, _R("RT_VAR_NAME2"), lpValue, RT_CHAR_THIRD_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (RtChar_CompareStrings(lpValue, _R(""))) goto handle_error;

  /* Remove PATH from zzEnvVars3. */
  if (!RtRemoveEnvVarFromEnvVars(&zzEnvVars3, _R("PATH"))) goto handle_error;
  if (!ZzCheckEnvVars(&zzEnvVars3)) goto handle_error;
  if (!RtEnvVarsContains(&zzEnvVars3, _R("PATH"), &bContains)) goto handle_error;
  if (bContains) goto handle_error;
  if (!RtEnvVarsContains(&zzEnvVars3, _R("RT_VAR_NAME2"), &bContains)) goto handle_error;
  if (!bContains) goto handle_error;

  /* Remove RT_VAR_NAME2 from zzEnvVars3. */
  if (!RtRemoveEnvVarFromEnvVars(&zzEnvVars3, _R("RT_VAR_NAME2"))) goto handle_error;
  if (!ZzCheckEnvVars(&zzEnvVars3)) goto handle_error;
  if (!RtEnvVarsContains(&zzEnvVars3, _R("RT_VAR_NAME2"), &bContains)) goto handle_error;
  if (bContains) goto handle_error;

  /* Add variable using merge. */
  if (!RtMergeEnvVarIntoEnvVars(&zzEnvVars3, _R("RT_VAR_NAME3"), _R("value3"))) goto handle_error;
  unWritten = 0;
  if (!RtEnvVars_GetEnvVar(&zzEnvVars3, _R("RT_VAR_NAME3"), lpValue, RT_CHAR_THIRD_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (RtChar_CompareStrings(lpValue, _R("value3"))) goto handle_error;

  /* Replace variable using merge. */
  if (!RtMergeEnvVarIntoEnvVars(&zzEnvVars3, _R("RT_VAR_NAME3"), _R("This is a variable value"))) goto handle_error;
  unWritten = 0;
  if (!RtEnvVars_GetEnvVar(&zzEnvVars3, _R("RT_VAR_NAME3"), lpValue, RT_CHAR_THIRD_BIG_STRING_SIZE, &unWritten)) goto handle_error;
  if (RtChar_CompareStrings(lpValue, _R("This is a variable value"))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bEnvVars3Created)
  {
    bEnvVars3Created = RT_FALSE;
    if (!RtFreeEnvVars(&zzEnvVars3) && bResult) goto handle_error;
  }
  if (bEnvVars2Created)
  {
    bEnvVars2Created = RT_FALSE;
    if (!RtFreeEnvVars(&zzEnvVars2) && bResult) goto handle_error;
  }
  if (bEnvVars1Created)
  {
    bEnvVars1Created = RT_FALSE;
    if (!RtFreeEnvVars(&zzEnvVars1) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
