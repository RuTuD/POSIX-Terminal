# POSIX-Terminal

# Shell Simulation Project

This project is a **custom shell simulation** implemented in **C++**. It mimics basic functionalities of a Unix shell, allowing users to execute commands, manage files, and handle processes in a simplified way. It supports fundamental commands like `cd`, `echo`, `pwd`, `ls`, and custom commands for command history, file searching, and more.

## Features

- **Directory Navigation**: Commands like `cd` to change directories and `pwd` to print the current directory.
- **Command Execution**: Run external commands in both **foreground** and **background** modes.
- **File Management**: `ls` command with options to list files in a directory, including hidden files and detailed information.
- **Redirection Handling**: Support for **input and output redirection** using symbols like `<`, `>`, and `>>`.
- **Process Information**: `pinfo` command to get details about a specific process.
- **Search Functionality**: `search` command to locate files in the current directory.
- **Command History**: Track up to 20 previous commands, which are stored in a history file and retrieved in future sessions.

## Requirements

- **C++ compiler** with C++11 support or higher.
- **UNIX-like environment** (Linux or MacOS) for full compatibility.

## File Structure

- **main.cpp**: The main file containing the shell implementation and functions for each command.
- **history.txt**: File used to store the command history across sessions.


## Usage

### Commands

- **cd [directory]**: Change to the specified directory. Supports:
  - `cd -`: Switch to the previous directory.
  - `cd ~`: Go to the home directory.
  - `cd ..`: Move up one directory level.

- **pwd**: Display the current working directory.

- **ls [-a -l] [directory]**: List files in the specified directory. Options:
  - `-a`: Include hidden files in the listing.
  - `-l`: Show detailed information (e.g., permissions, owner, size).
  
- **echo [text]**: Print the specified text to the console.

- **search [filename]**: Search for a specific file in the current directory and its subdirectories.

- **pinfo [pid]**: Display information about a process with the specified `pid`. If no `pid` is provided, it shows information for the current process.

- **history [number]**: Display the command history with the last `number` of commands. If no number is provided, the last 10 commands are shown.



### Directory Navigation

This shell supports standard directory navigation using the following commands:

- **cd [directory]**: Change to the specified directory. Supports:
  - `cd -`: Switch to the previous directory.
  - `cd ~`: Go to the home directory.
  - `cd ..`: Move up one directory level.

- **pwd**: Display the current working directory.

### File Management

This shell allows users to manage and view files in directories with the following commands:

- **ls [-a -l] [directory]**: List files in the specified directory. Options:
  - `-a`: Include hidden files in the listing.
  - `-l`: Show detailed information, including permissions, owner, size, and modification date.
  - **Example**: `ls -al` displays a detailed list of all files, including hidden ones, in the current directory.

### Text Output

This shell enables users to print text directly to the console:

- **echo [text]**: Display the specified text. Supports variable interpolation in a Unix-like environment.
  - **Example**: `echo Hello, World!` outputs "Hello, World!" to the console.

### File Search

This shell provides a search command to locate files in the current directory and subdirectories:

- **search [filename]**: Search for a specific file in the current directory recursively.
  - **Example**: `search myfile.txt` checks all files and subdirectories in the current directory for `myfile.txt`.

### Process Information

This shell allows users to view information about specific processes:

- **pinfo [pid]**: Display information about a process with a specified `pid`. If no `pid` is provided, it shows the information for the current process.
  - Information includes process ID, status, memory usage, and executable path.
  - **Example**: `pinfo 1234` provides details about the process with PID 1234.

### Command History

This shell maintains a history of commands used during the session. The `history` command allows users to retrieve previous commands.

- **history [number]**: Display the last `number` of commands entered. If no number is given, it displays the last 10 commands.
  - **Example**: `history 5` shows the last five commands entered in the shell.

### Input/Output Redirection

This shell supports standard input/output redirection using the following symbols:
- `>`: Redirects standard output to a file. Example: `echo Hello > file.txt`.
- `>>`: Appends output to an existing file. Example: `echo Hello >> file.txt`.
- `<`: Redirects standard input from a file. Example: `cat < file.txt`.

### Running Commands in Background

This shell supports running commands in the background by appending an `&` symbol at the end of the command. When a command is executed in the background, the shell immediately returns control to the user, allowing additional commands to be entered while the background command runs independently.

- **Usage**: To run a command in the background, type the command followed by `&`.
  - **Example**: `ls &` runs the `ls` command in the background and outputs the process ID (PID) for tracking the background task.

In the background command implementation, a **child process** is created using `fork()`. The child process then executes the specified command using `execvp`, while the parent process (shell) continues running without waiting for the child process to complete.

### Running Commands in Foreground

Commands executed without `&` at the end run in the foreground by default. When a command runs in the foreground, the shell waits until the command completes before allowing further input.

- **Usage**: To run a command in the foreground, type the command normally without `&`.
  - **Example**: `ls` runs the `ls` command in the foreground, and the shell waits until `ls` completes before accepting new input.

In the foreground command implementation, a **child process** is created using `fork()`, and the shell uses `waitpid` to wait for the child process to complete before returning control to the user.
