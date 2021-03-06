#ifndef RT_WIN32_EX_MEM_H
#define RT_WIN32_EX_MEM_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Sometimes (when?) compiler generates calls to memcmp/memcpy/memmove/memset without using intrinsic functions.<br>
 * This header can define corresponding symbols when CRT is not used.
 *
 * <p>
 * Partial copy from RtMemory.c, mostly changing:
 * </p>
 * <ul>
 * <li>function names to memcmp, memcpy, memmove and memset.</li>
 * <li>Calling convention from RT_API to RT_CDECL.</li>
 * <li>Orders of sources and destinations in prototypes.</li>
 * <li>RT_N into int.</li>
 * </ul>
 *
 * <p>
 * Also, when performing 64 bits arithmetic while targeting 32 bits, VC can
 * call arithmetic alldiv, allmul and  some other functions from CRT.
 * </p>
 */

/* There is no point to use this header if we use CRT. */
#ifndef RT_DEFINE_USE_CRT

/* Allow to define memcmp, memcpy and memset while they are intrinsic and intrinsics are activated. */
#ifdef RT_DEFINE_VC

/* memmove is not available as intrinsic. */
#pragma function(memcmp, memcpy, memset)

#endif

int RT_CDECL memcmp(const void* lpArea1, const void* lpArea2, size_t unSize)
{
  RT_UN unWordsCount;
  const RT_UN* lpWordArea1;
  const RT_UN* lpWordArea2;
  RT_UN unRemainder;
  const RT_UCHAR8* lpUCharArea1;
  const RT_UCHAR8* lpUCharArea2;
  RT_UN unI;

  unWordsCount = unSize / sizeof(RT_UN);
  if (unWordsCount)
  {
    lpWordArea1 = lpArea1;
    lpWordArea2 = lpArea2;
    for (unI = 0; unI < unWordsCount; unI++)
    {
      if (lpWordArea1[unI] != lpWordArea2[unI])
      {
        unRemainder = unSize - unI * sizeof(RT_UN);
        lpUCharArea1 = (RT_UCHAR8*)&lpWordArea1[unI];
        lpUCharArea2 = (RT_UCHAR8*)&lpWordArea2[unI];
        goto handleRemainder;
      }
    }
    unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
    lpUCharArea1 = (RT_UCHAR8*)&lpWordArea1[unWordsCount];
    lpUCharArea2 = (RT_UCHAR8*)&lpWordArea2[unWordsCount];
  }
  else
  {
    unRemainder = unSize;
    lpUCharArea1 = lpArea1;
    lpUCharArea2 = lpArea2;
  }

handleRemainder:
  unI = 0;
  while ((unI < unRemainder) && (lpUCharArea1[unI] == lpUCharArea2[unI]))
    unI++;
  if (unI == unRemainder) unI--;
  return lpUCharArea1[unI] - lpUCharArea2[unI];
}

void* RT_CDECL memcpy(void* lpDestination, const void* lpSource, size_t unSize)
{
  RT_UN unWordsCount;
  const RT_UN* lpWordSource;
  RT_UN* lpWordDestination;
  RT_UN unRemainder;
  const RT_CHAR8* lpCharSource;
  RT_CHAR8* lpCharDestination;
  RT_UN unI;

  unWordsCount = unSize / sizeof(RT_UN);
  if (unWordsCount)
  {
    lpWordSource = lpSource;
    lpWordDestination = lpDestination;
    for (unI = 0; unI < unWordsCount; unI++)
    {
      lpWordDestination[unI] = lpWordSource[unI];
    }
    unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
    if (unRemainder)
    {
      lpCharSource = (RT_CHAR8*)&lpWordSource[unWordsCount];
      lpCharDestination = (RT_CHAR8*)&lpWordDestination[unWordsCount];
      for (unI = 0; unI < unRemainder; unI++)
      {
        lpCharDestination[unI] = lpCharSource[unI];
      }
    }
  }
  else
  {
    lpCharSource = lpSource;
    lpCharDestination = lpDestination;
    for (unI = 0; unI < unSize; unI++)
    {
      lpCharDestination[unI] = lpCharSource[unI];
    }
  }

  return lpDestination;
}

void* RT_CDECL memmove(void* lpDestination, const void* lpSource, size_t unSize)
{
  RT_UN unWordsCount;
  const RT_UN* lpWordSource;
  RT_UN* lpWordDestination;
  RT_UN unRemainder;
  const RT_CHAR8* lpCharSource;
  RT_CHAR8* lpCharDestination;
  RT_UN unI;

  unWordsCount = unSize / sizeof(RT_UN);
  if (lpDestination > lpSource)
  {
    if (unWordsCount)
    {
      lpWordSource = (RT_UN*)&((RT_CHAR8*)lpSource)[unSize];
      lpWordDestination = (RT_UN*)&((RT_CHAR8*)lpDestination)[unSize];
      for (unI = 1; unI <= unWordsCount; unI++)
      {
        lpWordDestination[-(RT_N)unI] = lpWordSource[-(RT_N)unI];
      }
      unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
      lpCharSource = (RT_CHAR8*)&lpWordSource[-(RT_N)unWordsCount];
      lpCharDestination = (RT_CHAR8*)&lpWordDestination[-(RT_N)unWordsCount];;
    }
    else
    {
      unRemainder = unSize;
      lpCharSource = &((RT_CHAR8*)lpSource)[unSize];
      lpCharDestination = &((RT_CHAR8*)lpDestination)[unSize];
    }
    for (unI = 1; unI <= unRemainder; unI++)
    {
      lpCharDestination[-(RT_N)unI] = lpCharSource[-(RT_N)unI];
    }
  }
  else
  {
    /* No overwriting possible, same code as RtMemory_Copy. */
    if (unWordsCount)
    {
      lpWordSource = lpSource;
      lpWordDestination = lpDestination;
      for (unI = 0; unI < unWordsCount; unI++)
      {
        lpWordDestination[unI] = lpWordSource[unI];
      }
      unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
      if (unRemainder)
      {
        lpCharSource = (RT_CHAR8*)&lpWordSource[unWordsCount];
        lpCharDestination = (RT_CHAR8*)&lpWordDestination[unWordsCount];
        for (unI = 0; unI < unRemainder; unI++)
        {
          lpCharDestination[unI] = lpCharSource[unI];
        }
      }
    }
    else
    {
      lpCharSource = lpSource;
      lpCharDestination = lpDestination;
      for (unI = 0; unI < unSize; unI++)
      {
        lpCharDestination[unI] = lpCharSource[unI];
      }
    }
  }

  return lpDestination;
}

void* RT_CDECL memset(void* lpArea, int nValue, size_t unSize)
{
  RT_UN unWordsCount;
  RT_N* lpWordArea;
  RT_N nWord;
  RT_UN unRemainder;
  RT_CHAR8* lpCharArea;
  RT_UN unI;

  unWordsCount = unSize / sizeof(RT_N);
  if (unWordsCount)
  {
    if (nValue)
    {
#ifdef RT_DEFINE_64
      nWord = 0x0101010101010101 * (RT_UCHAR8)nValue;
#else
      nWord = 0x01010101 * (RT_UCHAR8)nValue;
#endif
    }
    else
    {
      nWord = 0;
    }

    lpWordArea = lpArea;
    for (unI = 0; unI < unWordsCount; unI++)
    {
      lpWordArea[unI] = nWord;
    }
    unRemainder = RT_MEMORY_MODULO_RT_UN_SIZE(unSize);
    if (unRemainder)
    {
      lpCharArea = (RT_CHAR8*)&lpWordArea[unWordsCount];
      for (unI = 0; unI < unRemainder; unI++)
      {
        lpCharArea[unI] = (RT_CHAR8)nValue;
      }
    }
  }
  else
  {
    lpCharArea = lpArea;
    for (unI = 0; unI < unSize; unI++)
    {
      lpCharArea[unI] = (RT_CHAR8)nValue;
    }
  }

  return lpArea;
}

/* 64 bits operations for VC 32 bits without CRT. */
/* Implementation from https://gist.github.com/mmozeiko/6a365d6c483fc721b63a#file-win32_crt_math-cpp */
#if defined(RT_DEFINE_VC) && defined(RT_DEFINE_32)

#define CRT_LOWORD(x) dword ptr [x+0]
#define CRT_HIWORD(x) dword ptr [x+4]

#if defined(__cplusplus)
extern "C"
{
#endif
    __declspec(naked) void _alldiv()
    {
        #define DVND    esp + 16      // stack address of dividend (a)
        #define DVSR    esp + 24      // stack address of divisor (b)

        __asm
        {
        push    edi
        push    esi
        push    ebx

; Determine sign of the result (edi = 0 if result is positive, non-zero
; otherwise) and make operands positive.

        xor     edi,edi         ; result sign assumed positive

        mov     eax,CRT_HIWORD(DVND) ; hi word of a
        or      eax,eax         ; test to see if signed
        jge     short L1        ; skip rest if a is already positive
        inc     edi             ; complement result sign flag
        mov     edx,CRT_LOWORD(DVND) ; lo word of a
        neg     eax             ; make a positive
        neg     edx
        sbb     eax,0
        mov     CRT_HIWORD(DVND),eax ; save positive value
        mov     CRT_LOWORD(DVND),edx
L1:
        mov     eax,CRT_HIWORD(DVSR) ; hi word of b
        or      eax,eax         ; test to see if signed
        jge     short L2        ; skip rest if b is already positive
        inc     edi             ; complement the result sign flag
        mov     edx,CRT_LOWORD(DVSR) ; lo word of a
        neg     eax             ; make b positive
        neg     edx
        sbb     eax,0
        mov     CRT_HIWORD(DVSR),eax ; save positive value
        mov     CRT_LOWORD(DVSR),edx
L2:

;
; Now do the divide.  First look to see if the divisor is less than 4194304K.
; If so, then we can use a simple algorithm with word divides, otherwise
; things get a little more complex.
;
; NOTE - eax currently contains the high order word of DVSR
;

        or      eax,eax         ; check to see if divisor < 4194304K
        jnz     short L3        ; nope, gotta do this the hard way
        mov     ecx,CRT_LOWORD(DVSR) ; load divisor
        mov     eax,CRT_HIWORD(DVND) ; load high word of dividend
        xor     edx,edx
        div     ecx             ; eax <- high order bits of quotient
        mov     ebx,eax         ; save high bits of quotient
        mov     eax,CRT_LOWORD(DVND) ; edx:eax <- remainder:lo word of dividend
        div     ecx             ; eax <- low order bits of quotient
        mov     edx,ebx         ; edx:eax <- quotient
        jmp     short L4        ; set sign, restore stack and return

;
; Here we do it the hard way.  Remember, eax contains the high word of DVSR
;

L3:
        mov     ebx,eax         ; ebx:ecx <- divisor
        mov     ecx,CRT_LOWORD(DVSR)
        mov     edx,CRT_HIWORD(DVND) ; edx:eax <- dividend
        mov     eax,CRT_LOWORD(DVND)
L5:
        shr     ebx,1           ; shift divisor right one bit
        rcr     ecx,1
        shr     edx,1           ; shift dividend right one bit
        rcr     eax,1
        or      ebx,ebx
        jnz     short L5        ; loop until divisor < 4194304K
        div     ecx             ; now divide, ignore remainder
        mov     esi,eax         ; save quotient

;
; We may be off by one, so to check, we will multiply the quotient
; by the divisor and check the result against the orignal dividend
; Note that we must also check for overflow, which can occur if the
; dividend is close to 2**64 and the quotient is off by 1.
;

        mul     CRT_HIWORD(DVSR) ; QUOT * CRT_HIWORD(DVSR)
        mov     ecx,eax
        mov     eax,CRT_LOWORD(DVSR)
        mul     esi             ; QUOT * CRT_LOWORD(DVSR)
        add     edx,ecx         ; EDX:EAX = QUOT * DVSR
        jc      short L6        ; carry means Quotient is off by 1

;
; do long compare here between original dividend and the result of the
; multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
; subtract one (1) from the quotient.
;

        cmp     edx,CRT_HIWORD(DVND) ; compare hi words of result and original
        ja      short L6        ; if result > original, do subtract
        jb      short L7        ; if result < original, we are ok
        cmp     eax,CRT_LOWORD(DVND) ; hi words are equal, compare lo words
        jbe     short L7        ; if less or equal we are ok, else subtract
L6:
        dec     esi             ; subtract 1 from quotient
L7:
        xor     edx,edx         ; edx:eax <- quotient
        mov     eax,esi

;
; Just the cleanup left to do.  edx:eax contains the quotient.  Set the sign
; according to the save value, cleanup the stack, and return.
;

L4:
        dec     edi             ; check to see if result is negative
        jnz     short L8        ; if EDI == 0, result should be negative
        neg     edx             ; otherwise, negate the result
        neg     eax
        sbb     edx,0

;
; Restore the saved registers and return.
;

L8:
        pop     ebx
        pop     esi
        pop     edi

        ret     16
        }

        #undef DVND
        #undef DVSR
    }

    __declspec(naked) void _alldvrm()
    {
        #define DVND    esp + 16      // stack address of dividend (a)
        #define DVSR    esp + 24      // stack address of divisor (b)

        __asm
        {
        push    edi
        push    esi
        push    ebp

; Determine sign of the quotient (edi = 0 if result is positive, non-zero
; otherwise) and make operands positive.
; Sign of the remainder is kept in ebp.

        xor     edi,edi         ; result sign assumed positive
        xor     ebp,ebp         ; result sign assumed positive

        mov     eax,CRT_HIWORD(DVND) ; hi word of a
        or      eax,eax         ; test to see if signed
        jge     short L1        ; skip rest if a is already positive
        inc     edi             ; complement result sign flag
        inc     ebp             ; complement result sign flag
        mov     edx,CRT_LOWORD(DVND) ; lo word of a
        neg     eax             ; make a positive
        neg     edx
        sbb     eax,0
        mov     CRT_HIWORD(DVND),eax ; save positive value
        mov     CRT_LOWORD(DVND),edx
L1:
        mov     eax,CRT_HIWORD(DVSR) ; hi word of b
        or      eax,eax         ; test to see if signed
        jge     short L2        ; skip rest if b is already positive
        inc     edi             ; complement the result sign flag
        mov     edx,CRT_LOWORD(DVSR) ; lo word of a
        neg     eax             ; make b positive
        neg     edx
        sbb     eax,0
        mov     CRT_HIWORD(DVSR),eax ; save positive value
        mov     CRT_LOWORD(DVSR),edx
L2:

;
; Now do the divide.  First look to see if the divisor is less than 4194304K.
; If so, then we can use a simple algorithm with word divides, otherwise
; things get a little more complex.
;
; NOTE - eax currently contains the high order word of DVSR
;

        or      eax,eax         ; check to see if divisor < 4194304K
        jnz     short L3        ; nope, gotta do this the hard way
        mov     ecx,CRT_LOWORD(DVSR) ; load divisor
        mov     eax,CRT_HIWORD(DVND) ; load high word of dividend
        xor     edx,edx
        div     ecx             ; eax <- high order bits of quotient
        mov     ebx,eax         ; save high bits of quotient
        mov     eax,CRT_LOWORD(DVND) ; edx:eax <- remainder:lo word of dividend
        div     ecx             ; eax <- low order bits of quotient
        mov     esi,eax         ; ebx:esi <- quotient
;
; Now we need to do a multiply so that we can compute the remainder.
;
        mov     eax,ebx         ; set up high word of quotient
        mul     CRT_LOWORD(DVSR) ; CRT_HIWORD(QUOT) * DVSR
        mov     ecx,eax         ; save the result in ecx
        mov     eax,esi         ; set up low word of quotient
        mul     CRT_LOWORD(DVSR) ; CRT_LOWORD(QUOT) * DVSR
        add     edx,ecx         ; EDX:EAX = QUOT * DVSR
        jmp     short L4        ; complete remainder calculation

;
; Here we do it the hard way.  Remember, eax contains the high word of DVSR
;

L3:
        mov     ebx,eax         ; ebx:ecx <- divisor
        mov     ecx,CRT_LOWORD(DVSR)
        mov     edx,CRT_HIWORD(DVND) ; edx:eax <- dividend
        mov     eax,CRT_LOWORD(DVND)
L5:
        shr     ebx,1           ; shift divisor right one bit
        rcr     ecx,1
        shr     edx,1           ; shift dividend right one bit
        rcr     eax,1
        or      ebx,ebx
        jnz     short L5        ; loop until divisor < 4194304K
        div     ecx             ; now divide, ignore remainder
        mov     esi,eax         ; save quotient

;
; We may be off by one, so to check, we will multiply the quotient
; by the divisor and check the result against the orignal dividend
; Note that we must also check for overflow, which can occur if the
; dividend is close to 2**64 and the quotient is off by 1.
;

        mul     CRT_HIWORD(DVSR) ; QUOT * CRT_HIWORD(DVSR)
        mov     ecx,eax
        mov     eax,CRT_LOWORD(DVSR)
        mul     esi             ; QUOT * CRT_LOWORD(DVSR)
        add     edx,ecx         ; EDX:EAX = QUOT * DVSR
        jc      short L6        ; carry means Quotient is off by 1

;
; do long compare here between original dividend and the result of the
; multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
; subtract one (1) from the quotient.
;

        cmp     edx,CRT_HIWORD(DVND) ; compare hi words of result and original
        ja      short L6        ; if result > original, do subtract
        jb      short L7        ; if result < original, we are ok
        cmp     eax,CRT_LOWORD(DVND) ; hi words are equal, compare lo words
        jbe     short L7        ; if less or equal we are ok, else subtract
L6:
        dec     esi             ; subtract 1 from quotient
        sub     eax,CRT_LOWORD(DVSR) ; subtract divisor from result
        sbb     edx,CRT_HIWORD(DVSR)
L7:
        xor     ebx,ebx         ; ebx:esi <- quotient

L4:
;
; Calculate remainder by subtracting the result from the original dividend.
; Since the result is already in a register, we will do the subtract in the
; opposite direction and negate the result if necessary.
;

        sub     eax,CRT_LOWORD(DVND) ; subtract dividend from result
        sbb     edx,CRT_HIWORD(DVND)

;
; Now check the result sign flag to see if the result is supposed to be positive
; or negative.  It is currently negated (because we subtracted in the 'wrong'
; direction), so if the sign flag is set we are done, otherwise we must negate
; the result to make it positive again.
;

        dec     ebp             ; check result sign flag
        jns     short L9        ; result is ok, set up the quotient
        neg     edx             ; otherwise, negate the result
        neg     eax
        sbb     edx,0

;
; Now we need to get the quotient into edx:eax and the remainder into ebx:ecx.
;
L9:
        mov     ecx,edx
        mov     edx,ebx
        mov     ebx,ecx
        mov     ecx,eax
        mov     eax,esi

;
; Just the cleanup left to do.  edx:eax contains the quotient.  Set the sign
; according to the save value, cleanup the stack, and return.
;

        dec     edi             ; check to see if result is negative
        jnz     short L8        ; if EDI == 0, result should be negative
        neg     edx             ; otherwise, negate the result
        neg     eax
        sbb     edx,0

;
; Restore the saved registers and return.
;

L8:
        pop     ebp
        pop     esi
        pop     edi

        ret     16
        }

        #undef DVND
        #undef DVSR
    }

    __declspec(naked) void _allmul()
    {
        #define A       esp + 8       // stack address of a
        #define B       esp + 16      // stack address of b

        __asm
        {
        push    ebx

        mov     eax,CRT_HIWORD(A)
        mov     ecx,CRT_LOWORD(B)
        mul     ecx             ;eax has AHI, ecx has BLO, so AHI * BLO
        mov     ebx,eax         ;save result

        mov     eax,CRT_LOWORD(A)
        mul     CRT_HIWORD(B)       ;ALO * BHI
        add     ebx,eax         ;ebx = ((ALO * BHI) + (AHI * BLO))

        mov     eax,CRT_LOWORD(A)   ;ecx = BLO
        mul     ecx             ;so edx:eax = ALO*BLO
        add     edx,ebx         ;now edx has all the LO*HI stuff

        pop     ebx

        ret     16              ; callee restores the stack
        }

        #undef A
        #undef B
    }

    __declspec(naked) void _allrem()
    {
        #define DVND    esp + 12      // stack address of dividend (a)
        #define DVSR    esp + 20      // stack address of divisor (b)

        __asm
        {
        push    ebx
        push    edi


; Determine sign of the result (edi = 0 if result is positive, non-zero
; otherwise) and make operands positive.

        xor     edi,edi         ; result sign assumed positive

        mov     eax,CRT_HIWORD(DVND) ; hi word of a
        or      eax,eax         ; test to see if signed
        jge     short L1        ; skip rest if a is already positive
        inc     edi             ; complement result sign flag bit
        mov     edx,CRT_LOWORD(DVND) ; lo word of a
        neg     eax             ; make a positive
        neg     edx
        sbb     eax,0
        mov     CRT_HIWORD(DVND),eax ; save positive value
        mov     CRT_LOWORD(DVND),edx
L1:
        mov     eax,CRT_HIWORD(DVSR) ; hi word of b
        or      eax,eax         ; test to see if signed
        jge     short L2        ; skip rest if b is already positive
        mov     edx,CRT_LOWORD(DVSR) ; lo word of b
        neg     eax             ; make b positive
        neg     edx
        sbb     eax,0
        mov     CRT_HIWORD(DVSR),eax ; save positive value
        mov     CRT_LOWORD(DVSR),edx
L2:

;
; Now do the divide.  First look to see if the divisor is less than 4194304K.
; If so, then we can use a simple algorithm with word divides, otherwise
; things get a little more complex.
;
; NOTE - eax currently contains the high order word of DVSR
;

        or      eax,eax         ; check to see if divisor < 4194304K
        jnz     short L3        ; nope, gotta do this the hard way
        mov     ecx,CRT_LOWORD(DVSR) ; load divisor
        mov     eax,CRT_HIWORD(DVND) ; load high word of dividend
        xor     edx,edx
        div     ecx             ; edx <- remainder
        mov     eax,CRT_LOWORD(DVND) ; edx:eax <- remainder:lo word of dividend
        div     ecx             ; edx <- final remainder
        mov     eax,edx         ; edx:eax <- remainder
        xor     edx,edx
        dec     edi             ; check result sign flag
        jns     short L4        ; negate result, restore stack and return
        jmp     short L8        ; result sign ok, restore stack and return

;
; Here we do it the hard way.  Remember, eax contains the high word of DVSR
;

L3:
        mov     ebx,eax         ; ebx:ecx <- divisor
        mov     ecx,CRT_LOWORD(DVSR)
        mov     edx,CRT_HIWORD(DVND) ; edx:eax <- dividend
        mov     eax,CRT_LOWORD(DVND)
L5:
        shr     ebx,1           ; shift divisor right one bit
        rcr     ecx,1
        shr     edx,1           ; shift dividend right one bit
        rcr     eax,1
        or      ebx,ebx
        jnz     short L5        ; loop until divisor < 4194304K
        div     ecx             ; now divide, ignore remainder

;
; We may be off by one, so to check, we will multiply the quotient
; by the divisor and check the result against the orignal dividend
; Note that we must also check for overflow, which can occur if the
; dividend is close to 2**64 and the quotient is off by 1.
;

        mov     ecx,eax         ; save a copy of quotient in ECX
        mul     CRT_HIWORD(DVSR)
        xchg    ecx,eax         ; save product, get quotient in EAX
        mul     CRT_LOWORD(DVSR)
        add     edx,ecx         ; EDX:EAX = QUOT * DVSR
        jc      short L6        ; carry means Quotient is off by 1

;
; do long compare here between original dividend and the result of the
; multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
; subtract the original divisor from the result.
;

        cmp     edx,CRT_HIWORD(DVND) ; compare hi words of result and original
        ja      short L6        ; if result > original, do subtract
        jb      short L7        ; if result < original, we are ok
        cmp     eax,CRT_LOWORD(DVND) ; hi words are equal, compare lo words
        jbe     short L7        ; if less or equal we are ok, else subtract
L6:
        sub     eax,CRT_LOWORD(DVSR) ; subtract divisor from result
        sbb     edx,CRT_HIWORD(DVSR)
L7:

;
; Calculate remainder by subtracting the result from the original dividend.
; Since the result is already in a register, we will do the subtract in the
; opposite direction and negate the result if necessary.
;

        sub     eax,CRT_LOWORD(DVND) ; subtract dividend from result
        sbb     edx,CRT_HIWORD(DVND)

;
; Now check the result sign flag to see if the result is supposed to be positive
; or negative.  It is currently negated (because we subtracted in the 'wrong'
; direction), so if the sign flag is set we are done, otherwise we must negate
; the result to make it positive again.
;

        dec     edi             ; check result sign flag
        jns     short L8        ; result is ok, restore stack and return
L4:
        neg     edx             ; otherwise, negate the result
        neg     eax
        sbb     edx,0

;
; Just the cleanup left to do.  edx:eax contains the quotient.
; Restore the saved registers and return.
;

L8:
        pop     edi
        pop     ebx

        ret     16
        }

        #undef DVND
        #undef DVSR
    }

    __declspec(naked) void _allshl()
    {
        __asm
        {
;
; Handle shifts of 64 or more bits (all get 0)
;
        cmp     cl, 64
        jae     short RETZERO

;
; Handle shifts of between 0 and 31 bits
;
        cmp     cl, 32
        jae     short MORE32
        shld    edx,eax,cl
        shl     eax,cl
        ret

;
; Handle shifts of between 32 and 63 bits
;
MORE32:
        mov     edx,eax
        xor     eax,eax
        and     cl,31
        shl     edx,cl
        ret

;
; return 0 in edx:eax
;
RETZERO:
        xor     eax,eax
        xor     edx,edx
        ret
        }
    }

    __declspec(naked) void _allshr()
    {
        __asm
        {
;
; Handle shifts of 64 bits or more (if shifting 64 bits or more, the result
; depends only on the high order bit of edx).
;
        cmp     cl,64
        jae     short RETSIGN

;
; Handle shifts of between 0 and 31 bits
;
        cmp     cl, 32
        jae     short MORE32
        shrd    eax,edx,cl
        sar     edx,cl
        ret

;
; Handle shifts of between 32 and 63 bits
;
MORE32:
        mov     eax,edx
        sar     edx,31
        and     cl,31
        sar     eax,cl
        ret

;
; Return double precision 0 or -1, depending on the sign of edx
;
RETSIGN:
        sar     edx,31
        mov     eax,edx
        ret
        }
    }

    __declspec(naked) void _aulldiv()
    {
        #define DVND    esp + 12      // stack address of dividend (a)
        #define DVSR    esp + 20      // stack address of divisor (b)

        __asm
        {
        push    ebx
        push    esi

;
; Now do the divide.  First look to see if the divisor is less than 4194304K.
; If so, then we can use a simple algorithm with word divides, otherwise
; things get a little more complex.
;

        mov     eax,CRT_HIWORD(DVSR) ; check to see if divisor < 4194304K
        or      eax,eax
        jnz     short L1        ; nope, gotta do this the hard way
        mov     ecx,CRT_LOWORD(DVSR) ; load divisor
        mov     eax,CRT_HIWORD(DVND) ; load high word of dividend
        xor     edx,edx
        div     ecx             ; get high order bits of quotient
        mov     ebx,eax         ; save high bits of quotient
        mov     eax,CRT_LOWORD(DVND) ; edx:eax <- remainder:lo word of dividend
        div     ecx             ; get low order bits of quotient
        mov     edx,ebx         ; edx:eax <- quotient hi:quotient lo
        jmp     short L2        ; restore stack and return

;
; Here we do it the hard way.  Remember, eax contains DVSRHI
;

L1:
        mov     ecx,eax         ; ecx:ebx <- divisor
        mov     ebx,CRT_LOWORD(DVSR)
        mov     edx,CRT_HIWORD(DVND) ; edx:eax <- dividend
        mov     eax,CRT_LOWORD(DVND)
L3:
        shr     ecx,1           ; shift divisor right one bit; hi bit <- 0
        rcr     ebx,1
        shr     edx,1           ; shift dividend right one bit; hi bit <- 0
        rcr     eax,1
        or      ecx,ecx
        jnz     short L3        ; loop until divisor < 4194304K
        div     ebx             ; now divide, ignore remainder
        mov     esi,eax         ; save quotient

;
; We may be off by one, so to check, we will multiply the quotient
; by the divisor and check the result against the orignal dividend
; Note that we must also check for overflow, which can occur if the
; dividend is close to 2**64 and the quotient is off by 1.
;

        mul     CRT_HIWORD(DVSR) ; QUOT * CRT_HIWORD(DVSR)
        mov     ecx,eax
        mov     eax,CRT_LOWORD(DVSR)
        mul     esi             ; QUOT * CRT_LOWORD(DVSR)
        add     edx,ecx         ; EDX:EAX = QUOT * DVSR
        jc      short L4        ; carry means Quotient is off by 1

;
; do long compare here between original dividend and the result of the
; multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
; subtract one (1) from the quotient.
;

        cmp     edx,CRT_HIWORD(DVND) ; compare hi words of result and original
        ja      short L4        ; if result > original, do subtract
        jb      short L5        ; if result < original, we are ok
        cmp     eax,CRT_LOWORD(DVND) ; hi words are equal, compare lo words
        jbe     short L5        ; if less or equal we are ok, else subtract
L4:
        dec     esi             ; subtract 1 from quotient
L5:
        xor     edx,edx         ; edx:eax <- quotient
        mov     eax,esi

;
; Just the cleanup left to do.  edx:eax contains the quotient.
; Restore the saved registers and return.
;

L2:

        pop     esi
        pop     ebx

        ret     16
        }

        #undef DVND
        #undef DVSR
    }

    __declspec(naked) void _aulldvrm()
    {
        #define DVND    esp + 8       // stack address of dividend (a)
        #define DVSR    esp + 16      // stack address of divisor (b)

        __asm
        {
        push    esi

;
; Now do the divide.  First look to see if the divisor is less than 4194304K.
; If so, then we can use a simple algorithm with word divides, otherwise
; things get a little more complex.
;

        mov     eax,CRT_HIWORD(DVSR) ; check to see if divisor < 4194304K
        or      eax,eax
        jnz     short L1        ; nope, gotta do this the hard way
        mov     ecx,CRT_LOWORD(DVSR) ; load divisor
        mov     eax,CRT_HIWORD(DVND) ; load high word of dividend
        xor     edx,edx
        div     ecx             ; get high order bits of quotient
        mov     ebx,eax         ; save high bits of quotient
        mov     eax,CRT_LOWORD(DVND) ; edx:eax <- remainder:lo word of dividend
        div     ecx             ; get low order bits of quotient
        mov     esi,eax         ; ebx:esi <- quotient

;
; Now we need to do a multiply so that we can compute the remainder.
;
        mov     eax,ebx         ; set up high word of quotient
        mul     CRT_LOWORD(DVSR) ; CRT_HIWORD(QUOT) * DVSR
        mov     ecx,eax         ; save the result in ecx
        mov     eax,esi         ; set up low word of quotient
        mul     CRT_LOWORD(DVSR) ; CRT_LOWORD(QUOT) * DVSR
        add     edx,ecx         ; EDX:EAX = QUOT * DVSR
        jmp     short L2        ; complete remainder calculation

;
; Here we do it the hard way.  Remember, eax contains DVSRHI
;

L1:
        mov     ecx,eax         ; ecx:ebx <- divisor
        mov     ebx,CRT_LOWORD(DVSR)
        mov     edx,CRT_HIWORD(DVND) ; edx:eax <- dividend
        mov     eax,CRT_LOWORD(DVND)
L3:
        shr     ecx,1           ; shift divisor right one bit; hi bit <- 0
        rcr     ebx,1
        shr     edx,1           ; shift dividend right one bit; hi bit <- 0
        rcr     eax,1
        or      ecx,ecx
        jnz     short L3        ; loop until divisor < 4194304K
        div     ebx             ; now divide, ignore remainder
        mov     esi,eax         ; save quotient

;
; We may be off by one, so to check, we will multiply the quotient
; by the divisor and check the result against the orignal dividend
; Note that we must also check for overflow, which can occur if the
; dividend is close to 2**64 and the quotient is off by 1.
;

        mul     CRT_HIWORD(DVSR) ; QUOT * CRT_HIWORD(DVSR)
        mov     ecx,eax
        mov     eax,CRT_LOWORD(DVSR)
        mul     esi             ; QUOT * CRT_LOWORD(DVSR)
        add     edx,ecx         ; EDX:EAX = QUOT * DVSR
        jc      short L4        ; carry means Quotient is off by 1

;
; do long compare here between original dividend and the result of the
; multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
; subtract one (1) from the quotient.
;

        cmp     edx,CRT_HIWORD(DVND) ; compare hi words of result and original
        ja      short L4        ; if result > original, do subtract
        jb      short L5        ; if result < original, we are ok
        cmp     eax,CRT_LOWORD(DVND) ; hi words are equal, compare lo words
        jbe     short L5        ; if less or equal we are ok, else subtract
L4:
        dec     esi             ; subtract 1 from quotient
        sub     eax,CRT_LOWORD(DVSR) ; subtract divisor from result
        sbb     edx,CRT_HIWORD(DVSR)
L5:
        xor     ebx,ebx         ; ebx:esi <- quotient

L2:
;
; Calculate remainder by subtracting the result from the original dividend.
; Since the result is already in a register, we will do the subtract in the
; opposite direction and negate the result.
;

        sub     eax,CRT_LOWORD(DVND) ; subtract dividend from result
        sbb     edx,CRT_HIWORD(DVND)
        neg     edx             ; otherwise, negate the result
        neg     eax
        sbb     edx,0

;
; Now we need to get the quotient into edx:eax and the remainder into ebx:ecx.
;
        mov     ecx,edx
        mov     edx,ebx
        mov     ebx,ecx
        mov     ecx,eax
        mov     eax,esi
;
; Just the cleanup left to do.  edx:eax contains the quotient.
; Restore the saved registers and return.
;

        pop     esi

        ret     16
        }

        #undef DVND
        #undef DVSR
    }

    __declspec(naked) void _aullrem()
    {
        #define DVND    esp + 8       // stack address of dividend (a)
        #define DVSR    esp + 16      // stack address of divisor (b)

        __asm
        {
        push    ebx

; Now do the divide.  First look to see if the divisor is less than 4194304K.
; If so, then we can use a simple algorithm with word divides, otherwise
; things get a little more complex.
;

        mov     eax,CRT_HIWORD(DVSR) ; check to see if divisor < 4194304K
        or      eax,eax
        jnz     short L1        ; nope, gotta do this the hard way
        mov     ecx,CRT_LOWORD(DVSR) ; load divisor
        mov     eax,CRT_HIWORD(DVND) ; load high word of dividend
        xor     edx,edx
        div     ecx             ; edx <- remainder, eax <- quotient
        mov     eax,CRT_LOWORD(DVND) ; edx:eax <- remainder:lo word of dividend
        div     ecx             ; edx <- final remainder
        mov     eax,edx         ; edx:eax <- remainder
        xor     edx,edx
        jmp     short L2        ; restore stack and return

;
; Here we do it the hard way.  Remember, eax contains DVSRHI
;

L1:
        mov     ecx,eax         ; ecx:ebx <- divisor
        mov     ebx,CRT_LOWORD(DVSR)
        mov     edx,CRT_HIWORD(DVND) ; edx:eax <- dividend
        mov     eax,CRT_LOWORD(DVND)
L3:
        shr     ecx,1           ; shift divisor right one bit; hi bit <- 0
        rcr     ebx,1
        shr     edx,1           ; shift dividend right one bit; hi bit <- 0
        rcr     eax,1
        or      ecx,ecx
        jnz     short L3        ; loop until divisor < 4194304K
        div     ebx             ; now divide, ignore remainder

;
; We may be off by one, so to check, we will multiply the quotient
; by the divisor and check the result against the orignal dividend
; Note that we must also check for overflow, which can occur if the
; dividend is close to 2**64 and the quotient is off by 1.
;

        mov     ecx,eax         ; save a copy of quotient in ECX
        mul     CRT_HIWORD(DVSR)
        xchg    ecx,eax         ; put partial product in ECX, get quotient in EAX
        mul     CRT_LOWORD(DVSR)
        add     edx,ecx         ; EDX:EAX = QUOT * DVSR
        jc      short L4        ; carry means Quotient is off by 1

;
; do long compare here between original dividend and the result of the
; multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
; subtract the original divisor from the result.
;

        cmp     edx,CRT_HIWORD(DVND) ; compare hi words of result and original
        ja      short L4        ; if result > original, do subtract
        jb      short L5        ; if result < original, we are ok
        cmp     eax,CRT_LOWORD(DVND) ; hi words are equal, compare lo words
        jbe     short L5        ; if less or equal we are ok, else subtract
L4:
        sub     eax,CRT_LOWORD(DVSR) ; subtract divisor from result
        sbb     edx,CRT_HIWORD(DVSR)
L5:

;
; Calculate remainder by subtracting the result from the original dividend.
; Since the result is already in a register, we will perform the subtract in
; the opposite direction and negate the result to make it positive.
;

        sub     eax,CRT_LOWORD(DVND) ; subtract original dividend from result
        sbb     edx,CRT_HIWORD(DVND)
        neg     edx             ; and negate it
        neg     eax
        sbb     edx,0

;
; Just the cleanup left to do.  dx:ax contains the remainder.
; Restore the saved registers and return.
;

L2:

        pop     ebx

        ret     16
        }

        #undef DVND
        #undef DVSR
    }

    __declspec(naked) void _aullshr()
    {
        __asm
        {
        cmp     cl,64
        jae     short RETZERO

;
; Handle shifts of between 0 and 31 bits
;
        cmp     cl, 32
        jae     short MORE32
        shrd    eax,edx,cl
        shr     edx,cl
        ret

;
; Handle shifts of between 32 and 63 bits
;
MORE32:
        mov     eax,edx
        xor     edx,edx
        and     cl,31
        shr     eax,cl
        ret

;
; return 0 in edx:eax
;
RETZERO:
        xor     eax,eax
        xor     edx,edx
        ret
        }
    }

#if defined(__cplusplus)
};
#endif

#undef CRT_LOWORD
#undef CRT_HIWORD

#endif /* RT_DEFINE_VC && RT_DEFINE_32 */

#endif /* RT_DEFINE_USE_CRT */

#endif /* RT_WIN32_EX_MEM_H */
