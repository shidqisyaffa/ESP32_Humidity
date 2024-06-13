from flask import request, jsonify
from flask_restful import Resource
from app.controller.mqtt.main import sensor_data
from types import SimpleNamespace

class Temperature(Resource):
    def get(self):
        return jsonify({"temperature": sensor_data["temperature"]})
    
class Humadity(Resource):
    def get(self):
        return jsonify({"humidity": sensor_data["humidity"]})
    
class Sensor(Resource):
    def get(self):
        return jsonify(sensor_data)
    
    def post(self):
        sensor_data["temperature"] = request.form.get('temperature')
        sensor_data["humidity"] = request.form.get('humidity')
        sensor_data["timestamp"] = request.form.get('timestamp')

        # Proses data sensor di sini, misalnya menyimpan ke database atau memproses lebih lanjut
        print(f"Received sensor data: Temperature: {sensor_data['temperature']}, Humidity: {sensor_data['humidity']}, Timestamp: {sensor_data['timestamp']}")

        return jsonify({"message": "Sensor data received", "data": sensor_data})