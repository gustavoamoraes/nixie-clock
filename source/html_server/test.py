from http_response import HTTPResponse
import os
import re

class Node ():

    ABSOLUTE = 1
    DYNAMIC = 2

    def __init__(self):
        self.path = None
        self.name = ''
        self.type = Node.ABSOLUTE
        self.childs = []

class Path ():
    def __init__(self, url, view):
        self.url = url
        self.view = view

first_node = Node ()

def parse_section (section):
    counter = 0
    par_indexes = []
    opened = []

    for index, char in enumerate(section):
        if char == '<':
            counter += 1
            opened.append(index)
        elif char == '>':
            if len(opened) > 0:
                par_indexes.append((opened[-1], index))
                opened.remove(opened[-1])
            counter -= 1

    if not counter == 0 or len(par_indexes) > 1:
        raise ValueError('Invalid URL section: {}'.format(section))

    if len(par_indexes) == 0:
        return (Node.ABSOLUTE, section)

    return (Node.DYNAMIC, section[par_indexes[0][0]+1:par_indexes[0][1]])

def create_tree (paths):
    for path in paths:

        path_sections = get_path_sections(path.url)

        current_node = first_node

        for i, path_section in enumerate(path_sections):
            
            section_type, section_name = parse_section(path_section)
            node_exits = False

            for node in current_node.childs:
                if node.name == section_name and node.type == section_type:
                    current_node = node
                    node_exits = True

            if not node_exits:
                new_node = Node()
                new_node.name = section_name
                new_node.type = section_type
                current_node.childs.append(new_node)
                current_node = new_node

            if i == len(path_sections) - 1:
                current_node.path = path
                break

def get_path_sections (path):

    path_sections = []
    
    def on_match (section):
        path_sections.append(section.group(0))
        return ''

    re.sub('([^/]+)', on_match, path)

    return path_sections or ['']

def get_view (request):

    path_sections = get_path_sections(request.path)
    
    current_node = first_node
    valid_path = True
    kwargs = {}

    for path_section in path_sections:

        matched_node = None

        for node in current_node.childs:
            if node.name == path_section and node.type == Node.ABSOLUTE:
                matched_node = node
            elif node.type == Node.DYNAMIC:
                kwargs[node.name] = path_section
                matched_node = node

        if not matched_node:
            valid_path = False
            break
        
        current_node = matched_node

    if not valid_path:
       view = HTTPResponse(status=404, reason='Not Found')
    else:
       view = current_node.path.view(request, **kwargs)

    if type(view) == HTTPResponse:
        return view.message()
    else:
        raise Exception('View from : {} does not return a HTTPResponse object'.format(path))