#include <RtWin32Ex.h>

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
    if (RtChar_CompareCStrings(lpEnvVarsArray[unI], lpEnvVarsBlock)) goto handle_error;

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
  RT_ARRAY zzValue;
  RT_CHAR lpValueBuffer[256];
  RT_B bEnvVars1Created;
  RT_B bEnvVars2Created;
  RT_B bEnvVars3Created;
  RT_B bContains;
  RT_B bResult;

  bEnvVars1Created = RT_FALSE;

  RtArray_Create(&zzValue, lpValueBuffer, sizeof(RT_CHAR), 256);

  if (!RtEnvVars_Create(&zzEnvVars1)) goto handle_error;
  bEnvVars1Created = RT_TRUE;
  if (!ZzCheckEnvVars(&zzEnvVars1)) goto handle_error;

  if (!RtEnvVars_Contains(&zzEnvVars1, _R("PATH"), &bContains)) goto handle_error;
  if (!bContains) goto handle_error;

  if (!RtEnvVars_Contains(&zzEnvVars1, _R("pATH"), &bContains)) goto handle_error;
#ifdef RT_DEFINE_WINDOWS
  if (!bContains) goto handle_error;
#else
  if (bContains) goto handle_error;
#endif

  if (!RtEnvVars_Contains(&zzEnvVars1, _R("RT_VAR_NAME"), &bContains)) goto handle_error;
  if (bContains) goto handle_error;

  /* Add a variable to process environment. */
  if (!RtEnvVar_Set(_R("RT_VAR_NAME"), _R("VALUE"))) goto handle_error;

  /* Check process environment */
  if (!RtEnvVar_Get(_R("RT_VAR_NAME"), &zzValue)) goto handle_error;
  if (!RtChar_StringEqualsCString(&zzValue, _R("VALUE"))) goto handle_error;

  /* Check that is not added to zzEnvVars1. */
  if (!RtEnvVars_Contains(&zzEnvVars1, _R("RT_VAR_NAME"), &bContains)) goto handle_error;
  if (bContains) goto handle_error;

  /* Create zzEnvVars2 with the variable. */
  if (!RtEnvVars_Create(&zzEnvVars2)) goto handle_error;
  bEnvVars2Created = RT_TRUE;
  if (!ZzCheckEnvVars(&zzEnvVars2)) goto handle_error;

  /* Check that the new variable is in zzEnvVars2. */
  if (!RtEnvVars_Contains(&zzEnvVars2, _R("RT_VAR_NAME"), &bContains)) goto handle_error;
  if (!bContains) goto handle_error;
  if (!RtEnvVars_GetEnvVar(&zzEnvVars2, _R("RT_VAR_NAME"), &zzValue)) goto handle_error;
  if (!RtChar_StringEqualsCString(&zzValue, _R("VALUE"))) goto handle_error;

  /* Remove the variable. */
  if (!RtEnvVar_Delete(_R("RT_VAR_NAME"))) goto handle_error;

  /* Check process environment */
  if (RtEnvVar_Get(_R("RT_VAR_NAME"), &zzValue)) goto handle_error;

  /* Check that the variable is still in zzEnvVars2. */
  if (!RtEnvVars_Contains(&zzEnvVars2, _R("RT_VAR_NAME"), &bContains)) goto handle_error;
  if (!bContains) goto handle_error;

  /* Create zzEnvVars3 with the variable. */
  if (!RtEnvVars_Create(&zzEnvVars3)) goto handle_error;
  bEnvVars3Created = RT_TRUE;
  if (!ZzCheckEnvVars(&zzEnvVars3)) goto handle_error;

  /* Check that the variable is not in zzEnvVars3. */
  if (!RtEnvVars_Contains(&zzEnvVars3, _R("RT_VAR_NAME"), &bContains)) goto handle_error;
  if (bContains) goto handle_error;

  /* Add RT_VAR_NAME1 to zzEnvVars3. */
  if (!RtEnvVars_AddEnvVar(&zzEnvVars3, _R("RT_VAR_NAME1"), _R("value1"))) goto handle_error;
  if (!ZzCheckEnvVars(&zzEnvVars3)) goto handle_error;
  if (!RtEnvVars_GetEnvVar(&zzEnvVars3, _R("RT_VAR_NAME1"), &zzValue)) goto handle_error;
  if (!RtChar_StringEqualsCString(&zzValue, _R("value1"))) goto handle_error;

  /* Add RT_VAR_NAME2 to zzEnvVars3. */
  if (!RtEnvVars_AddEnvVar(&zzEnvVars3, _R("RT_VAR_NAME2"), _R(""))) goto handle_error;
  if (!ZzCheckEnvVars(&zzEnvVars3)) goto handle_error;
  if (!RtEnvVars_GetEnvVar(&zzEnvVars3, _R("RT_VAR_NAME2"), &zzValue)) goto handle_error;
  if (!RtChar_StringEqualsCString(&zzValue, _R(""))) goto handle_error;

  /* Remove PATH from zzEnvVars3. */
  if (!RtEnvVars_RemoveEnvVar(&zzEnvVars3, _R("PATH"))) goto handle_error;
  if (!ZzCheckEnvVars(&zzEnvVars3)) goto handle_error;
  if (!RtEnvVars_Contains(&zzEnvVars3, _R("PATH"), &bContains)) goto handle_error;
  if (bContains) goto handle_error;
  if (!RtEnvVars_Contains(&zzEnvVars3, _R("RT_VAR_NAME2"), &bContains)) goto handle_error;
  if (!bContains) goto handle_error;

  /* Remove RT_VAR_NAME2 from zzEnvVars3. */
  if (!RtEnvVars_RemoveEnvVar(&zzEnvVars3, _R("RT_VAR_NAME2"))) goto handle_error;
  if (!ZzCheckEnvVars(&zzEnvVars3)) goto handle_error;
  if (!RtEnvVars_Contains(&zzEnvVars3, _R("RT_VAR_NAME2"), &bContains)) goto handle_error;
  if (bContains) goto handle_error;

  /* Add variable using merge. */
  if (!RtEnvVars_MergeEnvVar(&zzEnvVars3, _R("RT_VAR_NAME3"), _R("value3"))) goto handle_error;
  if (!RtEnvVars_GetEnvVar(&zzEnvVars3, _R("RT_VAR_NAME3"), &zzValue)) goto handle_error;
  if (!RtChar_StringEqualsCString(&zzValue, _R("value3"))) goto handle_error;

  /* Replace variable using merge. */
  if (!RtEnvVars_MergeEnvVar(&zzEnvVars3, _R("RT_VAR_NAME3"), _R("This is a variable value"))) goto handle_error;
  if (!RtEnvVars_GetEnvVar(&zzEnvVars3, _R("RT_VAR_NAME3"), &zzValue)) goto handle_error;
  if (!RtChar_StringEqualsCString(&zzValue, _R("This is a variable value"))) goto handle_error;

  bResult = RT_SUCCESS;
free_resources:
  if (bEnvVars3Created)
  {
    bEnvVars3Created = RT_FALSE;
    if (!RtEnvVars_Free(&zzEnvVars3) && bResult) goto handle_error;
  }
  if (bEnvVars2Created)
  {
    bEnvVars2Created = RT_FALSE;
    if (!RtEnvVars_Free(&zzEnvVars2) && bResult) goto handle_error;
  }
  if (bEnvVars1Created)
  {
    bEnvVars1Created = RT_FALSE;
    if (!RtEnvVars_Free(&zzEnvVars1) && bResult) goto handle_error;
  }
  return bResult;

handle_error:
  bResult = RT_FAILURE;
  goto free_resources;
}
