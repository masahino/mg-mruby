/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "def.h"
#include "kbd.h"

#include <mruby.h>
#include <mruby/string.h>

#include "mrb_mg.h"
#include "mrb_extend.h"

extern int indentmode(int, int);
extern int makebkfile(int, int);
extern int setfillcol(int, int);

/* 
    insert
            Insert a string, mainly for use from macros.
*/
mrb_value
mrb_mg_insert(mrb_state *mrb, mrb_value self)
{
     mrb_value str;
     char *cstr;
     int len, ret;
     mrb_get_args(mrb, "S", &str);
     len = RSTRING_LEN(str);
     cstr = strndup(RSTRING_PTR(str), len);
     ret = linsert_str(cstr, len);
     return mrb_fixnum_value(ret);
}


/*
     next-line
            Move forward n lines.
*/
mrb_value
mrb_mg_next_line(mrb_state *mrb, mrb_value self)
{
     mrb_int n_value = 1;
     int cur_line;
     int ret;

     mrb_get_args(mrb, "|i", &n_value);

     cur_line = curwp->w_dotline;
     ret = forwline(FFRAND, n_value);
     if (curwp->w_dotline == cur_line)
         return mrb_false_value();
     return mrb_true_value();
}

void
mrb_extend_init(mrb_state *mrb)
{
    struct RClass *mg;

    mg = mrb_module_get(mrb, "MG");

    mrb_define_module_function(mrb, mg, "mrb_insert",
			       mrb_mg_insert, ARGS_REQ(1));
    mrb_define_module_function(mrb, mg, "mrb_next_line",
			       mrb_mg_next_line, ARGS_OPT(1));
}

/*

     apropos
            Help Apropos.  Prompt the user for a string, open the *help*
            buffer, and list all mmgg commands that contain that string.

     auto-fill-mode
            Toggle auto-fill mode (sometimes called mail-mode), where text
            inserted past the fill column is automatically wrapped to a new
            line.

     auto-indent-mode
            Toggle indent mode, where indentation is preserved after a new-
            line.

     back-to-indentation
            Move the dot to the first non-whitespace character on the current
            line.

     backward-char
            Move cursor backwards one character.

     backward-kill-word
            Kill text backwards by n words.

     backward-paragraph
            Move cursor backwards n paragraphs.  Paragraphs are delimited by
            <NL><NL> or <NL><TAB> or <NL><SPACE>.

     backward-word
            Move cursor backwards by the specified number of words.

     beginning-of-buffer
            Move cursor to the top of the buffer.

     beginning-of-line
            Move cursor to the beginning of the line.

     blink-and-insert
            Self-insert a character, then search backwards and blink its
            matching delimeter.  For delimeters other than parenthesis, brack-
            ets, and braces, the character itself is used as its own match.

     bsmap-mode
            Toggle bsmap mode, where DEL and c-H are swapped.

     c-mode
            Toggle a KNF-compliant mode for editing C program files.

     call-last-kbd-macro
            Invoke the keyboard macro.

     capitalize-word
            Capitalize n words; i.e. convert the first character of the word
            to upper case, and subsequent letters to lower case.

     cd     Change the global working directory.  See also global-wd-mode.

     copy-region-as-kill
            Copy all of the characters in the region to the kill buffer,
            clearing the mark afterwards.  This is a bit like a kill-region
            followed by a yank.

     count-matches
            Count the number of lines matching the supplied regular expres-
            sion.

     count-non-matches
            Count the number of lines not matching the supplied regular
            expression.

     define-key
            Prompts the user for a named keymap (mode), a key, and an mg com-
            mand, then creates a keybinding in the appropriate map.

     delete-backward-char
            Delete backwards n characters.  Like delete-char, this actually
            does a kill if presented with an argument.

     delete-blank-lines
            Delete blank lines around dot.  If dot is sitting on a blank line,
            this command deletes all the blank lines above and below the cur-
            rent line.  Otherwise, it deletes all of the blank lines after the
            current line.

     delete-char
            Delete _n characters forward.  If any argument is present, it kills
            rather than deletes, saving the result in the kill buffer.

     delete-horizontal-space
            Delete any whitespace around the dot.

     delete-leading-space
            Delete leading whitespace on the current line.

     delete-trailing-space
            Delete trailing whitespace on the current line.

     delete-matching-lines
            Delete all lines after dot that contain a string matching the sup-
            plied regular expression.

     delete-non-matching-lines
            Delete all lines after dot that contain a string matching the sup-
            plied regular expression.

     delete-other-windows
            Make the current window the only window visible on the screen.

     delete-window
            Delete current window.

     describe-bindings
            List all global and local keybindings, putting the result in the
            *help* buffer.

     describe-key-briefly
            Read a key from the keyboard, and look it up in the keymap.  Dis-
            play the name of the function currently bound to the key.

     digit-argument
            Process a numerical argument for keyboard-invoked functions.

     downcase-region
            Set all characters in the region to lower case.

     downcase-word
            Set characters to lower case, starting at the dot, and ending _n
            words away.

     emacs-version
            Return an mg version string.

     end-kbd-macro
            Stop defining a keyboard macro.

     end-of-buffer
            Move cursor to the end of the buffer.

     end-of-line
            Move cursor to the end of the line.

     enlarge-window
            Enlarge the current window by shrinking either the window above or
            below it.

     eval-current-buffer
            Evaluate the current buffer as a series of mg commands.  Useful
            for testing mg startup files.

     eval-expression
            Get one line from the user, and run it.  Useful for testing
            expressions in mg startup files.

     exchange-point-and-mark
            Swap the values of "dot" and "mark" in the current window.  Return
            an error if no mark is set.

     execute-extended-command
            Invoke an extended command; i.e. M-x.  Call the message line rou-
            tine to read in the command name and apply autocompletion to it.
            When it comes back, look the name up in the symbol table and run
            the command if it is found, passing arguments as necessary.  Print
            an error if there is anything wrong.

     fill-paragraph
            Justify a paragraph, wrapping text at the current fill column.

     find-file
            Select a file for editing.  First check if the file can be found
            in another buffer; if it is there, just switch to that buffer.  If
            the file cannot be found, create a new buffer, read in the file
            from disk, and switch to the new buffer.

     find-file-read-only
            Same as find-file, except the new buffer is set to read-only.

     find-alternate-file
            Replace the current file with an alternate one.  Semantics for
            finding the replacement file are the same as find-file, except the
            current buffer is killed before the switch.  If the kill fails, or
            is aborted, revert to the original file.

     find-file-other-window
            Opens the specified file in a second buffer.  Splits the current
            window if necessary.

     forward-paragraph
            Move forward n paragraphs.  Paragraphs are delimited by <NL><NL>
            or <NL><TAB> or <NL><SPACE>.

     forward-char
            Move cursor forwards (or backwards, if n is negative) n charac-
            ters.  Returns an error if the end of buffer is reached.

     forward-word
            Move the cursor forward by the specified number of words.

     global-unset-key
            Unbind a key from the global (fundamental) key map; i.e. set it to
            ’rescan’.

     global-wd-mode
            Toggle global working-directory mode.  When enabled, mg defaults
            to opening files (and executing commands like compile and grep)
            relative to the global working directory.  When disabled, a work-
            ing directory is set for each buffer.

     goto-line
            Go to a specific line.  If an argument is present, then it is the
            line number, else prompt for a line number to use.

     help-help
            Prompts for one of (a)propos, (b)indings, des(c)ribe key briefly.

     insert-buffer
            Insert the contents of another buffer at dot.

     insert-file
            Insert a file into the current buffer at dot.

     insert-with-wrap
            Insert the bound character with word wrap.  Check to see if we’re
            past the fill column, and if so, justify this line.

     isearch-backward
            Use incremental searching, initially in the reverse direction.
            isearch ignores any explicit arguments.  If invoked during macro
            definition or evaluation, the non-incremental search-backward is
            invoked instead.

     isearch-forward
            Use incremental searching, initially in the forward direction.
            isearch ignores any explicit arguments.  If invoked during macro
            definition or evaluation, the non-incremental search-forward is
            invoked instead.

     join-line
            Join the current line to the previous.  If called with an argu-
            ment, join the next line to the current one.

     just-one-space
            Delete any whitespace around dot, then insert a space.

     keyboard-quit
            Abort the current action.

     kill-buffer
            Dispose of a buffer, by name.  If the buffer name does not start
            and end with an asterisk, prompt the user if the buffer has been
            changed.

     kill-line
            Kill line.  If called without an argument, it kills from dot to
            the end of the line, unless it is at the end of the line, when it
            kills the newline.  If called with an argument of 0, it kills from
            the start of the line to dot.  If called with a positive argument,
            it kills from dot forward over that number of newlines.  If called
            with a negative argument it kills any text before dot on the cur-
            rent line, then it kills back abs(n) lines.

     kill-paragraph
            Delete n paragraphs starting with the current one.

     kill-region
            Kill the currently defined region.

     kill-word
            Delete forward n words.

     line-number-mode
            Toggle whether line and column numbers are displayed in the mode-
            line.

     list-buffers
            Display the list of available buffers.

     load   Prompt the user for a filename, and then execute commands from
            that file.

     local-unset-key
            Unbind a key mapping in the local (topmost) mode.

     make-backup-files
            Toggle generation of backup files.

     meta-key-mode
            When disabled, the meta key can be used to insert extended-ascii
            (8-bit) characters.  When enabled, the meta key acts as usual.

     negative-argument
            Process a negative argument for keyboard-invoked functions.

     next-line
            Move forward n lines.

     newline
            Insert a newline into the current buffer.

     newline-and-indent
            Insert a newline, then enough tabs and spaces to duplicate the
            indentation of the previous line.  Assumes tabs are every eight
            characters.

     no-tab-mode
            Toggle notab mode.  In this mode, spaces are inserted rather than
            tabs.

     not-modified
            Turn off the modified flag in the current buffer.

     open-line
            Open up some blank space.  Essentially, insert n newlines, then
            back up over them.

     other-window
            The command to make the next (down the screen) window the current
            window.  There are no real errors, although the command does noth-
            ing if there is only 1 window on the screen.

     overwrite-mode
            Toggle overwrite mode, where typing in a buffer overwrites exist-
            ing characters rather than inserting them.

     prefix-region
            Inserts a prefix string before each line of a region.  The prefix
            string is settable by using ’set-prefix-string’.

     previous-line
            Move backwards n lines.

     previous-window
            This command makes the previous (up the screen) window the current
            window.  There are no errors, although the command does not do a
            lot if there is only 1 window.

     push-shell
            Suspend mg and switch to alternate screen, if available.

     pwd    Display current (global) working directory in the status area.

     query-replace
            Query Replace.  Search and replace strings selectively, prompting
            after each match.

     redraw-display
            Refresh the display.  Recomputes all window sizes in case some-
            thing has changed.

     replace-string
            Replace string globally without individual prompting.

     query-replace-regexp
            Replace strings selectively.  Does a search and replace operation
            using regular expressions for both patterns.

     quoted-insert
            Insert the next character verbatim into the current buffer; i.e.
            ignore any function bound to that key.

     recenter
            Reposition dot in the current window.  By default, the dot is cen-
            tered.  If given a positive argument (n), the display is reposi-
            tioned to line n.  If n is negative, it is that line from the bot-
            tom.

     re-search-again
            Perform a regular expression search again, using the same search
            string and direction as the last search command.

     re-search-backward
            Search backwards using a regular expression.  Get a search string
            from the user, and search, starting at dot and proceeding toward
            the front of the buffer.  If found, dot is left pointing at the
            first character of the pattern [the last character that was
            matched].

     re-search-forward
            Search forward using a regular expression.  Get a search string
            from the user and search for it starting at dot.  If found, move
            dot to just after the matched characters.  display does all the
            hard stuff.  If not found, it just prints a message.

     save-buffer
            Save the contents of the current buffer if it has been changed,
            optionally creating a backup copy.

     save-buffers-kill-emacs
            Offer to save modified buffers and quit mg.

     save-some-buffers
            Look through the list of buffers, offering to save any buffer that
            has been changed.  Buffers that are not associated with files
            (such as *scratch*, *grep*, *compile*) are ignored.

     scroll-down
            Scroll backwards n pages.  A two-line overlap between pages is
            assumed.  If given a repeat argument, scrolls back lines, not
            pages.

     scroll-one-line-down
            Scroll the display down n lines without changing the cursor posi-
            tion.

     scroll-one-line-up
            Scroll the display n lines up without moving the cursor position.

     scroll-other-window
            Scroll the next window in the window list window forward n pages.

     scroll-up
            Scroll forward one page.  A two-line overlap between pages is
            assumed.  If given a repeat argument, scrolls back lines, not
            pages.

     search-again
            Search again, using the same search string and direction as the
            last search command.

     search-backward
            Reverse search.  Get a search string from the user, and search,
            starting at dot and proceeding toward the front of the buffer.  If
            found, dot is left pointing at the first character of the pattern
            (the last character that was matched).

     search-forward
            Search forward.  Get a search string from the user, and search for
            it starting at dot.  If found, dot gets moved to just after the
            matched characters, if not found, print a message.

     self-insert-command
            Insert a character.

     set-case-fold-search
            Set case-fold searching, causing case nopt to matter in regular
            expression searches.  This is the default.

     set-default-mode
            Append the supplied mode to the list of default modes used by sub-
            sequent buffer creation.  Built in modes include: fill, indent,
            overwrite, and notab.

     set-fill-column
            Prompt the user for a fill column.  Used by auto-fill-mode.

     set-mark-command
            Sets the mark in the current window to the current dot location.

     set-prefix-string
            Sets the prefix string to be used by the ’prefix-region’ command.

     shrink-window
            Shrink current window by one line.  The window immediately below
            is expanded to pick up the slack.  If only one window is present,
            this command has no effect.

     space-to-tabstop
            Insert enough spaces to reach the next tab-stop position.  By
            default, tab-stops occur every 8 characters.

     split-window-vertically
            Split the current window.  A window smaller than 3 lines cannot be
            split.

     start-kbd-macro
            Start defining a keyboard macro.  Macro definition is ended by
            invoking end-kbd-macro.

     suspend-emacs
            Suspend mg and switch back to alternate screen, if in use.

     switch-to-buffer
            Prompt and switch to a new buffer in the current window.

     switch-to-buffer-other-window
            Switch to buffer in another window.

     toggle-read-only
            Toggle the read-only flag on the current buffer.

     transpose-chars
            Transpose the two characters on either side of dot.  If dot is at
            the end of the line, transpose the two characters before it.
            Return with an error if dot is at the beginning of line; it seems
            to be a bit pointless to make this work.

     undo   Undo the most recent action.  If invoked again without an inter-
            vening command, move the undo pointer to the previous action and
            undo it.

     undo-boundary
            Add an undo boundary.  This is not usually done interactively.

     undo-boundary-toggle
            Toggle whether undo boundaries are generated.  Undo boundaries are
            often disabled before operations that should be considered atomi-
            cally undoable.

     undo-enable
            Toggle whether undo information is kept.

     undo-list
            Show the undo records for the current buffer in a new buffer.

     universal-argument
            Repeat the next command 4 times.  Usually bound to C-u.  This com-
            mand may be stacked; e.g. C-u C-u C-f moves the cursor forward 16
            characters.

     upcase-region
            Upper case region.  Change all of the lower case characters in the
            region to upper case.

     upcase-word
            Move the cursor forward by the specified number of words.  As it
            moves, convert any characters to upper case.

     what-cursor-position
            Display a bunch of useful information about the current location
            of dot.  The character under the cursor (in octal), the current
            line, row, and column, and approximate position of the cursor in
            the file (as a percentage) is displayed.  The column position
            assumes an infinite position display; it does not truncate just
            because the screen does.

     write-file
            Ask for a file name and write the contents of the current buffer
            to that file.  Update the remembered file name and clear the
            buffer changed flag.

     yank   Yank text from kill-buffer.  Unlike emacs, the mg kill buffer con-
            sists only of the most recent kill.  It is not a ring.
*/
