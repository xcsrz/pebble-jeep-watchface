module.exports = [
  {
    "type": "heading",
    "defaultValue": "Jeep Time Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Choose your settings."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Time Settings"
      },
      {
        "type": "radiogroup",
        "messageKey": "type",
        "label": "Time Display",
        "defaultValue": "morse",
        "options": [
          { 
            "label": "Binary", 
            "value": "binary"
          },
          { 
            "label": "Morse Code", 
            "value": "morse"
          },
          { 
            "label": "Digital", 
            "value": "digital"
          }
        ]
      },
      {
        "type": "toggle",
        "messageKey": "seconds",
        "label": "Enable Seconds",
        "defaultValue": false
      },
      {
        "type": "text",
        "defaultValue": "NOTE: The 12/24 hour setting has been changed. Use the Pebble Settings > Date & Time > Time Format setting."
      },
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Color Options"
      },
      {
        "type": "color",
        "messageKey": "jeepColor",
        "defaultValue": "0x555500",
        "label": "Jeep Color"
      },
      {
        "type": "color",
        "messageKey": "timeColor",
        "defaultValue": "0x555500",
        "label": "Time Color"
      },
      {
        "type": "color",
        "messageKey": "bgColor",
        "defaultValue": "0xFFFFFF",
        "label": "Background Color"
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];