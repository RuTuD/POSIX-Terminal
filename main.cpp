#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <map>
#include <vector>
#include <bits/stdc++.h>
// Used for handling directory files
#include <dirent.h>
#include <string.h>
// For EXIT codes and error handling
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <deque>
#include <fcntl.h>     // Header for file control options
#include <sys/stat.h>  // Header for file status information
#include <sys/types.h> //
#include <sys/wait.h>

using namespace std;

vector<string> history_of_commands;

void set_choices(map<string, int> &mp)
{
  mp["cd"] = 1;
  mp["echo"] = 2;
  mp["pwd"] = 3;
  mp["ls"] = 4;
  mp["search"] = 5;
  mp["pinfo"] = 6;
  mp["history"] = 7;
}

void pwd_command()
{

  cout << get_current_dir_name() << endl;
}

void cd_command(vector<char *> &list)
{
  if (list.size() > 2)
  {
    cout << "bash: cd: too many arguments" << endl;
  }

  if (list.size() == 1)
    return;

  string k = list[1];

  if (k == "-")
    pwd_command();

  else if (k == "~")
  {
    int count = 0;
    string t = get_current_dir_name();
    while (chdir("..") == 0 && t != "/")
    {
      t = get_current_dir_name();
    }
  }

  else
  {
    if (chdir(list[1]) != 0)
      perror("failed");
  }
}

void echo_command(vector<char *> &list)
{

  // if(list.size()<2)
  // return;

  if (list[1] == NULL)
    cout << endl;

  else
    cout << list[1] << endl;
}

int checkDirectory(char *fileName)
{
  struct stat sfile;

  stat(fileName, &sfile);

  return S_ISREG(sfile.st_mode);
}

void print_file_details(char *file, char *username)
{
  struct stat sfile;
  stat(file, &sfile);

  if (stat(file, &sfile) == -1)
  {
    perror("Error");
  }

  if (checkDirectory(file) == 0)
    printf("d");

  printf((sfile.st_mode & S_IRUSR) ? "r" : "-");
  printf((sfile.st_mode & S_IWUSR) ? "w" : "-");
  printf((sfile.st_mode & S_IXUSR) ? "x" : "-");
  printf((sfile.st_mode & S_IRGRP) ? "r" : "-");
  printf((sfile.st_mode & S_IWGRP) ? "w" : "-");
  printf((sfile.st_mode & S_IXGRP) ? "x" : "-");
  printf((sfile.st_mode & S_IROTH) ? "r" : "-");
  printf((sfile.st_mode & S_IWOTH) ? "w" : "-");
  printf((sfile.st_mode & S_IXOTH) ? "x" : "-");
  printf(" %u", (unsigned int)sfile.st_nlink);
  printf(" %s", username);
  printf(" %s", username);
  printf(" %ld ", sfile.st_size);
  // cout<<ctime(&sfile.st_mtime);
  string time_date = ctime(&sfile.st_mtime);
  // cout<<endl;
  // cout<<time_date<<endl;
  int n = time_date.length();

  vector<string> words;

  int i = 0;

  while (i < n)
  {
    string temp;
    while (i < n && time_date[i] != ' ')
    {
      temp.push_back(time_date[i]);
      i++;
    }

    if (temp.length() > 0)
      words.push_back(temp);

    while (i < n && time_date[i] == ' ')
      i++;
  }

  for (int i = 1; i < words.size() - 2; i++)
  {
    cout << words[i] << " ";
  }

  string a = words[3];

  i = a.length() - 1;

  while (i >= 0 && a[i] != ':')
  {
    a.pop_back();
    i--;
  }
  a.pop_back();
  cout << a << " ";
  // cout<<endl;
  // printf(" %s",sfile.st_mtime);
  printf(" %s", file);
}

void ls_command(vector<char *> &list, char *username)
{

  struct dirent *d;
  int i = 1;
  int flaga = 0;
  int flagl = 0;
  int flagenter = 0;
  string res = get_current_dir_name();
  while (i < list.size())
  {
    string k = list[i];
    if (k != "-a" && k != "-l" && k != "-al" && k != "-la" && k != "~" && k != "." && k != "..")
    {

      res.push_back('/');
      res = res + list[i];
      i++;
    }
    else if (k == ".")
    {
      i++;
    }
    else if (k == "..")
    {

      res = res + "/..";
      i++;
    }
    else if (k == "~")
    {
      res = "/";
      i++;
    }
    else if (k == "-la" || k == "-al")
    {
      flaga = 1;
      flagenter = 1;
      flagl = 1;
      i++;
    }
    else if (k == "-a")
    {
      flaga = 1;
      i++;
    }
    else if (k == "-l")
    {
      flagl = 1;
      flagenter = 1;
      i++;
    }

    else
    {
      if (k[0] == '-')
        cout << "ls: invalid option -- " << k << endl;

      else
        cout << "ls: cannot access " << k << " : No such file or directory" << endl;

      return;
    }
  }

  int size = res.length();

  char directory[size + 1];
  // char*directory = res;

  for (int j = 0; j < size; j++)
  {
    directory[j] = res[j];
  }
  directory[size] = '\0';

  DIR *dh = opendir(directory);

  if (!dh)
  {
    if (errno = ENOENT)
    {
      // If the directory is not found
      cout << "Directory doesn't exist" << endl;
    }
    else
    {
      // If the directory is not readable then throw error and exit
      cout << "Unable to read directory" << endl;
    }
    exit(EXIT_FAILURE);
  }
  // While the next entry is not readable we will print directory files
  while ((d = readdir(dh)) != NULL)
  {
    // If hidden files are found we continue
    if (!flaga && d->d_name[0] == '.')
      continue;
    if (flagl)
    {
      print_file_details(d->d_name, username);
    }
    else
    {
      printf("%s  ", d->d_name);
    }
    if (flagenter)
      printf("\n");
  }
  if (!flagenter)
    printf("\n");
}


int search_command(vector<char *> &list)
{

  struct dirent *d;

  if (list.size() == 1)
  {
    cout << "Specify file name to search" << endl;
    return -1;
  }

  else if (list.size() > 2)
  {
    cout << "bash: cd: too many arguments" << endl;
    return -1;
  }

  char *directory = get_current_dir_name();
  DIR *dh = opendir(directory);
  string dir = list[1];

  if (!dh)
  {
    if (errno = ENOENT)
    {
      // If the directory is not found
      perror("Directory doesn't exist");
    }
    else
    {
      // If the directory is not readable then throw error and exit
      perror("Unable to read directory");
    }
    exit(EXIT_FAILURE);
  }
  // While the next entry is not readable we will print directory files
  while ((d = readdir(dh)) != NULL)
  {
    // If hidden files are found we continue
    if (d->d_name[0] == '.')
      continue;
    string k = d->d_name;
    if (dir == k)
      return 1;

    else
    {
      if (checkDirectory(d->d_name) == 0)
      {
        chdir(d->d_name);
        int val = search_command(list);
        chdir("..");
        if (val == 1)
          return 1;
      }
    }
  }
  return 0;
}

void pinfo_command(vector<char *> &list)
{
  int pid;

  if (list.size() > 2)
  {
    cout << "bash: cd: too many arguments" << endl;
    return;
  }
  if (list.size() == 1)
  {
    pid = getpid();
  }
  else
  {
    pid = stoi(list[1]);
  }

  string process_dir = "/proc/" + to_string(pid) + "/";

  // Check if the process exists by checking if its directory exists

  char check_dir[process_dir.length() + 1];
  for (int i = 0; i < process_dir.length(); i++)
  {
    check_dir[i] = process_dir[i];
  }

  check_dir[process_dir.length()] = '\0';

  struct stat sts;

  if (stat(check_dir, &sts) == -1 && errno == ENOENT)
  {
    cout << "Process with PID " << pid << " does not exist." << endl;
    return;
  }

  // Read and display process status from /proc/<pid>/status

  ifstream process_status(process_dir + "status");

  if (!process_status.is_open())
  {
    perror("Unable to open file");
    return; // Exit with an error code
  }

  string line;
  while (getline(process_status, line))
  {
    if (line.find("State:") != string::npos)
    {
      cout << "pid -- " << pid << endl;

      pid_t check_process_nature = tcgetpgrp(STDIN_FILENO);

      if (check_process_nature == pid)
      {
        cout << "Process Status -- " << line.substr(7, 1) + "+" << line.substr(8) << endl;
      }
      else
      {
        cout << "Process Status -- " << line.substr(7) << endl;
      }

      break;
    }
  }
  process_status.close();

  // Read and display memory info from /proc/<pid>/status
  ifstream process_memory(process_dir + "status");

  if (!process_memory.is_open())
  {
    perror("Unable to open file");
    return; // Exit with an error code
  }

  while (getline(process_memory, line))
  {
    if (line.find("VmSize:") != string::npos)
    {
      cout << "memory -- " << line.substr(8) << " {Virtual Memory}" << endl;
      break;
    }
  }
  process_memory.close();

  // Read and display executable path from /proc/<pid>/exe
  char executable_path[1024];
  int size = process_dir.length();

  char path_dir[size + 4];
  for (int i = 0; i < size; i++)
  {
    path_dir[i] = process_dir[i];
  }

  path_dir[size] = 'e';
  path_dir[size + 1] = 'x';
  path_dir[size + 2] = 'e';
  path_dir[size + 3] = '\0';

  ssize_t len = readlink(path_dir, executable_path, sizeof(executable_path) - 1);
  if (len != -1)
  {
    executable_path[len] = '\0';
    cout << "Executable Path -- " << executable_path << endl;
  }
}

void background_command(vector<char *> &list)
{
  char *res[list.size() + 1];

  for (int i = 0; i < list.size(); i++)
  {
    res[i] = list[i];

    // res = res + temp;
  }

  res[list.size()] = NULL;

  if (list.size() == 0)
    return;

  pid_t pid = fork();

  if (pid < 0)
  {
    perror("Failed");
    return;
  }
  else if (pid == 0)
  {
    // Child process
    if (execvp(list[0], res) == -1)
    {
      perror("unscuccessful:");
      exit(1);
    }
  }
  else
  {
    // Parent process

    // Print the PID of the background process
    cout << pid << endl;
    // backgroundPids.push_back(pid);
  }
}

void history_command(vector<char *> &list)
{

  if (list.size() > 2)
  {
    cout << "bash: cd: too many arguments" << endl;
    return;
  }

  int history_count = history_of_commands.size();

  if (list.size() == 1)
    history_count = min(10, history_count);

  else if (list.size() == 2)
  {
    int res = stoi(list[1]);
    history_count = min(history_count, res);
  }

  int j = history_of_commands.size() - 1;

  while (history_count--)
  {
    cout << history_of_commands[j] << endl;
    j--;
  }

  return;
}

bool check_redirection(string str)
{
  int w = str.length();
  char query[w + 1];

  for (int i = 0; i < w; i++)
    query[i] = str[i];

  query[w] = '\0';

  char *token = strtok(query, " ");

  while (token)
  {

    string k = token;

    if (k == ">" || k == "<" || k == ">>")
      return true;
    // take subsequent tokens
    token = strtok(NULL, " ");
  }

  return false;
}

void redirection_command(string str)
{

  int w = str.length();
  char query[w + 1];

  for (int i = 0; i < w; i++)
    query[i] = str[i];

  query[w] = '\0';

  char *toke = strtok(query, " ");

  vector<char *> list;

  while (toke)
  {

    string k = toke;
    //cout << k << " -> ";
    list.push_back(toke);

    // take subsequent tokens
    toke = strtok(NULL, " ");
  }
  cout << endl;

  int count = 0;

  string check = list[0];

  if (check == "<" || check == ">" || check == ">>")
    return;

  vector<char *> commands;

  while (count < list.size())
  {
    string k = list[count];

    if (k == "<" || k == ">" || k == ">>")
      break;

    commands.push_back(list[count]);
    count++;
  }

  cout << "count " << count << endl;

  int output = -1;
  int input = -1;
  int append = -1;

  int j = count;

  while (j < list.size())
  {

    string t = list[j];
    if (t == "<")
    {
      if (j + 1 >= list.size())
        return;
      check = list[j + 1];
      if (check == "<" || check == ">" || check == ">>")
        return;
      input = j + 1;
    }
    else if (t == ">")
    {

      if (j + 1 >= list.size())
        return;
      check = list[j + 1];
      if (check == "<" || check == ">" || check == ">>")
        return;
      output = j + 1;
    }

    else if (t == ">>")
    {

      if (j + 1 >= list.size())
        return;
      check = list[j + 1];
      if (check == "<" || check == ">" || check == ">>")
        return;

      append = j + 1;
    }

    j = j + 2;
  }

  if (input != -1)
  {
    commands.push_back(list[input]);
  }

  char *res[commands.size() + 1];
  for (int i = 0; i < commands.size(); i++)
  {

    res[i] = commands[i];
    //cout << commands[i] << " ";
  }
  //cout << endl;
  res[commands.size()] = NULL;

  //cout << input << " " << output << " " << append << endl;

  int inputFD;
  int outputFD;

  int in;
  int out;
 int saved_stdin = dup(STDIN_FILENO);
  if (input != -1)
  {
   
    inputFD = open(list[input], O_RDONLY);
    // int inputFD = open(list[input], O_RDONLY);
    if (inputFD == -1)
    {
      cerr << "Error opening input file" << endl;
      exit(1);
    }

    //in = dup(0);
    if (dup2(inputFD, STDIN_FILENO) == -1)
    {
      cerr << "Error redirecting stdin" << endl;
      close(inputFD);
      exit(1);
    }

    close(inputFD);
    

  }

 int saved_stdout = dup(STDOUT_FILENO);

  if (append != -1 && output != -1)
  {
    if (append > output)
    {
      outputFD = open(list[append], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR, 0644);

      if (outputFD == -1)
      {
        cerr << "Error opening output file" << endl;
        exit(1);
      }

      if (dup2(outputFD, 1) == -1)
      {
        cerr << "Error redirecting stdin" << endl;
        close(outputFD); // Close the original input file descriptor
        exit(1);
      }
      close(outputFD);
    }
    else
    {
      outputFD = open(list[output], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR, 0644);

      if (outputFD == -1)
      {
        cerr << "Error opening output file" << endl;
        exit(1);
      }

      if (dup2(outputFD, 1) == -1)
      {
        cerr << "Error redirecting stdin" << endl;
        close(outputFD); // Close the original input file descriptor
        exit(1);
      }
      close(outputFD);
    }
  }

  else if (output != -1 && append == -1)
  {
    int outputFD = open(list[output], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR, 0644);

    if (outputFD == -1)
    {
      cerr << "Error opening output file" << endl;
      exit(1);
    }

    if (dup2(outputFD, 1) == -1)
    {
      cerr << "Error redirecting stdin" << endl;
      close(outputFD); // Close the original input file descriptor
      exit(1);
    }
    close(outputFD);
  }
  else if (output == -1 && append != -1)
  {
    int outputFD = open(list[append], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR, 0644);

    if (outputFD == -1)
    {
      cerr << "Error opening output file" << endl;
      exit(1);
    }

    if (dup2(outputFD, 1) == -1)
    {
      cerr << "Error redirecting stdin" << endl;
      close(outputFD); // Close the original input file descriptor
      exit(1);
    }
    close(outputFD);
  }

  pid_t pid = fork(); // Create a new process

  if (pid == 0)
  {
    // This code runs in the child process
    execvp(commands[0], res); // Replace the child process with the desired command
    perror("execvp");         // This line will execute only if execvp fails
    exit(EXIT_FAILURE);       // Terminate the child process in case of an error
  }
  else if (pid > 0)
  {
    // This code runs in the parent process
    int status;
    waitpid(pid, &status, 0); // Wait for the specific child process to complete
    
  }
  else
  {
    perror("fork"); // Handle error if fork fails
  }


dup2(saved_stdin,STDIN_FILENO);
    close(saved_stdin);

  dup2(saved_stdout,STDOUT_FILENO);
  close(saved_stdout);  
  // cout<<"ok"<<endl;

  // dup2(1, outputFD);
  // dup2(in, inputFD);
  // if (input != -1)
  // {
  //   close(inputFD);
  //   close(in);
  // }
  // if (output != -1 && append != -1)
  // {
  //   close(outputFD);
  // }
  return;
}

void foreground_command(vector<char *> &list)
{
  char *res[list.size() + 1];

  for (int i = 0; i < list.size(); i++)
  {
    res[i] = list[i];
  }
  res[list.size()] = NULL;

  pid_t pid = fork(); // Create a new process

  if (pid == 0)
  {
    // This code runs in the child process

    execvp(res[0], res); // Replace the child process with the "ls" program
    perror("execvp");    // This line will execute only if execvp fails
  }
  else if (pid > 0)
  {
    int status;
    waitpid(pid, &status, 0);

    // This code runs in the parent process
    // Parent can do something else while the child is running
  }
  else
  {
    perror("fork"); // Handle error if fork fails
  }
}

void restore_history_session(char* username)
{
    ifstream inputFile;
    
    // Open the file
       string user = username;
    string path = "/home/" + user + "/Desktop/2023202007_A2/history.txt";
    inputFile.open(path); // Replace "example.txt" with your file's name

    // Check if the file is open successfully
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return; // Return an error code
    }

    // Read and print the file content line by line
    string line;
    while (std::getline(inputFile, line)) {
        history_of_commands.push_back(line); 
    }

    // Close the file
    inputFile.close();
}

void store_history_session(char*username)
{
  

    // Declare an ofstream object
    ofstream outputFile;
    
    // Open the file for writing
    string user = username;
    string path = "/home/" + user + "/Desktop/2023202007_A2/history.txt";
    outputFile.open(path,ios::trunc); // Replace "output.txt" with your desired file name

    // Check if the file is open successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open the file for writing." << std::endl;
        return; // Return an error code
    }

    // Write content to the file
  
    for(int i=0;i<history_of_commands.size()-1;i++)
    outputFile << history_of_commands[i] << endl;
    
    outputFile<<history_of_commands[history_of_commands.size()-1];

    // Close the file
    outputFile.close();

    return;


}

void execute_command(string str,char *username)
{

     if (str.length() == 0)
      return;


    if (check_redirection(str))
    {
      cout << "redirection" << endl;

      redirection_command(str);
      return;
    }

    int w = str.length();
    char query[w + 1];

    for (int i = 0; i < w; i++)
      query[i] = str[i];

    query[w] = '\0';

    char *token = strtok(query, " ");

    string k = token;

    vector<char *> list;

    if (k == "echo")
    {
      // cout<<k<<endl;
      list.push_back(token);

      token = strtok(NULL, "");

      if (token != NULL)
        list.push_back(token);
    }

    else
    {

      while (token)
      {

        list.push_back(token);
        // take subsequent tokens
        token = strtok(NULL, " ");
      }
    }

    if (list.size() == 0)
      return;

    string isbackground = list[list.size() - 1];

    if (isbackground == "&")
    {
      list.pop_back();
      background_command(list);
      return;
    }

    string choice = list[0];

    map<string, int> mp;

    set_choices(mp);
    int res;

    switch (mp[choice])
    {

    case 1:
      cd_command(list);
      // cd - need to do properly
      break;

    case 2:
      echo_command(list);
      break;

    case 3:
      pwd_command();
      break;

    case 4:
      ls_command(list, username);
      break;

    case 5:
      res = search_command(list);
      if (res == 0)
      {
        cout << "False" << endl;
      }
      else if (res == 1)
      {
        cout << "True" << endl;
      }
      break;

    case 6:
      pinfo_command(list);
      break;

    case 7:
      history_command(list);
      break;
    default:
      foreground_command(list);
      // cout<<choice<<": command not found"<<endl;
        break;
    }


  }


int main()
{

  char hostname[HOST_NAME_MAX];
  char username[LOGIN_NAME_MAX];
  int result;
  result = gethostname(hostname, HOST_NAME_MAX);

  if (result)
  {
    perror("gethostname");
    return EXIT_FAILURE;
  }
  result = getlogin_r(username, LOGIN_NAME_MAX);
  if (result)
  {
    perror("getlogin_r");
    return EXIT_FAILURE;
  }
  

  //cout << username << " " << hostname << endl;
   restore_history_session(username);
  int res;
  while (1)
  {

    string current_directory = get_current_dir_name();

    if (current_directory.length() == 1 && current_directory[0] == '/')
    {
      current_directory.pop_back();
    }

    cout << username << "@" << hostname << ":~" << current_directory << ">";
    string str;
    getline(cin, str);
    

    if (history_of_commands.size() == 20)
    {
      auto it = history_of_commands.begin();
      history_of_commands.erase(it);
    }
    history_of_commands.push_back(str);
    //cout<<str<<endl;
    int w = str.length();
    char query[w + 1];

    for (int i = 0; i < w; i++)
      query[i] = str[i];

    query[w] = '\0';

    char *token = strtok(query, ";");
    
    vector<string>all_commands;
    // string k = token;
    // all_commands.push_back(k); 

     while (token)
      {
        string k = token;
         all_commands.push_back(k);
        token = strtok(NULL, ";");
        
       
      }

    for(int i=0;i<all_commands.size();i++)
    {  
    execute_command(all_commands[i],username);
    }
    

    store_history_session(username);

  }

  
    return 0;
}
