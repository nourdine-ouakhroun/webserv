from flask import Flask

app = Flask(__name__)

@app.route('/')
def index():
    return '''
    <!DOCTYPE html>
    <html>
    <head>
        <title>My Website</title>
    </head>
    <body>
        <h1>Hello, World!</h1>
        <p>This is a simple website created with Python and Flask.</p>
    </body>
    </html>
    '''

if __name__ == '__main__':
    app.run(debug=True)