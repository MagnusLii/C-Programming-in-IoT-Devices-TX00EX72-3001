import json

def decode_json(json_string):
    try:
        decoded_message = json.loads(json_string)
        return decoded_message
    except json.decoder.JSONDecodeError as e:
        return f"Error: Invalid JSON format - {e}"

# Test cases
def test_decode_json():
    # Test valid JSON
    valid_json = '{"name": "John", "age": 30, "city": "New York"}'
    result_valid = decode_json(valid_json)
    if isinstance(result_valid, dict):
        print("Valid JSON Test Passed")
    else:
        print("Valid JSON Test Failed")

    # Test invalid JSON
    invalid_json = 'not a valid JSON'
    result_invalid = decode_json(invalid_json)
    if isinstance(result_invalid, str) and result_invalid.startswith("Error: Invalid JSON format"):
        print("Invalid JSON Test Passed")
    else:
        print("Invalid JSON Test Failed")

# Run the tests
test_decode_json()
