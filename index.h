const char _index[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="styles.css">
    <title>ESP8266FSAutoConnect</title>
</head>

<body>
    <div class="container">

        <div class="topbar">
            <p class="title">Control Panel</p>
            <button id="ID_THEME_BTN" class="theme-toggle-btn">
                <svg xmlns="http://www.w3.org/2000/svg" id="ID_DARK_IMG" class="theme-svg" viewBox="0 0 24 24"
                    fill="none" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"
                    class="feather feather-moon">
                    <path d="M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z"></path>
                </svg>
                <svg xmlns="http://www.w3.org/2000/svg" id="ID_LIGHT_IMG" class="d-none" viewBox="0 0 24 24" fill="none"
                    stroke-width="2" stroke-linecap="round" stroke-linejoin="round"
                    class="feather feather-sun">
                    <circle cx="12" cy="12" r="5"></circle>
                    <line x1="12" y1="1" x2="12" y2="3"></line>
                    <line x1="12" y1="21" x2="12" y2="23"></line>
                    <line x1="4.22" y1="4.22" x2="5.64" y2="5.64"></line>
                    <line x1="18.36" y1="18.36" x2="19.78" y2="19.78"></line>
                    <line x1="1" y1="12" x2="3" y2="12"></line>
                    <line x1="21" y1="12" x2="23" y2="12"></line>
                    <line x1="4.22" y1="19.78" x2="5.64" y2="18.36"></line>
                    <line x1="18.36" y1="5.64" x2="19.78" y2="4.22"></line>
                </svg>
            </button>
        </div>

        <div class="d-none" id="ID_ERR_BG">
            <p id="ID_ERR_MSG"></p>
        </div>
        <div class="content">
            <div class="card">
                <p class="card-title">Servo Controller</p>
                <div class="card-row">
                    <input id="ID_ANGLE" class="slider" type="range" min="0" max="179" value="0">
                    <div class="angle-wrap">
                        <p id="ID_ANGLE_VALUE" class="slider-value">-</p>
                    </div>
                </div>
                <div class="card-row" id="ID_ANGLE_BTNS">
                </div>
            </div>
            
            <div class="card">
                <p class="card-title">AP Credentials</p>
                <div class="card-row">
                    <input type="text" class="card-textbox" id="ID_AP_SSID" placeholder="Enter SSID">
                    <input type="password" class="card-textbox" id="ID_AP_PASS" placeholder="Enter Password">
                </div>
                <div class="card-row">
                    <button id="ID_AP_SUBMIT" class="btn">Submit</button>
                </div>
            </div>
            <div class="card">
                <p class="card-title">Settings</p>
                <div class="card-row">
                    <button id="ID_AP_RESET" class="btn">Reset AP Credentials</button>
                    <button id="ID_ALL_RESET" class="btn">Factory Reset</button>
                </div>
            </div>
        </div>

    </div>
    <script src="script.js"></script>
</body>

</html>
)=====";