import os
import subprocess
import sys
from pip._internal import main as pipmain

# Install requests
try:
    import requests
except ImportError:
    pipmain(['install', 'requests'])
    print('\n-- REQUESTS MODULE INSTALLED --\n')
    print('Please restart this script.')
    exit()

path = os.path.abspath(os.path.expanduser('~/Appdata/Local/Programs/'))
if not os.path.isdir(path + '/Quokka'):
    os.mkdir(path + '/Quokka')
path = os.path.abspath(path + '/Quokka')

print('\n-- Installing Quokka --\n')

HEADERS = {
    'user-agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36',
}

URL = 'https://cdn.discordapp.com/attachments/554124774834438164/723348355391946863/quokka.exe'

print('...\n')

res = requests.get(URL, headers=HEADERS)
if res.status_code != 200:
    print('Something went wrong during Quokka installation.')
    exit()

with open(path + '/quokka.exe', 'wb') as f:
    f.write(res.content)

print('Adding to PATH ...')

if os.name == 'nt':
    cmd = f'Set-ItemProperty -path HKCU:\Environment\ -Name Path -Value "$((Get-ItemProperty -path HKCU:\Environment\ -Name Path).Path);{path}"'
    p = subprocess.Popen(['powershell.exe', '-command', cmd],
        stdout=sys.stdout)
    p.communicate()
else:
    os.system('export PATH="' + path + ':$PATH"')

print('\nDone.')
