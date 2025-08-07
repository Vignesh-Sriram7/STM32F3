from flask import Flask, request, jsonify, send_from_directory
import os

app = Flask(__name__)
UPLOAD_FOLDER = 'firmware'
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

@app.route('/')
def index():
    return '''
    <form method="post" enctype="multipart/form-data" action="/upload">
        <input type="file" name="firmware">
        <input type="submit" value="Upload Firmware">
    </form>
    '''

@app.route('/upload', methods=['POST'])
def upload_firmware():
    f = request.files['firmware']
    f.save(os.path.join(UPLOAD_FOLDER, 'firmware.bin'))
    with open(os.path.join(UPLOAD_FOLDER, 'ready.json'), 'w') as flag:
        flag.write('{"ready": true}')
    return 'Firmware uploaded and ready!'

@app.route('/firmware.bin')
def get_firmware():
    return send_from_directory(UPLOAD_FOLDER, 'firmware.bin')

@app.route('/ready.json')
def get_ready_flag():
    path = os.path.join(UPLOAD_FOLDER, 'ready.json')
    if os.path.exists(path):
        return send_from_directory(UPLOAD_FOLDER, 'ready.json')
    return jsonify({"ready": False})

@app.route('/clear')
def clear_flag():
    try:
        os.remove(os.path.join(UPLOAD_FOLDER, 'ready.json'))
        return 'Flag cleared.'
    except:
        return 'No flag to clear.'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)
