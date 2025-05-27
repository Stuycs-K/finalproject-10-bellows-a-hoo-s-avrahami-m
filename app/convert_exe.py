import sys
import subprocess
import os

filename = sys.argv[1]
batfile = f"batfiles/{filename}.bat"
exe_output = f"executables/{filename}.exe"
ico_path = f"icos/{filename}.ico"
BAT_TO_EXE_PATH = "/home/stanley/B2E.exe"

env = os.environ.copy()
env["PATH"] = "/usr/local/bin:/usr/bin:/bin"

res = subprocess.run([
    "/usr/bin/xvfb-run", "-a", "wine", BAT_TO_EXE_PATH,
    "/bat", batfile,
    "/exe", exe_output,
    "/icon", ico_path,
    "/invisible"
])
