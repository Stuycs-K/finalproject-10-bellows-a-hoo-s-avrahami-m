# Dev Log:

This document must be updated daily every time you finish a work session.

## Stanley Hoo

### 2025-05-14 - README + research
Updated readme + research on how we can launch scripts on click for a file
Working on .bat file to run scripts on click
Fixed c script pathing issues

### 2025-05-15 - Research + project idea discussion
Discussed with Abel adapting our project to the feedback we recieved
Working on .bat file

### 2025-05-16
.bat file now runs c script and restarts the ~/.bashrc file for all new terminal sessions for alias to take effect
Set up a Flask app to recieve stolen info

### 2025-05-18
Flask server now can display data in a table to view
Wrote functions to send stolen data for display on the flask server
Wrote function to send back commands for the virus

### 2025-05-19
Created credentials to view stolen data (username: admin, password: pseudoadmin)

### 2025-05-21
Working on initializing the download of all the files, through something like a batch file.

### 2025-05-22
Created .exe file to download pseudo file using curl on click.

### 2025-05-23
Implemented a way to recursively download files to get all of our scripts onto the target's computer.

### 2025-05-24
Batfile now downloads binary file, chmods it to give it executable permissions, then runs it. Also curls cat.png and opens it to avoid suspicions.

### 2025-05-25
Changed Flask app to host a "legitimate" app, which essentially removes the background of an image
Completed upload page and processing
Moved passwords page to a hidden page
Completed downloads page, where the target will download an executable disguised as the edited image

### 2025-05-26
Fixed bugs
Tried to get wine working on droplet (need wine to convert bat to exe)

### 2025-05-27
Working on .desktop file for linux

### 2025-05-28
Added additional routes to the Flask app

### 2025-05-29
Working on .desktop file, figured out basics and how to run it

### 2025-05-30
Finished .desktop file, does same thing as bat file and self deletes on completion (hopefully, haven't tested yet)
Implemented a function to download both the desktop and png by clicking on the button (linux only) so that the png shows and target may be misled
Updated README

### 2025-06-02
Updated README with description and instructions

### 2025-06-04
Fixed routing error for downloads
Created presentation slides
Edited video and uploaded on Youtube