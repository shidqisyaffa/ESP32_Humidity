from flask import Flask
from flask_restful import Api
from flask_cors import CORS

api = Api()

def create_app():
    app = Flask(__name__)

    # atur origins (domain yang di izinkan) jika sudah tahap produksi
    CORS(app, resources={r"/*": {"origins": "*"}})

    from app.urls import __URLPATH__

    __URLPATH__()

    api.init_app(app)

    return app
