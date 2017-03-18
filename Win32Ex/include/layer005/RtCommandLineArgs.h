#ifndef RT_COMMAND_LINE_ARGS_H
#define RT_COMMAND_LINE_ARGS_H

#include "layer000/RtWin32ExTypes.h"

/**
 * @file
 * Parse command line arguments.
 *
 * <p>
 * Arguments are:
 * </p>
 * <ul>
 *   <li>Short options (One letter like -a).</li>
 *   <li>Long options (Dash separated words like --block-size).</li>
 *   <li>Non-options like u+rx or /tmp.</li>
 * </ul>
 *
 * <p>
 * Options can have no argument, an optional argument or a required argument.<br>
 * Arguments are re-ordered so that options precede non-options. 'mv src -f dest' become 'mv -f src dest'<br>
 * Options can appear multiple times.<br>
 * A single dash '-' is a non-option typically used to represent one of the standard input streams.<br>
 * Two dashes '--' terminates all options, any following arguments are considered non-option (Including any other '--').
 * </p>
 *
 * <p>
 * For short options:<br>
 * Value of an option can be separated from the option by a space or not: 'touch -r file1 file2' or 'touch -rfile1 file2'.<br>
 * Options can be concatenated after a single dash and can finish with an option with an optional/mandatory value: 'rm -rf dir', 'touch -mrRef.txt New.txt', 'touch -mr Ref.txt New.txt'.<br>
 * If the value is optional and available, it must be concatenated with the option: 'awk -dvars.txt '{ print }' readme.txt' is valid but 'awk -d vars.txt '{ print }' readme.txt' is not.<br>
 * When options are concatenated, all letters after the first option with optional/mandatory value are considered to be the value of this option (There is no more concatenated options): 'awk -PdO '{ print }' readme.txt' print variables in O file.
 * </p>
 *
 * <p>
 * For long options:<br>
 * Argument can be separated by a space or an equal: '--var-name=value' or '--var-name value'.<br>
 * Notice that 'ls --color never' search in never folder while 'ls --color=never' deactivate colors (Color option is defined as --color[=WHEN]).<br>
 * So when a long option has an optional argument, '=' must be used to set the argument.<br>
 * <a href="https://www.gnu.org/prep/standards/html_node/Option-Table.html#Option-Table">GNU classical long options.</a>
 * </p>
 *
 * <p>
 * Some examples:
 * </p>
 * <pre>
 * ps -u, ps u -> Display the user.
 * ps -u root -> Display all processes for root user.
 * rm -dv temp -> Remove directory temp in verbose mode.
 * mkdir temp1 temp2/rm -d temp1 temp2 -> Create and remove to directories.
 * touch -- -name -> Create -name file.
 * rm -r -r -R --recursive foo -> You can use same option several times.
 * ps -u oracle -u root u -> Display processes of root and oracle users with user names.
 * ls -T5 -> 5 tabs for ls command.
 * </pre>
 */

#define RT_COMMAND_LINE_ARG_TYPE_SHORT           0
#define RT_COMMAND_LINE_ARG_TYPE_LONG            1
#define RT_COMMAND_LINE_ARG_TYPE_NON_OPTION      2
#define RT_COMMAND_LINE_ARG_TYPE_END_OF_OPTIONS  3

#define RT_COMMAND_LINE_ARG_VALUE_NONE      0
#define RT_COMMAND_LINE_ARG_VALUE_OPTIONAL  1
#define RT_COMMAND_LINE_ARG_VALUE_REQUIRED  2

/**
 * @param unArgType RT_COMMAND_LINE_ARG_TYPE_SHORT or RT_COMMAND_LINE_ARG_TYPE_LONG.
 * @param bValid Whether the option has been recognized. Value cardinality is not taken in account.
 * @param nShortOption Short option letter if unArgType is RT_COMMAND_LINE_ARG_TYPE_SHORT.
 * @param lpLongOption Long option name if unArgType is RT_COMMAND_LINE_ARG_TYPE_LONG.
 * @param unValueCardinality One of RT_COMMAND_LINE_ARG_VALUE_XXXX.
 * @param lpValue Value for the option, can be null even for options with an argument. Can be valued even for options with no args if the user wrongly put a value.
 */
typedef RT_B (RT_CALL *RT_COMMAND_LINE_ARGS_CALLBACK)(RT_UN unArgType, RT_B bValid, RT_CHAR nShortOption, RT_CHAR* lpLongOption,
                                                      RT_UN unValueCardinality, RT_CHAR* lpValue, void* lpContext);

/**
 *
 * <p>
 * First argument is ignored as it should be the process name.
 * </p>
 *
 * <p>
 * Long options must not contain prepended '--'.
 * </p>
 *
 * @param lpArgC Can be updated if one of the argument is '--'.
 * @param lpArgV Beware that arguments can be re-ordered so watch out for concurrency and writing rights.
 * @param lpShortOptionsWithoutArg Short options without argument. Can be RT_NULL.
 * @param lpShortOptionsWithOptionalArg Short options with optional argument. Can be RT_NULL.
 * @param lpShortOptionsWithArg Short options with required argument. Can be RT_NULL.
 * @param lpLongOptionsWithoutArg Long options without argument. Can be RT_NULL.
 * @param lpLongOptionsWithOptionalArg Long options with optional argument. Can be RT_NULL.
 * @param lpLongOptionsWithArg Long options without argument. Can be RT_NULL.
 * @param lpNonOptionsIndex Index in lpArgV of the first non-option argument. All non-options are pushed at the end of lpArgV. Set to RT_TYPE_MAX_N32 if there are no non-options.
 */
RT_B RT_API RtParseCommandLineArgs(RT_N32* lpArgC, RT_CHAR* lpArgV[], RT_COMMAND_LINE_ARGS_CALLBACK lpCommandLineArgsCallback, void* lpContext,
                                   RT_CHAR* lpShortOptionsWithoutArg,  RT_CHAR* lpShortOptionsWithOptionalArg,  RT_CHAR* lpShortOptionsWithArg,
                                   RT_CHAR* lpLongOptionsWithoutArg[], RT_CHAR* lpLongOptionsWithOptionalArg[], RT_CHAR* lpLongOptionsWithArg[],
                                   RT_N32* lpNonOptionsIndex);

#endif /* RT_COMMAND_LINE_ARGS_H */
