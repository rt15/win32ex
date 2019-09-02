#include "layer002/RtQuickSort.h"

#include "layer001/RtMemory.h"

/***
*shortsort(hi, lo, width, comp, context) - insertion sort for sorting short arrays
*
*Purpose:
*       sorts the sub-array of elements between lo and hi (inclusive)
*       side effects:  sorts in place
*       assumes that lo < hi
*
*Entry:
*       char *lo = pointer to low element to sort
*       char *hi = pointer to high element to sort
*       size_t width = width in bytes of each array element
*       int (*comp)() = pointer to function returning analog of strcmp for
*               strings, but supplied by user for comparing the array elements.
*               it accepts 2 pointers to elements, together with a pointer to a context
*               (if present). Returns neg if 1<2, 0 if 1=2, pos if 1>2.
*       void *context - pointer to the context in which the function is
*               called. This context is passed to the comparison function.
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/

static RT_B RT_API RtQuickSort_ShortSort(char* lo, char* hi, RT_UN unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext)
{
    char *p, *max;
    RT_N nComparisonResult;

    /* Note: in assertions below, i and j are alway inside original bound of
       array to sort. */

    while (hi > lo) {
        /* A[i] <= A[j] for i <= j, j > hi */
        max = lo;
        for (p = lo+unItemSize; p <= hi; p += unItemSize) {
            /* A[i] <= A[max] for lo <= i < p */

            if (!lpComparisonCallback(p, max, lpContext, &nComparisonResult))
            {
              return RT_FALSE;
            }
            if (nComparisonResult > 0) {
                max = p;
            }
            /* A[i] <= A[max] for lo <= i <= p */
        }

        /* A[i] <= A[max] for lo <= i <= hi */

        RtMemory_Swap(max, hi, unItemSize);

        /* A[i] <= A[hi] for i <= hi, so A[i] <= A[j] for i <= j, j >= hi */

        hi -= unItemSize;

        /* A[i] <= A[j] for i <= j, j > hi, loop top condition established */
    }
    /* A[i] <= A[j] for i <= j, j > lo, which implies A[i] <= A[j] for i < j,
       so array is sorted */
    return RT_TRUE;
}

/***
*qsort(base, num, wid, comp) - quicksort function for sorting arrays
*
*Purpose:
*   quicksort the array of elements
*   side effects:  sorts in place
*   maximum array size is number of elements times size of elements,
*   but is limited by the virtual address space of the processor
*
*Entry:
*   char *base = pointer to base of array
*   size_t num  = number of elements in the array
*   size_t width = width in bytes of each array element
*   int (*comp)() = pointer to function returning analog of strcmp for
*           strings, but supplied by user for comparing the array elements.
*           it accepts 2 pointers to elements.
*           Returns neg if 1<2, 0 if 1=2, pos if 1>2.
*
*Exit:
*   returns void
*
*Exceptions:
*   Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/


RT_B RT_API RtQuickSort_Sort(void* lpArea, RT_UN unSize, RT_UN unItemSize, RT_COMPARISON_CALLBACK lpComparisonCallback, void* lpContext)
{
    /* Note: the number of stack entries required is no more than
       1 + log2(unSize), so 30 is sufficient for any array.
       Calculated with STKSIZ (8*sizeof(void*) - 2) = 62 for 64 bits. */
    char *lo, *hi;              /* ends of sub-array currently sorting */
    char *mid;                  /* points to middle of subarray */
    char *loguy, *higuy;        /* traveling pointers for partition step */
    RT_UN size;                /* size of the sub-array */
    char *lostk[62], *histk[62];
    int stkptr;                 /* stack for saving sub-array to be processed */
    RT_N nComparisonResult;

    if (unSize < 2)
        return RT_TRUE;                 /* nothing to do */

    stkptr = 0;                 /* initialize stack */

    lo = (char *)lpArea;
    hi = (char *)lpArea + unItemSize * (unSize-1);        /* initialize limits */

    /* this entry point is for pseudo-recursion calling: setting
       lo and hi and jumping to here is like recursion, but stkptr is
       preserved, locals aren't, so we preserve stuff on the stack */
recurse:

    size = (RT_UN)((hi - lo) / unItemSize + 1);        /* number of el's to sort */

    /* below a certain size, it is faster to use a O(n^2) sorting method */
    if (size <= 8) {
        if (!RtQuickSort_ShortSort(lo, hi, unItemSize, lpComparisonCallback, lpContext))
        {
          return RT_FALSE;
        }
    }
    else {
        /* First we pick a partitioning element.  The efficiency of the
           algorithm demands that we find one that is approximately the median
           of the values, but also that we select one fast.  We choose the
           median of the first, middle, and last elements, to avoid bad
           performance in the face of already sorted data, or data that is made
           up of multiple sorted runs appended together.  Testing shows that a
           median-of-three algorithm provides better performance than simply
           picking the middle element for the latter case. */

        mid = lo + (size / 2) * unItemSize;      /* find middle element */

        /* Sort the first, middle, last elements into order */

        if (!lpComparisonCallback(lo, mid, lpContext, &nComparisonResult))
        {
          return RT_FALSE;
        }
        if (nComparisonResult > 0) {
            RtMemory_Swap(lo, mid, unItemSize);
        }
        if (!lpComparisonCallback(lo, hi, lpContext, &nComparisonResult))
        {
          return RT_FALSE;
        }
        if (nComparisonResult > 0) {
            RtMemory_Swap(lo, hi, unItemSize);
        }
        if (!lpComparisonCallback(mid, hi, lpContext, &nComparisonResult))
        {
          return RT_FALSE;
        }
        if (nComparisonResult > 0) {
            RtMemory_Swap(mid, hi, unItemSize);
        }

        /* We now wish to partition the array into three pieces, one consisting
           of elements <= partition element, one of elements equal to the
           partition element, and one of elements > than it.  This is done
           below; comments indicate conditions established at every step. */

        loguy = lo;
        higuy = hi;

        /* Note that higuy decreases and loguy increases on every iteration,
           so loop must terminate. */
        for (;;) {
            /* lo <= loguy < hi, lo < higuy <= hi,
               A[i] <= A[mid] for lo <= i <= loguy,
               A[i] > A[mid] for higuy <= i < hi,
               A[hi] >= A[mid] */

            /* The doubled loop is to avoid calling comp(mid,mid), since some
               existing comparison funcs don't work when passed the same
               value for both pointers. */

            if (mid > loguy) {
                while (RT_TRUE)  {
                    loguy += unItemSize;
                    if (loguy >= mid) break;
                    if (!lpComparisonCallback(loguy, mid, lpContext, &nComparisonResult)) return RT_FALSE;
                    if (nComparisonResult > 0) break;
                }
            }
            if (mid <= loguy) {
                while (RT_TRUE)  {
                    loguy += unItemSize;
                    if (loguy > hi) break;
                    if (!lpComparisonCallback(loguy, mid, lpContext, &nComparisonResult)) return RT_FALSE;
                    if (nComparisonResult > 0) break;
                }
            }

            /* lo < loguy <= hi+1, A[i] <= A[mid] for lo <= i < loguy,
               either loguy > hi or A[loguy] > A[mid] */

            while (RT_TRUE) {
                higuy -= unItemSize;
                if (higuy <= mid) break;
                if (!lpComparisonCallback(higuy, mid, lpContext, &nComparisonResult)) return RT_FALSE;
                if (nComparisonResult <= 0) break;
            }

            /* lo <= higuy < hi, A[i] > A[mid] for higuy < i < hi,
               either higuy == lo or A[higuy] <= A[mid] */

            if (higuy < loguy)
                break;

            /* if loguy > hi or higuy == lo, then we would have exited, so
               A[loguy] > A[mid], A[higuy] <= A[mid],
               loguy <= hi, higuy > lo */

            RtMemory_Swap(loguy, higuy, unItemSize);

            /* If the partition element was moved, follow it.  Only need
               to check for mid == higuy, since before the swap,
               A[loguy] > A[mid] implies loguy != mid. */

            if (mid == higuy)
                mid = loguy;

            /* A[loguy] <= A[mid], A[higuy] > A[mid]; so condition at top
               of loop is re-established */
        }

        /*     A[i] <= A[mid] for lo <= i < loguy,
               A[i] > A[mid] for higuy < i < hi,
               A[hi] >= A[mid]
               higuy < loguy
           implying:
               higuy == loguy-1
               or higuy == hi - 1, loguy == hi + 1, A[hi] == A[mid] */

        /* Find adjacent elements equal to the partition element.  The
           doubled loop is to avoid calling comp(mid,mid), since some
           existing comparison funcs don't work when passed the same value
           for both pointers. */

        higuy += unItemSize;
        if (mid < higuy) {
            do  {
                higuy -= unItemSize;
                if (higuy <= mid) break;
                if (!lpComparisonCallback(higuy, mid, lpContext, &nComparisonResult)) return RT_FALSE;
            } while (nComparisonResult == 0);
        }
        if (mid >= higuy) {
            do  {
                higuy -= unItemSize;
                if (higuy <= lo) break;
                if (!lpComparisonCallback(higuy, mid, lpContext, &nComparisonResult)) return RT_FALSE;
            } while (nComparisonResult == 0);
        }

        /* OK, now we have the following:
              higuy < loguy
              lo <= higuy <= hi
              A[i]  <= A[mid] for lo <= i <= higuy
              A[i]  == A[mid] for higuy < i < loguy
              A[i]  >  A[mid] for loguy <= i < hi
              A[hi] >= A[mid] */

        /* We've finished the partition, now we want to sort the subarrays
           [lo, higuy] and [loguy, hi].
           We do the smaller one first to minimize stack usage.
           We only sort arrays of size 2 or more.*/

        if ( higuy - lo >= hi - loguy ) {
            if (lo < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy;
                ++stkptr;
            }                           /* save big recursion for later */

            if (loguy < hi) {
                lo = loguy;
                goto recurse;           /* do small recursion */
            }
        }
        else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;               /* save big recursion for later */
            }

            if (lo < higuy) {
                hi = higuy;
                goto recurse;           /* do small recursion */
            }
        }
    }

    /* We have sorted the array, except for any pending sorts on the stack.
       Check if there are any, and do them. */

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           /* pop subarray from stack */
    }
    else
        return RT_TRUE;                 /* all subarrays done */
}
