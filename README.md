# Status Board

This project is a simple, Arduino-controlled work-from-home status board based on the Adafruit Huzzah. It uses the the Huzzah's WiFi capabilities to run a lightweight web server with a basic set of API endpoints used to control the following:

* 1 x Neopixel strip
* 3 x Neopixel LEDs

## API Endpoints

The API assumes your board has these labels for the LEDs:

* In a Meeting
* Do Not Disturb
* Headphones
* Needs Coffee

You probably have use of the first three, but the last one is flexible. You can re-label any of the endpoints in the script. 

_/status_

Returns a JSON response with the current state of each LED

```json
{"meeting":true, "headphones":false, "dnd":false, "coffee":false}
```

_/reset_

Resets all LEDs to their off state

_/meeting_

Toggle the state of the Neopixel LED strip

_/dnd_

Toggle the state of the Do Not Distrub LED

_/headphones_

Toggle the state of the headphones LED

_/coffee_

Toggle the state of the "needs coffee" LED

Each endpoint returns a JSON response with a `result` and `messge`.

```json
{"result": false, "message": "Meeting LED Off"}
```

## Using the Web App

The web server root returns the contents of the `index.h` file. It is formatted for mobile phones. You can access it by saving it to your home screen. On iOS, you can create a shortcut that acts like an app. Use the "Open Safari" action to open your sign's IP address. You can set a custom app icon and the shortcut is smart enough to know if you already have the app open in a Safari tab. When you tap the icon, Safari will open and switch to your open tab or open a new tab.

The webpage has a running job to check the LED status every 2.5 seconds, so if your dashboards stay synced if you are controlling the sign from multiple browsers.

![App](https://github.com/chrismaddalena/StatusBoard/raw/main/Web_App_Preview.png)
