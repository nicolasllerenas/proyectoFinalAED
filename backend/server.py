from flask import Flask, request, jsonify, send_file, send_from_directory
import ctypes
import os

app = Flask(__name__, static_folder='../frontend', static_url_path='/')

# Cargar la librería como DLL
dll_path = os.path.abspath('./libsuffixarray.dll')
if not os.path.exists(dll_path):
    raise RuntimeError(f"Falta el archivo DLL: {dll_path}")

lib = ctypes.CDLL(dll_path)


# Crear SuffixArray
lib.create_sa.argtypes = [ctypes.c_char_p, ctypes.c_int]
lib.create_sa.restype = ctypes.c_void_p

# Liberar SuffixArray
lib.free_sa.argtypes = [ctypes.c_void_p]
lib.free_sa.restype = None

# Obtener SA como string
lib.get_sa.argtypes = [ctypes.c_void_p]
lib.get_sa.restype = ctypes.c_char_p

# Buscar patrón
lib.search_pattern.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
lib.search_pattern.restype = ctypes.c_int
# --------------------------------------------------

@app.route('/')
def index():
    return send_file('../frontend/index.html')

@app.route('/<path:path>')
def static_files(path):
    return send_from_directory('../frontend', path)

@app.route('/build', methods=['POST'])
def build_sa():
    # Obtener datos como string UTF-8
    text_str = request.json['text']
    text_bytes = text_str.encode('utf-8')
    
    # Crear SA en C++
    sa_ptr = lib.create_sa(text_bytes, len(text_bytes))
    
    # Obtener resultado
    sa_cstr = lib.get_sa(sa_ptr)
    sa_str = sa_cstr.decode('utf-8')
    positions = [int(x) for x in sa_str.split()]
    
    # Liberar memoria
    lib.free_sa(sa_ptr)
    
    return jsonify({
        'positions': positions,
        'text': text_str
    })

@app.route('/search', methods=['POST'])
def search():
    # Obtener datos
    text_str = request.json['text']
    pattern_str = request.json['pattern']
    
    text_bytes = text_str.encode('utf-8')
    pattern_bytes = pattern_str.encode('utf-8')
    
    # Crear SA
    sa_ptr = lib.create_sa(text_bytes, len(text_bytes))
    
    # Buscar patrón
    position = lib.search_pattern(sa_ptr, pattern_bytes)
    
    # Liberar memoria
    lib.free_sa(sa_ptr)
    
    return jsonify({'position': position})

if __name__ == '__main__':
    app.run(port=5000, debug=True)