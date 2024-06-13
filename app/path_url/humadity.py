from app import api
from app.controller.humadity.main import *


def humadity_api_path():
    api.add_resource(Temperature, "/api/temperature")
    api.add_resource(Humadity, "/api/humidity")
    api.add_resource(Sensor, "/api/sensor_data")