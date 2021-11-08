from http_response import HTTPResponse
from http_request import HTTPRequest
import uasyncio as asyncio
import ujson as json
from test import *
import socket
import sys
import os 

# current_path = os.path.dirname(os.path.abspath(__file__))
# parrent_dir = os.path.dirname(current_path)
# sys.path.append(os.path.normpath(parrent_dir))
# from main import set_config, get_config

with open('file-extension-to-mime-types.json') as json_file:
    extention_to_mimes_types = json.load(json_file)
    json_file.close()

static_files_path = './static/'

def static_files_list (request):
    files = os.listdir(static_files_path)
    file_list = ''.join('<li> <a href="/static/{file_name}"> {file_name} </a> </li>'.format(file_name=name) for name in files)
    return HTTPResponse('<ul> {} </ul>'.format(file_list).encode())

def get_static_file (request, file_name):

    file_path = static_files_path + file_name
    file_extention = '.' + file_name.rsplit('.', 1)[-1]

    if file_extention in extention_to_mimes_types:
        mime_type = extention_to_mimes_types[file_extention]
    else:
        return HTTPResponse(b'MIME type list does not have this file extention.', status=404, reason='Not Found') 

    try:    
        file = open(file_path, 'rb')
    except:
        file = None

    if file:
        file_content = file.read()
        return HTTPResponse(file_content, mime_type)

    return HTTPResponse(b'File does not exist.', status=404, reason='Not Found')

def home_page (request):
    return HTTPResponse(b'<script> window.location.href=\'./static/page.html\'; </script>')

clock_config = {'foda': 123}

def config_api (request):

    global clock_config

    if request.method == 'POST' and (post := request.POST):
        clock_config = post
        return HTTPResponse()
    else:
        data = json.dumps(clock_config).encode()
        return HTTPResponse(data, 'application/json')

paths = [Path ('/', home_page),
    Path ('static', static_files_list),
    Path ('static/<file_name>', get_static_file),
    Path ('api/config', config_api)]

create_tree(paths)

async def handle_client (reader, writer):

    length = 0
    path = ''
    line_index = 0

    request = HTTPRequest()

    while True:
        line = (await reader.readline()).decode()

        if not line or line == '\r\n':
            break

        if line_index == 0:
            request.method, request.path, http_version = line.split()
        
        if 'Content-Length: ' in line:
            length = int(line[16:-2])

        line_index += 1

    if length:
        try:
            postquery = (await reader.read(length)).decode()
            post_request = json.loads(postquery)
            request.POST = post_request
        except:
            print('Error parsing postquery: ' + postquery)
    
    if request.path and (view := get_view(request)):
        await writer.awrite(view)
    
    await writer.wait_closed()

def run_server():
    loop = asyncio.get_event_loop()
    coro = asyncio.start_server(handle_client, '0.0.0.0', 80)
    server = loop.run_until_complete(coro)
    loop.run_forever()