from flask import Flask, request, jsonify

app = Flask(__name__)
temp_list = []

@app.route('/sensor/data', methods=["POST", "GET"])
def sensor():
    if request.method == 'POST':
        data = request.get_json()
        
        # Error handling
        if not data or "temperature" not in data or "kelembapan" not in data or "timestamp" not in data:
            return jsonify({'message': 'Invalid data'}), 400
        
        temperature = data["temperature"]
        kelembapan = data["kelembapan"]
        timestamp = data["timestamp"]

        temp_list.append([temperature, kelembapan, timestamp])

        print("Temperature:", temperature, "Kelembapan:", kelembapan, "Timestamp:", timestamp)
        
        return jsonify({'message': 'Data saved successfully'}), 200

    else:
        return jsonify({'temperature_list': temp_list}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)