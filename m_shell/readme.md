# M_SHELL

<p>An effort to write the shell again to gain understanding of how shell works and how OS create and handle processes and allow processes to communicate with each other through it's IPC interface.<br>
~Αρσλάν
</p>

## Introduction

<p>This section defines the basic structure of the shell how it handles and parse the command that are passed to it</p>

### Handling Commands with logical Operators

<p>We will scanning the input first from this regex to handle if there is any logical operator such as && or ||
This can be done by using the following regex:<br>
<code>(.+)(&&|'|''|')(.+)</code>
</p>
<p>
<code>int handle_logical_command(char *command, regex_t *logic_preg, regex_t *pipe_preg)</code> will use the above compiled regex in order to check weather the command has logical operators. if the regex didn't find any match then it will pass the control to <code>int handle_command_pipes(char *command, regex_t *pipe_preg)</code>. If the regex finds the match it will break the command in segments on the basis of logical operators and pass the control to <code>int handle_command_pipes(char *command, regex_t *pipe_preg)</code> in sequence and later the function will pass the control to <code>int handle_command_generic(char *command)</code>. Once the control will return back to the main logical_handler function it will return the status</p>

### Handling Commands with Pipes

<p>
Once the function <code>int handle_command_pipes(char *command, regex_t *pipe_preg)</code> get's called it will check weather the command has pipes or not this can be done by using another regex :<br>
<code>(.+)\|(.+)</code><br>
if the regex did not find any match we will return to main function if there is any pipe to handle then this function will handle them in sequence and pass control to generic command handler function once pipe function will be done processing the command it will return it's to it's parent calling function.
</p>
