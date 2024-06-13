from app import create_app
from app.controller.mqtt.main import mqtt_loop
import threading

app = create_app()

# mqtt_thread = threading.Thread(target=mqtt_loop)
# mqtt_thread.start()

if __name__ == "__main__":
    app.run(debug=True, use_reloader=True)