## Functions and JSON Responses

## Function: `get_registered_esps()`

### Description:
This function retrieves all registered ESPs along with associated user information from the database.

### Arguments:
None

### JSON Return:
The function returns a JSON array containing objects representing registered ESPs and their associated users. 

#### Example JSON Response:
```json
[
    {
        "DeviceIndex": "ESP_DeviceIndex_1",
        "DeviceID": "ESP_DeviceID_1",
        "RegistrationTime": "YYYY-MM-DD HH:MM:SS",
        "LastActiveTime": "YYYY-MM-DD HH:MM:SS",
        "Assigned": true,
        "Registered": true,
        "MacAddress": "ESP_MacAddress_1",
        "Users": [
            {
                "UserID": "User_ID_1",
                "Username": "Username_1",
                "RegistrationDate": "YYYY-MM-DD HH:MM:SS",
                "DeviceIndex": "User_DeviceIndex_1"
            },
        ]
    },
    // More ESP objects if available
]
´´´
In case of an exception, the function returns a JSON object with an error message and a status code of 500:
´´´{
    "error": "Error message here"
}
´´´

## Function: `get_all_esps()`

### Description:
This function retrieves all Electronic Service Providers (ESPs) from the database.

### Arguments:
None

### JSON Return:
The function returns a JSON array containing objects representing all ESPs available in the database.

#### Example JSON Response:
```json
[
    {
        "DeviceIndex": "ESP_DeviceIndex_1",
        "DeviceID": "ESP_DeviceID_1",
        "RegistrationTime": "YYYY-MM-DD HH:MM:SS",
        "LastActiveTime": "YYYY-MM-DD HH:MM:SS",
        "Assigned": true,
        "Registered": true,
        "MacAddress": "ESP_MacAddress_1"
    },
    // More ESP objects if available
]
´´´
In case of an exception, the function returns a JSON object with an error message and a status code of 500:
´´´{
    "error": "Error message here"
}
´´´